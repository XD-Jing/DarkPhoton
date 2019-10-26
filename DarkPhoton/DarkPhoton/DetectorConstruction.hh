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
/// \file DetectorConstruction.hh
/// \brief Definition of the DetectorConstruction class

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4SubtractionSolid.hh"
#include "TrackerSD.hh"
#include "CalorimeterSD.hh"
#include "G4UniformMagField.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"


class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class G4UserLimits;

class DetectorMessenger;
class MagneticField;

/// Detector construction class to define materials and geometry.
///
/// In addition a transverse uniform magnetic field is defined in
/// SetMagField() method which can be activated via a command
/// defined in the DetectorMessenger class. 

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();

    // Set methods
    void SetMagField(G4double );
    void SetTargetMaterial (G4String );
    //void SetChamberMaterial(G4String );
    void SetMaxStep (G4double );
    void SetCheckOverlaps(G4bool );
    void SetTTrkMagField(G4double);
    void SetRTrkMagField(G4double);

  private:
    // methods
    void  DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
    void  DefineTarget(G4bool RealConstruct);
    void  DefineTagTracker(G4bool RealConstruct);
    void  DefineRecTracker(G4bool RealConstruct);
    void  DefineHCal(G4bool RealConstruct);
    void  DefineECal(G4bool RealConstruct);
    void  DefineHCE(G4bool RealConstruct);
    void  DefineHCECap(G4bool RealConstruct);
    void  DefineHCESide(G4bool RealConstruct);
    void  DefineWorld();
    //basic flag
    G4bool constructed;

    //global detector parts put here!
    G4VPhysicalVolume* worldPV;
    G4LogicalVolume*   worldLV;
    G4LogicalVolume*   fLogicTarget;     // use to set the target material outside
    G4LogicalVolume*   trackerTLV; //outline of TagTracker, use to set e.g. vis att 
    G4LogicalVolume*   trackerRLV;
    G4LogicalVolume*   ECalLV;
    G4LogicalVolume*   HCalLV;
    G4LogicalVolume*   HCalELV;
    G4LogicalVolume*   ECalShellLV;

    //global geometry parameter put here!
    //Target
    G4double targetRadius;
    G4double  targetLength; // full length of Target

    //Tracker
    G4double  tag_chamberLength;
    G4LogicalVolume**  fLogicChamber_tag;    // no use now outside
    //Recoil
    G4double recChamberBeSpacing;
    G4double recoil_chamberLength;
    G4double recChamberAfSpacing;
    G4LogicalVolume**  fLogicChamber_rec;    // no use now outside

    //ECal
    G4Box* ECalS;
    G4Box* ECalBox;
    G4double ecalradius;
    G4double ecalLength;
    G4SubtractionSolid* ECalShell;
    G4ThreeVector positionECal;
    G4LogicalVolume**  fLogicChamber_ECal;   // no use now outside
    G4LogicalVolume**  fLogicChamber_ECal_Gas;
    
    //HCal
    G4double  HCalLength;
    G4double outHCal_xlen;
    G4double outHCal_ylen;
    G4LogicalVolume**  HCal_RPC_LV;
    G4LogicalVolume**  HCal_RPC_GAS_LV;
    //HCE
    G4LogicalVolume**  HCECap_RPC_LV;
    G4LogicalVolume**  HCECap_RPC_GAS_LV;
    G4LogicalVolume**  HCESide_RPC_LV;
    G4LogicalVolume**  HCESide_RPC_GAS_LV;

    //global pointer: will use in other function; put here!
    G4double fTTrkMagField;
    G4double fRTrkMagField;
    TrackerSD* aTrackerSD_tag;
    TrackerSD* aTrackerSD_rec; // pointer to the SD on Rec
    CalorimeterSD* HCalSD;
    CalorimeterSD* HCECapSD;
    CalorimeterSD* HCESideSD;
    G4Material* fTargetMaterial;   

    //basic components
    DetectorMessenger*  fMessenger;   // messenger
    MagneticField*      fMagField;     // magnetic field
    //global option
    G4UserLimits* fStepLimit; // pointer to user step limits
    G4bool  fCheckOverlaps;   // option to activate checking of volumes overlaps 
    // Visualization attributes
    G4VisAttributes* boxVisAtt;
    G4VisAttributes* chamberVisAtt;
    G4VisAttributes* TarVisAtt;
    G4VisAttributes* ECalVisAtt;
    G4VisAttributes* HCalVisAtt;
    // Visibility of certain volumes
    G4bool vis_World;
    G4bool vis_tagTracker;
    G4bool vis_recTracker;
    G4bool vis_Target;
    G4bool vis_ECal;    
    G4bool vis_HCal;
    G4bool vis_HCECap;
    G4bool vis_HCESide;
};
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
