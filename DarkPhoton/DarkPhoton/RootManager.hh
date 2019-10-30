#ifndef RootManager_h
#define RootManager_h 1

#include "CalorHit.hh"
#include "TrackerHit.hh"
#include "MCParticle.hh"
#include "EventMessenger.hh"

#include "TFile.h"
#include "TTree.h"
#include "TString.h"
#include "TVector3.h"

#include <iostream>
#include <stdexcept>
#include <sstream>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class TFile;
class TTree;
class EventMessenger;

const int MaxHits = 1e6;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RootManager {
    public:
        RootManager();
        ~RootManager();

        void book(G4String fileName);
        void save();

        void SetStartID(int id) { fStart = id; };
        void SetNbEvent(int id) { fEvtNb = id; };

        void FillTree(int    EventID, 
                      Double_t* Rndm,
                      TrackerHitsCollection* tagHC,
                      TrackerHitsCollection* recHC,
                      CalorHitsCollection* ,
                      CalorHitsCollection* ,
                      CalorHitsCollection* ,
                      CalorHitsCollection* 
                     );

        void FillEG(int EventID, MCParticle*, MCParticle* );
        void FillMC(int EventID, MCParticle* );


    private:
        EventMessenger* fMessenger;
        TFile*      rootFile;
        TTree*      tr;
        TTree*      tmc;
        TTree*      te;

        int       fStart;
        int       fEvtNb;

        int       EventID;
        float    Rndm[4];

        // MCParticle
        int           _id;
        int           _PDG;
        int           _ParentID;
        float        _Energy;
        float        _Momentum[3];
        float        _VPos[3]; // vertex position
        float        _EPos[3]; // end position
        char            _CreateProcess[20];

        int           eid;
        int           ePDG;
        int           eParentID;
        float        eEnergy;
        float        eMomentum[3];
        float        eVPos[3]; // vertex position
        float        eEPos[3]; // end position
        char            eCreateProcess[20];

        // Electron 
        int           mc_id;
        int           mc_PDG;
        int           mc_ParentID;
        float        mc_Energy;
        float        mc_Momentum[3];
        float        mc_Pos[3]; // position
        char            mc_CreateProcess[20];

        // Hit Collections
        int       Nb_tagTrk;
        int       tTrkID[MaxHits];
        int       tTrkChamNb[MaxHits];
        float    tTrkTime[MaxHits];
        float    tTrkE[MaxHits];
        float    tTrkPosX[MaxHits];    
        float    tTrkPosY[MaxHits];    
        float    tTrkPosZ[MaxHits];    

        int       Nb_recTrk;
        int       rTrkID[MaxHits];
        int       rTrkChamNb[MaxHits];
        float    rTrkTime[MaxHits];
        float    rTrkE[MaxHits];
        float    rTrkPosX[MaxHits];    
        float    rTrkPosY[MaxHits];    
        float    rTrkPosZ[MaxHits];    

        int       Nb_ECal;
        float    ECalTime[MaxHits];
        float    ECalE[MaxHits];
        float    ECalPosX[MaxHits];    
        float    ECalPosY[MaxHits];    
        float    ECalPosZ[MaxHits];    

        int       Nb_HCal;
        float    HCalTime[MaxHits];
        float    HCalE[MaxHits];
        float    HCalPosX[MaxHits];    
        float    HCalPosY[MaxHits];    
        float    HCalPosZ[MaxHits];    

        int       Nb_HCap;
        float    HCapTime[MaxHits];
        float    HCapE[MaxHits];
        float    HCapPosX[MaxHits];    
        float    HCapPosY[MaxHits];    
        float    HCapPosZ[MaxHits];    

        int       Nb_HSide;
        float    HSideTime[MaxHits];
        float    HSideE[MaxHits];
        float    HSidePosX[MaxHits];    
        float    HSidePosY[MaxHits];    
        float    HSidePosZ[MaxHits];    

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
