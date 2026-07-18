//NilayBostan, CERN July 18, 2026

#include <iostream>
#include <vector>
#include <TChain.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH1D.h>
#include <TH1I.h>

void PDVD_Beam_Data2() {
    // 1. TChain
    const char* inputPath =
        "/pnfs/dune/scratch/users/ykermaid/fnal/18176/1/001/"
        "*_beamselection.root";

    TChain *chain = new TChain("beamselection/tree");
    chain->Add(inputPath);

    Long64_t nentries = chain->GetEntries();
    if (nentries == 0) {
        std::cerr << "Hata: Herhangi bir entry bulunamadi!" << std::endl;
        delete chain;
        return;
    }

    // 2.  Histogram Dosyasının Oluşturulması
    TFile *outputFile = new TFile("Pandora_Histograms.root", "RECREATE");

    // 3. Veri Değişkenlerinin Tanımlanması
    UInt_t   eventID;
    Float_t  BeamTOF;
    Short_t  BeamCKov0;
    Short_t  BeamCKov1;
    Double_t BeamMomentum;
    Int_t    PrimaryKey;
    Int_t    PrimaryPdg;
    Int_t    NPFParticles;
    Int_t    NPrimaryChildren;
    
    Float_t  PrimBeamPFPCNNtrack;
    Float_t  PrimBeamPFPCNNshower;
    Float_t  PrimBeamPFPCNNmichel;
    Int_t    PrimBeamPFPTrackPid;
    Float_t  PrimBeamPFPTrackScore;
    Float_t  PrimBeamPFPTrackVertexX;
    Float_t  PrimBeamPFPTrackVertexY;
    Float_t  PrimBeamPFPTrackVertexZ;
    Float_t  PrimBeamPFPTrackDirX;
    Float_t  PrimBeamPFPTrackDirY;
    Float_t  PrimBeamPFPTrackDirZ;
    Float_t  PrimBeamPFPTrackStartX;
    Float_t  PrimBeamPFPTrackStartY;
    Float_t  PrimBeamPFPTrackStartZ;
    Float_t  PrimBeamPFPTrackEndX;
    Float_t  PrimBeamPFPTrackEndY;
    Float_t  PrimBeamPFPTrackEndZ;
    Float_t  PrimBeamPFPTrackLength;
    Float_t  PrimBeamPFPShowerVertexX;
    Float_t  PrimBeamPFPShowerVertexY;
    Float_t  PrimBeamPFPShowerVertexZ;
    Float_t  PrimBeamPFPShowerDirX;
    Float_t  PrimBeamPFPShowerDirY;
    Float_t  PrimBeamPFPShowerDirZ;
    Float_t  PrimBeamPFPShowerdEdx;
    Float_t  PrimBeamPFPShowerLength;
    Float_t  PrimBeamPFPShowerEnergy;

    std::vector<double> *BeamPositions = nullptr;
    std::vector<float>  *PrimBeamPFPX = nullptr;
    std::vector<float>  *PrimBeamPFPY = nullptr;
    std::vector<float>  *PrimBeamPFPZ = nullptr;
    
    std::vector<int>    *ChildBeamPFPKey = nullptr;
    std::vector<int>    *ChildBeamPFPPdg = nullptr;
    std::vector<int>    *ChildBeamPFPChildren = nullptr;
    std::vector<float>  *ChildBeamPFPTrackScore = nullptr;
    std::vector<float>  *ChildBeamPFPCNNtrack = nullptr;
    std::vector<float>  *ChildBeamPFPCNNshower = nullptr;
    std::vector<float>  *ChildBeamPFPCNNmichel = nullptr;
    std::vector<int>    *ChildBeamPFPTrackPid = nullptr;
    std::vector<float>  *ChildBeamPFPTrackVertexX = nullptr;
    std::vector<float>  *ChildBeamPFPTrackVertexY = nullptr;
    std::vector<float>  *ChildBeamPFPTrackVertexZ = nullptr;
    std::vector<float>  *ChildBeamPFPTrackDirX = nullptr;
    std::vector<float>  *ChildBeamPFPTrackDirY = nullptr;
    std::vector<float>  *ChildBeamPFPTrackDirZ = nullptr;
    std::vector<float>  *ChildBeamPFPTrackStartX = nullptr;
    std::vector<float>  *ChildBeamPFPTrackStartY = nullptr;
    std::vector<float>  *ChildBeamPFPTrackStartZ = nullptr;
    std::vector<float>  *ChildBeamPFPTrackEndX = nullptr;
    std::vector<float>  *ChildBeamPFPTrackEndY = nullptr;
    std::vector<float>  *ChildBeamPFPTrackEndZ = nullptr;
    std::vector<float>  *ChildBeamPFPTrackLength = nullptr;
    std::vector<float>  *ChildBeamPFPShowerVertexX = nullptr;
    std::vector<float>  *ChildBeamPFPShowerVertexY = nullptr;
    std::vector<float>  *ChildBeamPFPShowerVertexZ = nullptr;
    std::vector<float>  *ChildBeamPFPShowerDirX = nullptr;
    std::vector<float>  *ChildBeamPFPShowerDirY = nullptr;
    std::vector<float>  *ChildBeamPFPShowerDirZ = nullptr;
    std::vector<float>  *ChildBeamPFPShowerdEdx = nullptr;
    std::vector<float>  *ChildBeamPFPShowerLength = nullptr;
    std::vector<float>  *ChildBeamPFPShowerEnergy = nullptr;

    std::vector<std::vector<float>> *ChildBeamPFPX = nullptr;
    std::vector<std::vector<float>> *ChildBeamPFPY = nullptr;
    std::vector<std::vector<float>> *ChildBeamPFPZ = nullptr;

    // 4. Adresleri TChain'e Bağlama
    chain->SetBranchAddress("eventID", (UInt_t*)&eventID);
    chain->SetBranchAddress("BeamTOF", &BeamTOF);
    chain->SetBranchAddress("BeamCKov0", &BeamCKov0);
    chain->SetBranchAddress("BeamCKov1", &BeamCKov1);
    chain->SetBranchAddress("BeamMomentum", &BeamMomentum);
    chain->SetBranchAddress("BeamPositions", &BeamPositions);
    chain->SetBranchAddress("PrimaryKey", &PrimaryKey);
    chain->SetBranchAddress("PrimaryPdg", &PrimaryPdg);
    chain->SetBranchAddress("NPFParticles", &NPFParticles);
    chain->SetBranchAddress("NPrimaryChildren", &NPrimaryChildren);
    
    chain->SetBranchAddress("PrimBeamPFPCNNtrack", &PrimBeamPFPCNNtrack);
    chain->SetBranchAddress("PrimBeamPFPCNNshower", &PrimBeamPFPCNNshower);
    chain->SetBranchAddress("PrimBeamPFPCNNmichel", &PrimBeamPFPCNNmichel);
    chain->SetBranchAddress("PrimBeamPFPTrackPid", &PrimBeamPFPTrackPid);
    chain->SetBranchAddress("PrimBeamPFPTrackScore", &PrimBeamPFPTrackScore);
    chain->SetBranchAddress("PrimBeamPFPTrackVertexX", &PrimBeamPFPTrackVertexX);
    chain->SetBranchAddress("PrimBeamPFPTrackVertexY", &PrimBeamPFPTrackVertexY);
    chain->SetBranchAddress("PrimBeamPFPTrackVertexZ", &PrimBeamPFPTrackVertexZ);
    chain->SetBranchAddress("PrimBeamPFPTrackDirX", &PrimBeamPFPTrackDirX);
    chain->SetBranchAddress("PrimBeamPFPTrackDirY", &PrimBeamPFPTrackDirY);
    chain->SetBranchAddress("PrimBeamPFPTrackDirZ", &PrimBeamPFPTrackDirZ);
    chain->SetBranchAddress("PrimBeamPFPTrackStartX", &PrimBeamPFPTrackStartX);
    chain->SetBranchAddress("PrimBeamPFPTrackStartY", &PrimBeamPFPTrackStartY);
    chain->SetBranchAddress("PrimBeamPFPTrackStartZ", &PrimBeamPFPTrackStartZ);
    chain->SetBranchAddress("PrimBeamPFPTrackEndX", &PrimBeamPFPTrackEndX);
    chain->SetBranchAddress("PrimBeamPFPTrackEndY", &PrimBeamPFPTrackEndY);
    chain->SetBranchAddress("PrimBeamPFPTrackEndZ", &PrimBeamPFPTrackEndZ);
    chain->SetBranchAddress("PrimBeamPFPTrackLength", &PrimBeamPFPTrackLength);
    chain->SetBranchAddress("PrimBeamPFPShowerVertexX", &PrimBeamPFPShowerVertexX);
    chain->SetBranchAddress("PrimBeamPFPShowerVertexY", &PrimBeamPFPShowerVertexY);
    chain->SetBranchAddress("PrimBeamPFPShowerVertexZ", &PrimBeamPFPShowerVertexZ);
    chain->SetBranchAddress("PrimBeamPFPShowerDirX", &PrimBeamPFPShowerDirX);
    chain->SetBranchAddress("PrimBeamPFPShowerDirY", &PrimBeamPFPShowerDirY);
    chain->SetBranchAddress("PrimBeamPFPShowerDirZ", &PrimBeamPFPShowerDirZ);
    chain->SetBranchAddress("PrimBeamPFPShowerdEdx", &PrimBeamPFPShowerdEdx);
    chain->SetBranchAddress("PrimBeamPFPShowerLength", &PrimBeamPFPShowerLength);
    chain->SetBranchAddress("PrimBeamPFPShowerEnergy", &PrimBeamPFPShowerEnergy);

    chain->SetBranchAddress("PrimBeamPFPX", &PrimBeamPFPX);
    chain->SetBranchAddress("PrimBeamPFPY", &PrimBeamPFPY);
    chain->SetBranchAddress("PrimBeamPFPZ", &PrimBeamPFPZ);

    chain->SetBranchAddress("ChildBeamPFPKey", &ChildBeamPFPKey);
    chain->SetBranchAddress("ChildBeamPFPPdg", &ChildBeamPFPPdg);
    chain->SetBranchAddress("ChildBeamPFPChildren", &ChildBeamPFPChildren);
    chain->SetBranchAddress("ChildBeamPFPTrackScore", &ChildBeamPFPTrackScore);
    chain->SetBranchAddress("ChildBeamPFPCNNtrack", &ChildBeamPFPCNNtrack);
    chain->SetBranchAddress("ChildBeamPFPCNNshower", &ChildBeamPFPCNNshower);
    chain->SetBranchAddress("ChildBeamPFPCNNmichel", &ChildBeamPFPCNNmichel);
    chain->SetBranchAddress("ChildBeamPFPTrackPid", &ChildBeamPFPTrackPid);
    chain->SetBranchAddress("ChildBeamPFPTrackVertexX", &ChildBeamPFPTrackVertexX);
    chain->SetBranchAddress("ChildBeamPFPTrackVertexY", &ChildBeamPFPTrackVertexY);
    chain->SetBranchAddress("ChildBeamPFPTrackVertexZ", &ChildBeamPFPTrackVertexZ);
    chain->SetBranchAddress("ChildBeamPFPTrackDirX", &ChildBeamPFPTrackDirX);
    chain->SetBranchAddress("ChildBeamPFPTrackDirY", &ChildBeamPFPTrackDirY);
    chain->SetBranchAddress("ChildBeamPFPTrackDirZ", &ChildBeamPFPTrackDirZ);
    chain->SetBranchAddress("ChildBeamPFPTrackStartX", &ChildBeamPFPTrackStartX);
    chain->SetBranchAddress("ChildBeamPFPTrackStartY", &ChildBeamPFPTrackStartY);
    chain->SetBranchAddress("ChildBeamPFPTrackStartZ", &ChildBeamPFPTrackStartZ);
    chain->SetBranchAddress("ChildBeamPFPTrackEndX", &ChildBeamPFPTrackEndX);
    chain->SetBranchAddress("ChildBeamPFPTrackEndY", &ChildBeamPFPTrackEndY);
    chain->SetBranchAddress("ChildBeamPFPTrackEndZ", &ChildBeamPFPTrackEndZ);
    chain->SetBranchAddress("ChildBeamPFPTrackLength", &ChildBeamPFPTrackLength);
    chain->SetBranchAddress("ChildBeamPFPShowerVertexX", &ChildBeamPFPShowerVertexX);
    chain->SetBranchAddress("ChildBeamPFPShowerVertexY", &ChildBeamPFPShowerVertexY);
    chain->SetBranchAddress("ChildBeamPFPShowerVertexZ", &ChildBeamPFPShowerVertexZ);
    chain->SetBranchAddress("ChildBeamPFPShowerDirX", &ChildBeamPFPShowerDirX);
    chain->SetBranchAddress("ChildBeamPFPShowerDirY", &ChildBeamPFPShowerDirY);
    chain->SetBranchAddress("ChildBeamPFPShowerDirZ", &ChildBeamPFPShowerDirZ);
    chain->SetBranchAddress("ChildBeamPFPShowerdEdx", &ChildBeamPFPShowerdEdx);
    chain->SetBranchAddress("ChildBeamPFPShowerLength", &ChildBeamPFPShowerLength);
    chain->SetBranchAddress("ChildBeamPFPShowerEnergy", &ChildBeamPFPShowerEnergy);

    chain->SetBranchAddress("ChildBeamPFPX", &ChildBeamPFPX);
    chain->SetBranchAddress("ChildBeamPFPY", &ChildBeamPFPY);
    chain->SetBranchAddress("ChildBeamPFPZ", &ChildBeamPFPZ);

    // 5. Histogramların Tanımlanması (Binning ayarlarını kendinize göre optimize edebilirsiniz)
    TH1I *h_eventID = new TH1I("h_eventID", "Event ID;ID;Entries", 100, 0, 100000);
    TH1F *h_BeamTOF = new TH1F("h_BeamTOF", "Beam TOF;TOF (ns);Entries", 100, 0, 200);
    TH1I *h_BeamCKov0 = new TH1I("h_BeamCKov0", "Beam CKov0;Status;Entries", 5, -1, 4);
    TH1I *h_BeamCKov1 = new TH1I("h_BeamCKov1", "Beam CKov1;Status;Entries", 5, -1, 4);
    TH1D *h_BeamMomentum = new TH1D("h_BeamMomentum", "Beam Momentum;p (GeV/c);Entries", 100, 0, 10);
    TH1I *h_PrimaryKey = new TH1I("h_PrimaryKey", "Primary Key;Key;Entries", 100, 0, 1000);
    TH1I *h_PrimaryPdg = new TH1I("h_PrimaryPdg", "Primary PDG;PDG;Entries", 100, -5000, 5000);
    TH1I *h_NPFParticles = new TH1I("h_NPFParticles", "Number of PFParticles;N;Entries", 50, 0, 200);
    TH1I *h_NPrimaryChildren = new TH1I("h_NPrimaryChildren", "Number of Primary Children;N;Entries", 20, 0, 50);

    // CNN ve Track/Shower Skalerleri için Histogramlar
    TH1F *h_PrimBeamPFPCNNtrack = new TH1F("h_PrimBeamPFPCNNtrack", "PrimBeam CNN Track Score;Score;Entries", 50, 0, 1);
    TH1F *h_PrimBeamPFPCNNshower = new TH1F("h_PrimBeamPFPCNNshower", "PrimBeam CNN Shower Score;Score;Entries", 50, 0, 1);
    TH1F *h_PrimBeamPFPCNNmichel = new TH1F("h_PrimBeamPFPCNNmichel", "PrimBeam CNN Michel Score;Score;Entries", 50, 0, 1);
    TH1I *h_PrimBeamPFPTrackPid = new TH1I("h_PrimBeamPFPTrackPid", "PrimBeam Track PID;PID;Entries", 100, -5000, 5000);
    TH1F *h_PrimBeamPFPTrackScore = new TH1F("h_PrimBeamPFPTrackScore", "PrimBeam Track Score;Score;Entries", 50, 0, 1);
    
    // Vertex ve Doğrultu (Direction) Histogramları
    TH1F *h_PrimBeamPFPTrackVertexX = new TH1F("h_PrimBeamPFPTrackVertexX", "PrimBeam Vertex X;X (cm);Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPTrackVertexY = new TH1F("h_PrimBeamPFPTrackVertexY", "PrimBeam Vertex Y;Y (cm);Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPTrackVertexZ = new TH1F("h_PrimBeamPFPTrackVertexZ", "PrimBeam Vertex Z;Z (cm);Entries", 100, -50, 500);
    TH1F *h_PrimBeamPFPTrackDirX = new TH1F("h_PrimBeamPFPTrackDirX", "PrimBeam Track Dir X;cos#theta_{x};Entries", 50, -1, 1);
    TH1F *h_PrimBeamPFPTrackDirY = new TH1F("h_PrimBeamPFPTrackDirY", "PrimBeam Track Dir Y;cos#theta_{y};Entries", 50, -1, 1);
    TH1F *h_PrimBeamPFPTrackDirZ = new TH1F("h_PrimBeamPFPTrackDirZ", "PrimBeam Track Dir Z;cos#theta_{z};Entries", 50, -1, 1);
    
    // Start / End Noktaları ve Uzunluk
    TH1F *h_PrimBeamPFPTrackStartX = new TH1F("h_PrimBeamPFPTrackStartX", "PrimBeam Track Start X;X (cm);Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPTrackStartY = new TH1F("h_PrimBeamPFPTrackStartY", "PrimBeam Track Start Y;Y (cm);Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPTrackStartZ = new TH1F("h_PrimBeamPFPTrackStartZ", "PrimBeam Track Start Z;Z (cm);Entries", 100, -50, 500);
    TH1F *h_PrimBeamPFPTrackEndX = new TH1F("h_PrimBeamPFPTrackEndX", "PrimBeam Track End X;X (cm);Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPTrackEndY = new TH1F("h_PrimBeamPFPTrackEndY", "PrimBeam Track End Y;Y (cm);Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPTrackEndZ = new TH1F("h_PrimBeamPFPTrackEndZ", "PrimBeam Track End Z;Z (cm);Entries", 100, -50, 500);
    TH1F *h_PrimBeamPFPTrackLength = new TH1F("h_PrimBeamPFPTrackLength", "PrimBeam Track Length;Length (cm);Entries", 100, 0, 500);

    // Shower Detayları
    TH1F *h_PrimBeamPFPShowerVertexX = new TH1F("h_PrimBeamPFPShowerVertexX", "PrimBeam Shower Vertex X;X (cm);Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPShowerVertexY = new TH1F("h_PrimBeamPFPShowerVertexY", "PrimBeam Shower Vertex Y;Y (cm);Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPShowerVertexZ = new TH1F("h_PrimBeamPFPShowerVertexZ", "PrimBeam Shower Vertex Z;Z (cm);Entries", 100, -50, 500);
    TH1F *h_PrimBeamPFPShowerDirX = new TH1F("h_PrimBeamPFPShowerDirX", "PrimBeam Shower Dir X;cos#theta_{x};Entries", 50, -1, 1);
    TH1F *h_PrimBeamPFPShowerDirY = new TH1F("h_PrimBeamPFPShowerDirY", "PrimBeam Shower Dir Y;cos#theta_{y};Entries", 50, -1, 1);
    TH1F *h_PrimBeamPFPShowerDirZ = new TH1F("h_PrimBeamPFPShowerDirZ", "PrimBeam Shower Dir Z;cos#theta_{z};Entries", 50, -1, 1);
    TH1F *h_PrimBeamPFPShowerdEdx = new TH1F("h_PrimBeamPFPShowerdEdx", "PrimBeam Shower dE/dx;dE/dx (MeV/cm);Entries", 100, 0, 20);
    TH1F *h_PrimBeamPFPShowerLength = new TH1F("h_PrimBeamPFPShowerLength", "PrimBeam Shower Length;Length (cm);Entries", 100, 0, 500);
    TH1F *h_PrimBeamPFPShowerEnergy = new TH1F("h_PrimBeamPFPShowerEnergy", "PrimBeam Shower Energy;Energy (MeV);Entries", 100, 0, 5000);

    // Vektörlerin (1B ve 2B) Değerlerini Toplamak İçin Ortak Histogramlar
    TH1D *h_BeamPositions_val = new TH1D("h_BeamPositions_val", "Beam Positions Values;Pos;Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPX_val = new TH1F("h_PrimBeamPFPX_val", "PrimBeam PFP X Values;X;Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPY_val = new TH1F("h_PrimBeamPFPY_val", "PrimBeam PFP Y Values;Y;Entries", 100, -100, 100);
    TH1F *h_PrimBeamPFPZ_val = new TH1F("h_PrimBeamPFPZ_val", "PrimBeam PFP Z Values;Z;Entries", 100, -50, 500);

    TH1I *h_ChildBeamPFPKey_val = new TH1I("h_ChildBeamPFPKey_val", "Child PFP Key Values;Key;Entries", 100, 0, 1000);
    TH1I *h_ChildBeamPFPPdg_val = new TH1I("h_ChildBeamPFPPdg_val", "Child PFP PDG Values;PDG;Entries", 100, -5000, 5000);
    TH1I *h_ChildBeamPFPChildren_val = new TH1I("h_ChildBeamPFPChildren_val", "Child PFP Children Values;Value;Entries", 50, 0, 100);
    TH1F *h_ChildBeamPFPTrackScore_val = new TH1F("h_ChildBeamPFPTrackScore_val", "Child Track Score;Score;Entries", 50, 0, 1);
    TH1F *h_ChildBeamPFPCNNtrack_val = new TH1F("h_ChildBeamPFPCNNtrack_val", "Child CNN Track;Score;Entries", 50, 0, 1);
    TH1F *h_ChildBeamPFPCNNshower_val = new TH1F("h_ChildBeamPFPCNNshower_val", "Child CNN Shower;Score;Entries", 50, 0, 1);
    TH1F *h_ChildBeamPFPCNNmichel_val = new TH1F("h_ChildBeamPFPCNNmichel_val", "Child CNN Michel;Score;Entries", 50, 0, 1);
    TH1I *h_ChildBeamPFPTrackPid_val = new TH1I("h_ChildBeamPFPTrackPid_val", "Child Track PID;PID;Entries", 100, -5000, 5000);
    
    TH1F *h_ChildBeamPFPTrackVertexX_val = new TH1F("h_ChildBeamPFPTrackVertexX_val", "Child Vertex X;X (cm);Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPTrackVertexY_val = new TH1F("h_ChildBeamPFPTrackVertexY_val", "Child Vertex Y;Y (cm);Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPTrackVertexZ_val = new TH1F("h_ChildBeamPFPTrackVertexZ_val", "Child Vertex Z;Z (cm);Entries", 100, -50, 500);
    TH1F *h_ChildBeamPFPTrackDirX_val = new TH1F("h_ChildBeamPFPTrackDirX_val", "Child Track Dir X;cos#theta;Entries", 50, -1, 1);
    TH1F *h_ChildBeamPFPTrackDirY_val = new TH1F("h_ChildBeamPFPTrackDirY_val", "Child Track Dir Y;cos#theta;Entries", 50, -1, 1);
    TH1F *h_ChildBeamPFPTrackDirZ_val = new TH1F("h_ChildBeamPFPTrackDirZ_val", "Child Track Dir Z;cos#theta;Entries", 50, -1, 1);
    
    TH1F *h_ChildBeamPFPTrackStartX_val = new TH1F("h_ChildBeamPFPTrackStartX_val", "Child Track Start X;X;Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPTrackStartY_val = new TH1F("h_ChildBeamPFPTrackStartY_val", "Child Track Start Y;Y;Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPTrackStartZ_val = new TH1F("h_ChildBeamPFPTrackStartZ_val", "Child Track Start Z;Z;Entries", 100, -50, 500);
    TH1F *h_ChildBeamPFPTrackEndX_val = new TH1F("h_ChildBeamPFPTrackEndX_val", "Child Track End X;X;Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPTrackEndY_val = new TH1F("h_ChildBeamPFPTrackEndY_val", "Child Track End Y;Y;Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPTrackEndZ_val = new TH1F("h_ChildBeamPFPTrackEndZ_val", "Child Track End Z;Z;Entries", 100, -50, 500);
    TH1F *h_ChildBeamPFPTrackLength_val = new TH1F("h_ChildBeamPFPTrackLength_val", "Child Track Length;L (cm);Entries", 100, 0, 500);

    TH1F *h_ChildBeamPFPShowerVertexX_val = new TH1F("h_ChildBeamPFPShowerVertexX_val", "Child Shower Vertex X;X;Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPShowerVertexY_val = new TH1F("h_ChildBeamPFPShowerVertexY_val", "Child Shower Vertex Y;Y;Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPShowerVertexZ_val = new TH1F("h_ChildBeamPFPShowerVertexZ_val", "Child Shower Vertex Z;Z;Entries", 100, -50, 500);
    TH1F *h_ChildBeamPFPShowerDirX_val = new TH1F("h_ChildBeamPFPShowerDirX_val", "Child Shower Dir X;cos#theta;Entries", 50, -1, 1);
    TH1F *h_ChildBeamPFPShowerDirY_val = new TH1F("h_ChildBeamPFPShowerDirY_val", "Child Shower Dir Y;cos#theta;Entries", 50, -1, 1);
    TH1F *h_ChildBeamPFPShowerDirZ_val = new TH1F("h_ChildBeamPFPShowerDirZ_val", "Child Shower Dir Z;cos#theta;Entries", 50, -1, 1);
    TH1F *h_ChildBeamPFPShowerdEdx_val = new TH1F("h_ChildBeamPFPShowerdEdx_val", "Child Shower dE/dx;dE/dx;Entries", 100, 0, 20);
    TH1F *h_ChildBeamPFPShowerLength_val = new TH1F("h_ChildBeamPFPShowerLength_val", "Child Shower Length;L (cm);Entries", 100, 0, 500);
    TH1F *h_ChildBeamPFPShowerEnergy_val = new TH1F("h_ChildBeamPFPShowerEnergy_val", "Child Shower Energy;E (MeV);Entries", 100, 0, 5000);

    // 2B Vektörlerin Değerleri İçin Histogramlar (Br 70 - 72)
    TH1F *h_ChildBeamPFPX_2B_val = new TH1F("h_ChildBeamPFPX_2B_val", "Child 2B PFP X Values;X;Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPY_2B_val = new TH1F("h_ChildBeamPFPY_2B_val", "Child 2B PFP Y Values;Y;Entries", 100, -100, 100);
    TH1F *h_ChildBeamPFPZ_2B_val = new TH1F("h_ChildBeamPFPZ_2B_val", "Child 2B PFP Z Values;Z;Entries", 100, -50, 500);


    // 6. Döngü İçinde Veriler (Fill)
    std::cout << "Histogramlar dolduruluyor. Toplam Entry: " << nentries << std::endl;
    for (Long64_t i = 0; i < nentries; i++) {
        chain->GetEntry(i);
        
        // Skaler Değerler
        h_eventID->Fill(eventID);
        h_BeamTOF->Fill(BeamTOF);
        h_BeamCKov0->Fill(BeamCKov0);
        h_BeamCKov1->Fill(BeamCKov1);
        h_BeamMomentum->Fill(BeamMomentum);
        h_PrimaryKey->Fill(PrimaryKey);
        h_PrimaryPdg->Fill(PrimaryPdg);
        h_NPFParticles->Fill(NPFParticles);
        h_NPrimaryChildren->Fill(NPrimaryChildren);
        
        h_PrimBeamPFPCNNtrack->Fill(PrimBeamPFPCNNtrack);
        h_PrimBeamPFPCNNshower->Fill(PrimBeamPFPCNNshower);
        h_PrimBeamPFPCNNmichel->Fill(PrimBeamPFPCNNmichel);
        h_PrimBeamPFPTrackPid->Fill(PrimBeamPFPTrackPid);
        h_PrimBeamPFPTrackScore->Fill(PrimBeamPFPTrackScore);
        h_PrimBeamPFPTrackVertexX->Fill(PrimBeamPFPTrackVertexX);
        h_PrimBeamPFPTrackVertexY->Fill(PrimBeamPFPTrackVertexY);
        h_PrimBeamPFPTrackVertexZ->Fill(PrimBeamPFPTrackVertexZ);
        h_PrimBeamPFPTrackDirX->Fill(PrimBeamPFPTrackDirX);
        h_PrimBeamPFPTrackDirY->Fill(PrimBeamPFPTrackDirY);
        h_PrimBeamPFPTrackDirZ->Fill(PrimBeamPFPTrackDirZ);
        h_PrimBeamPFPTrackStartX->Fill(PrimBeamPFPTrackStartX);
        h_PrimBeamPFPTrackStartY->Fill(PrimBeamPFPTrackStartY);
        h_PrimBeamPFPTrackStartZ->Fill(PrimBeamPFPTrackStartZ);
        h_PrimBeamPFPTrackEndX->Fill(PrimBeamPFPTrackEndX);
        h_PrimBeamPFPTrackEndY->Fill(PrimBeamPFPTrackEndY);
        h_PrimBeamPFPTrackEndZ->Fill(PrimBeamPFPTrackEndZ);
        h_PrimBeamPFPTrackLength->Fill(PrimBeamPFPTrackLength);
        
        h_PrimBeamPFPShowerVertexX->Fill(PrimBeamPFPShowerVertexX);
        h_PrimBeamPFPShowerVertexY->Fill(PrimBeamPFPShowerVertexY);
        h_PrimBeamPFPShowerVertexZ->Fill(PrimBeamPFPShowerVertexZ);
        h_PrimBeamPFPShowerDirX->Fill(PrimBeamPFPShowerDirX);
        h_PrimBeamPFPShowerDirY->Fill(PrimBeamPFPShowerDirY);
        h_PrimBeamPFPShowerDirZ->Fill(PrimBeamPFPShowerDirZ);
        h_PrimBeamPFPShowerdEdx->Fill(PrimBeamPFPShowerdEdx);
        h_PrimBeamPFPShowerLength->Fill(PrimBeamPFPShowerLength);
        h_PrimBeamPFPShowerEnergy->Fill(PrimBeamPFPShowerEnergy);

        // 1B Vektörlerin İçindeki Elemanlar
        if (BeamPositions) { for (double val : *BeamPositions) h_BeamPositions_val->Fill(val); }
        if (PrimBeamPFPX)  { for (float val : *PrimBeamPFPX)  h_PrimBeamPFPX_val->Fill(val); }
        if (PrimBeamPFPY)  { for (float val : *PrimBeamPFPY)  h_PrimBeamPFPY_val->Fill(val); }
        if (PrimBeamPFPZ)  { for (float val : *PrimBeamPFPZ)  h_PrimBeamPFPZ_val->Fill(val); }

        if (ChildBeamPFPKey)        { for (int val : *ChildBeamPFPKey)        h_ChildBeamPFPKey_val->Fill(val); }
        if (ChildBeamPFPPdg)        { for (int val : *ChildBeamPFPPdg)        h_ChildBeamPFPPdg_val->Fill(val); }
        if (ChildBeamPFPChildren)   { for (int val : *ChildBeamPFPChildren)   h_ChildBeamPFPChildren_val->Fill(val); }
        if (ChildBeamPFPTrackScore) { for (float val : *ChildBeamPFPTrackScore) h_ChildBeamPFPTrackScore_val->Fill(val); }
        if (ChildBeamPFPCNNtrack)   { for (float val : *ChildBeamPFPCNNtrack)   h_ChildBeamPFPCNNtrack_val->Fill(val); }
        if (ChildBeamPFPCNNshower)  { for (float val : *ChildBeamPFPCNNshower)  h_ChildBeamPFPCNNshower_val->Fill(val); }
        if (ChildBeamPFPCNNmichel)  { for (float val : *ChildBeamPFPCNNmichel)  h_ChildBeamPFPCNNmichel_val->Fill(val); }
        if (ChildBeamPFPTrackPid)   { for (int val : *ChildBeamPFPTrackPid)   h_ChildBeamPFPTrackPid_val->Fill(val); }
        
        if (ChildBeamPFPTrackVertexX) { for (float val : *ChildBeamPFPTrackVertexX) h_ChildBeamPFPTrackVertexX_val->Fill(val); }
        if (ChildBeamPFPTrackVertexY) { for (float val : *ChildBeamPFPTrackVertexY) h_ChildBeamPFPTrackVertexY_val->Fill(val); }
        if (ChildBeamPFPTrackVertexZ) { for (float val : *ChildBeamPFPTrackVertexZ) h_ChildBeamPFPTrackVertexZ_val->Fill(val); }
        if (ChildBeamPFPTrackDirX)    { for (float val : *ChildBeamPFPTrackDirX)    h_ChildBeamPFPTrackDirX_val->Fill(val); }
        if (ChildBeamPFPTrackDirY)    { for (float val : *ChildBeamPFPTrackDirY)    h_ChildBeamPFPTrackDirY_val->Fill(val); }
        if (ChildBeamPFPTrackDirZ)    { for (float val : *ChildBeamPFPTrackDirZ)    h_ChildBeamPFPTrackDirZ_val->Fill(val); }
        
        if (ChildBeamPFPTrackStartX) { for (float val : *ChildBeamPFPTrackStartX) h_ChildBeamPFPTrackStartX_val->Fill(val); }
        if (ChildBeamPFPTrackStartY) { for (float val : *ChildBeamPFPTrackStartY) h_ChildBeamPFPTrackStartY_val->Fill(val); }
        if (ChildBeamPFPTrackStartZ) { for (float val : *ChildBeamPFPTrackStartZ) h_ChildBeamPFPTrackStartZ_val->Fill(val); }
        if (ChildBeamPFPTrackEndX)   { for (float val : *ChildBeamPFPTrackEndX)   h_ChildBeamPFPTrackEndX_val->Fill(val); }
        if (ChildBeamPFPTrackEndY)   { for (float val : *ChildBeamPFPTrackEndY)   h_ChildBeamPFPTrackEndY_val->Fill(val); }
        if (ChildBeamPFPTrackEndZ)   { for (float val : *ChildBeamPFPTrackEndZ)   h_ChildBeamPFPTrackEndZ_val->Fill(val); }
        if (ChildBeamPFPTrackLength) { for (float val : *ChildBeamPFPTrackLength) h_ChildBeamPFPTrackLength_val->Fill(val); }

        if (ChildBeamPFPShowerVertexX) { for (float val : *ChildBeamPFPShowerVertexX) h_ChildBeamPFPShowerVertexX_val->Fill(val); }
        if (ChildBeamPFPShowerVertexY) { for (float val : *ChildBeamPFPShowerVertexY) h_ChildBeamPFPShowerVertexY_val->Fill(val); }
        if (ChildBeamPFPShowerVertexZ) { for (float val : *ChildBeamPFPShowerVertexZ) h_ChildBeamPFPShowerVertexZ_val->Fill(val); }
        if (ChildBeamPFPShowerDirX)    { for (float val : *ChildBeamPFPShowerDirX)    h_ChildBeamPFPShowerDirX_val->Fill(val); }
        if (ChildBeamPFPShowerDirY)    { for (float val : *ChildBeamPFPShowerDirY)    h_ChildBeamPFPShowerDirY_val->Fill(val); }
        if (ChildBeamPFPShowerDirZ)    { for (float val : *ChildBeamPFPShowerDirZ)    h_ChildBeamPFPShowerDirZ_val->Fill(val); }
        if (ChildBeamPFPShowerdEdx)    { for (float val : *ChildBeamPFPShowerdEdx)    h_ChildBeamPFPShowerdEdx_val->Fill(val); }
        if (ChildBeamPFPShowerLength)  { for (float val : *ChildBeamPFPShowerLength)  h_ChildBeamPFPShowerLength_val->Fill(val); }
        if (ChildBeamPFPShowerEnergy)  { for (float val : *ChildBeamPFPShowerEnergy)  h_ChildBeamPFPShowerEnergy_val->Fill(val); }

        // 2B Vektörlerin İçindeki Tüm Elemanlar (Nested Loops)
        if (ChildBeamPFPX) { for (const auto& vec : *ChildBeamPFPX) { for (float val : vec) h_ChildBeamPFPX_2B_val->Fill(val); } }
        if (ChildBeamPFPY) { for (const auto& vec : *ChildBeamPFPY) { for (float val : vec) h_ChildBeamPFPY_2B_val->Fill(val); } }
        if (ChildBeamPFPZ) { for (const auto& vec : *ChildBeamPFPZ) { for (float val : vec) h_ChildBeamPFPZ_2B_val->Fill(val); } }
    }

    
    outputFile->Write();
    outputFile->Close();
    
    std::cout << "Islem tamamlandi! 'Pandora_Histograms.root' dosyasi kaydedildi." << std::endl;

  
    delete chain;
}
[nbos
