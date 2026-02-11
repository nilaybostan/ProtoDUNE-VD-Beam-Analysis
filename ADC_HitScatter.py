#how to run: python hit_evt.py -f np02vd_raw_run039636_0282_df-s04-d0_dw_0_20250926T014636_reco_stage0_20260127T094834_offline.root

from argparse import ArgumentParser
import ROOT
import numpy as np
import matplotlib
matplotlib.use("Agg")  # use non-interactive backend
import matplotlib.pyplot as plt
from matplotlib.colors import LogNorm

# ------------------------------------------------------------
# Constants
# ------------------------------------------------------------
TICK_US = 0.5
V_DRIFT = 0.16   # cm/us

# ProtoDUNE vertical drift typical wire pitch (cm)
WIRE_PITCH = 0.5

# Optional fiducial margins (cm)
Y_MIN, Y_MAX = -250, 250
Z_MIN, Z_MAX = 20, 680
X_MIN, X_MAX = 10, 350


# ------------------------------------------------------------
def main():

    parser = ArgumentParser()
    parser.add_argument("-f", required=True, help="Input reco ROOT file")
    parser.add_argument("--fiducial", action="store_true",
                        help="Apply fiducial cut")
    args = parser.parse_args()

    # ------------------------------------------------------------
    # Open event with gallery
    # ------------------------------------------------------------
    ev = ROOT.gallery.Event([args.f])
    ev.goToEntry(0)

    HitVec = ROOT.std.vector("recob::Hit")
    tag = ROOT.art.InputTag("gaushit")

    handle = ev.getValidHandle[HitVec](tag)
    hits = handle.product()

    print(f"Loaded {len(hits)} hits")

    # Storage arrays
    X, Y, Z, Q = [], [], [], []

    # ------------------------------------------------------------
    # Loop over hits
    # ------------------------------------------------------------
    for hit in hits:

        # ---- X from drift time
        peaktime = hit.PeakTime()
        x = peaktime * TICK_US * V_DRIFT

        # ---- ADC charge
        charge = hit.Integral()

        # ---- Get wire ID info
        wid = hit.WireID()
        plane = wid.Plane
        wire_number = wid.Wire

        # --------------------------------------------------------
        # Approximate geometry without larcore
        # --------------------------------------------------------
        z = wire_number * WIRE_PITCH
        y = 0.0  # simple approximation

        # ---- Fiducial cut (optional)
        if args.fiducial:
            if not (Y_MIN < y < Y_MAX): continue
            if not (Z_MIN < z < Z_MAX): continue
            if not (X_MIN < x < X_MAX): continue

        X.append(x)
        Y.append(y)
        Z.append(z)
        Q.append(charge)

    X = np.array(X)
    Y = np.array(Y)
    Z = np.array(Z)
    Q = np.array(Q)

    print(f"Selected {len(X)} hits after cuts")

    # ------------------------------------------------------------
    # X–Z scatter (ADC colored)
    # ------------------------------------------------------------
    plt.figure(figsize=(7,6))
    plt.scatter(X, Z, c=Q, s=2, cmap="viridis")
    plt.colorbar(label="ADC charge")
    plt.xlabel("X [cm]")
    plt.ylabel("Z [cm]")
    plt.title("ADC Charge: X vs Z")
    plt.tight_layout()
    plt.savefig("XZ_scatter.png")
    plt.close()

    # ------------------------------------------------------------
    # Z distribution
    # ------------------------------------------------------------
    plt.figure(figsize=(7,6))
    plt.hist(Z, bins=200)
    plt.xlabel("Z [cm]")
    plt.ylabel("Hit count")
    plt.title("Wire distribution")
    plt.tight_layout()
    plt.savefig("Z_hist.png")
    plt.close()

    # ------------------------------------------------------------
    # Log-density 2D histogram
    # ------------------------------------------------------------
    plt.figure(figsize=(7,6))
    plt.hist2d(X, Z, bins=200, norm=LogNorm())
    plt.colorbar(label="Hit count (log)")
    plt.xlabel("X [cm]")
    plt.ylabel("Z [cm]")
    plt.title("Hit Density: X vs Z (log)")
    plt.tight_layout()
    plt.savefig("XZ_hist2d.png")
    plt.close()


# ------------------------------------------------------------
if __name__ == "__main__":
    main()

