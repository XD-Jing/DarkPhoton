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
/// \file CalorimeterSD.cc
/// \brief Implementation of the CalorimeterSD class

#include "CalorimeterSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorimeterSD::CalorimeterSD(
                            const G4String& name, 
                            const G4String& hitsCollectionName,
                            G4int xnofCells,
                            G4int ynofCells,
                            G4int znofCells
                            )
 : G4VSensitiveDetector(name),
   fHitsCollection(0),
   fNofCells(xnofCells*ynofCells*znofCells),
   fxNofCells(xnofCells),
   fyNofCells(ynofCells),
   fzNofCells(znofCells)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorimeterSD::~CalorimeterSD() 
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CalorimeterSD::Initialize(G4HCofThisEvent* hce)
{
  // Create hits collection
  fHitsCollection 
    = new CalorHitsCollection(SensitiveDetectorName, collectionName[0]); 

  vHitsCollection
    = new CalorHitsCollection();

  // Add this collection in hce
  G4int hcID 
    = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
  hce->AddHitsCollection( hcID, fHitsCollection ); 

  // Create hits
  // fNofCells for cells + one more for total sums 
  for (G4int i=0; i<fNofCells; i++ ) {
      vHitsCollection->insert( new CalorHit() );
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool CalorimeterSD::ProcessHits(G4Step* step, 
                                     G4TouchableHistory*)
{  
  // energy deposit
  G4double edep = step->GetTotalEnergyDeposit();
  
  // step length
  G4double stepLength = 0.;
  if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
  }

  if ( edep==0. && stepLength == 0. ) return false;      

  G4TouchableHistory* touchable
    = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    
  // Get calorimeter cell id 
  G4int reNumber = touchable->GetReplicaNumber();
  G4int xNb = (reNumber%(fxNofCells*fyNofCells)) % fxNofCells + 1; //depend on Global Setting!!!! 
  G4int yNb = (int)((reNumber%(fxNofCells*fyNofCells))/fyNofCells) + 1;
  G4int zNb = (int)reNumber/(fxNofCells*fyNofCells) + 1;
  
  // Get hit accounting data for this cell
  CalorHit* hit = (*vHitsCollection)[reNumber];
  if ( ! hit ) {
    G4cerr << "Cannot access hit " << reNumber << G4endl;
    exit(1);
  }         

  double w1 = (edep + hit->GetEdep() <= 0) ? 0 : edep/( edep+hit->GetEdep() );
  double w2 = (edep + hit->GetEdep() <= 0) ? 0 : hit->GetEdep()/( edep+hit->GetEdep() );
  double hx = hit->Getx();
  double hy = hit->Gety();
  double hz = hit->Getz();
  double ht = hit->GetTime();
  // Add values
  hit->setx( step->GetPostStepPoint()->GetPosition()[0]*w1 + w2*hx );
  hit->sety( step->GetPostStepPoint()->GetPosition()[1]*w1 + w2*hy );
  hit->setz( step->GetPostStepPoint()->GetPosition()[2]*w1 + w2*hz );
  //hit->setx( xNb );
  //hit->sety( yNb );
  //hit->setz( zNb );

  hit->sett( step->GetPostStepPoint()->GetGlobalTime()*w1 + w2*ht );
  hit->Add(edep, stepLength);
      
  return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void CalorimeterSD::EndOfEvent(G4HCofThisEvent*)
{

  for (G4int i=0; i<fNofCells; i++ ) {
    CalorHit* Hit = (*vHitsCollection)[i];
    if( Hit->GetEdep() > 1e-10)
        fHitsCollection->insert( Hit );
    else
        delete Hit;
  }

  if ( verboseLevel>=1 ) { 
     G4int nofHits = fHitsCollection->entries();
     G4cout << "\n-------->Hits Collection: in this event they are " << nofHits 
            << " hits in the tracker chambers: " << G4endl;
     for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
