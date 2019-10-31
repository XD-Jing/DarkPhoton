#ifndef SlimROOT_h
#define SlimROOT_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class SlimROOT {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           EventID;
   float        Rndm[4];
   Int_t           Nb_tagTrk;
   Int_t           tTrkID[1000];   //[Nb_tagTrk]
   Int_t           tChamberNb[1000];   //[Nb_tagTrk]
   float        tTrkE[1000];   //[Nb_tagTrk]
   float        tTrkTime[1000];   //[Nb_tagTrk]
   float        tTrkPosX[1000];   //[Nb_tagTrk]
   float        tTrkPosY[1000];   //[Nb_tagTrk]
   float        tTrkPosZ[1000];   //[Nb_tagTrk]
   Int_t           Nb_recTrk;
   Int_t           rTrkID[5000];   //[Nb_recTrk]
   Int_t           rChamberNb[5000];   //[Nb_recTrk]
   float        rTrkTime[5000];   //[Nb_recTrk]
   float        rTrkE[5000];   //[Nb_recTrk]
   float        rTrkPosX[5000];   //[Nb_recTrk]
   float        rTrkPosY[5000];   //[Nb_recTrk]
   float        rTrkPosZ[5000];   //[Nb_recTrk]
   Int_t           Nb_ECal;
   float        ECalE[5000];   //[Nb_ECal]
   float        ECalTime[5000];   //[Nb_ECal]
   float        ECalPosX[5000];   //[Nb_ECal]
   float        ECalPosY[5000];   //[Nb_ECal]
   float        ECalPosZ[5000];   //[Nb_ECal]
   Int_t           Nb_HCal;
   float        HCalE[5000];   //[Nb_HCal]
   float        HCalTime[5000];   //[Nb_HCal]
   float        HCalPosX[5000];   //[Nb_HCal]
   float        HCalPosY[5000];   //[Nb_HCal]
   float        HCalPosZ[5000];   //[Nb_HCal]
   Int_t           Nb_HCap;
   float        HCapE[1000];   //[Nb_HCap]
   float        HCapTime[1000];   //[Nb_HCap]
   float        HCapPosX[1000];   //[Nb_HCap]
   float        HCapPosY[1000];   //[Nb_HCap]
   float        HCapPosZ[1000];   //[Nb_HCap]
   Int_t           Nb_HSide;
   float        HSideE[1000];   //[Nb_HSide]
   float        HSideTime[1000];   //[Nb_HSide]
   float        HSidePosX[1000];   //[Nb_HSide]
   float        HSidePosY[1000];   //[Nb_HSide]
   float        HSidePosZ[1000];   //[Nb_HSide]

   // List of branches
   TBranch        *b_EventID;   //!
   TBranch        *b_Rndm;   //!
   TBranch        *b_Nb_tagTrk;   //!
   TBranch        *b_tTrkID;   //!
   TBranch        *b_tChamberNb;   //!
   TBranch        *b_tTrkE;   //!
   TBranch        *b_tTrkTime;   //!
   TBranch        *b_tTrkPosX;   //!
   TBranch        *b_tTrkPosY;   //!
   TBranch        *b_tTrkPosZ;   //!
   TBranch        *b_Nb_recTrk;   //!
   TBranch        *b_rTrkID;   //!
   TBranch        *b_rChamberNb;   //!
   TBranch        *b_rTrkTime;   //!
   TBranch        *b_rTrkE;   //!
   TBranch        *b_rTrkPosX;   //!
   TBranch        *b_rTrkPosY;   //!
   TBranch        *b_rTrkPosZ;   //!
   TBranch        *b_Nb_ECal;   //!
   TBranch        *b_ECalE;   //!
   TBranch        *b_ECalTime;   //!
   TBranch        *b_ECalPosX;   //!
   TBranch        *b_ECalPosY;   //!
   TBranch        *b_ECalPosZ;   //!
   TBranch        *b_Nb_HCal;   //!
   TBranch        *b_HCalE;   //!
   TBranch        *b_HCalTime;   //!
   TBranch        *b_HCalPosX;   //!
   TBranch        *b_HCalPosY;   //!
   TBranch        *b_HCalPosZ;   //!
   TBranch        *b_Nb_HCap;   //!
   TBranch        *b_HCapE;   //!
   TBranch        *b_HCapTime;   //!
   TBranch        *b_HCapPosX;   //!
   TBranch        *b_HCapPosY;   //!
   TBranch        *b_HCapPosZ;   //!
   TBranch        *b_Nb_HSide;   //!
   TBranch        *b_HSideE;   //!
   TBranch        *b_HSideTime;   //!
   TBranch        *b_HSidePosX;   //!
   TBranch        *b_HSidePosY;   //!
   TBranch        *b_HSidePosZ;   //!

   SlimROOT(TTree *tree=0);
   virtual ~SlimROOT();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef SlimROOT_cxx
SlimROOT::SlimROOT(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("darkphoton.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("darkphoton.root");
      }
      f->GetObject("treeDP",tree);

   }
   Init(tree);
}

SlimROOT::~SlimROOT()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t SlimROOT::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t SlimROOT::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void SlimROOT::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("EventID", &EventID, &b_EventID);
   fChain->SetBranchAddress("Rndm", Rndm, &b_Rndm);
   fChain->SetBranchAddress("Nb_tagTrk", &Nb_tagTrk, &b_Nb_tagTrk);
   fChain->SetBranchAddress("tTrkID", tTrkID, &b_tTrkID);
   fChain->SetBranchAddress("tChamberNb", tChamberNb, &b_tChamberNb);
   fChain->SetBranchAddress("tTrkE", tTrkE, &b_tTrkE);
   fChain->SetBranchAddress("tTrkTime", tTrkTime, &b_tTrkTime);
   fChain->SetBranchAddress("tTrkPosX", tTrkPosX, &b_tTrkPosX);
   fChain->SetBranchAddress("tTrkPosY", tTrkPosY, &b_tTrkPosY);
   fChain->SetBranchAddress("tTrkPosZ", tTrkPosZ, &b_tTrkPosZ);
   fChain->SetBranchAddress("Nb_recTrk", &Nb_recTrk, &b_Nb_recTrk);
   fChain->SetBranchAddress("rTrkID", rTrkID, &b_rTrkID);
   fChain->SetBranchAddress("rChamberNb", rChamberNb, &b_rChamberNb);
   fChain->SetBranchAddress("rTrkTime", rTrkTime, &b_rTrkTime);
   fChain->SetBranchAddress("rTrkE", rTrkE, &b_rTrkE);
   fChain->SetBranchAddress("rTrkPosX", rTrkPosX, &b_rTrkPosX);
   fChain->SetBranchAddress("rTrkPosY", rTrkPosY, &b_rTrkPosY);
   fChain->SetBranchAddress("rTrkPosZ", rTrkPosZ, &b_rTrkPosZ);
   fChain->SetBranchAddress("Nb_ECal", &Nb_ECal, &b_Nb_ECal);
   fChain->SetBranchAddress("ECalE", ECalE, &b_ECalE);
   fChain->SetBranchAddress("ECalTime", ECalTime, &b_ECalTime);
   fChain->SetBranchAddress("ECalPosX", ECalPosX, &b_ECalPosX);
   fChain->SetBranchAddress("ECalPosY", ECalPosY, &b_ECalPosY);
   fChain->SetBranchAddress("ECalPosZ", ECalPosZ, &b_ECalPosZ);
   fChain->SetBranchAddress("Nb_HCal", &Nb_HCal, &b_Nb_HCal);
   fChain->SetBranchAddress("HCalE", HCalE, &b_HCalE);
   fChain->SetBranchAddress("HCalTime", HCalTime, &b_HCalTime);
   fChain->SetBranchAddress("HCalPosX", HCalPosX, &b_HCalPosX);
   fChain->SetBranchAddress("HCalPosY", HCalPosY, &b_HCalPosY);
   fChain->SetBranchAddress("HCalPosZ", HCalPosZ, &b_HCalPosZ);
   fChain->SetBranchAddress("Nb_HCap", &Nb_HCap, &b_Nb_HCap);
   fChain->SetBranchAddress("HCapE", HCapE, &b_HCapE);
   fChain->SetBranchAddress("HCapTime", HCapTime, &b_HCapTime);
   fChain->SetBranchAddress("HCapPosX", HCapPosX, &b_HCapPosX);
   fChain->SetBranchAddress("HCapPosY", HCapPosY, &b_HCapPosY);
   fChain->SetBranchAddress("HCapPosZ", HCapPosZ, &b_HCapPosZ);
   fChain->SetBranchAddress("Nb_HSide", &Nb_HSide, &b_Nb_HSide);
   fChain->SetBranchAddress("HSideE", HSideE, &b_HSideE);
   fChain->SetBranchAddress("HSideTime", HSideTime, &b_HSideTime);
   fChain->SetBranchAddress("HSidePosX", HSidePosX, &b_HSidePosX);
   fChain->SetBranchAddress("HSidePosY", HSidePosY, &b_HSidePosY);
   fChain->SetBranchAddress("HSidePosZ", HSidePosZ, &b_HSidePosZ);
   Notify();
}

Bool_t SlimROOT::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void SlimROOT::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t SlimROOT::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef SlimROOT_cxx
