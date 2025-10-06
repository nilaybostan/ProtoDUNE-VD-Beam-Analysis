# after merging the beam info root file from different runs, run this code by using this command to get the TOF values: 
# use this command: python3 dump_beaminst_compare.py -f merged_39252.root merged_39255.root merged_39273.root merged_39324.root --out comparison.root
from argparse import ArgumentParser as ap
import ROOT as RT
from gallery_utils import read_header, provide_list
import numpy as np

# Setup gallery and product definitions
read_header('gallery/ValidHandle.h')
mcprodv = 'std::vector<beam::ProtoDUNEBeamEvent>'
classes = [mcprodv]
provide_list(classes)

if __name__ == '__main__':
    parser = ap()
    parser.add_argument('-f', type=str, nargs='+', required=True,
                        help='List of input ROOT files')
    parser.add_argument('--tag', type=str, default='beamevent')
    parser.add_argument('--out', type=str, default='output.root',
                        help='Output ROOT file with histograms')
    args = parser.parse_args()

    colors = [RT.kRed, RT.kBlue, RT.kGreen+2, RT.kMagenta]  # up to 4 files
    hists = []

    # Loop over all input files
    for ifile, fname in enumerate(args.f):
        ev = RT.gallery.Event(RT.vector(RT.string)(1, fname))
        nevents = ev.numberOfEventsInFile()
        get_prods = ev.getValidHandle[mcprodv]

        # Create histogram per file
                # Create histogram per file
        run_number = fname.split('_')[-1].replace('.root','')  # e.g., 39252
        h_tof = RT.TH1F(f"h_tof_{run_number}",
                        f"TOF Distribution ProtoDUNE VD Runs;TOF [ns];Events",
                        200, 0, 150)
        h_tof.SetDirectory(0)   # prevent it being tied to input file


        for i in range(nevents):
            ev.goToEntry(i)
            prods = get_prods(RT.art.InputTag(args.tag))
            prod = prods.product()[0]

            tof = prod.GetTOF()
            if 0 < tof < 500:
                h_tof.Fill(tof)

        h_tof.SetLineColor(colors[ifile % len(colors)])
        h_tof.SetLineWidth(2)
        hists.append((h_tof, run_number))

        print(f"Run {run_number}: TOF RMS = {h_tof.GetRMS():.2f} ns")

    # Save histograms into output file
    fout = RT.TFile(args.out, "RECREATE")
    for h, rn in hists:
        h.Write()
    fout.Close()

    # Draw comparison plot
        # Draw comparison plot
    canvas = RT.TCanvas("c_comp", "TOF Comparison", 800, 600)
    RT.gStyle.SetOptStat(0)  # no stats box for overlay plots
    
    # Global title
    frame = canvas.DrawFrame(0, 0, 500, max(h.GetMaximum() for h, _ in hists)*1.2)
    frame.SetTitle("TOF Distribution ProtoDUNE VD Runs;TOF [ns];Events")

    # Legend position adjusted (left + lower than before)
    leg = RT.TLegend(0.65, 0.55, 0.90, 0.75)
    leg.SetBorderSize(0)

    for i, (h, rn) in enumerate(hists):
        drawopt = "HIST SAME" if i > 0 else "HIST"
        h.Draw(drawopt)
        leg.AddEntry(h, f"Run {rn}", "l")

    leg.Draw()
    canvas.SaveAs("TOF_comparison.png")


    print(f"Comparison plot saved as TOF_comparison.png, histograms saved in {args.out}")
