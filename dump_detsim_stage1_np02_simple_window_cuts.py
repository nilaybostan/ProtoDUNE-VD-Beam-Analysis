from argparse import ArgumentParser
import ROOT
import numpy as np
import matplotlib.pyplot as plt

from gallery_utils import read_header, provide_list

# ------------------------------------------------------------
# Constants
# ------------------------------------------------------------
NTICKS = 8000
TICK_NS = 512

# Simple Window parameters
WINDOW_TIME_MS = 0.1
WINDOW_TICKS = int((WINDOW_TIME_MS * 1e-3) / (TICK_NS * 1e-9))

ENERGY_THRESHOLD = 8e6
CHANNEL_FRAC_THRESHOLD = 0.99
CHANNEL_ACTIVITY_THRESHOLD = 1.0  # ADC

# Plot scales
IND_VMIN, IND_VMAX = -40, 40
COL_VMIN, COL_VMAX = 0, 100

CMAP_IND = "RdBu_r"
CMAP_COL = "viridis"

# Channel ranges
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
        origin="lower",
        vmin=vmin,
        vmax=vmax,
        cmap=cmap,
        extent=[ch_range[0], ch_range[1], 0, NTICKS],
    )
    ax.set_title(title)
    ax.set_xlabel("Channel Number")
    ax.set_ylabel("Time Ticks (512 ns / tick)")
    return img

# ------------------------------------------------------------
def pass_simple_window(wfs_window):
    """
    wfs_window shape: (nchan, nticks)
    """
    energy = np.sum(np.abs(wfs_window))
    if energy < ENERGY_THRESHOLD:
        return False

    active_channels = np.sum(
        np.max(np.abs(wfs_window), axis=1) > CHANNEL_ACTIVITY_THRESHOLD
    )
    frac = active_channels / wfs_window.shape[0]

    return frac > CHANNEL_FRAC_THRESHOLD

# ------------------------------------------------------------
def apply_simple_window_cut(wfs, ch_range):
    ch0, ch1 = ch_range
    mask = np.zeros(NTICKS, dtype=bool)

    for t0 in range(0, NTICKS - WINDOW_TICKS):
        window = wfs[ch0:ch1, t0:t0 + WINDOW_TICKS]
        if pass_simple_window(window):
            mask[t0:t0 + WINDOW_TICKS] = True

    return mask

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

    handle = ev.getValidHandle[prodv](ROOT.art.InputTag(args.tag))
    rawdigits = handle.product()

    nchan = len(rawdigits)
    print("Number of RawDigits:", nchan)

    # ------------------------------------------------------------
    # Fill waveform array
    wfs = np.zeros((nchan, NTICKS), dtype=np.float32)

    for rd in rawdigits:
        ch = rd.Channel()
        if ch >= nchan:
            continue

        adc = np.array(rd.ADCs(), dtype=np.float32)
        adc -= rd.GetPedestal()

        nt = min(len(adc), NTICKS)
        wfs[ch, :nt] = adc[:nt]

    # ------------------------------------------------------------
    # Apply Simple Window cuts (Collected planes)
    mask_crp4z = apply_simple_window_cut(wfs, CRP4_Z)
    mask_crp5z = apply_simple_window_cut(wfs, CRP5_Z)

    combined_mask = mask_crp4z | mask_crp5z

    wfs_cut = wfs.copy()
    wfs_cut[:, ~combined_mask] = 0.0

    # ------------------------------------------------------------
    # Plot
    fig, axes = plt.subplots(2, 3, figsize=(18, 10))
    fig.suptitle(
        "CRP4+5 Event Display (Simple Window Cuts Applied)",
        fontsize=16,
    )

    im1 = plot_view(
        axes[0, 0], wfs_cut, CRP4_U,
        "CRP4 - View 0 / U (Induced)",
        IND_VMIN, IND_VMAX, CMAP_IND
    )
    im2 = plot_view(
        axes[0, 1], wfs_cut, CRP4_V,
        "CRP4 - View 1 / V (Induced)",
        IND_VMIN, IND_VMAX, CMAP_IND
    )
    im3 = plot_view(
        axes[0, 2], wfs_cut, CRP4_Z,
        "CRP4 - View 2 / Z (Collected)",
        COL_VMIN, COL_VMAX, CMAP_COL
    )

    im4 = plot_view(
        axes[1, 0], wfs_cut, CRP5_U,
        "CRP5 - View 0 / U (Induced)",
        IND_VMIN, IND_VMAX, CMAP_IND
    )
    im5 = plot_view(
        axes[1, 1], wfs_cut, CRP5_V,
        "CRP5 - View 1 / V (Induced)",
        IND_VMIN, IND_VMAX, CMAP_IND
    )
    im6 = plot_view(
        axes[1, 2], wfs_cut, CRP5_Z,
        "CRP5 - View 2 / Z (Collected)",
        COL_VMIN, COL_VMAX, CMAP_COL
    )

    for ax, im in zip(
        [axes[0, 0], axes[0, 1], axes[1, 0], axes[1, 1]],
        [im1, im2, im4, im5],
    ):
        plt.colorbar(im, ax=ax, label="ADC", pad=0.04)

    plt.colorbar(im3, ax=axes[0, 2], label="ADC", pad=0.04)
    plt.colorbar(im6, ax=axes[1, 2], label="ADC", pad=0.04)

    plt.subplots_adjust(
        left=0.06,
        right=0.96,
        top=0.90,
        bottom=0.07,
        wspace=0.30,
        hspace=0.30,
    )

    plt.show()

# ------------------------------------------------------------
if __name__ == "__main__":
    main()
