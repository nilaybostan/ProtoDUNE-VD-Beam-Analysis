#!/usr/bin/env python3
# -*- coding: utf-8 -*-
#to run: chmod +x plot_rawdigits.py then ./plot_rawdigits.py -f input.root ya da mesela in apptainer use for running: python dump_detsim_stage1_np02.py -f np02vd_raw_run040014_0010_df-s04-d3_dw_0_20251011T181420_reco_stage1_20251011T195216_keepup.root
# 0-951,952-2*952 -1, 2*952-3072 for induction 1, induction 2, and collection
from argparse import ArgumentParser as ap
import ROOT as RT
from gallery_utils import read_header, provide_list
import numpy as np
import matplotlib.pyplot as plt


def main():
    # --------------------------------------------------
    # gallery setup
    # --------------------------------------------------
    read_header('gallery/ValidHandle.h')

    prodv = 'std::vector<raw::RawDigit>'
    provide_list([prodv])

    # --------------------------------------------------
    # argument parsing
    # --------------------------------------------------
    parser = ap(description="Plot RawDigit waveforms as an image")
    parser.add_argument('-f', type=str, required=True,
                        help='Input ROOT file')
    parser.add_argument('-n', type=int, default=-1,
                        help='Event number (currently unused)')
    parser.add_argument(
        '--tag',
        type=str,
        default='tpcrawdecoder:daq:pdvdkeepupstage1',
        help='InputTag for RawDigits'
    )
    parser.add_argument('--nticks', type=int, default=6000,
                        help='Number of ticks to read')

    args = parser.parse_args()

    # --------------------------------------------------
    # Event setup
    # --------------------------------------------------
    ev = RT.gallery.Event([args.f])
    get_prods = ev.getValidHandle[prodv]

    ev.goToEntry(0)

    # --------------------------------------------------
    # Get product
    # --------------------------------------------------
    prod = get_prods(RT.art.InputTag(args.tag))
    rawdigits = prod.product()

    print("Number of RawDigits:", len(rawdigits))

    # --------------------------------------------------
    # Build waveform array
    # --------------------------------------------------
    apa1_wfs = np.zeros((2560, args.nticks))

    for wf in rawdigits:
        channel = wf.Channel()
        if channel < 2560:
            apa1_wfs[channel] = np.array(wf.ADCs()[:args.nticks])

    img = apa1_wfs.T - np.mean(apa1_wfs, axis=1)

    print("img shape:", img.shape)

    # --------------------------------------------------
    # Plot
    # --------------------------------------------------
    plt.figure(figsize=(10, 6))
    plt.imshow(
        img,
        aspect='auto',
        vmin=-20,
        vmax=20,
        interpolation='nearest'
    )

    plt.gca().invert_yaxis()
    plt.ylabel('Time [tick]')
    plt.xlabel('Channel')
    plt.colorbar(label='ADC')
    plt.tight_layout()
    plt.show()


if __name__ == '__main__':
    main()
