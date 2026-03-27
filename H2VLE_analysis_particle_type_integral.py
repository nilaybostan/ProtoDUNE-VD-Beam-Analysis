import os
import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import glob

# ---------------- USER SETTINGS ----------------
datasets = {
    "0.5 GeV": {
        "pattern_main": "/pnfs/dune/scratch/users/nbostan/g4beamline_prod/H2main/05GeV_analysis_new/fnal/14769/1/001/*.npz",
        "pattern_custom": "/pnfs/dune/scratch/users/nbostan/g4beamline_prod/H2custom/05GeV_analysis_new/fnal/14767/1/001/*.npz",
        "momentum_range": (0.0, 3.0)
    },
    "8 GeV": {
        "pattern_main": "/pnfs/dune/scratch/users/nbostan/g4beamline_prod/H2main/8GeV_analysis_new/fnal/14768/1/001/*.npz",
        "pattern_custom": "/pnfs/dune/scratch/users/nbostan/g4beamline_prod/H2custom/8GeV_analysis_new/fnal/14770/1/001/*.npz",
        "momentum_range": (6.0, 9.0)
    }
}

particle_map = {
    -11: "e+",
     11: "e-",
    -13: "mu+",
     13: "mu-",
    211: "pi+",
   -211: "pi-",
    321: "K+",
   -321: "K-",
   2212: "p",
  -2212: "pbar"
}

colors = {"main":"navy", "custom":"tab:red"}
bins_momentum = 60
bins_xy = 50
POINT_SIZE = 8
ALPHA_MAIN = 0.25
ALPHA_CUSTOM = 0.8
ALPHA_HIST = 0.7

# ---------------- Output folder ----------------
output_dir = "integral_plots"
os.makedirs(output_dir, exist_ok=True)

# ---------------- Data Storage ----------------
def init_storage():
    return {pdg: {"mom":[], "x":[], "y":[]} for pdg in particle_map}

# ---------------- Fast Loader ----------------
def load_dataset(pattern, storage):
    files = sorted(glob.glob(pattern))
    for file_path in files:
        t = np.load(file_path, allow_pickle=True)
        for key, event in t.items():
            for particle in event:
                if particle[2] != "primary":
                    continue
                pdg = int(particle[1])
                if pdg not in storage:
                    continue
                px, py, pz = float(particle[4]), float(particle[5]), float(particle[6])
                storage[pdg]["mom"].append(np.sqrt(px**2 + py**2 + pz**2))
                storage[pdg]["x"].append(float(particle[7]))
                storage[pdg]["y"].append(float(particle[8]))

# ---------------- Main Loop ----------------
for energy, info in datasets.items():
    print(f"\nProcessing {energy} dataset...")

    main_data = init_storage()
    custom_data = init_storage()

    print("Loading MAIN dataset...")
    load_dataset(info["pattern_main"], main_data)
    print("Loading CUSTOM dataset...")
    load_dataset(info["pattern_custom"], custom_data)

    # convert lists → numpy arrays
    for pdg in particle_map:
        for key in ["mom","x","y"]:
            main_data[pdg][key] = np.array(main_data[pdg][key])
            custom_data[pdg][key] = np.array(custom_data[pdg][key])

    for pdg_code, name in particle_map.items():
        momentum_main = main_data[pdg_code]["mom"]
        momentum_custom = custom_data[pdg_code]["mom"]
        x_main = main_data[pdg_code]["x"]
        y_main = main_data[pdg_code]["y"]
        x_custom = custom_data[pdg_code]["x"]
        y_custom = custom_data[pdg_code]["y"]

        if (len(momentum_main)+len(momentum_custom)==0) and (len(x_main)+len(x_custom)==0):
            print(f"No data for {name}, skipping all plots")
            continue

        rng = np.random.default_rng(seed=42)
        if len(x_main)>0 and len(x_custom)>0:
            N = min(len(x_main), len(x_custom))
            idx_main = rng.choice(len(x_main), N, replace=False)
            idx_custom = rng.choice(len(x_custom), N, replace=False)
            x_main, y_main = x_main[idx_main], y_main[idx_main]
            x_custom, y_custom = x_custom[idx_custom], y_custom[idx_custom]

        fig, axes = plt.subplots(2,2,figsize=(12,10))
        ax_xy, ax_x, ax_y, ax_mom = axes[0,0], axes[0,1], axes[1,0], axes[1,1]

        # ---------- XY scatter ----------
        handles=[]
        if len(x_main)>0:
            handles.append(ax_xy.scatter(x_main, y_main, s=POINT_SIZE, alpha=ALPHA_MAIN,
                                         color=colors['main'], marker="o", label="Main"))
        if len(x_custom)>0:
            handles.append(ax_xy.scatter(x_custom, y_custom, s=POINT_SIZE, alpha=ALPHA_CUSTOM,
                                         facecolors="none", edgecolors=colors['custom'],
                                         linewidths=0.8, marker="o", label="Custom"))
        ax_xy.set_xlabel("Vertex X [cm]")
        ax_xy.set_ylabel("Vertex Y [cm]")
        ax_xy.set_title(f"Primary Vertex Positions: {name} ({energy})")
        if handles:
            ax_xy.legend()
        ax_xy.grid(True)

        # ---------- X histogram ----------
        x_all = np.concatenate([x_main, x_custom])
        if len(x_all)>0 and x_all.min()!=x_all.max():
            bins_x = np.linspace(x_all.min(), x_all.max(), bins_xy)
            if len(x_main)>0:
                ax_x.hist(x_main, bins=bins_x, density=True, alpha=ALPHA_HIST,
                          color=colors['main'], edgecolor="black", label="Main")
            if len(x_custom)>0:
                ax_x.hist(x_custom, bins=bins_x, density=True, alpha=ALPHA_HIST,
                          color=colors['custom'], edgecolor="black", label="Custom")
            ax_x.legend()
        else:
            print(f"Warning: X positions are degenerate for {name}, skipping X histogram")

        ax_x.set_xlabel("Vertex X [cm]")
        ax_x.set_ylabel("Normalized Counts")
        ax_x.set_title("X Projection (Normalized)")

        # ---------- Y histogram ----------
        y_all = np.concatenate([y_main, y_custom])
        if len(y_all)>0 and y_all.min()!=y_all.max():
            bins_y = np.linspace(y_all.min(), y_all.max(), bins_xy)
            if len(y_main)>0:
                ax_y.hist(y_main, bins=bins_y, density=True, alpha=ALPHA_HIST,
                          color=colors['main'], edgecolor="black", label="Main")
            if len(y_custom)>0:
                ax_y.hist(y_custom, bins=bins_y, density=True, alpha=ALPHA_HIST,
                          color=colors['custom'], edgecolor="black", label="Custom")
            ax_y.legend()
        else:
            print(f"Warning: Y positions are degenerate for {name}, skipping Y histogram")

        ax_y.set_xlabel("Vertex Y [cm]")
        ax_y.set_ylabel("Normalized Counts")
        ax_y.set_title("Y Projection (Normalized)")

        # ---------- Momentum histogram ----------
        mom_all = np.concatenate([momentum_main, momentum_custom])
        if len(mom_all)>0 and mom_all.min()!=mom_all.max():
            if len(momentum_main)>0:
                ax_mom.hist(momentum_main, bins=bins_momentum, range=info["momentum_range"],
                            density=True, histtype='stepfilled', color=colors['main'],
                            alpha=ALPHA_HIST, edgecolor="black", linewidth=1, label="Main")
            if len(momentum_custom)>0:
                ax_mom.hist(momentum_custom, bins=bins_momentum, range=info["momentum_range"],
                            density=True, histtype='stepfilled', color=colors['custom'],
                            alpha=ALPHA_HIST, edgecolor="black", linewidth=1, label="Custom")
            ax_mom.legend()
        else:
            print(f"Warning: Momentum values are degenerate for {name}, skipping momentum histogram")

        ax_mom.set_xlabel("Momentum [GeV/c]")
        ax_mom.set_ylabel("Normalized Counts")
        ax_mom.set_title("Momentum Histogram (Normalized)")

        plt.tight_layout()
        save_path = os.path.join(output_dir, f"particle_overview_{name}_{energy}_normalized.png")
        plt.savefig(save_path, dpi=300)
        plt.close()
        print(f"Saved overview plot: {save_path}")
