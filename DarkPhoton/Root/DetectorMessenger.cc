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
/// \file DetectorMessenger.cc
/// \brief Implementation of the DetectorMessenger class

#include "DetectorMessenger.hh"
#include "DetectorConstruction.hh"
#include "Randomize.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::DetectorMessenger(DetectorConstruction* Det)
 : G4UImessenger(),
   fDetectorConstruction(Det)
{
  fDirectory = new G4UIdirectory("/DP/");
  fDirectory->SetGuidance("UI commands specific to this example.");

  fDetDirectory = new G4UIdirectory("/DP/det/");
  fDetDirectory->SetGuidance("Detector construction control");

  fTargMatCmd = new G4UIcmdWithAString("/DP/det/setTargetMaterial",this);
  fTargMatCmd->SetGuidance("Select Material of the Target.");
  fTargMatCmd->SetParameterName("choice",false);
  fTargMatCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetFieldCmd = new G4UIcmdWithADoubleAndUnit("/DP/det/setField",this);
  fSetFieldCmd->SetGuidance("Define magnetic field.");
  fSetFieldCmd->SetGuidance("Magnetic field will be in X direction.");
  fSetFieldCmd->SetParameterName("Bx",false);
  fSetFieldCmd->SetUnitCategory("Magnetic flux density");
  fSetFieldCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetTBxCmd = new G4UIcmdWithADouble("/DP/det/setTBxField",this);
  fSetTBxCmd->SetGuidance("Define tagging tracker x-direction magnetic field.");
  fSetTBxCmd->SetGuidance("Magnetic field will be in X direction.");
  fSetTBxCmd->SetParameterName("tBx",false);
  //fSetTBxCmd->SetUnitCategory("Magnetic flux density");
  //fSetTBxCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fSetRBxCmd = new G4UIcmdWithADouble("/DP/det/setRBxField",this);
  fSetRBxCmd->SetGuidance("Define recoil tracker x-direction magnetic field.");
  fSetRBxCmd->SetGuidance("Magnetic field will be in y direction.");
  fSetRBxCmd->SetParameterName("rBy",false);
  //fSetRByCmd->SetUnitCategory("Magnetic flux density");
  //fSetRByCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fEngDirectory = new G4UIdirectory("/DP/Engine/");
  fEngDirectory->SetGuidance("Simulation Engine control");

  fSetRndEngCmd = new G4UIcmdWithAnInteger("/DP/Engine/randomSeed",this);
  fSetRndEngCmd->SetGuidance("Set the random seed for Rnd Engine.");
  fSetRndEngCmd->SetParameterName("rndseed",false);
  fSetRndEngCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorMessenger::~DetectorMessenger()
{
  delete fTargMatCmd;
  delete fSetFieldCmd;
  delete fDirectory;
  delete fDetDirectory;
  delete fSetRndEngCmd;
  delete fEngDirectory;
  delete fSetTBxCmd;
  delete fSetRBxCmd;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fTargMatCmd )
   { fDetectorConstruction->SetTargetMaterial(newValue);}

  if( command == fSetFieldCmd ) {
    fDetectorConstruction
      ->SetMagField(fSetFieldCmd->GetNewDoubleValue(newValue));
  }

  if( command == fSetRndEngCmd ) {
	  CLHEP::HepRandom::getTheEngine()
		  ->setSeed(fSetRndEngCmd->GetNewIntValue(newValue),0);
  }

  if( command == fSetTBxCmd ) {
    fDetectorConstruction
      ->SetTTrkMagField(fSetTBxCmd->GetNewDoubleValue(newValue));
  }

  if( command == fSetRBxCmd ) {
    fDetectorConstruction
      ->SetRTrkMagField(fSetRBxCmd->GetNewDoubleValue(newValue));
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
