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
/// \file EventAction.hh
/// \brief Definition of the EventAction class

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4EventManager.hh"
#include "CalorHit.hh"
#include "TrackerHit.hh"
#include "RootManager.hh"
#include "globals.hh"

#include "G4THitsCollection.hh"
#include <fstream>

class EventActionMessenger;
/// Event action class

class EventAction : public G4UserEventAction
{
  public:
    EventAction(RootManager*);
    virtual ~EventAction();

    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );

    // Set methods
    void SetPrintModulo(G4int value);
	CalorHitsCollection* GetCalHitsCollection(const G4String& hcName,
										   const G4Event* event) const;
	TrackerHitsCollection* GetTrkHitsCollection(const G4String& hcName,
										   const G4Event* event) const;
	void PrintEventStatistics(G4double absoEdep, G4double absoTrackLength,
		                      G4int x, G4int y, G4int z) const;
    void SetStartID(int startID);
    void SetEvtNb(int evtNb);

  private:
    G4int  fPrintModulo;
    G4int  fStartID;
    G4int  fEvtNb;
    RootManager* frootMng;
};

// inline functions

inline void EventAction::SetPrintModulo(G4int value) {
  fPrintModulo = value;
} 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
