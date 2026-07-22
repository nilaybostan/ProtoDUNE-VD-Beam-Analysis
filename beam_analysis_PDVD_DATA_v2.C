#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TStyle.h>
#include <TLegend.h>
#include <TPad.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

void beam_analysis_PDVD_DATA_v2()
{
    //==================================================
    // 1. Input
    //==================================================

    const char* inputPath =
        "/pnfs/dune/scratch/users/ykermaid/fnal/18125/1/001/"
        "*_beamselection.root";

    TChain chain("beamselection/tree");

    int nFiles = chain.Add(inputPath);

    std::cout << "Number of files added: "
              << nFiles << std::endl;

    Long64_t nEntries = chain.GetEntries();

    std::cout << "Total entries: "
              << nEntries << std::endl;

    if (nEntries == 0)
    {
        std::cerr << "No entries found." << std::endl;
        return;
    }


    //==================================================
    // 2. Cut and fit values
    //==================================================

    const double beamPosMin = -50.0;
    const double beamPosMax =  50.0;

    const double nominalMomentum = 1.0; // GeV/c

    const double momentumFitMin = 0.85;
    const double momentumFitMax = 1.15;

    const double resolutionFitMin = -0.15;
    const double resolutionFitMax =  0.15;


    //==================================================
    // 3. Variables
    //==================================================

    UInt_t eventID;

    Float_t  BeamTOF;
    Short_t  BeamCKov0;
    Short_t  BeamCKov1;
    Double_t BeamMomentum;

    std::vector<double>* BeamPositions = nullptr;

    Int_t PrimaryKey;
    Int_t PrimaryPdg;

    Int_t NPFParticles;
    Int_t NPrimaryChildren;

    Float_t PrimBeamPFPCNNtrack;
    Float_t PrimBeamPFPCNNshower;
    Float_t PrimBeamPFPCNNmichel;

    Int_t   PrimBeamPFPTrackPid;
    Float_t PrimBeamPFPTrackScore;
    Float_t PrimBeamPFPTrackLength;

    Float_t PrimBeamPFPTrackVertexX;
    Float_t PrimBeamPFPTrackVertexY;
    Float_t PrimBeamPFPTrackVertexZ;

    Float_t PrimBeamPFPTrackStartX;
    Float_t PrimBeamPFPTrackStartY;
    Float_t PrimBeamPFPTrackStartZ;

    Float_t PrimBeamPFPTrackEndX;
    Float_t PrimBeamPFPTrackEndY;
    Float_t PrimBeamPFPTrackEndZ;

    std::vector<int>* ChildBeamPFPPdg = nullptr;
    std::vector<float>* ChildBeamPFPTrackScore = nullptr;
    std::vector<float>* ChildBeamPFPTrackLength = nullptr;

   std::vector<int>* ChildBeamPFPKey = nullptr;

   std::vector<float>* ChildBeamPFPTrackVertexX = nullptr; 
   std::vector<float>* ChildBeamPFPTrackVertexY = nullptr;
   std::vector<float>* ChildBeamPFPTrackVertexZ = nullptr;

    //==================================================
    // 4. Branch addresses
    //==================================================

    chain.SetBranchAddress("eventID", &eventID);

    chain.SetBranchAddress("BeamTOF", &BeamTOF);
    chain.SetBranchAddress("BeamCKov0", &BeamCKov0);
    chain.SetBranchAddress("BeamCKov1", &BeamCKov1);
    chain.SetBranchAddress("BeamMomentum", &BeamMomentum);

    chain.SetBranchAddress("BeamPositions", &BeamPositions);

    chain.SetBranchAddress("PrimaryKey", &PrimaryKey);
    chain.SetBranchAddress("PrimaryPdg", &PrimaryPdg);

    chain.SetBranchAddress("NPFParticles", &NPFParticles);
    chain.SetBranchAddress("NPrimaryChildren", &NPrimaryChildren);

    chain.SetBranchAddress(
        "PrimBeamPFPCNNtrack",
        &PrimBeamPFPCNNtrack
    );

    chain.SetBranchAddress(
        "PrimBeamPFPCNNshower",
        &PrimBeamPFPCNNshower
    );

    chain.SetBranchAddress(
        "PrimBeamPFPCNNmichel",
        &PrimBeamPFPCNNmichel
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackPid",
        &PrimBeamPFPTrackPid
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackScore",
        &PrimBeamPFPTrackScore
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackLength",
        &PrimBeamPFPTrackLength
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackVertexX",
        &PrimBeamPFPTrackVertexX
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackVertexY",
        &PrimBeamPFPTrackVertexY
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackVertexZ",
        &PrimBeamPFPTrackVertexZ
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackStartX",
        &PrimBeamPFPTrackStartX
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackStartY",
        &PrimBeamPFPTrackStartY
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackStartZ",
        &PrimBeamPFPTrackStartZ
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackEndX",
        &PrimBeamPFPTrackEndX
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackEndY",
        &PrimBeamPFPTrackEndY
    );

    chain.SetBranchAddress(
        "PrimBeamPFPTrackEndZ",
        &PrimBeamPFPTrackEndZ
    );

    chain.SetBranchAddress(
        "ChildBeamPFPPdg",
        &ChildBeamPFPPdg
    );

    chain.SetBranchAddress(
        "ChildBeamPFPTrackScore",
        &ChildBeamPFPTrackScore
    );

    chain.SetBranchAddress(
        "ChildBeamPFPTrackLength",
        &ChildBeamPFPTrackLength
    );

    chain.SetBranchAddress(
    "ChildBeamPFPKey",
    &ChildBeamPFPKey
);

chain.SetBranchAddress(
    "ChildBeamPFPTrackVertexX",
    &ChildBeamPFPTrackVertexX
);

chain.SetBranchAddress(
    "ChildBeamPFPTrackVertexY",
    &ChildBeamPFPTrackVertexY
);

chain.SetBranchAddress(
    "ChildBeamPFPTrackVertexZ",
    &ChildBeamPFPTrackVertexZ
);
    //==================================================
    // 5. Beam histograms
    //==================================================

    TH1F* hBeamMomentum =
        new TH1F(
            "hBeamMomentum",
            "Beam Momentum After All Cuts;"
            "Momentum [GeV/c];Events",
            200, 0, 10
        );

TH1F *hFiberIndexUpX =
new TH1F(
    "hFiberIndexUpX",
    "Upstream X Active Fiber Index;Fiber Index;Entries",
    196,0,196);

TH1F *hFiberIndexUpY =
new TH1F(
    "hFiberIndexUpY",
    "Upstream Y Active Fiber Index;Fiber Index;Entries",
    196,0,196);

TH1F *hFiberIndexDownX =
new TH1F(
    "hFiberIndexDownX",
    "Downstream X Active Fiber Index;Fiber Index;Entries",
    196,0,196);

TH1F *hFiberIndexDownY =
new TH1F(
    "hFiberIndexDownY",
    "Downstream Y Active Fiber Index;Fiber Index;Entries",
    196,0,196);
	

    //==================================================
    // Momentum resolution
    //==================================================

    TH1F* hDeltaPOverP =
        new TH1F(
            "hDeltaPOverP",
            "1 GeV/c;"
            "(P_{rec}-P)/P;"
            "Events",
            100, -1.0, 1.0
        );

    hDeltaPOverP->SetLineColor(kBlue);
    hDeltaPOverP->SetLineWidth(2);


    TH1F* hBeamTOF =
        new TH1F(
            "hBeamTOF",
            "Beam TOF After All Cuts;"
            "TOF [ns];Events",
            120, 50, 110
        );

    TH1F* hBeamCKov0 =
        new TH1F(
            "hBeamCKov0",
            "Beam Cherenkov 0;CKov0;Events",
            10, 0, 10
        );

    TH1F* hBeamCKov1 =
        new TH1F(
            "hBeamCKov1",
            "Beam Cherenkov 1;CKov1;Events",
            10, 0, 10
        );


    //==================================================
    // NEW: TOF histograms for CKov combinations
    //
    // These are filled BEFORE the beam-position cut.
    // Selection:
    //   0 < BeamTOF < 500
    //   BeamMomentum > 0
    //==================================================

    TH1F* hTOF_CKov11 =
        new TH1F(
            "hTOF_CKov11",
            "TOF for CKov (1,1);TOF [ns];Entries",
            110, 65, 120
        );

    TH1F* hTOF_CKov10 =
        new TH1F(
            "hTOF_CKov10",
            "TOF for CKov (1,0);TOF [ns];Entries",
            110, 65, 120
        );

    TH1F* hTOF_CKov01 =
        new TH1F(
            "hTOF_CKov01",
            "TOF for CKov (0,1);TOF [ns];Entries",
            110, 65, 120
        );

    TH1F* hTOF_CKov00 =
        new TH1F(
            "hTOF_CKov00",
            "TOF for CKov (0,0);TOF [ns];Entries",
            110, 65, 120
        );


    // Styles matching the reference plot

    hTOF_CKov11->SetLineColor(kBlue);
    hTOF_CKov11->SetLineWidth(2);

    hTOF_CKov10->SetLineColor(kRed);
    hTOF_CKov10->SetLineWidth(2);

    hTOF_CKov01->SetLineColor(kGray + 1);
    hTOF_CKov01->SetLineWidth(2);

    hTOF_CKov00->SetLineColor(kGreen + 2);
    hTOF_CKov00->SetLineWidth(2);


    //==================================================
    // 6. Beam-position diagnostics
    //==================================================

    TH1F* hBeamPositionsBeforeCut =
        new TH1F(
            "hBeamPositionsBeforeCut",
            "Beam Positions Before Position Cut;"
            "Beam Position;Entries",
            400, -200, 200
        );

    TH1F* hBeamPositionsAfterCut =
        new TH1F(
            "hBeamPositionsAfterCut",
            "Beam Positions After Position Cut;"
            "Beam Position;Entries",
            400, -200, 200
        );

    TH2F* hMomentumVsBeamPositionBeforeCut =
        new TH2F(
            "hMomentumVsBeamPositionBeforeCut",
            "Momentum vs Beam Position Before Cut;"
            "Beam Position;Momentum [GeV/c]",
            400, -200, 200,
            200, 0, 10
        );

    TH2F* hMomentumVsBeamPositionAfterCut =
        new TH2F(
            "hMomentumVsBeamPositionAfterCut",
            "Momentum vs Beam Position After Cut;"
            "Beam Position;Momentum [GeV/c]",
            400, -200, 200,
            200, 0, 10
        );


    //==================================================
    // 7. PFParticle and track histograms
    //==================================================

    TH1F* hLowTrackScore =
        new TH1F(
            "hLowTrackScore",
            "Track Score < 0.5;Flag;Events",
            2, 0, 2
        );

    TH1F* hPrimaryPdg =
        new TH1F(
            "hPrimaryPdg",
            "Primary PDG;PDG;Events",
            500, -250, 250
        );

    TH1F* hNPFParticles =
        new TH1F(
            "hNPFParticles",
            "Number of PFParticles;N;Events",
            100, 0, 100
        );

    TH1F* hNPrimaryChildren =
        new TH1F(
            "hNPrimaryChildren",
            "N Primary Children;N;Events",
            100, 0, 100
        );

    TH1F* hTrackScore =
        new TH1F(
            "hTrackScore",
            "Primary Track Score;Score;Events",
            100, 0, 1
        );

    TH1F* hTrackLength =
        new TH1F(
            "hTrackLength",
            "Primary Track Length;Length [cm];Events",
            200, 0, 1000
        );


    //==================================================
    //==================================================

    TH1F* hCNNtrack =
        new TH1F(
            "hCNNtrack",
            "CNN Track Score;Score;Events",
            100, 0, 1
        );

    TH1F* hCNNshower =
        new TH1F(
            "hCNNshower",
            "CNN Shower Score;Score;Events",
            100, 0, 1
        );

    TH1F* hCNNmichel =
        new TH1F(
            "hCNNmichel",
            "CNN Michel Score;Score;Events",
            100, 0, 1
        );


    //==================================================
    // 9. Vertex histograms
    //==================================================

    TH1F* hVertexX =
        new TH1F(
            "hVertexX",
            "Beam Vertex X;X [cm];Events",
            200, -100, 100
        );

    TH1F* hVertexY =
        new TH1F(
            "hVertexY",
            "Beam Vertex Y;Y [cm];Events",
            200, -100, 100
        );

    TH1F* hVertexZ =
        new TH1F(
            "hVertexZ",
            "Beam Vertex Z;Z [cm];Events",
            200, -100, 700
        );

   TH2F *hVertexXY =
new TH2F(
    "hVertexXY",
    "Primary Vertex XY;X [cm];Y [cm]",
    100,-100,100,
    100,-100,100);

TH2F *hVertexXZ =
new TH2F(
    "hVertexXZ",
    "Primary Vertex XZ;X [cm];Z [cm]",
    100,-100,100,
    100,-50,500);

TH2F *hVertexYZ =
new TH2F(
    "hVertexYZ",
    "Primary Vertex YZ;Y [cm];Z [cm]",
    100,-100,100,
    100,-50,500);
    //==================================================
    // 10. Track start histograms
    //==================================================

    TH1F* hStartX =
        new TH1F(
            "hStartX",
            "Track Start X;X [cm];Events",
            200, -100, 100
        );

    TH1F* hStartY =
        new TH1F(
            "hStartY",
            "Track Start Y;Y [cm];Events",
            200, -100, 100
        );

    TH1F* hStartZ =
        new TH1F(
            "hStartZ",
            "Track Start Z;Z [cm];Events",
            200, -100, 700
        );

TH2F *hStartXY =
new TH2F(
    "hStartXY",
    "Track Start XY",
    100,-100,100,
    100,-100,100);

TH2F *hStartXZ =
new TH2F(
    "hStartXZ",
    "Track Start XZ",
    100,-100,100,
    100,-50,500);

TH2F *hStartYZ =
new TH2F(
    "hStartYZ",
    "Track Start YZ",
    100,-100,100,
    100,-50,500);

    //==================================================
    // 11. Track end histograms
    //==================================================

    TH1F* hEndX =
        new TH1F(
            "hEndX",
            "Track End X;X [cm];Events",
            200, -100, 100
        );

    TH1F* hEndY =
        new TH1F(
            "hEndY",
            "Track End Y;Y [cm];Events",
            200, -100, 100
        );

    TH1F* hEndZ =
        new TH1F(
            "hEndZ",
            "Track End Z;Z [cm];Events",
            200, -100, 700
        );

TH2F *hEndXY =
new TH2F(
    "hEndXY",
    "Track End XY",
    100,-100,100,
    100,-100,100);

TH2F *hEndXZ =
new TH2F(
    "hEndXZ",
    "Track End XZ",
    100,-100,100,
    100,-50,500);

TH2F *hEndYZ =
new TH2F(
    "hEndYZ",
    "Track End YZ",
    100,-100,100,
    100,-50,500);

    //==================================================
    // 12. Child particle histogram
    //==================================================

    TH1F* hChildTrackLength =
        new TH1F(
            "hChildTrackLength",
            "Child Track Length;Length [cm];Entries",
            200, 0, 1000
        );

TH1I *hChildKey =
new TH1I(
    "hChildKey",
    "Child Key",
    100,0,1000);

TH1I *hChildPDG =
new TH1I(
    "hChildPDG",
    "Child PDG",
    100,-5000,5000);

TH1F *hChildVertexX =
new TH1F(
    "hChildVertexX",
    "Child Vertex X",
    100,-100,100);

TH1F *hChildVertexY =
new TH1F(
    "hChildVertexY",
    "Child Vertex Y",
    100,-100,100);

TH1F *hChildVertexZ =
new TH1F(
    "hChildVertexZ",
    "Child Vertex Z",
    100,-50,500);

TH2F *hChildVertexXY =
new TH2F(
    "hChildVertexXY",
    "Child Vertex XY",
    100,-100,100,
    100,-100,100);

TH2F *hChildVertexXZ =
new TH2F(
    "hChildVertexXZ",
    "Child Vertex XZ",
    100,-100,100,
    100,-50,500);

TH2F *hChildVertexYZ =
new TH2F(
    "hChildVertexYZ",
    "Child Vertex YZ",
    100,-100,100,
    100,-50,500);

    //==================================================
    // 13. 2D histograms
    //==================================================

    TH2F* hTOFvsMomentum =
        new TH2F(
            "hTOFvsMomentum",
            "TOF vs Momentum After All Cuts;"
            "Momentum [GeV/c];TOF [ns]",
            200, 0, 10,
            120, 50, 110
        );

    TH2F* hTOFvsMomentum_Pion =
        new TH2F(
            "hTOFvsMomentum_Pion",
            "TOF vs Momentum (Primary PDG = 211);"
            "Momentum [GeV/c];TOF [ns]",
            200, 0, 10,
            120, 50, 110
        );

    TH2F* hTOFvsMomentum_Electron =
        new TH2F(
            "hTOFvsMomentum_Electron",
            "TOF vs Momentum (Primary PDG = 11);"
            "Momentum [GeV/c];TOF [ns]",
            200, 0, 10,
            120, 50, 110
        );

    TH2F* hTOFvsCKov0 =
        new TH2F(
            "hTOFvsCKov0",
            "TOF vs CKov0;CKov0;TOF [ns]",
            10, 0, 10,
            120, 50, 110
        );

    TH2F* hTOFvsCKov1 =
        new TH2F(
            "hTOFvsCKov1",
            "TOF vs CKov1;CKov1;TOF [ns]",
            10, 0, 10,
            120, 50, 110
        );

    TH2F* hMomentumVsLength =
        new TH2F(
            "hMomentumVsLength",
            "Momentum vs Track Length;"
            "Momentum [GeV/c];Length [cm]",
            200, 0, 10,
            200, 0, 1000
        );

    TH2F* hScoreVsLength =
        new TH2F(
            "hScoreVsLength",
            "Track Score vs Length;"
            "Track Score;Length [cm]",
            100, 0, 1,
            200, 0, 1000
        );

    TH2F* hScoreVsCNN =
        new TH2F(
            "hScoreVsCNN",
            "Track Score vs CNN Track;"
            "Track Score;CNN Track Score",
            100, 0, 1,
            100, 0, 1
        );

    TH2F* hMomentumVsPDG =
        new TH2F(
            "hMomentumVsPDG",
            "Momentum vs PDG;Momentum [GeV/c];PDG",
            200, 0, 10,
            500, -250, 250
        );

    TH2F* hTOFvsPDG =
        new TH2F(
            "hTOFvsPDG",
            "TOF vs PDG;TOF [ns];PDG",
            120, 50, 110,
            500, -250, 250
        );

    TH2F* hCKovCorrelation =
        new TH2F(
            "hCKovCorrelation",
            "CKov0 vs CKov1;CKov0;CKov1",
            10, 0, 10,
            10, 0, 10
        );

    TH2F* hBeamStartXY =
        new TH2F(
            "hBeamStartXY",
            "Beam Track Start XY;X [cm];Y [cm]",
            200, -100, 100,
            200, -100, 100
        );

    TH2F* hBeamStartYZ =
        new TH2F(
            "hBeamStartYZ",
            "Beam Track Start YZ;Y [cm];Z [cm]",
            200, -100, 100,
            200, -100, 700
        );

	//==================================================
	// Beam profile plots
	//==================================================

//==================================================
// Beam profile histograms
//==================================================

TH1F *hUpstreamX =
new TH1F(
    "hUpstreamX",
    "Upstream Beam Profile X;Fiber position;Entries",
    100,-100,100);

TH1F *hUpstreamY =
new TH1F(
    "hUpstreamY",
    "Upstream Beam Profile Y;Fiber position;Entries",
    100,-100,100);

TH1F *hDownstreamX =
new TH1F(
    "hDownstreamX",
    "Downstream Beam Profile X;Fiber position;Entries",
    100,-100,100);

TH1F *hDownstreamY =
new TH1F(
    "hDownstreamY",
    "Downstream Beam Profile Y;Fiber position;Entries",
    100,-100,100);

TH2F *hBeamSpotUp =
new TH2F(
    "hBeamSpotUp",
    "Upstream Beam Spot;X;Y",
    100,-100,100,
    100,-100,100);

TH2F *hBeamSpotDown =
new TH2F(
    "hBeamSpotDown",
    "Downstream Beam Spot;X;Y",
    100,-100,100,
    100,-100,100);

    //==================================================
    // 14. Event loop
    //==================================================

    Long64_t nBasicSelected = 0;
    Long64_t nPositionSelected = 0;

    Long64_t nCKov11 = 0;
    Long64_t nCKov10 = 0;
    Long64_t nCKov01 = 0;
    Long64_t nCKov00 = 0;


    for (Long64_t i = 0; i < nEntries; i++)
    {
        chain.GetEntry(i);


        //==============================================
        // BASIC CUTS
        //==============================================

        if (BeamTOF <= 0.0)
            continue;

        if (BeamTOF >= 500.0)
            continue;

        if (BeamMomentum <= 0.0)
            continue;

        nBasicSelected++;


        //==============================================
        // NEW: CKov combination TOF spectra
        //
        // Filled BEFORE beam-position cut.
        //==============================================

        if (BeamCKov0 == 1 && BeamCKov1 == 1)
        {
            hTOF_CKov11->Fill(BeamTOF);
            nCKov11++;
        }
        else if (BeamCKov0 == 1 && BeamCKov1 == 0)
        {
            hTOF_CKov10->Fill(BeamTOF);
            nCKov10++;
        }
        else if (BeamCKov0 == 0 && BeamCKov1 == 1)
        {
            hTOF_CKov01->Fill(BeamTOF);
            nCKov01++;
        }
        else if (BeamCKov0 == 0 && BeamCKov1 == 0)
        {
            hTOF_CKov00->Fill(BeamTOF);
            nCKov00++;
        }


        //==============================================
        // BEFORE BEAM-POSITION CUT
        //==============================================

        if (BeamPositions)
        {
            for (double position : *BeamPositions)
            {
                hBeamPositionsBeforeCut->Fill(position);

                hMomentumVsBeamPositionBeforeCut->Fill(
                    position,
                    BeamMomentum
                );
            }
        }


	//==============================================
// BEAM PROFILE (UPSTREAM / DOWNSTREAM)
//==============================================

if (BeamPositions && BeamPositions->size() >= 4)
{

    double upX = BeamPositions->at(0);
    double upY = BeamPositions->at(1);

    double downX = BeamPositions->at(2);
    double downY = BeamPositions->at(3);
    
    
    // Remove events with missing BPM information
if (upX == 0 || upY == 0 || downX == 0 || downY == 0)
    continue;
  


const double fiberSize = 1.0;   // mm

auto PosToFiber = [&](double pos)
{
    return std::lround(96 - (pos + fiberSize/2.) / fiberSize);
};


    // Upstream module 55 (X) and 56 (Y)
//==================================================
// Beam profile histograms
//==================================================

// Upstream module
hUpstreamX->Fill(upX);
hUpstreamY->Fill(upY);

int fiberUpX = PosToFiber(upX);
int fiberUpY = PosToFiber(upY);

if (fiberUpX >= 0 && fiberUpX < 196)
    hFiberIndexUpX->Fill(fiberUpX);

if (fiberUpY >= 0 && fiberUpY < 196)
    hFiberIndexUpY->Fill(fiberUpY);


// Downstream module
hDownstreamX->Fill(downX);
hDownstreamY->Fill(downY);

int fiberDownX = PosToFiber(downX);
int fiberDownY = PosToFiber(downY);

if (fiberDownX >= 0 && fiberDownX < 196)
    hFiberIndexDownX->Fill(fiberDownX);

if (fiberDownY >= 0 && fiberDownY < 196)
    hFiberIndexDownY->Fill(fiberDownY);


// Beam spots
hBeamSpotUp->Fill(upX, upY);
hBeamSpotDown->Fill(downX, downY);
}
        //==============================================
        // BEAM-POSITION CUT
        //==============================================

        bool passBeamPositionCut = true;

        if (!BeamPositions || BeamPositions->empty())
        {
            passBeamPositionCut = false;
        }
        else
        {
            for (double position : *BeamPositions)
            {
                if (position <= beamPosMin ||
                    position >= beamPosMax)
                {
                    passBeamPositionCut = false;
                    break;
                }
            }
        }

        if (!passBeamPositionCut)
            continue;

        nPositionSelected++;


        //==============================================
        // AFTER BEAM-POSITION CUT
        //==============================================

        for (double position : *BeamPositions)
        {
            hBeamPositionsAfterCut->Fill(position);

            hMomentumVsBeamPositionAfterCut->Fill(
                position,
                BeamMomentum
            );
        }


        //==============================================
        // FINAL HISTOGRAMS
        //==============================================

        hBeamMomentum->Fill(BeamMomentum);

        double deltaPOverP =
            (BeamMomentum - nominalMomentum)
            / nominalMomentum;

        hDeltaPOverP->Fill(deltaPOverP);

        hBeamTOF->Fill(BeamTOF);

        hBeamCKov0->Fill(BeamCKov0);
        hBeamCKov1->Fill(BeamCKov1);


        if (PrimBeamPFPTrackScore < 0.5)
            hLowTrackScore->Fill(1);
        else
            hLowTrackScore->Fill(0);


        hPrimaryPdg->Fill(PrimaryPdg);

        hNPFParticles->Fill(NPFParticles);

        hNPrimaryChildren->Fill(
            NPrimaryChildren
        );

        hTrackScore->Fill(
            PrimBeamPFPTrackScore
        );

        hTrackLength->Fill(
            PrimBeamPFPTrackLength
        );

        hCNNtrack->Fill(
            PrimBeamPFPCNNtrack
        );

        hCNNshower->Fill(
            PrimBeamPFPCNNshower
        );

        hCNNmichel->Fill(
            PrimBeamPFPCNNmichel
        );

        hVertexX->Fill(
            PrimBeamPFPTrackVertexX
        );

        hVertexY->Fill(
            PrimBeamPFPTrackVertexY
        );

        hVertexZ->Fill(
            PrimBeamPFPTrackVertexZ
        );
        
        hVertexXY->Fill(
    PrimBeamPFPTrackVertexX,
    PrimBeamPFPTrackVertexY);

hVertexXZ->Fill(
    PrimBeamPFPTrackVertexX,
    PrimBeamPFPTrackVertexZ);

hVertexYZ->Fill(
    PrimBeamPFPTrackVertexY,
    PrimBeamPFPTrackVertexZ);

        hStartX->Fill(
            PrimBeamPFPTrackStartX
        );

        hStartY->Fill(
            PrimBeamPFPTrackStartY
        );

        hStartZ->Fill(
            PrimBeamPFPTrackStartZ
        );
        
        hStartXY->Fill(
    PrimBeamPFPTrackStartX,
    PrimBeamPFPTrackStartY);

hStartXZ->Fill(
    PrimBeamPFPTrackStartX,
    PrimBeamPFPTrackStartZ);

hStartYZ->Fill(
    PrimBeamPFPTrackStartY,
    PrimBeamPFPTrackStartZ);

        hEndX->Fill(
            PrimBeamPFPTrackEndX
        );

        hEndY->Fill(
            PrimBeamPFPTrackEndY
        );

        hEndZ->Fill(
            PrimBeamPFPTrackEndZ
        );
        
        hEndXY->Fill(
    PrimBeamPFPTrackEndX,
    PrimBeamPFPTrackEndY);

hEndXZ->Fill(
    PrimBeamPFPTrackEndX,
    PrimBeamPFPTrackEndZ);

hEndYZ->Fill(
    PrimBeamPFPTrackEndY,
    PrimBeamPFPTrackEndZ);

        hBeamStartXY->Fill(
            PrimBeamPFPTrackStartX,
            PrimBeamPFPTrackStartY
        );
        
      

        hBeamStartYZ->Fill(
            PrimBeamPFPTrackStartY,
            PrimBeamPFPTrackStartZ
        );


        if (ChildBeamPFPTrackLength)
{
    for (float length : *ChildBeamPFPTrackLength)
    {
        hChildTrackLength->Fill(length);
    }
}

//--------------------------------------------------
// Child vertex information
//--------------------------------------------------

if (ChildBeamPFPKey &&
    ChildBeamPFPPdg &&
    ChildBeamPFPTrackVertexX &&
    ChildBeamPFPTrackVertexY &&
    ChildBeamPFPTrackVertexZ)
{
    size_t n = ChildBeamPFPKey->size();

    for (size_t j = 0; j < n; ++j)
    {
        hChildKey->Fill(
            ChildBeamPFPKey->at(j));

        hChildPDG->Fill(
            ChildBeamPFPPdg->at(j));

        hChildVertexX->Fill(
            ChildBeamPFPTrackVertexX->at(j));

        hChildVertexY->Fill(
            ChildBeamPFPTrackVertexY->at(j));

        hChildVertexZ->Fill(
            ChildBeamPFPTrackVertexZ->at(j));

        hChildVertexXY->Fill(
            ChildBeamPFPTrackVertexX->at(j),
            ChildBeamPFPTrackVertexY->at(j));

        hChildVertexXZ->Fill(
            ChildBeamPFPTrackVertexX->at(j),
            ChildBeamPFPTrackVertexZ->at(j));

        hChildVertexYZ->Fill(
            ChildBeamPFPTrackVertexY->at(j),
            ChildBeamPFPTrackVertexZ->at(j));
    }
}

//==============================================
// CORRELATIONS
//==============================================


        //==============================================
        // CORRELATIONS
        //==============================================

        hTOFvsMomentum->Fill(
            BeamMomentum,
            BeamTOF
        );


        if (PrimaryPdg == 211)
        {
            hTOFvsMomentum_Pion->Fill(
                BeamMomentum,
                BeamTOF
            );
        }


        if (PrimaryPdg == 11)
        {
            hTOFvsMomentum_Electron->Fill(
                BeamMomentum,
                BeamTOF
            );
        }


        hTOFvsCKov0->Fill(
            BeamCKov0,
            BeamTOF
        );

        hTOFvsCKov1->Fill(
            BeamCKov1,
            BeamTOF
        );

        hMomentumVsLength->Fill(
            BeamMomentum,
            PrimBeamPFPTrackLength
        );

        hScoreVsLength->Fill(
            PrimBeamPFPTrackScore,
            PrimBeamPFPTrackLength
        );

        hScoreVsCNN->Fill(
            PrimBeamPFPTrackScore,
            PrimBeamPFPCNNtrack
        );

        hMomentumVsPDG->Fill(
            BeamMomentum,
            PrimaryPdg
        );

        hTOFvsPDG->Fill(
            BeamTOF,
            PrimaryPdg
        );

        hCKovCorrelation->Fill(
            BeamCKov0,
            BeamCKov1
        );


        if (i % 100000 == 0)
        {
            std::cout
                << "Processed "
                << i
                << " events"
                << std::endl;
        }
    }


    //==================================================
    // 15. Gaussian fits
    //==================================================

    std::cout
        << "Performing Gaussian fits..."
        << std::endl;


    if (hBeamMomentum->GetEntries() > 0)
    {
        hBeamMomentum->Fit(
            "gaus",
            "R",
            "",
            momentumFitMin,
            momentumFitMax
        );
    }




//==================================================
// Upstream Beam Profile Canvas
//==================================================

TCanvas *cBeamProfileUp =
new TCanvas(
    "cBeamProfileUp",
    "Upstream Beam Profile",
    900,
    900
);


TPad *padUpX =
new TPad(
    "padUpX",
    "",
    0.25,0.75,
    1.0,1.0
);


TPad *padUpY =
new TPad(
    "padUpY",
    "",
    0.0,0.0,
    0.25,0.75
);


TPad *padUpXY =
new TPad(
    "padUpXY",
    "",
    0.25,0.0,
    1.0,0.75
);


padUpX->Draw();
padUpY->Draw();
padUpXY->Draw();


padUpX->cd();
hUpstreamX->Draw();


padUpY->cd();
hUpstreamY->Draw("HBAR");


padUpXY->cd();
gStyle->SetPalette(kBird);
hBeamSpotUp->Draw("COLZ");


cBeamProfileUp->SaveAs(
    "beam_profile_upstream.png"
);

cBeamProfileUp->SaveAs(
    "beam_profile_upstream.pdf"
);



//==================================================
// Downstream Beam Profile Canvas
//==================================================

TCanvas *cBeamProfileDown =
new TCanvas(
    "cBeamProfileDown",
    "Downstream Beam Profile",
    900,
    900
);


TPad *padDownX =
new TPad(
    "padDownX",
    "",
    0.25,0.75,
    1.0,1.0
);


TPad *padDownY =
new TPad(
    "padDownY",
    "",
    0.0,0.0,
    0.25,0.75
);


TPad *padDownXY =
new TPad(
    "padDownXY",
    "",
    0.25,0.0,
    1.0,0.75
);


padDownX->Draw();
padDownY->Draw();
padDownXY->Draw();


padDownX->cd();
hDownstreamX->Draw();


padDownY->cd();
hDownstreamY->Draw("HBAR");


padDownXY->cd();
gStyle->SetPalette(kBird);
hBeamSpotDown->Draw("COLZ");


cBeamProfileDown->SaveAs(
    "beam_profile_downstream.png"
);

cBeamProfileDown->SaveAs(
    "beam_profile_downstream.pdf"
);

TCanvas *cFiberIndex =
new TCanvas("cFiberIndex","Fiber Index",1200,800);

cFiberIndex->Divide(2,2);

cFiberIndex->cd(1);
hFiberIndexUpX->Draw();

cFiberIndex->cd(2);
hFiberIndexUpY->Draw();

cFiberIndex->cd(3);
hFiberIndexDownX->Draw();

cFiberIndex->cd(4);
hFiberIndexDownY->Draw();

cFiberIndex->SaveAs("fiber_index.png");
cFiberIndex->SaveAs("fiber_index.pdf");

    //==================================================
    // Momentum-resolution fit
    //==================================================

    TF1* momentumResolutionFit = nullptr;

    double resolutionMean = 0.0;
    double resolutionSigma = 0.0;
    double resolutionPercent = 0.0;


    if (hDeltaPOverP->GetEntries() > 0)
    {
        momentumResolutionFit =
            new TF1(
                "momentumResolutionFit",
                "gaus",
                resolutionFitMin,
                resolutionFitMax
            );

        momentumResolutionFit->SetLineColor(kRed);
        momentumResolutionFit->SetLineWidth(2);

        hDeltaPOverP->Fit(
            momentumResolutionFit,
            "R"
        );

        resolutionMean =
            momentumResolutionFit->GetParameter(1);

        resolutionSigma =
            std::abs(
                momentumResolutionFit->GetParameter(2)
            );

        resolutionPercent =
            100.0 * resolutionSigma;
    }


    //==================================================
    // TOF fit
    //==================================================

    if (hBeamTOF->GetEntries() > 0)
    {
        double peak =
            hBeamTOF->GetBinCenter(
                hBeamTOF->GetMaximumBin()
            );

        TF1* tofGaus =
            new TF1(
                "tofGaus",
                "gaus",
                peak - 1.5,
                peak + 1.5
            );

        hBeamTOF->Fit(
            tofGaus,
            "R"
        );
    }


    //==================================================
    // 16. Print fit results
    //==================================================

    TF1* momentumFit =
        hBeamMomentum->GetFunction("gaus");

    if (momentumFit)
    {
        std::cout
            << "Momentum Gaussian mean  = "
            << momentumFit->GetParameter(1)
            << " GeV/c"
            << std::endl;

        std::cout
            << "Momentum Gaussian sigma = "
            << momentumFit->GetParameter(2)
            << " GeV/c"
            << std::endl;
    }


    if (momentumResolutionFit)
    {
        std::cout
            << "========================================"
            << std::endl;

        std::cout
            << "Momentum resolution fit:"
            << std::endl;

        std::cout
            << "Mean of (P_rec-P)/P = "
            << resolutionMean
            << std::endl;

        std::cout
            << "Sigma of (P_rec-P)/P = "
            << resolutionSigma
            << std::endl;

        std::cout
            << "Momentum resolution = "
            << resolutionPercent
            << " %"
            << std::endl;

        std::cout
            << "========================================"
            << std::endl;
    }


    TF1* tofFit =
        hBeamTOF->GetFunction("tofGaus");

    if (tofFit)
    {
        std::cout
            << "TOF Gaussian mean  = "
            << tofFit->GetParameter(1)
            << " ns"
            << std::endl;

        std::cout
            << "TOF Gaussian sigma = "
            << tofFit->GetParameter(2)
            << " ns"
            << std::endl;
    }


    //==================================================
    // 17. Selection summary
    //==================================================

    std::cout
        << "========================================"
        << std::endl;

    std::cout
        << "Total events: "
        << nEntries
        << std::endl;

    std::cout
        << "After TOF and momentum cuts: "
        << nBasicSelected
        << std::endl;

    std::cout
        << "CKov (1,1): "
        << nCKov11
        << std::endl;

    std::cout
        << "CKov (1,0): "
        << nCKov10
        << std::endl;

    std::cout
        << "CKov (0,1): "
        << nCKov01
        << std::endl;

    std::cout
        << "CKov (0,0): "
        << nCKov00
        << std::endl;

    std::cout
        << "After beam-position cut: "
        << nPositionSelected
        << std::endl;

    if (nBasicSelected > 0)
    {
        std::cout
            << "Beam-position cut efficiency: "
            << 100.0 * nPositionSelected
               / nBasicSelected
            << " %"
            << std::endl;
    }

    std::cout
        << "========================================"
        << std::endl;


    //==================================================
    // 18. Draw momentum-resolution plot
    //==================================================

    gStyle->SetOptStat(1110);
    gStyle->SetOptFit(1111);

    TCanvas* cMomentumResolution =
        new TCanvas(
            "cMomentumResolution",
            "Momentum Resolution",
            900,
            650
        );

    cMomentumResolution->SetGrid();

    hDeltaPOverP->SetTitle("");

    hDeltaPOverP->GetXaxis()->SetTitle(
        "(P_{rec}-P)/P"
    );

    hDeltaPOverP->GetYaxis()->SetTitle(
        "Events"
    );

    hDeltaPOverP->Draw("HIST");

    if (momentumResolutionFit)
    {
        momentumResolutionFit->Draw("SAME");
    }


    TLatex titleText;

    titleText.SetNDC();
    titleText.SetTextAlign(22);
    titleText.SetTextSize(0.055);
    titleText.SetTextColor(kBlue + 1);
    titleText.SetTextFont(62);

    titleText.DrawLatex(
        0.50,
        0.94,
        "1 GeV/c"
    );


    TLatex resolutionText;

    resolutionText.SetNDC();
    resolutionText.SetTextSize(0.050);
    resolutionText.SetTextFont(62);
    resolutionText.SetTextColor(kRed);

    resolutionText.DrawLatex(
        0.15,
        0.68,
        Form("%.2f%%", resolutionPercent)
    );


    cMomentumResolution->Update();

    cMomentumResolution->SaveAs(
        "momentum_resolution_1GeV.png"
    );

    cMomentumResolution->SaveAs(
        "momentum_resolution_1GeV.pdf"
    );



    //==================================================
    // 19. NEW: Draw TOF distributions by CKov status
    //==================================================

    gStyle->SetOptStat(0);

    TCanvas* cTOFCKov =
        new TCanvas(
            "cTOFCKov",
            "TOF by CKov Combination",
            900,
            700
        );

    cTOFCKov->SetGrid();


    // Find maximum among all four histograms

    double maxTOF =
        std::max(
            std::max(
                hTOF_CKov11->GetMaximum(),
                hTOF_CKov10->GetMaximum()
            ),
            std::max(
                hTOF_CKov01->GetMaximum(),
                hTOF_CKov00->GetMaximum()
            )
        );


    hTOF_CKov11->SetTitle("");

    hTOF_CKov11->GetXaxis()->SetTitle(
        "TOF (ns)"
    );

    hTOF_CKov11->GetYaxis()->SetTitle(
        "Entries"
    );

    hTOF_CKov11->GetXaxis()->SetTitleSize(0.045);
    hTOF_CKov11->GetYaxis()->SetTitleSize(0.045);

    hTOF_CKov11->GetXaxis()->SetLabelSize(0.04);
    hTOF_CKov11->GetYaxis()->SetLabelSize(0.04);

    hTOF_CKov11->SetMaximum(
        1.20 * maxTOF
    );


    // Draw all four spectra

    hTOF_CKov11->Draw("HIST");
    hTOF_CKov10->Draw("HIST SAME");
    hTOF_CKov01->Draw("HIST SAME");
    hTOF_CKov00->Draw("HIST SAME");


    //==================================================
    // Legend
    //==================================================

    TLegend* legendCKov =
        new TLegend(
            0.60,
            0.64,
            0.88,
            0.88
        );

    legendCKov->SetBorderSize(1);
    legendCKov->SetFillStyle(1001);
    legendCKov->SetTextSize(0.040);

    legendCKov->AddEntry(
        hTOF_CKov11,
        "CKov (1, 1)",
        "l"
    );

    legendCKov->AddEntry(
        hTOF_CKov10,
        "CKov (1, 0)",
        "l"
    );

    legendCKov->AddEntry(
        hTOF_CKov01,
        "CKov (0, 1)",
        "l"
    );

    legendCKov->AddEntry(
        hTOF_CKov00,
        "CKov (0, 0)",
        "l"
    );

    legendCKov->Draw();


    cTOFCKov->Update();


    //==================================================
    // Save CKov-TOF plot
    //==================================================

    cTOFCKov->SaveAs(
        "tof_ckov_combinations_1GeV.png"
    );

    cTOFCKov->SaveAs(
        "tof_ckov_combinations_1GeV.pdf"
    );


    //==================================================
    // 20. Save output ROOT file
    //==================================================

    TFile output(
        "beamselection_histograms.root",
        "RECREATE"
    );


    // Beam instrumentation

    hBeamMomentum->Write();

    hDeltaPOverP->Write();

    if (momentumResolutionFit)
    {
        momentumResolutionFit->Write();
    }

    cMomentumResolution->Write();

    hBeamTOF->Write();

    hBeamCKov0->Write();
    hBeamCKov1->Write();


    // NEW: CKov combination histograms and canvas

    hTOF_CKov11->Write();
    hTOF_CKov10->Write();
    hTOF_CKov01->Write();
    hTOF_CKov00->Write();

    cTOFCKov->Write();


    // Beam-position diagnostics

    hBeamPositionsBeforeCut->Write();
    hBeamPositionsAfterCut->Write();

    hMomentumVsBeamPositionBeforeCut->Write();
    hMomentumVsBeamPositionAfterCut->Write();


    // PFParticle / track

    hLowTrackScore->Write();
    hPrimaryPdg->Write();
    hNPFParticles->Write();
    hNPrimaryChildren->Write();
    hTrackScore->Write();
    hTrackLength->Write();


    // CNN

    hCNNtrack->Write();
    hCNNshower->Write();
    hCNNmichel->Write();


    // Vertex

    hVertexX->Write();
    hVertexY->Write();
    hVertexZ->Write();


    // Start

    hStartX->Write();
    hStartY->Write();
    hStartZ->Write();


    // End

    hEndX->Write();
    hEndY->Write();
    hEndZ->Write();


    // Child particles

    hChildTrackLength->Write();


    // 2D histograms

    hTOFvsMomentum->Write();

    hTOFvsMomentum_Pion->Write();
    hTOFvsMomentum_Electron->Write();

    hTOFvsCKov0->Write();
    hTOFvsCKov1->Write();

    hMomentumVsLength->Write();
    hScoreVsLength->Write();
    hScoreVsCNN->Write();

    hMomentumVsPDG->Write();
    hTOFvsPDG->Write();

    hCKovCorrelation->Write();

    hBeamStartXY->Write();
    hBeamStartYZ->Write();
    
   hUpstreamX->Write();
hUpstreamY->Write();

hDownstreamX->Write();
hDownstreamY->Write();


hBeamSpotUp->Write();
hBeamSpotDown->Write();


cBeamProfileUp->Write();
cBeamProfileDown->Write();

hFiberIndexUpX->Write();
hFiberIndexUpY->Write();
hFiberIndexDownX->Write();
hFiberIndexDownY->Write();

cFiberIndex->Write();

hVertexXY->Write();
hVertexXZ->Write();
hVertexYZ->Write();

hStartXY->Write();
hStartXZ->Write();
hStartYZ->Write();

hEndXY->Write();
hEndXZ->Write();
hEndYZ->Write();

hChildKey->Write();
hChildPDG->Write();

hChildVertexX->Write();
hChildVertexY->Write();
hChildVertexZ->Write();

hChildVertexXY->Write();
hChildVertexXZ->Write();
hChildVertexYZ->Write();


    output.Close();


    //==================================================
    // 21. Final message
    //==================================================

    std::cout
        << "Done."
        << std::endl;

    std::cout
        << "Histograms saved to:"
        << std::endl;

    std::cout
        << "  beamselection_histograms.root"
        << std::endl;

    std::cout
        << "Plots saved to:"
        << std::endl;

    std::cout
        << "  momentum_resolution_1GeV.png"
        << std::endl;

    std::cout
        << "  momentum_resolution_1GeV.pdf"
        << std::endl;

    std::cout
        << "  tof_ckov_combinations_1GeV.png"
        << std::endl;

    std::cout
        << "  tof_ckov_combinations_1GeV.pdf"
        << std::endl;
}
