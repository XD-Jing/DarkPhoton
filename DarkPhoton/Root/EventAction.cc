//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file EventAction.cc
/// \brief Implementation of the EventAction class

#include "EventAction.hh"
#include "CalorimeterSD.hh"
#include "CalorHit.hh"
#include "TrackerSD.hh"
#include "TrackerHit.hh"
#include "RootManager.hh"

#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4EventManager.hh"
#include "G4RunManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4HCofThisEvent.hh"
#include "G4ios.hh"
#include "G4THitsCollection.hh"
#include "G4UnitsTable.hh"

#include <iostream>
#include <fstream>
#include <iterator>

using namespace std;
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction(RootManager* rootMng)
: G4UserEventAction(),
  frootMng(rootMng),
  fPrintModulo(1000),
  fStartID(0),
  fEvtNb(1e8)
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorHitsCollection*
EventAction::GetCalHitsCollection(const G4String& hcName,
		                                  const G4Event* event) const
{
  G4int hcID
		= G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
  if( hcID == -1 ) return NULL;
  CalorHitsCollection* hitsCollection
    	= static_cast<CalorHitsCollection*>(
    				event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
	  G4cerr << "Cannot access hitsCollection " << hcName << G4endl;
	  exit(1);
  }

  return hitsCollection;
}

TrackerHitsCollection*
EventAction::GetTrkHitsCollection(const G4String& hcName,
		                                  const G4Event* event) const
{
  G4int hcID
		= G4SDManager::GetSDMpointer()->GetCollectionID(hcName);
  if( hcID == -1 ) return NULL;
  TrackerHitsCollection* hitsCollection
    	= static_cast<TrackerHitsCollection*>(
    				event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
	  G4cerr << "Cannot access hitsCollection " << hcName << G4endl;
	  exit(1);
  }

  return hitsCollection;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::PrintEventStatistics(
		G4double absoEdep, G4double absoTrackLength,
        G4int x, G4int y, G4int z) const
{
	 G4cout
         << "   Position (x,y,z): "
         << std::setw(7) << x <<", "
         << std::setw(3) << y <<", "
         << std::setw(3) << z <<", "
		 << "   Absorber: total energy: "
		 << std::setw(7) << G4BestUnit(absoEdep, "Energy")
		 << "       total track length: "
		 << std::setw(7) << G4BestUnit(absoTrackLength, "Length")
		 << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* event)
{  
  G4int eventID = event->GetEventID();
  if ( eventID < 10 || eventID % fPrintModulo == 0) { 
    G4cout << "\n---> Begin of event: " << eventID << G4endl;
    //CLHEP::HepRandom::showEngineStatus();
  }

   G4RunManager* fRunManager = G4RunManager::GetRunManager();
   fRunManager->StoreRandomNumberStatusToG4Event(1);
										  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{
    G4RunManager* fRunManager = G4RunManager::GetRunManager();
    G4String RndmS = fRunManager->GetRandomNumberStatusForThisEvent();
    const char* rn = RndmS.data();
    
    long double r1 = 0;
    int Nr = 0;
    double nr = 10000;
    double rndm[4];
    for(int i = 24; i < 100; i++) {
        if( Nr > 3) break;
        if( (int)rn[i]-(int)'0' == -38) {
            rndm[Nr] = r1*pow(10, -1-(log(nr)/log(10)));
            nr = 10000;
            r1 = 0;
            Nr++;
            //G4cout<<"RR: "<<rndm[Nr-1]<<G4endl;
            continue;
        }
    
        r1 = r1+nr* (double)((int)rn[i]-(int)'0');
        nr/=10.0;
    }
    
    // print per event (modulo n)
    
    G4int eventID = event->GetEventID();
    if ( eventID <100 || eventID % fPrintModulo == 0) {
      G4cout << "---> End of event: " << eventID << G4endl;
    }
    
    // Get hits collections
    TrackerHitsCollection* tagHC
          = GetTrkHitsCollection("TagTrackerHitsCollection", event);
    
    TrackerHitsCollection* recHC
          = GetTrkHitsCollection("RecoilTrackerHitsCollection", event);
    
    // Get hits collections
    CalorHitsCollection* ECalHC
      	= GetCalHitsCollection("ECalHitsCollection", event);
    
    CalorHitsCollection* HCalHC
      	= GetCalHitsCollection("HCalHitsCollection", event);
    
    CalorHitsCollection* HCECapHC
      	= GetCalHitsCollection("HCECapHitsCollection", event);
    
    CalorHitsCollection* HCESideHC
      	= GetCalHitsCollection("HCESideHitsCollection", event);
    
    // Save Result to ROOT FILE
    frootMng->FillTree( fStartID*fEvtNb + eventID, rndm, tagHC, recHC, 
                        ECalHC, HCalHC, HCECapHC, HCESideHC);
    
    // periodic printing
    
    if ( eventID < 100 || eventID % fPrintModulo == 0) {
      G4cout << ">>> Event: " << eventID  << G4endl;
    
      if( tagHC ) G4cout << "    "  << tagHC->entries() << " hits stored in Tagging Tracker" << G4endl;
      if( recHC ) G4cout << "    "  << recHC->entries() << " hits stored in Recoil Tracker" << G4endl;
      if( ECalHC ) G4cout << "    "  << ECalHC->entries() << " hits stored in ECAL" << G4endl;
      if( HCalHC ) G4cout << "    "  << HCalHC->entries() << " hits stored in HCAL" << G4endl;
      if( HCECapHC ) G4cout << "    "  << HCECapHC->entries() << " hits stored in HCAL CAP" << G4endl;
      if( HCESideHC ) G4cout << "    "  << HCESideHC->entries() << " hits stored in HCAL Side" << G4endl;
    
      G4EventManager* fEvtManager = G4EventManager::GetEventManager();
      if( fEvtManager->GetVerboseLevel()>0 ) {
          for(int i = 0; i<tagHC->entries();i++) {
              (*tagHC)[i]->Print();
          }
          for(int i = 0; i<recHC->entries();i++) {
              (*recHC)[i]->Print();
          }
          G4double TE = 0;
          for (int i = 0; i<ECalHC->entries(); i++) {
              if( (*ECalHC)[i]->GetEdep() ){ 
                  if(i < ECalHC->entries()-1)TE += (*ECalHC)[i]->GetEdep();
                  PrintEventStatistics(
                  (*ECalHC)[i]->GetEdep(), (*ECalHC)[i]->GetTrackLength(),
                  (*ECalHC)[i]->Getx(), (*ECalHC)[i]->Gety(), (*ECalHC)[i]->Getz());
              }
          }
          G4cout<<"Total Energy: "<<TE<<G4endl;
      }
    }

}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::SetStartID(int startID)
{
  fStartID = startID; 
}

void EventAction::SetEvtNb(int evtNb)
{
  fEvtNb = evtNb; 
}
