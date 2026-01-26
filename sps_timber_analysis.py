#!/usr/bin/env python3
#written by Nilay B.
# in CERN SWAN, run with the command: python sps_timber_analysis.py 
# In Swan, in the software stack part, you should select the NXCALS cluster, then run the code. 
import pytimber
import pandas as pd
from pyspark.sql import SparkSession
import pytz

# --------------------------------------------------
# Timezones
# --------------------------------------------------
UTC = pytz.UTC
CEST = pytz.timezone("Europe/Zurich")

# --------------------------------------------------
# Spark session
# --------------------------------------------------
spark = (
    SparkSession.builder
    .appName("TIMBER_SPS_Analysis")
    .getOrCreate()
)

# --------------------------------------------------
# TIMBER
# --------------------------------------------------
logdb = pytimber.LoggingDB(spark_session=spark)

# --------------------------------------------------
# Inputs (UTC!)
# --------------------------------------------------
t1 = "2025-10-17 19:00:00"
t2 = "2025-10-17 19:05:00"

pv_int = "SPS.T2:INTENSITY"
pv_xtim = "XTIM.SX.WE-CT:Acquisition:acqC"

# --------------------------------------------------
# TIMBER query
# --------------------------------------------------
data = logdb.get([pv_int, pv_xtim], t1, t2)

time_int, intensity = data[pv_int]
time_xt, acqC = data[pv_xtim]

# --------------------------------------------------
# Detect timestamp unit
# --------------------------------------------------
def detect_unit(ts_array):
    max_val = float(ts_array.max())
    if max_val > 1e18:
        return "ns"
    if max_val > 1e12:
        return "ms"
    if max_val > 1e9:
        return "s"
    raise ValueError("Unknown timestamp unit")

unit = detect_unit(time_int)
print("Detected time unit:", unit)

# --------------------------------------------------
# Build DataFrames
# --------------------------------------------------
df_int = pd.DataFrame(
    {
        "TIME": time_int,
        "SPS.T2:INTENSITY": intensity,
    }
)

df_xt = pd.DataFrame(
    {
        "TIME": time_xt,
        "XTIM.SX.WE-CT:Acquisition:acqC": acqC,
    }
)

# --------------------------------------------------
# Align signals in time
# --------------------------------------------------
df = pd.merge_asof(
    df_int.sort_values("TIME"),
    df_xt.sort_values("TIME"),
    on="TIME",
    direction="nearest",
)

# --------------------------------------------------
# Convert TIME → LOG_TIMESTAMP (seconds)
# --------------------------------------------------
if unit == "ns":
    df["LOG_TIMESTAMP"] = df["TIME"] / 1e9
elif unit == "ms":
    df["LOG_TIMESTAMP"] = df["TIME"] / 1e3
else:
    df["LOG_TIMESTAMP"] = df["TIME"]

# --------------------------------------------------
# UTC and CEST conversion
# --------------------------------------------------
df["UTC_DATE"] = pd.to_datetime(
    df["LOG_TIMESTAMP"], unit="s", utc=True
)

df["CEST_DATE"] = df["UTC_DATE"].dt.tz_convert(CEST)

# --------------------------------------------------
# Final column order
# --------------------------------------------------
df = df[
    [
        "LOG_TIMESTAMP",
        "XTIM.SX.WE-CT:Acquisition:acqC",
        "SPS.T2:INTENSITY",
        "UTC_DATE",
        "CEST_DATE",
    ]
]

# --------------------------------------------------
# Save CSV
# --------------------------------------------------
df.to_csv("sps_t2_intensity_xtim.csv", index=False)

print(df.head())
