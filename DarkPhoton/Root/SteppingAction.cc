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
/// \file SteppingAction.cc
/// \brief Implementation of the SteppingAction class

#include "SteppingAction.hh"
#include "RootManager.hh"
#include "MCParticle.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4VProcess.hh"
#include "G4RunManager.hh"
#include "G4EventManager.hh"
#include "G4StackManager.hh"
#include "G4SystemOfUnits.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::SteppingAction(RootManager * rootMng)
 : G4UserSteppingAction(),
   if_select(false), if_kill(true), if_biased(false)
{
    froot = rootMng;
    G4cout<<"Stepping Initialized!!"<<G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SteppingAction::~SteppingAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SteppingAction::UserSteppingAction(const G4Step* aStep)
{ 
    G4StepPoint* post = aStep->GetPostStepPoint();
    if ( aStep->GetTrack()->GetTrackID()==1 ) {
        MCParticle* p = new MCParticle();

        p->SetPDG( aStep->GetTrack()->GetParticleDefinition()->GetPDGEncoding() );
        p->Setid( aStep->GetTrack()->GetTrackID());
        p->SetEnergy( post->GetTotalEnergy() );
        p->SetMomentum( post->GetMomentum() );
        p->SetVPos( post->GetPosition() );

        G4int id = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
        //froot->FillMC( id, p); 
        
        // search for all the secondary particles produced in this step
        const std::vector< const G4Track* > * secondary = aStep->GetSecondaryInCurrentStep();
        std::vector< const G4Track* >::const_iterator itr_sec;

        MCParticle* gamma = new MCParticle();
        for( itr_sec = (*secondary).begin(); itr_sec != (*secondary).end(); itr_sec++ ) {

            const G4Track* aTrack = (*itr_sec); 
            gamma->SetPDG( aTrack->GetParticleDefinition()->GetPDGEncoding());

            //G4cout<<"D: "<< aTrack->GetParticleDefinition()->GetPDGEncoding() 
            //      << ", "<< aTrack->GetCreatorProcess()->GetProcessName() <<G4endl;

            if ( gamma->GetPDG() != 22 ) continue;

            gamma->Setid( aTrack->GetTrackID() );
            gamma->SetParentID( aTrack->GetParentID() );
            gamma->SetEnergy( aTrack->GetTotalEnergy() );
            gamma->SetMomentum( aTrack->GetMomentum() );
            gamma->SetVPos( p->GetVPos() );
            if ( aTrack->GetCreatorProcess() ) 
                gamma->SetProcess( aTrack->GetCreatorProcess()->GetProcessName() );

            // selection criteria
            if(gamma->GetEnergy() > 4.0*GeV) {
                froot->FillEG(id, gamma, p);
                if_select = true;
            }
        }

    }

    if(0) {
        //if_select = true;
        //G4cout<<"Track ID: "<< aStep->GetTrack()->GetTrackID() << ", bool: "<< if_select<<G4endl;
        if ( aStep->GetTrack()->GetTrackID() != 1 && if_select == true) {
            if_select = false;
            if_kill = false;
        }

        if ( aStep->GetTrack()->GetTrackID() != 1 && if_kill == true) {
            G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
            G4EventManager::GetEventManager()->AbortCurrentEvent();
            return;
        }

        if ( aStep->GetTrack()->GetTrackID() == 1 && if_kill == false && if_select == false) {
            if_kill = true;
            if_biased = false;
        }

        // Select Certain Biased Process
        
        //G4cout<< post->GetProcessDefinedStep()->GetProcessType() <<", "<<post->GetProcessDefinedStep()->GetProcessSubType() << G4endl;
        if  ( (post->GetProcessDefinedStep()->GetProcessType() == 2 
            && post->GetProcessDefinedStep()->GetProcessSubType() == 15)
            //|| (post->GetProcessDefinedStep()->GetProcessType() == 4
            //&& post->GetProcessDefinedStep()->GetProcessSubType() == 121)
                ) {
            if_biased = true;
        }
        
        if (  aStep->GetTrack()->GetTrackID() != 1
           && G4EventManager::GetEventManager()->GetStackManager()-> GetNTotalTrack () == 0 
           && if_biased == false ) {
            G4EventManager::GetEventManager()->GetNonconstCurrentEvent()->SetEventAborted();
            G4EventManager::GetEventManager()->AbortCurrentEvent();
            return;
        }

    }

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


