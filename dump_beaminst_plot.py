#run this code by using the command: python3 dump_beamins_plot.py -f np02_*_beam.root
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
    parser.add_argument('-f', type=str, required=True)
    parser.add_argument('--tag', type=str, default='beamevent')
    parser.add_argument('--out', type=str, default='output.root',
                        help='Output ROOT file with histograms')
    args = parser.parse_args()
    
    # Input file
    ev = RT.gallery.Event(RT.vector(RT.string)(1, args.f))
    nevents = ev.numberOfEventsInFile()
    get_prods = ev.getValidHandle[mcprodv]
    
    # Define histograms
    # Updated main title includes ProtoDUNE VD Run No: 39324
    h_tof = RT.TH1F("h_tof", "ProtoDUNE-VD Run No: 39324;TOF [ns];Events", 200, 0, 500)
    h_trigger = RT.TH1I("h_trigger", "Timing Trigger;Trigger value;Events", 10, 0, 10)
    h_ckov0 = RT.TH1I("h_ckov0", "High Pressure Ckov Status;Status;Events", 5, 0, 5)
    h_ckov1 = RT.TH1I("h_ckov1", "Low Pressure Ckov Status;Status;Events", 5, 0, 5)
    h_momenta = RT.TH1F("h_momenta", "Reco Beam Momenta;Momentum [GeV/c];Events", 100, 0, 10)

    for i in range(nevents):
        ev.goToEntry(i)
        prods = get_prods(RT.art.InputTag(args.tag))
        prod = prods.product()[0]

        tof = prod.GetTOF()
        trig = prod.GetTimingTrigger()
        ckov0 = prod.GetCKov0Status()
        ckov1 = prod.GetCKov1Status()
        momenta = prod.GetRecoBeamMomenta()

        # Fill histograms only if 0 < TOF < 500 ns
        if 0 < tof < 500:
            h_tof.Fill(tof)
            h_trigger.Fill(trig)
            h_ckov0.Fill(ckov0)
            h_ckov1.Fill(ckov1)
            for p in momenta:
                h_momenta.Fill(p)

            # (Optional) still print for checking
            momenta_str = [float(f'{p:.2f}') for p in momenta]
            print(f'Trigger:{trig}, TOF:{tof:.2f}, '
                  f'High Pres. CKov:{ckov0}, '
                  f'Low Pres. CKov:{ckov1}, '
                  f'Possible Momenta:{momenta_str}')

    # Save histograms to file
    fout = RT.TFile(args.out, "RECREATE")
    h_tof.Write()
    h_trigger.Write()
    h_ckov0.Write()
    h_ckov1.Write()
    h_momenta.Write()
    fout.Close()

    # Draw TOF histogram with stats box
    canvas = RT.TCanvas("c1", "TOF Plot", 800, 600)
    RT.gStyle.SetOptStat(1111)   # Show entries, mean, RMS
    h_tof.Draw()
    canvas.SaveAs("TOF_distribution.png")

    # Print TOF width (RMS)
    print(f"TOF distribution width (RMS): {h_tof.GetRMS():.2f} ns")
    print(f"Histograms saved in {args.out}, plot saved as TOF_distribution.png")
