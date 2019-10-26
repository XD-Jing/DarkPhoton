#include "RootManager.hh"
#include "EventMessenger.hh"
#include "CalorHit.hh"
#include "TrackerHit.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TVector3.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RootManager::RootManager()
:rootFile(0), tr(0), tmc(0),fStart(0),fEvtNb(100000)
{
    fMessenger = new EventMessenger(this);
    // Initialization
    EventID	            =0;
    for(int i=0;i<4;i++){
        Rndm[i]    = 0 ;
    }
    
    _id         = 0 ;
    _PDG        = 0 ;
    _ParentID   = 0 ;
    _Energy     = 0 ;
    eid         = 0 ;
    ePDG        = 0 ;
    eParentID   = 0 ;
    eEnergy     = 0 ;
    mc_Energy    = 0 ;
    for(int i=0;i<3;i++){
        _Momentum[i]    = 0 ;
        _VPos[i]        = 0 ;
        _EPos[i]        = 0 ; 
        eMomentum[i]    = 0 ;
        eVPos[i]        = 0 ;
        eEPos[i]        = 0 ; 
        mc_Momentum[i]   = 0 ;
        mc_Pos[i]        = 0 ;
    }

    Nb_tagTrk	        =0;
    Nb_recTrk	        =0;
    Nb_ECal             =0;
    Nb_HCal             =0;
    Nb_HCap             =0;
    Nb_HSide            =0;
    for(int i=0; i<MaxHits; i++) {
        tTrkID[i]	    = 0;
        tTrkTime[i]	    = 0;
        tTrkChamNb[i]   = 0;
        tTrkE[i]	    = 0;
        tTrkPosX[i]	    = 0;    
        tTrkPosY[i]	    = 0;    
        tTrkPosZ[i]	    = 0;    

        rTrkID[i]	    = 0;
        rTrkTime[i]	    = 0;
        rTrkChamNb[i]   = 0;
        rTrkE[i]	    = 0;
        rTrkPosX[i]	    = 0;    
        rTrkPosY[i]	    = 0;    
        rTrkPosZ[i]	    = 0;    

        ECalTime[i]	    = 0;
        ECalE[i]	    = 0;
        ECalPosX[i]	    = 0;
        ECalPosY[i]	    = 0;
        ECalPosZ[i]	    = 0;    

        HCalTime[i]	    = 0;
        HCalE[i]	    = 0;
        HCalPosX[i]	    = 0;
        HCalPosY[i]	    = 0;
        HCalPosZ[i]	    = 0;    

        HCapTime[i]	    = 0;
        HCapE[i]	    = 0;
        HCapPosX[i]	    = 0;
        HCapPosY[i]	    = 0;
        HCapPosZ[i]	    = 0;    

        HSideTime[i]	= 0;
        HSideE[i]	    = 0;
        HSidePosX[i]	= 0;
        HSidePosY[i]	= 0;
        HSidePosZ[i]	= 0;    
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RootManager::~RootManager()
{
    if(rootFile) delete rootFile;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RootManager::book(G4String fileName) 
{
    rootFile = new TFile(fileName,"RECREATE");

    if(!rootFile) { 
        G4cout << " RootManager::book :"
               << " problem creating the ROOT TFile "
               << G4endl;
        return;
    }

    tr = new TTree("treeDP","Dark Photon");
    tmc= new TTree("treeEGamma","Electron Gamma");
    te = new TTree("treeMC","MC");

    tmc->Branch("EventID",      &EventID    ,   "EventID/I");
    tmc->Branch("_id",          &_id        ,   "_id/I");
    tmc->Branch("_PDG",         &_PDG       ,   "_PDG/I");
    tmc->Branch("_ParentID",    &_ParentID  ,   "_ParentID/I");
    tmc->Branch("_Energy",      &_Energy    ,   "_Energy/D");
    tmc->Branch("_Momentum",    &_Momentum  ,   "_Momentum[3]/D");
    tmc->Branch("_VPos",        &_VPos      ,   "_VPos[3]/D");
    tmc->Branch("_EPos",        &_EPos      ,   "_EPos[3]/D");
    tmc->Branch("_CrePro",      &_CreateProcess,"_CrePro[20]/C");
    tmc->Branch("eid",          &eid        ,   "eid/I");
    tmc->Branch("ePDG",         &ePDG       ,   "ePDG/I");
    tmc->Branch("eParentID",    &eParentID  ,   "eParentID/I");
    tmc->Branch("eEnergy",      &eEnergy    ,   "eEnergy/D");
    tmc->Branch("eMomentum",    &eMomentum  ,   "eMomentum[3]/D");
    tmc->Branch("eVPos",        &eVPos      ,   "eVPos[3]/D");
    tmc->Branch("eEPos",        &eEPos      ,   "eEPos[3]/D");
    tmc->Branch("eCrePro",      &eCreateProcess,"eCrePro[20]/C");

    te->Branch("id",            &mc_id        , "id/I");
    te->Branch("PDG",           &mc_PDG       , "PDG/I");
    te->Branch("ParentID",      &mc_ParentID  , "ParentID/I");
    te->Branch("EventID",       &EventID    ,   "EventID/I");
    te->Branch("Energy",        &mc_Energy   ,   "Energy/D");
    te->Branch("Momentum",      &mc_Momentum ,   "Momentum[3]/D");
    te->Branch("Position",      &mc_Pos      ,   "Position[3]/D");
    te->Branch("CreProc" ,      &mc_CreateProcess,"CreProc[20]/C");

    tr->Branch("EventID",       &EventID    ,   "EventID/I");
    tr->Branch("Rndm",          &Rndm       ,   "Rndm[4]/D");

    tr->Branch("Nb_tagTrk",     &Nb_tagTrk  ,   "Nb_tagTrk/I");
    tr->Branch("tTrkID",         &tTrkID    ,   "tTrkID[Nb_tagTrk]/I");
    tr->Branch("tChamberNb",     &tTrkChamNb,   "tChamberNb[Nb_tagTrk]/I");
    tr->Branch("tTrkE",          &tTrkE     ,   "tTrkE[Nb_tagTrk]/D");
    tr->Branch("tTrkTime",       &tTrkTime  ,   "tTrkTime[Nb_tagTrk]/D");
    tr->Branch("tTrkPosX",       &tTrkPosX  ,   "tTrkPosX[Nb_tagTrk]/D");
    tr->Branch("tTrkPosY",       &tTrkPosY  ,   "tTrkPosY[Nb_tagTrk]/D");
    tr->Branch("tTrkPosZ",       &tTrkPosZ  ,   "tTrkPosZ[Nb_tagTrk]/D");

    tr->Branch("Nb_recTrk",     &Nb_recTrk  ,   "Nb_recTrk/I");
    tr->Branch("rTrkID",         &rTrkID    ,   "rTrkID[Nb_recTrk]/I");
    tr->Branch("rChamberNb",     &rTrkChamNb,   "rChamberNb[Nb_recTrk]/I");
    tr->Branch("rTrkTime",       &rTrkTime  ,   "rTrkTime[Nb_recTrk]/D");
    tr->Branch("rTrkE",          &rTrkE     ,   "rTrkE[Nb_recTrk]/D");
    tr->Branch("rTrkPosX",       &rTrkPosX  ,   "rTrkPosX[Nb_recTrk]/D");
    tr->Branch("rTrkPosY",       &rTrkPosY  ,   "rTrkPosY[Nb_recTrk]/D");
    tr->Branch("rTrkPosZ",       &rTrkPosZ  ,   "rTrkPosZ[Nb_recTrk]/D");

    tr->Branch("Nb_ECal",        &Nb_ECal   ,   "Nb_ECal/I");
    tr->Branch("ECalE",          &ECalE     ,   "ECalE[Nb_ECal]/D");
    tr->Branch("ECalTime",       &ECalTime  ,   "ECalTime[Nb_ECal]/D");
    tr->Branch("ECalPosX",       &ECalPosX  ,   "ECalPosX[Nb_ECal]/D");
    tr->Branch("ECalPosY",       &ECalPosY  ,   "ECalPosY[Nb_ECal]/D");
    tr->Branch("ECalPosZ",       &ECalPosZ  ,   "ECalPosZ[Nb_ECal]/D");

    tr->Branch("Nb_HCal",       &Nb_HCal    ,   "Nb_HCal/I");
    tr->Branch("HCalE",          &HCalE     ,   "HCalE[Nb_HCal]/D");
    tr->Branch("HCalTime",       &HCalTime  ,   "HCalTime[Nb_HCal]/D");
    tr->Branch("HCalPosX",       &HCalPosX  ,   "HCalPosX[Nb_HCal]/D");
    tr->Branch("HCalPosY",       &HCalPosY  ,   "HCalPosY[Nb_HCal]/D");
    tr->Branch("HCalPosZ",       &HCalPosZ  ,   "HCalPosZ[Nb_HCal]/D");

    tr->Branch("Nb_HCap",       &Nb_HCap    ,   "Nb_HCap/I");
    tr->Branch("HCapE",          &HCapE     ,   "HCapE[Nb_HCap]/D");
    tr->Branch("HCapTime",       &HCapTime  ,   "HCapTime[Nb_HCap]/D");
    tr->Branch("HCapPosX",       &HCapPosX  ,   "HCapPosX[Nb_HCap]/D");
    tr->Branch("HCapPosY",       &HCapPosY  ,   "HCapPosY[Nb_HCap]/D");
    tr->Branch("HCapPosZ",       &HCapPosZ  ,   "HCapPosZ[Nb_HCap]/D");

    tr->Branch("Nb_HSide",       &Nb_HSide    ,   "Nb_HSide/I");
    tr->Branch("HSideE",          &HSideE     ,   "HSideE[Nb_HSide]/D");
    tr->Branch("HSideTime",       &HSideTime  ,   "HSideTime[Nb_HSide]/D");
    tr->Branch("HSidePosX",       &HSidePosX  ,   "HSidePosX[Nb_HSide]/D");
    tr->Branch("HSidePosY",       &HSidePosY  ,   "HSidePosY[Nb_HSide]/D");
    tr->Branch("HSidePosZ",       &HSidePosZ  ,   "HSidePosZ[Nb_HSide]/D");


    G4cout << "===> ROOT file is opened in " << fileName << G4endl;
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::save()
{
    if (rootFile) {
        rootFile->WriteTObject(tr,"","Overwrite");
        rootFile->WriteTObject(tmc,"","Overwrite");
        rootFile->WriteTObject(te,"","Overwrite");
        rootFile->Close();
        G4cout << "===> Simulation Tree is saved \n" << G4endl;
    }
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillEG(Int_t ID, MCParticle* mc, MCParticle* mce) {

    EventID     = ID + fEvtNb*fStart ;
    _id         = mc->Getid()        ;
    _PDG        = mc->GetPDG()       ;
    _ParentID   = mc->GetParentID()  ;
    _Energy     = mc->GetEnergy()    ;

    eid         = mce->Getid()        ;
    ePDG        = mce->GetPDG()       ;
    eParentID   = mce->GetParentID()  ;
    eEnergy     = mce->GetEnergy()    ;
    for(int i=0;i<3;i++) {
        _Momentum[i] = mc->GetMomentum()[i];
        _VPos[i]     = mc->GetVPos()[i] ;
        _EPos[i]     = mc->GetEPos()[i] ;

        eMomentum[i] = mce->GetMomentum()[i];
        eVPos[i]     = mce->GetVPos()[i] ;
        eEPos[i]     = mce->GetEPos()[i] ;
    }

    for(int i=0;i<20;i++) {
        _CreateProcess[i] = mc->GetProcess().data()[i];
        eCreateProcess[i] = mce->GetProcess().data()[i];
    }

    tmc->Fill();
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillMC(Int_t ID, MCParticle* mc) {

    EventID     = ID + fEvtNb*fStart ;
    mc_id         = mc->Getid()        ;
    mc_PDG        = mc->GetPDG()       ;
    mc_ParentID   = mc->GetParentID()  ;
    mc_Energy     = mc->GetEnergy()    ;

    for(int i=0;i<3;i++) {
        mc_Momentum[i] = mc->GetMomentum()[i];
        mc_Pos[i]     = mc->GetVPos()[i] ;
    }
    for(int i=0;i<20;i++) {
        mc_CreateProcess[i] = mc->GetProcess().data()[i];
    }

    te->Fill();
}

//....ooooo0ooooo........ooooo0ooooo........ooooo0ooooo........ooooo0ooooo......

void RootManager::FillTree(Int_t ID, 
        Double_t* Rnd,
        TrackerHitsCollection* tagHC,
        TrackerHitsCollection* recHC,
        CalorHitsCollection* ECal,
        CalorHitsCollection* HCal,
        CalorHitsCollection* HCap,
        CalorHitsCollection* HSide
        )
{

    Nb_tagTrk   = 0; 
    Nb_recTrk   = 0;
    Nb_ECal     = 0;
    Nb_HCal     = 0;
    Nb_HCap     = 0;
    Nb_HSide    = 0;

    EventID     = ID + fEvtNb*fStart ;
    for(int i=0;i<4;i++){
        Rndm[i] = *(Rnd+i) ;
    }

    if ( tagHC ) Nb_tagTrk   = tagHC->entries(); 
    if ( recHC ) Nb_recTrk   = recHC->entries();
    if ( ECal  ) Nb_ECal     = ECal->entries() ;
    if ( HCal  ) Nb_HCal     = HCal->entries() ;
    if ( HCap  ) Nb_HCap     = HCap->entries() ;
    if ( HSide ) Nb_HSide    = HSide->entries();

    for(int i=0;i<Nb_tagTrk;i++) {
        tTrkID[i]	    = (*tagHC)[i]->GetTrackID();
        tTrkChamNb[i]	= (*tagHC)[i]->GetChamberNb() + 1;
        tTrkTime[i]	    = (*tagHC)[i]->GetTime();
        tTrkE[i]	    = (*tagHC)[i]->GetEdep();
        tTrkPosX[i]	    = (*tagHC)[i]->GetPos()[0];    
        tTrkPosY[i]	    = (*tagHC)[i]->GetPos()[1];    
        tTrkPosZ[i]	    = (*tagHC)[i]->GetPos()[2];    
    }

    for(int i=0;i<Nb_recTrk;i++) {
        rTrkID[i]	    = (*recHC)[i]->GetTrackID();
        rTrkChamNb[i]	= (*recHC)[i]->GetChamberNb() + 1;
        rTrkTime[i]	    = (*recHC)[i]->GetTime();
        rTrkE[i]	    = (*recHC)[i]->GetEdep();
        rTrkPosX[i]	    = (*recHC)[i]->GetPos()[0];    
        rTrkPosY[i]	    = (*recHC)[i]->GetPos()[1];    
        rTrkPosZ[i]	    = (*recHC)[i]->GetPos()[2];    
    }

    for(int i=0;i<Nb_ECal;i++) {
        ECalTime[i]	    = (*ECal)[i]->GetTime();
        ECalE[i]	    = (*ECal)[i]->GetEdep();
        ECalPosX[i]	    = (*ECal)[i]->Getx();    
        ECalPosY[i]	    = (*ECal)[i]->Gety();    
        ECalPosZ[i]	    = (*ECal)[i]->Getz();    
    }

    for(int i=0;i<Nb_HCal;i++) {
        HCalTime[i]	    = (*HCal)[i]->GetTime();
        HCalE[i]	    = (*HCal)[i]->GetEdep();
        HCalPosX[i]	    = (*HCal)[i]->Getx();    
        HCalPosY[i]	    = (*HCal)[i]->Gety();    
        HCalPosZ[i]	    = (*HCal)[i]->Getz();    
    }

    for(int i=0;i<Nb_HCap;i++) {
        HCapTime[i]	    = (*HCap)[i]->GetTime();
        HCapE[i]	    = (*HCap)[i]->GetEdep();
        HCapPosX[i]	    = (*HCap)[i]->Getx();    
        HCapPosY[i]	    = (*HCap)[i]->Gety();    
        HCapPosZ[i]	    = (*HCap)[i]->Getz();    
    }

    for(int i=0;i<Nb_HSide;i++) {
        HSideTime[i]	= (*HSide)[i]->GetTime();
        HSideE[i]	    = (*HSide)[i]->GetEdep();
        HSidePosX[i]	= (*HSide)[i]->Getx();    
        HSidePosY[i]	= (*HSide)[i]->Gety();    
        HSidePosZ[i]	= (*HSide)[i]->Getz();    
    }

    tr->Fill();
}



