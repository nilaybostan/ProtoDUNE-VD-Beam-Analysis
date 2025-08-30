#!/usr/bin/env python3
import os
import math
import requests
import sys
import urllib3
import numpy as np
import matplotlib.pyplot as plt


def BeamInfo_from_ifbeam(t0: int, t1: int):
    beam_infos = []
    run=0; evt=0; t=0; mom=0

    # --- TOF values as before ---
    tofs = get_tofs(t0, t1, 60.0, 0.0)

    # --- Cherenkov devices (two in NP02) ---
    ckov1_counts, ckov1_trig = get_ckov_values(t0, t1, "XCET021667")
    ckov2_counts, ckov2_trig = get_ckov_values(t0, t1, "XCET021669")

    # zip everything together (lengths may differ: handle carefully!)
    for i, tof in enumerate(tofs):
        c0 = ckov1_trig[i] if i < len(ckov1_trig) else 0
        c1 = ckov2_trig[i] if i < len(ckov2_trig) else 0
        beam_infos.append((run, evt, t, mom, tof, c0, c1))
    return beam_infos


def get_ckov_values(t0: str, t1: str, dev: str):
    """Fetch Cherenkov counts for device XCET021667 or XCET021669"""
    prefix = f"dip/acc/NORTH/NP02/BI/XCET/{dev}"
    counts     = get_var_values(t0, t1, prefix + ":counts")
    countsTrig = get_var_values(t0, t1, prefix + ":countsTrig")
    return counts, countsTrig


def get_tofs(t0: str, t1: str, delta_trig: float, offset: float):
    trig_n, trig_s, trig_c, trig_f = get_trigger_values(t0, t1)
    tof_s, tof_c, tof_f = [], [], []    
    tof_name = ['XBTF022638A','XBTF022638B','XBTF022670A','XBTF022670B']
    
    for name in tof_name:
        ni,si,ci,fi = get_tof_vars_values(t0, t1, name)
        tof_s.append(si); tof_c.append(ci); tof_f.append(fi)

    fDownstreamToGenTrig = delta_trig 
    tofs = []

    for i in range(len(trig_c)):
        trig_sec = trig_s[i*2+1] 
        trig_ns  = (trig_c[i]+offset)*8 + trig_f[i]/512.
        if trig_sec == 0:
            break

        # First check 2A
        for j in range(len(tof_c[2])):
            tof2A_sec = tof_s[2][j*2+1]
            tof2A_ns  = tof_c[2][j]*8 + tof_f[2][j]/512.             
            if tof2A_sec == 0: break
            delta_2A = 1e9*(trig_sec-tof2A_sec) + trig_ns - tof2A_ns
            if 0 < delta_2A < fDownstreamToGenTrig:
                check_valid_tof(tof2A_sec,tof2A_ns,tof_s[0],tof_c[0],tof_f[0],tofs)
                check_valid_tof(tof2A_sec,tof2A_ns,tof_s[1],tof_c[1],tof_f[1],tofs)

        # Then check 2B
        for j in range(len(tof_c[3])):
            tof2B_sec = tof_s[3][j*2+1]
            tof2B_ns  = tof_c[3][j]*8 + tof_f[3][j]/512.             
            if tof2B_sec == 0: break
            delta_2B = 1e9*(trig_sec-tof2B_sec) + trig_ns - tof2B_ns
            if 0 < delta_2B < fDownstreamToGenTrig:
                check_valid_tof(tof2B_sec,tof2B_ns,tof_s[0],tof_c[0],tof_f[0],tofs)
                check_valid_tof(tof2B_sec,tof2B_ns,tof_s[1],tof_c[1],tof_f[1],tofs)
                                
    return tofs


def check_valid_tof(tof_ref_sec, tof_ref_ns, tof_s, tof_c, tof_f, tofs: []):
    fUpstreamToDownstream = 500.
    for k in range(len(tof_c)):
        tof_sec = tof_s[k*2+1]
        tof_ns  = tof_c[k]*8 + tof_f[k]/512.
        if tof_sec == 0: break
        delta = 1e9*(tof_ref_sec-tof_sec) + tof_ref_ns - tof_ns
        if 0 < delta < fUpstreamToDownstream:
            tofs.append(delta)


def get_tof_vars_values(t0: str, t1: str, tof_var_name: str):
    prefix = "dip/acc/NORTH/NP02/BI/XTOF/"+tof_var_name
    return get_relevant_values(t0, t1, prefix)


def get_trigger_values(t0: str, t1: str):
    prefix = "dip/acc/NORTH/NP02/BI/TDC/GeneralTrigger"
    return get_relevant_values(t0, t1, prefix)
    

def get_relevant_values(t0: str, t1: str, prefix: str):
    seconds_data = get_var_values(t0, t1, prefix+":seconds[]")
    coarse_data  = get_var_values(t0, t1, prefix+":coarse[]")
    frac_data    = get_var_values(t0, t1, prefix+":frac[]")
    count_var    = get_var_values(t0, t1, prefix+":timestampCount")
    return count_var, seconds_data, coarse_data, frac_data
    

def get_var_values(t0: str, t1: str, var_name: str):
    event  = "z,pdune"
    lines = fetch_ifbeam(var_name, event, t0, t1)
    data = parse_csv_value(lines)
    return data    


def fetch_ifbeam(var, event, t0, t1):
    url = (
        "https://dbdata3vm.fnal.gov:9443/ifbeam/data/data"
        f"?e={event}&v={var}&t0={t0}&t1={t1}&f=csv"
    )
    urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)
    r = requests.get(url, verify=False)
    r.raise_for_status()
    return r.text.splitlines()


def parse_csv_value(lines):
    values = []
    for row in lines[1:]:  # skip header
        parts = row.strip().split(",")
        if len(parts) < 4:
            continue
        try:
            for i in range(5,len(parts)):
                val = int(float(parts[i]))
                values.append(val)
        except ValueError:
            continue
    return values


# -------------------------------
# Main function
# -------------------------------
def main():
    time_ranges = [
        ("08/26/2025 21:15:00", "08/27/2025 09:46:00"),
        ("08/27/2025 14:43:00", "08/27/2025 16:16:00"),
        ("08/27/2025 16:16:00", "08/27/2025 16:54:00")
    ]
    
    run_labels = [
        "Run 39132 (0.3 GeV, Cu target)",
        "Run 39136 (0.7 GeV, Cu target)",
        "Run 39137 (0.7 GeV, Cu target)"
    ]

    delta_trig = 60.0
    offset = 0.0
    nbins = 100
    xmin, xmax = 60, 90
    colors = ["blue", "red", "green"]

    plt.figure(figsize=(8,6))
    for i, ((t0, t1), run_label) in enumerate(zip(time_ranges, run_labels)):
        beam_infos = BeamInfo_from_ifbeam(t0, t1)
        tofs = [b[4] for b in beam_infos]   # extract TOF
        ckov1 = [b[5] for b in beam_infos]  # Cherenkov1
        ckov2 = [b[6] for b in beam_infos]  # Cherenkov2

        print(f"{run_label} ({t0} → {t1}): Collected {len(tofs)} TOF values, "
              f"with Ckov1={len(ckov1)} and Ckov2={len(ckov2)} entries")

        if len(tofs) == 0:
            continue
        tofs_array = np.array(tofs)

        plt.hist(tofs_array, bins=nbins, range=(xmin, xmax),
                 histtype="step", linewidth=2,
                 label=run_label,
                 color=colors[i % len(colors)])

        # Save individual run histograms
        plt.figure(figsize=(8,6))
        plt.hist(tofs_array, bins=nbins, range=(xmin, xmax),
                 histtype="stepfilled", alpha=0.6,
                 color=colors[i % len(colors)])
        plt.xlabel("TOF [ns]")
        plt.ylabel("Counts")
        plt.title(f"TOF Distribution ({run_label})")
        plt.grid(True, linestyle="--", alpha=0.7)
        fname = f"tof_run_{run_label.split()[1]}.png"
        plt.savefig(fname, dpi=150)
        plt.close()
        print(f"Saved {fname}")

    plt.xlabel("TOF [ns]")
    plt.ylabel("Counts")
    plt.title("TOF Distribution (Overlay of Runs)")
    plt.grid(True, linestyle="--", alpha=0.7)
    plt.legend(fontsize=10, loc="upper right")
    plt.savefig("tof_all_runs.png", dpi=150)
    plt.close()
    print("Saved combined histogram as tof_all_runs.png")


if __name__ == "__main__":
    main()
