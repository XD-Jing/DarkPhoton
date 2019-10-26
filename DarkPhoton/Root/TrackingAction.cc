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
/// \file TrackingAction.cc
/// \brief Implementation of the TrackingAction class

#include "TrackingAction.hh"

#include "G4Track.hh"
#include "G4TrackingManager.hh"
#include "G4EventManager.hh"
#include "G4Event.hh"
#include "MCParticle.hh"
#include "RootManager.hh"
#include "G4SystemOfUnits.hh"

class MCParticle;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::TrackingAction(RootManager* rootMng)
 : G4UserTrackingAction()
{
    froot = rootMng;
    fMC = new MCParticle(); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

TrackingAction::~TrackingAction()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{ 
    fMC->SetPDG( aTrack->GetParticleDefinition()->GetPDGEncoding());
    fMC->Setid( aTrack->GetTrackID() );
    fMC->SetParentID( aTrack->GetParentID() );
    fMC->SetEnergy( aTrack->GetTotalEnergy() );
    fMC->SetMomentum( aTrack->GetMomentum() );
    fMC->SetVPos( aTrack->GetVertexPosition() );
    if ( aTrack->GetCreatorProcess() ) {
        fMC->SetProcess( aTrack->GetCreatorProcess()->GetProcessName() );
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void TrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
    // currently useless
    G4int id = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    fMC->SetEPos( aTrack->GetStep()->GetPreStepPoint()->GetPosition() );

    G4double pm = sqrt(fMC->GetMomentum()[0]*fMC->GetMomentum()[0]+
                       fMC->GetMomentum()[1]*fMC->GetMomentum()[1]+
                       fMC->GetMomentum()[2]*fMC->GetMomentum()[2]);

    if (  fMC->Getid() == 1 || pm > 0.1*MeV
       || abs(fMC->GetPDG()) == 13 ) froot->FillMC(id,fMC);

    if(aTrack->GetTrackID() == 1) {
        //G4cout<< " " << G4endl;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
