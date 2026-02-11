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

#how to run: root -l -q 'PDVD_ClusterPlot_last.C+("output_singlehist_neutrino_stage1.root")'

using namespace std;

void PDVD_ClusterPlot_last(const char* filename)
{
    gROOT->SetBatch(kTRUE);
    gSystem->mkdir("plots", kTRUE);

    gStyle->SetPalette(1);   // gPalette1
    gStyle->SetOptStat(0);

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
        d->ls();
        return;
    }

    cout << "Total entries: " << t->GetEntries() << endl;

    // =====================
    // Branches
    // =====================
    vector<float> *Y = nullptr;
    vector<float> *Z = nullptr;
    vector<float> *PeakTime = nullptr;

    t->SetBranchAddress("Y", &Y);
    t->SetBranchAddress("Z", &Z);
    t->SetBranchAddress("PeakTime", &PeakTime);

    // =====================
    // Drift parameters
    // =====================
    const double tick_us = 0.5;
    const double v_drift = 0.16;

    // =====================
    // 2D Histograms
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
    // Profiles
    // =====================
    TProfile *pYvsPT = new TProfile("pYvsPT",
        "Mean Y vs PeakTime;PeakTime (ticks);<Y> (cm)",
        100,0,6000);

    TProfile *pYvsPT_Anode = new TProfile("pYvsPT_Anode",
        "Mean Y vs PeakTime (Anode);PeakTime (ticks);<Y> (cm)",
        100,0,6000);

    TProfile *pYvsPT_Cathode = new TProfile("pYvsPT_Cathode",
        "Mean Y vs PeakTime (Cathode);PeakTime (ticks);<Y> (cm)",
        100,0,6000);

    const double midX = 300.0;

    // =====================
    // Event loop
    // =====================
    Long64_t nentries = t->GetEntries();

    for (Long64_t i = 0; i < nentries; i++) {

        t->GetEntry(i);
        if (!Y || !Z || !PeakTime) continue;

        for (size_t j = 0; j < PeakTime->size(); j++) {

            double y  = Y->at(j);
            double z  = Z->at(j);
            double pt = PeakTime->at(j);

            double X = pt * tick_us * v_drift;

            hYX->Fill(X, y);
            hXZ->Fill(z, X);
            hYZ->Fill(z, y);
            hYPT->Fill(pt, y);
            hXPT->Fill(pt, X);

            pYvsPT->Fill(pt, y);

            if (X < midX)
                pYvsPT_Anode->Fill(pt, y);
            else
                pYvsPT_Cathode->Fill(pt, y);
        }
    }

    // =====================
    // Draw & Save (Log Z)
    // =====================

    TCanvas *c1 = new TCanvas();
    c1->SetLogz();
    hYX->SetMimimum(0);
    hYX->Draw("COLZ");
    c1->SaveAs("plots/Y_vs_X.png");

    TCanvas *c2 = new TCanvas();
    c2->SetLogz();
    hXZ->Draw("COLZ");
    c2->SaveAs("plots/X_vs_Z.png");

    TCanvas *c3 = new TCanvas();
    c3->SetLogz();
    hYZ->Draw("COLZ");
    c3->SaveAs("plots/Y_vs_Z.png");

    TCanvas *c4 = new TCanvas();
    c4->SetLogz();
    hYPT->Draw("COLZ");
    c4->SaveAs("plots/Y_vs_PeakTime.png");

    TCanvas *c5 = new TCanvas();
    c5->SetLogz();
    hXPT->Draw("COLZ");
    c5->SaveAs("plots/X_vs_PeakTime.png");

    // ===== Profile plots (no log) =====
    TCanvas *c6 = new TCanvas();
    pYvsPT->Draw();
    c6->SaveAs("plots/Profile_Y_vs_PeakTime.png");

    TCanvas *c7 = new TCanvas();
    pYvsPT_Anode->SetLineColor(kBlue);
    pYvsPT_Cathode->SetLineColor(kGreen+2);

    pYvsPT_Anode->Draw();
    pYvsPT_Cathode->Draw("SAME");

    TLegend *leg = new TLegend(0.6,0.75,0.88,0.88);
    leg->AddEntry(pYvsPT_Anode,"Anode side","l");
    leg->AddEntry(pYvsPT_Cathode,"Cathode side","l");
    leg->Draw();

    c7->SaveAs("plots/Profile_Y_vs_PeakTime_AnodeCathode.png");

    cout << "\nAll plots saved successfully (log scale for 2D).\n";
}

