#include <iostream>
#include <cmath>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TProfile.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TSystem.h>
#include <TROOT.h>
#include <TStyle.h>

using namespace std;

void PDVD_ClusterPlot_last_withEnergy(const char* filename)
{
    gROOT->SetBatch(kTRUE);
    gSystem->mkdir("plots", kTRUE);

    // =====================
    // Style
    // =====================
    gStyle->SetPalette(1);        // gPalette1 (ROOT default)
    gStyle->SetOptStat(0);
    gStyle->SetNumberContours(100);

    // =====================
    // Open file
    // =====================
    TFile *f = TFile::Open(filename, "READ");
    if (!f || f->IsZombie()) {
        cout << "Cannot open file!" << endl;
        return;
    }

    TDirectory *d = (TDirectory*)f->Get("ana");
    if (!d) {
        cout << "Directory 'ana' not found!" << endl;
        return;
    }

    TTree *t = (TTree*)d->Get("ClusterTree");
    if (!t) {
        cout << "ClusterTree not found!" << endl;
        return;
    }

    cout << "Total entries: " << t->GetEntries() << endl;

    // =====================
    // Branches
    // =====================
    vector<float> *Y = nullptr;
    vector<float> *Z = nullptr;
    vector<float> *PeakTime = nullptr;

    vector<float> *EnergyCollection = nullptr;
    vector<float> *EnergyPlane0     = nullptr;
    vector<float> *EnergyPlane1     = nullptr;

    t->SetBranchAddress("Y", &Y);
    t->SetBranchAddress("Z", &Z);
    t->SetBranchAddress("PeakTime", &PeakTime);
    t->SetBranchAddress("EnergyCollection", &EnergyCollection);
    t->SetBranchAddress("EnergyPlane0", &EnergyPlane0);
    t->SetBranchAddress("EnergyPlane1", &EnergyPlane1);

    // =====================
    // Drift parameters
    // =====================
    const double tick_us = 0.5;
    const double v_drift = 0.16;

    // =====================
    // Geometry histograms
    // =====================
    TH2F *hYX = new TH2F("hYX","Y vs X;X (cm);Y (cm)",
                         120,0,600,120,-600,600);

    TH2F *hXZ = new TH2F("hXZ","X vs Z;Z (cm);X (cm)",
                         120,-500,500,120,0,600);

    TH2F *hYZ = new TH2F("hYZ","Y vs Z;Z (cm);Y (cm)",
                         120,-500,500,120,-600,600);

    TH2F *hYPT = new TH2F("hYPT","Y vs PeakTime;PeakTime (ticks);Y (cm)",
                          120,0,6000,120,-600,600);

    TH2F *hXPT = new TH2F("hXPT","X vs PeakTime;PeakTime (ticks);X (cm)",
                          120,0,6000,120,0,600);

    // =====================
    // Logarithmic Energy Binning
    // =====================
    const int nEBins = 120;
    const double eMin = 0.1;
    const double eMax = 10000.0;

    double eBins[nEBins+1];
    double logemin = log10(eMin);
    double logemax = log10(eMax);

    for (int i = 0; i <= nEBins; ++i) {
        eBins[i] = pow(10.0, logemin + i*(logemax-logemin)/nEBins);
    }

    // =====================
    // Energy vs PeakTime
    // =====================
    TH2F *hEColPT = new TH2F("hEColPT",
        "EnergyCollection vs PeakTime;PeakTime (ticks);Energy [MeV]",
        240, 0, 6000,
        nEBins, eBins);

    TH2F *hEPlane0PT = new TH2F("hEPlane0PT",
        "EnergyPlane0 vs PeakTime;PeakTime (ticks);Energy [MeV]",
        240, 0, 6000,
        nEBins, eBins);

    TH2F *hEPlane1PT = new TH2F("hEPlane1PT",
        "EnergyPlane1 vs PeakTime;PeakTime (ticks);Energy [MeV]",
        240, 0, 6000,
        nEBins, eBins);

    // =====================
    // Profile
    // =====================
    TProfile *pYvsPT = new TProfile("pYvsPT",
        "Mean Y vs PeakTime;PeakTime (ticks);<Y> (cm)",
        100,0,6000);

    // =====================
    // Event loop
    // =====================
    Long64_t nentries = t->GetEntries();

    for (Long64_t i = 0; i < nentries; i++) {
        t->GetEntry(i);

        for (size_t j = 0; j < PeakTime->size(); j++) {

            double y  = Y->at(j);
            double z  = Z->at(j);
            double pt = PeakTime->at(j);
            double X  = pt * tick_us * v_drift;

            hYX->Fill(X, y);
            hXZ->Fill(z, X);
            hYZ->Fill(z, y);
            hYPT->Fill(pt, y);
            hXPT->Fill(pt, X);
            pYvsPT->Fill(pt, y);

            if (EnergyCollection && j < EnergyCollection->size()) {
                double e = EnergyCollection->at(j);
                if (e > 0) hEColPT->Fill(pt, e);
            }
            if (EnergyPlane0 && j < EnergyPlane0->size()) {
                double e = EnergyPlane0->at(j);
                if (e > 0) hEPlane0PT->Fill(pt, e);
            }
            if (EnergyPlane1 && j < EnergyPlane1->size()) {
                double e = EnergyPlane1->at(j);
                if (e > 0) hEPlane1PT->Fill(pt, e);
            }
        }
    }

    // =====================
    // Mild smoothing
    // =====================
    hEColPT->Smooth(1);
    hEPlane0PT->Smooth(1);
    hEPlane1PT->Smooth(1);

    // =====================
    // Draw helper
    // =====================
    auto draw2D = [](TH2F* h, const char* out, bool logy=false) {
        TCanvas *c = new TCanvas();
        c->SetLogz();
        if (logy) c->SetLogy();
        h->Draw("COLZ");
        c->SaveAs(out);
        delete c;
    };

    draw2D(hYX, "plots/Y_vs_X.png");
    draw2D(hXZ, "plots/X_vs_Z.png");
    draw2D(hYZ, "plots/Y_vs_Z.png");
    draw2D(hYPT,"plots/Y_vs_PeakTime.png");
    draw2D(hXPT,"plots/X_vs_PeakTime.png");

    draw2D(hEColPT,   "plots/EnergyCollection_vs_PeakTime.png", true);
    draw2D(hEPlane0PT,"plots/EnergyPlane0_vs_PeakTime.png", true);
    draw2D(hEPlane1PT,"plots/EnergyPlane1_vs_PeakTime.png", true);

    TCanvas *cProf = new TCanvas();
    pYvsPT->Draw();
    cProf->SaveAs("plots/Profile_Y_vs_PeakTime.png");

    cout << "\nAll plots saved successfully (gPalette1 + log binning).\n";
}
