#to run: chmod +x plot_rawdigits.py then ./plot_rawdigits.py -f input.root ya da 
#mesela in apptainer use for running: python  evt_plot_rawdigits.py -f np02vd_raw_run040014_0010_df-s04-d3_dw_0_20251011T181420_reco_stage1_20251011T195216_keepup.root
# 0-951,952-2*952 -1, 2*952-3072 for induction 1, induction 2, and collection
# Please copy also the gallery_utils.py in this folder, then save this code and gallery_utils.py together to this folder, and run this code.
#!/usr/bin/env python3

from argparse import ArgumentParser
import ROOT
import numpy as np
import matplotlib.pyplot as plt

from gallery_utils import read_header, provide_list

# ------------------------------------------------------------
# Constants
# ------------------------------------------------------------
NTICKS = 8000

IND_VMIN, IND_VMAX = -40, 40
COL_VMIN, COL_VMAX = 0, 100

CMAP_IND = "RdBu_r"
CMAP_COL = "viridis"

CRP4_U = (0, 1000)
CRP4_V = (1000, 2000)
CRP4_Z = (2000, 3000)

CRP5_U = (3000, 4000)
CRP5_V = (4000, 5000)
CRP5_Z = (5000, 6200)

# ------------------------------------------------------------
def plot_view(ax, wfs, ch_range, title, vmin, vmax, cmap):
    img = ax.imshow(
        wfs[ch_range[0]:ch_range[1], :].T,
        aspect="auto",
        origin="lower",                     # <-- FIX
        vmin=vmin,
        vmax=vmax,
        cmap=cmap,
        extent=[ch_range[0], ch_range[1], 0, NTICKS],  # <-- FIX
    )
    ax.set_title(title)
    ax.set_xlabel("Channel Number")
    ax.set_ylabel("Time Ticks (512 ns / tick)")
    return img


# ------------------------------------------------------------
def main():
    read_header("gallery/ValidHandle.h")

    prodv = "std::vector<raw::RawDigit>"
    provide_list([prodv])

    parser = ArgumentParser()
    parser.add_argument("-f", required=True, help="Input ROOT file")
    parser.add_argument(
        "--tag",
        default="tpcrawdecoder:daq:pdvdkeepupstage1",
        help="RawDigit InputTag",
    )
    args = parser.parse_args()

    ev = ROOT.gallery.Event([args.f])
    ev.goToEntry(0)

    get_handle = ev.getValidHandle[prodv]
    handle = get_handle(ROOT.art.InputTag(args.tag))
    rawdigits = handle.product()

    nchan = len(rawdigits)
    print("Number of RawDigits:", nchan)

    wfs = np.zeros((nchan, NTICKS), dtype=np.float32)

    for rd in rawdigits:
        ch = rd.Channel()
        if ch >= nchan:
            continue

        adc = np.array(rd.ADCs(), dtype=np.float32)
        adc -= rd.GetPedestal()

        nt = min(len(adc), NTICKS)
        wfs[ch, :nt] = adc[:nt]

    fig, axes = plt.subplots(2, 3, figsize=(18, 10))

    fig.suptitle(
        "CRP4+5 Event Display: Induced and Collected Charge Views",
        fontsize=16,
    )

    im1 = plot_view(axes[0,0], wfs, CRP4_U, "CRP4 - View 0 / U (Induced)",
                    IND_VMIN, IND_VMAX, CMAP_IND)
    im2 = plot_view(axes[0,1], wfs, CRP4_V, "CRP4 - View 1 / V (Induced)",
                    IND_VMIN, IND_VMAX, CMAP_IND)
    im3 = plot_view(axes[0,2], wfs, CRP4_Z, "CRP4 - View 2 / Z (Collected)",
                    COL_VMIN, COL_VMAX, CMAP_COL)

    im4 = plot_view(axes[1,0], wfs, CRP5_U, "CRP5 - View 0 / U (Induced)",
                    IND_VMIN, IND_VMAX, CMAP_IND)
    im5 = plot_view(axes[1,1], wfs, CRP5_V, "CRP5 - View 1 / V (Induced)",
                    IND_VMIN, IND_VMAX, CMAP_IND)
    im6 = plot_view(axes[1,2], wfs, CRP5_Z, "CRP5 - View 2 / Z (Collected)",
                    COL_VMIN, COL_VMAX, CMAP_COL)

    for ax, im in zip(
        [axes[0,0], axes[0,1], axes[1,0], axes[1,1]],
        [im1, im2, im4, im5],
    ):
        plt.colorbar(im, ax=ax, label="ADC", pad=0.04)

    plt.colorbar(im3, ax=axes[0,2], label="ADC", pad=0.04)
    plt.colorbar(im6, ax=axes[1,2], label="ADC", pad=0.04)

    plt.subplots_adjust(
        left=0.06,
        right=0.96,
        top=0.90,
        bottom=0.07,
        wspace=0.30,
        hspace=0.30,
    )

    for ax in axes.flatten():
        ax.tick_params(axis="x", pad=8)

    plt.show()


if __name__ == "__main__":
    main()

