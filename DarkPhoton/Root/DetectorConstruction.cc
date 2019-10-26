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
/// \file DetectorConstruction.cc
/// \brief Implementation of the DetectorConstruction class
 
#include "DetectorConstruction.hh"
#include "DetectorMessenger.hh"
#include "ChamberParameterisation.hh"
#include "MagneticField.hh"
#include "TrackerSD.hh"
#include "CalorimeterSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4RunManager.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4SDManager.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

//#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::DetectorConstruction()
:constructed(false), fTTrkMagField(0.), fRTrkMagField(0.), 
 fLogicTarget(NULL), fLogicChamber_rec(NULL), fLogicChamber_tag(NULL), 
fLogicChamber_ECal(NULL),fLogicChamber_ECal_Gas(NULL), 
 fTargetMaterial(NULL), 
 fStepLimit(NULL),
 fCheckOverlaps(false)
{
    fMessenger = new DetectorMessenger(this);
    fMagField  = new MagneticField();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
DetectorConstruction::~DetectorConstruction()
{
    delete fMagField;
    delete fStepLimit;
    delete fMessenger;             
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* DetectorConstruction::Construct()
{
    
    if(!constructed) constructed = true;
    // Define materials
    DefineMaterials();
    
    // Define volumes
    return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{
    G4double z, a, fractionmass, density;
    G4String name, symbol;
    G4int ncomponents, natoms;
    
    a = 14.00674*g/mole;
    G4Element* elN  = new G4Element("Nitrogen", symbol="N", z=7., a);
    
    a = 174.967*g/mole;
    G4Element* elLu  = new G4Element(name="Lutetium",symbol="Lu" , z= 71., a);
    
    a = 88.906*g/mole;
    G4Element* elY  = new G4Element(name="Yttrium"  ,symbol="Y" , z= 39., a);
    
    a = 28.086*g/mole;
    G4Element* elSi  = new G4Element(name="Silicon"  ,symbol="Si" , z= 14., a);
    
    a = 15.999*g/mole;
    G4Element* elO  = new G4Element(name="Oxygen"  ,symbol="O" , z= 8., a);
    
    a=55.850*g/mole;
    G4Element* elFe = new G4Element(name="Iron",symbol="Fe",z=26.,a);
    
    G4double lC=12.01*g/mole;
    G4double lS=32.065*g/mole;
    G4double lF=18.998*g/mole;
    G4double lH=1.01*g/mole;
    G4Element* elC = new G4Element("Carbon","C", 6.0, lC);
    G4Element* elS = new G4Element("Sulfur","S", 16.0, lS);
    G4Element* elF = new G4Element("Fluorine","F", 9.0, lF);
    G4Element* elH = new G4Element("Hydrogen","H", 1.0, lH);  
    
    
    // Material definition 
    
    G4NistManager* nistManager = G4NistManager::Instance();
    
    G4Element* B  = nistManager->FindOrBuildElement("B");
    G4Element* Na = nistManager->FindOrBuildElement("Na");
    G4Element* K  = nistManager->FindOrBuildElement("K");
    G4Element* Al = nistManager->FindOrBuildElement("Al");
    
    
    G4bool fromIsotopes = false;
    
    // Vacuum
    density = universe_mean_density;
    G4Material* Vacuum = new G4Material("vacuum", density, ncomponents=2);
    Vacuum-> AddElement(elN, .7);
    Vacuum-> AddElement(elO, .3);
                        
    // Defining LYSO
    density = 7.4*g/cm3;
    G4Material* LSO = new G4Material(name="LSO",density,ncomponents=3);
    LSO->AddElement(elLu, natoms=2);
    LSO->AddElement(elSi, natoms=1);
    LSO->AddElement(elO, natoms=5);
    
    density = 4.5*g/cm3;
    G4Material* YSO = new G4Material(name="YSO",density,ncomponents=3);
    YSO->AddElement(elY, natoms=2);
    YSO->AddElement(elSi, natoms=1);
    YSO->AddElement(elO, natoms=5);
    
    density = 7.1*g/cm3;
    G4Material* LYSO = new G4Material(name="LYSO",density,ncomponents=2,kStateSolid);
    LYSO->AddMaterial(LSO,fractionmass=90*perCent);
    LYSO->AddMaterial(YSO,fractionmass=10*perCent);
    
    // Defining RPC Gas
    G4double chf_den = 0.00425*g/cm3;
    G4double isoch_den=0.00251*g/cm3;
    G4double sf_den=0.00617*g/cm3;
    G4Material* C2H2F4 = new G4Material("F134a",chf_den,3);
    C2H2F4->AddElement(elC,2);
    C2H2F4->AddElement(elH,2); 
    C2H2F4->AddElement(elF,4);    //define F-134a  94.7percent
    
    G4Material* iC4H10 = new G4Material("iso-butane",isoch_den,2);
    iC4H10->AddElement(elC,4);
    iC4H10->AddElement(elO,10);   //define iso-C4H10 5percent
    
    G4Material* SF6 = new G4Material("sulfur hexafluoride",sf_den,2);
    SF6->AddElement(elS,1);
    SF6->AddElement(elF,6);       //define SF6 0.3percent
    
    G4double rpc_density = 0.00406*g/cm3;
    G4double fractionvolume1=94.7*perCent;
    G4double fractionvolume2=5*perCent;
    G4double fractionvolume3=0.3*perCent;
    
    G4Material* RPCGas = new G4Material("RPCGas",rpc_density,ncomponents=3);
    RPCGas->AddMaterial(C2H2F4,fractionvolume1);
    RPCGas->AddMaterial(iC4H10,fractionvolume2);
    RPCGas->AddMaterial(SF6   ,fractionvolume3);  //define ATLAS mixture gas component
    
    // Defining Polystyrene
    G4Material* PS = new G4Material("Polystyrene",density = 1.04*g/cm3, ncomponents = 2);
    PS->AddElement(elC, natoms=8);
    PS->AddElement(elH, natoms=8);

    // Defining Glass
    G4Material* glass = new G4Material("Borosilicate glass", density= 2.23*g/cm3, ncomponents=6);
    glass->AddElement(B, fractionmass=0.040064);
    glass->AddElement(elO, fractionmass=0.539562); 
    glass->AddElement(Na, fractionmass=0.028191);
    glass->AddElement(Al, fractionmass=0.011644);
    glass->AddElement(elSi, fractionmass=0.377220);
    glass->AddElement(K, fractionmass=0.003321);
    
    // Si defined using NIST Manager
    nistManager->FindOrBuildMaterial("G4_Si", fromIsotopes); //important! let nistMgr prepare these material for later use!
    nistManager->FindOrBuildMaterial("G4_Fe", fromIsotopes);
    nistManager->FindOrBuildMaterial("G4_Al", fromIsotopes);
    
    // Tung defined using NIST Manager //move to local funct?
    fTargetMaterial  = nistManager->FindOrBuildMaterial("G4_W", fromIsotopes);
    
    // Print materials
    G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{

    // Visualization attributes
    boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
    chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));
    TarVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,0.0));
    ECalVisAtt = new G4VisAttributes(G4Colour(0.0,1.0,1.0));
    HCalVisAtt = new G4VisAttributes(G4Colour(1.0,0.0,1.0));

    // Visibility of certain volumes
    vis_World = true;
    vis_tagTracker = true;
    vis_recTracker = true;
    vis_Target = true;
    vis_ECal = true;
    vis_HCal = false;
    vis_HCECap = false;
    vis_HCESide = false;

    // Calculate geometey parameters, NOT real construct
    DefineTarget(false);     // Target Parameters
    DefineTagTracker(false); // Tagging Tracker Parameters
    DefineRecTracker(false); // Recoil Tracker Parameters
    DefineECal(false);       // ECal Parameters
    DefineHCal(false);       // HCal Parameters
    DefineHCE(false);        // HCal Edge Parameters

    // Build from now on
    DefineWorld();           // Build World
    DefineTarget(true);      // Build Target
    DefineTagTracker(true);  // Build Tagging Tracker
    DefineRecTracker(true);  // Build Recoil Tracker
    DefineECal(true);        // Build ECal
    DefineHCal(true);        // Build HCal
    DefineHCE(true);         // Build HCal Edge

    // Set User Limit 
    G4double maxStep = 0.5*1*mm;
    fStepLimit = new G4UserLimits(maxStep, DBL_MAX, 200*s, 0.01*MeV);
    
    return worldPV;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineTarget(G4bool RealConstruct=false)
{
    targetRadius  = 20.0*cm;   // Radius of Target; maybe move to main function is also ok.
    targetLength =  350*um;    // full length of Target
    if(!RealConstruct) return;

    ////////////////////////////////////////////////////////////////////////

    G4ThreeVector positionTarget = G4ThreeVector(0,0,0);
    G4Box* targetS
        = new G4Box("target",0.5*targetRadius,targetRadius,0.5*targetLength);
    fLogicTarget
        = new G4LogicalVolume(targetS, fTargetMaterial,"Target",0,0,0);
    new G4PVPlacement(0,              // no rotation
                      positionTarget, // at (x,y,z)
                      fLogicTarget,   // its logical volume
                      "Target",       // its name
                      worldLV,        // its mother volume
                      false,          // no boolean operations
                      0,              // copy number
                      fCheckOverlaps); // checking overlaps 
    
    fLogicTarget ->SetVisAttributes(TarVisAtt); 
    if(!vis_Target) fLogicTarget->SetVisAttributes(G4VisAttributes::Invisible);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineTagTracker(G4bool RealConstruct=false)
{
    G4double trackerTRadius = targetRadius;//former: G4double trackerTRadius = targetRadius;
    const G4int NbOfTagChambers = 7;
    
    G4double tag_chamberSpacing[NbOfTagChambers-1]
        = { 100*mm, 100*mm, 100*mm, 100*mm, 100*mm, 100*mm };
    
    G4double tag_chamberWidth[NbOfTagChambers]
        = { 0.1*mm, 0.1*mm, 0.1*mm, 0.1*mm, 0.1*mm, 0.1*mm, 0.1*mm };
    
    G4double tagChamberAfSpacing = 7.5*mm-0.5*tag_chamberWidth[NbOfTagChambers-1];
    
    tag_chamberLength = 0.5*tag_chamberWidth[0]
                         + 0.5*tag_chamberWidth[NbOfTagChambers-1];
    for (int i=0;i<NbOfTagChambers-1;i++){
      tag_chamberLength += tag_chamberSpacing[i];
    }
    if(!RealConstruct) return;
    
    ////////////////////////////////////////////////////////////////////////
    
    G4Material* vacuum = G4Material::GetMaterial("vacuum");
    G4Material* Si  = G4Material::GetMaterial("G4_Si");
    
    G4ThreeVector positionTracker
      = G4ThreeVector(0,0,-1.0*(0.5*tag_chamberLength+tagChamberAfSpacing));
    
    G4Box* trackerT
      = new G4Box("tracker",trackerTRadius,trackerTRadius,0.5*tag_chamberLength);
    trackerTLV
      = new G4LogicalVolume(trackerT, vacuum, "Tracker",0,0,0);
    new G4PVPlacement(0,               // no rotation
                      positionTracker, // at (x,y,z)
                      trackerTLV,       // its logical volume
                      "Tracker",       // its name
                      worldLV,         // its mother  volume
                      false,           // no boolean operations
                      0,               // copy number
                      fCheckOverlaps); // checking overlaps 
    trackerTLV->SetVisAttributes(boxVisAtt);
    trackerTLV->SetVisAttributes(G4VisAttributes::Invisible);

    //set sensitive detecor
    G4String trackerChamberSDname = "TagTrackerChamberSD";
    aTrackerSD_tag = new TrackerSD(trackerChamberSDname, "TagTrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector( aTrackerSD_tag );
    
    G4double tag_Zposition = -0.5*tag_chamberLength + 0.5*tag_chamberWidth[0];
    fLogicChamber_tag = new G4LogicalVolume*[NbOfTagChambers];
    for (G4int copyNo=0; copyNo<NbOfTagChambers; copyNo++) {
    
        G4double tag_halfWidth = 0.5*tag_chamberWidth[copyNo];
        G4double r = targetRadius;//former: G4double r=targetRaduis;
        if(copyNo > 0) tag_Zposition += tag_chamberSpacing[copyNo-1];
    
        G4Box* chamberS_tag
            = new G4Box("tag_chamber", 0.5*r, r, tag_halfWidth);
        fLogicChamber_tag[copyNo] 
            = new G4LogicalVolume(chamberS_tag,Si,"tag_Chamber",0,0,0);
        fLogicChamber_tag[copyNo]->SetSensitiveDetector( aTrackerSD_tag );
        fLogicChamber_tag[copyNo]->SetVisAttributes(chamberVisAtt);
        if(!vis_tagTracker) fLogicChamber_tag[copyNo]->SetVisAttributes(G4VisAttributes::Invisible);
    
        new G4PVPlacement(0,                  // no rotation
                          G4ThreeVector(0,0,tag_Zposition), // at (x,y,z)
                          fLogicChamber_tag[copyNo],        // its logical volume
                          "TagChamber",                    // its name
                          trackerTLV,                    // its mother  volume
                          false,                        // no boolean operations
                          copyNo,                       // copy number
                          fCheckOverlaps);              // checking overlaps
    
        G4cout << "TagZ" <<tag_Zposition + positionTracker[2]<<G4endl;
    }
    //uniform magnetic field in Tracker
    G4UniformMagField* magFieldT
              = new G4UniformMagField(G4ThreeVector(fTTrkMagField*tesla,0.,0.)); // Tagging Tracker
    G4FieldManager* localFieldMgrT=
            new G4FieldManager(magFieldT);
    trackerTLV->SetFieldManager(localFieldMgrT, true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineRecTracker(G4bool RealConstruct=false)
{

//////  This part will use these global varible:  ////////
//NEED:
  //worldLV :world 
  //fCheckOverlaps :global option
  //fRTrkMagField : local magnetic 
  //targetRadius : use to calculate the weith change
  //ecalradius : use to calculate the weith change
//SET:
  //recChamberAfSpacing : global geometry
  //recoil_chamberLength : global geometry
  //recChamberBeSpacing : global geometry
  //aTrackerSD_rec : SD on Rec
  //trackerRLV : need to set field outside the function 
  //fLogicChamber_rec : might useful outside
//////////////////////////////////////////////////////////////
    const G4int NbOfRecoilChambers = 6;
    G4double rec_chamberSpacing[NbOfRecoilChambers-1] 
        = { 15*mm, 15*mm, 15*mm, 37.5*mm, 90*mm };
    G4double rec_chamberWidth[NbOfRecoilChambers]
        = { 0.1*mm, 0.1*mm, 0.1*mm, 0.1*mm, 0.1*mm, 0.1*mm };
    recChamberBeSpacing = 7.5*mm-0.5*rec_chamberWidth[NbOfRecoilChambers-1];
    recChamberAfSpacing = 19.5*mm;
    recoil_chamberLength = 0.5*rec_chamberWidth[0] 
                         + 0.5*rec_chamberWidth[NbOfRecoilChambers-1];
    for (int i=0;i<NbOfRecoilChambers-1;i++){
      recoil_chamberLength += rec_chamberSpacing[i];
    }

    if(!RealConstruct) return;

    ////////////////////////////////////////////////////////////////////////

    //preparation materials (note: need define before or use builtin material)
    G4Material* vacuum  = G4Material::GetMaterial("vacuum");
    G4Material* Si  = G4Material::GetMaterial("G4_Si");

    G4double trackerRRadius = (ecalradius >= targetRadius ? ecalradius : targetRadius );
    std::cout<<trackerRRadius<<std::endl;

    //build
    G4ThreeVector positionTrackerR 
      = G4ThreeVector(0,0, 0.5*recoil_chamberLength+recChamberBeSpacing);
    G4Box* trackerR
      = new G4Box("tracker",trackerRRadius,trackerRRadius,0.5*recoil_chamberLength);
    trackerRLV
      = new G4LogicalVolume(trackerR, vacuum, "Tracker",0,0,0);  
    new G4PVPlacement(0,               // no rotation
                      positionTrackerR, // at (x,y,z)
                      trackerRLV,       // its logical volume
                      "Tracker",       // its name
                      worldLV,         // its mother  volume
                      false,           // no boolean operations
                      0,               // copy number
                      fCheckOverlaps); // checking overlaps 
    
    trackerRLV->SetVisAttributes(boxVisAtt);
    trackerRLV->SetVisAttributes (G4VisAttributes::Invisible);

    // Set Sensitive detectors
    G4String trackerChamberSDname = "RecoilTrackerChamberSD";
    aTrackerSD_rec = new TrackerSD(trackerChamberSDname, "RecoilTrackerHitsCollection");
    G4SDManager::GetSDMpointer()->AddNewDetector( aTrackerSD_rec );
    
    G4double firstLength   = 2.*targetRadius;
    G4double lastLength    = 2.*ecalradius;
    G4double rmaxFirst = 0.5 * firstLength;
    G4double rmaxIncr = 0.0;

    if( NbOfRecoilChambers > 0 ){
    	rmaxIncr =  0.5 * (lastLength-firstLength)/(NbOfRecoilChambers-1);
    }
    
    G4double Zposition = -0.5*recoil_chamberLength + 0.5*rec_chamberWidth[0];;
    G4Box** chamberS_rec = new G4Box*[NbOfRecoilChambers];
    fLogicChamber_rec = new G4LogicalVolume*[NbOfRecoilChambers];
    for (G4int copyNo=0; copyNo<NbOfRecoilChambers; copyNo++) {
    
        if(copyNo > 0) Zposition += rec_chamberSpacing[copyNo-1] ;
        G4double halfWidth = 0.5*rec_chamberWidth[copyNo];
        G4double rmax =  rmaxFirst + copyNo * rmaxIncr;
        
        chamberS_rec[copyNo]
      	    = new G4Box("recoil_Chamber", 0.5* rmax, rmax, halfWidth);
    
        fLogicChamber_rec[copyNo] 
            = new G4LogicalVolume(chamberS_rec[copyNo],Si,"Chamber",0,0,0);
        
        fLogicChamber_rec[copyNo]->SetSensitiveDetector( aTrackerSD_rec );
        fLogicChamber_rec[copyNo]->SetVisAttributes(chamberVisAtt);
        if(!vis_recTracker) fLogicChamber_rec[copyNo]->SetVisAttributes(G4VisAttributes::Invisible);  

        new G4PVPlacement(0,                  // no rotation
      		  G4ThreeVector(0,0,Zposition), // at (x,y,z)
      		  fLogicChamber_rec[copyNo],        // its logical volume
      		  "RecoilChamber",                    // its name
      		  trackerRLV,                    // its mother  volume
      		  false,                        // no boolean operations
      		  copyNo,                       // copy number
      		  fCheckOverlaps);              // checking overlaps
        G4cout << "RecZ" << Zposition + positionTrackerR[2]<<G4endl;
    }

    //fRTrkMagField initial is zero.
    G4UniformMagField* magFieldR
	    = new G4UniformMagField(G4ThreeVector(fRTrkMagField*tesla,0.,0.)); // Recoil Tracker

    G4FieldManager* localFieldMgrR = new G4FieldManager(magFieldR);
    trackerRLV->SetFieldManager(localFieldMgrR, true);

    return ;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::DefineECal(G4bool RealConstruct=false)
{
    //NEED:
        //recoil_chamberLength
        //recChamberAfSpacing
        //recChamberBeSpacing
        
    //SET:
        //ecalLength
        //positionECal
        //ecalradius
        //ECalS : !!!
        //ECalLV
        //fLogicChamber_ECal
    ////////////////////////////////////////////////////////////////////////////
    const G4int cell_nb = 45; // affect CalorimeterSD.cc !!!!!
    G4double cell_length = 1*cm;
    G4double cell_thick = 0.35*mm;
    G4double holder_gap = 0.5*mm;
    G4double cellgas_length_xy = cell_length - 2.0 * cell_thick;
    ecalradius = 0.5*( cell_length * cell_nb + cell_nb/5*holder_gap );
    ecalLength = 44*cm;// length of ECal;
    G4double cellgas_length_z = ecalLength - 2.0 * cell_thick;
    positionECal = G4ThreeVector(0,0,0.5*ecalLength + recoil_chamberLength + recChamberBeSpacing + recChamberAfSpacing);
    G4Material* Al= G4Material::GetMaterial("G4_Al");
    if(!RealConstruct) return;

    ////////////////////////////////////////////////////////////////////////

    G4Material* vacuum = G4Material::GetMaterial("vacuum");
    G4Material* LYSO  = G4Material::GetMaterial("LYSO");
    ECalS
        = new G4Box("ecal", ecalradius, ecalradius, 0.5*ecalLength); //This box will be used in HCap substract!! dual define might be ok.
    ECalLV
        = new G4LogicalVolume(ECalS, vacuum, "ECAL",0,0,0);  
    new G4PVPlacement(0,               // no rotation
            positionECal, // at (x,y,z)
            ECalLV,       // its logical volume
            "ECAL",       // its name
            worldLV,         // its mother  volume
            false,           // no boolean operations
            0,               // copy number
            fCheckOverlaps); // checking overlaps 
    G4cout << " (ECal)" << recoil_chamberLength + recChamberBeSpacing + recChamberAfSpacing 
        << ", " << recoil_chamberLength + recChamberBeSpacing + recChamberAfSpacing + ecalLength << G4endl;

    G4String ECalSDname = "ECalSD";
    CalorimeterSD* ECalSD = new CalorimeterSD(ECalSDname,"ECalHitsCollection", cell_nb,cell_nb,1);
    G4SDManager::GetSDMpointer()->AddNewDetector( ECalSD );

    ECalLV->SetVisAttributes(ECalVisAtt);
    //ECalLV->SetVisAttributes(G4VisAttributes::Invisible);

    G4double ECal_X = 0;
    G4double ECal_Y = 0;
    G4double ECal_Z = 0;
    const G4int cell_total = cell_nb*cell_nb;
    G4double rx,ry,rz; //local temp variable
    G4double gx,gy,gz;
    rz = 0.5 * ecalLength;
    rx = 0.5 * cell_length;
    ry = 0.5 * cell_length;
    gx = 0.5 * cellgas_length_xy;
    gy = 0.5 * cellgas_length_xy;
    gz = 0.5 * cellgas_length_z;

    G4Box* chamberS_ECal
        = new G4Box("eCalchamber", rx, ry, rz);
    G4Box* chamberS_ECal_Gas
	= new G4Box("eCalgaschamber",gx,gy,gz);
    G4SubtractionSolid* ECal_Shell
	= new G4SubtractionSolid("eCalShell",chamberS_ECal,chamberS_ECal_Gas);

    fLogicChamber_ECal = new G4LogicalVolume*[cell_total];
    fLogicChamber_ECal_Gas = new G4LogicalVolume*[cell_total];
    ECal_Y =  1.0*( ecalradius ) - 0.5*cell_length - 0.5*holder_gap;
    for (G4int yNo=0; yNo<cell_nb; yNo++) {
        ECal_X = -1.0*( ecalradius ) + 0.5*cell_length + 0.5*holder_gap;
        for (G4int xNo=0; xNo<cell_nb; xNo++) {
            //G4cout<<xNo<<", X: "<<ECal_X<<G4endl;
            //G4cout<<yNo<<", Y: "<<ECal_Y<<G4endl;

            fLogicChamber_ECal[yNo*cell_nb+xNo] =
                new G4LogicalVolume(ECal_Shell,Al,"ECalChamber",0,0,0);
	        fLogicChamber_ECal_Gas[yNo*cell_nb+xNo] = 
		        new G4LogicalVolume(chamberS_ECal_Gas,LYSO,"ECalgasChamber",0,0,0);

            fLogicChamber_ECal_Gas[yNo*cell_nb+xNo]->SetSensitiveDetector( ECalSD );
            fLogicChamber_ECal_Gas[yNo*cell_nb+xNo]->SetVisAttributes(ECalVisAtt);
	        fLogicChamber_ECal[yNo*cell_nb+xNo]->SetVisAttributes(chamberVisAtt);
            fLogicChamber_ECal[yNo*cell_nb+xNo]->SetVisAttributes(G4VisAttributes::Invisible);
            if(!vis_ECal) fLogicChamber_ECal_Gas[yNo*cell_nb+xNo]->SetVisAttributes(G4VisAttributes::Invisible);

            new G4PVPlacement(0,                  // no rotation
                    G4ThreeVector( ECal_X, ECal_Y, ECal_Z ), // at (x,y,z)
                    fLogicChamber_ECal[yNo*cell_nb+xNo],      // its logical volume
                    "ECALChamber",                    // its name
                    ECalLV,                    // its mother  volume
                    false,                        // no boolean operations
                    xNo*cell_nb+yNo,                       // copy number
                    fCheckOverlaps);              // checking overlaps

            new G4PVPlacement(0,                  // no rotation
                    G4ThreeVector( ECal_X, ECal_Y, ECal_Z ), // at (x,y,z)
                    fLogicChamber_ECal_Gas[yNo*cell_nb+xNo],      // its logical volume
                    "ECALgasChamber",                    // its name
                    ECalLV,                    // its mother  volume
                    false,                        // no boolean operations
                    xNo*cell_nb+yNo,                       // copy number
                    fCheckOverlaps);              // checking overlaps
           
		    ECal_X += (cell_length + ( (xNo % 5 == 4) ? holder_gap : 0) );
            //ECal_X += cell_length;
        }
        ECal_Y -= (cell_length + ( (yNo % 5 == 4) ? holder_gap : 0) );
        //ECal_Y -= cell_length;
    }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void DetectorConstruction::DefineHCal(G4bool RealConstruct=false)
{
//////////////////////////////////////////////////////////////////////
//NEED:
  //ecalLength
  //recoil_chamberLength 
  //recChamberBeSpacing
  //recChamberAfSpacing 
//SET:
  //cellH_xlen
  //cellH_ylen
  //HCalLength
  //outHCal_xlen
  //outHCal_ylen
//////////////////////////////////////////////////////////////////////

    //define variable
    
    const G4int cellH_nz = 100;
    const G4int cellH_nx = 1;
    const G4int cellH_ny = 60;
    G4double cellH_xlen = 300.0*cm; // total length of RPC
    G4double cellH_ylen = 5.0*cm;
    G4double cellH_zlen = 3.0*cm;
    outHCal_xlen = cellH_xlen*cellH_nx;
    outHCal_ylen = cellH_ylen*cellH_ny;
    HCalLength = cellH_nz * cellH_zlen;
    if(!RealConstruct) return;

    ////////////////////////////////////////////////////////////////////////

    G4double cellH_xthick = 1.0*mm; // the thickness of the outer shell
    G4double cellH_ythick = 1.0*mm;
    G4double cellH_zthick = 1.0*cm;
    G4double cellgas_xlen = cellH_xlen - 2.0*cellH_xthick; // the thickness of the outer shell
    G4double cellgas_ylen = cellH_ylen - 2.0*cellH_ythick;
    G4double cellgas_zlen = cellH_zlen - 2.0*cellH_zthick;    
    G4double Edge_Spacing = 0.1*mm;

    //preparation materials (note: need define before or use builtin material)
    G4Material* vacuum  = G4Material::GetMaterial("vacuum");
    G4Material* Iron  = G4Material::GetMaterial("G4_Fe");
    //G4Material* RPCGas = G4Material::GetMaterial("RPCGas");
    G4Material* RPCGas = G4Material::GetMaterial("Polystyrene");
    //G4Material* Glass = G4Material::GetMaterial("Borosilicate glass");

    //build
    G4ThreeVector positionHCal = G4ThreeVector( 0, 0,
                                                ecalLength + recoil_chamberLength 
                                              + recChamberBeSpacing + recChamberAfSpacing 
                                              + 0.5*HCalLength + Edge_Spacing);
    G4Box* HCalS
	    = new G4Box("hcal", 0.5*cellH_nx*cellH_xlen, 0.5*cellH_ny*cellH_ylen, 0.5*HCalLength);
    HCalLV
        = new G4LogicalVolume(HCalS, vacuum, "HCAL",0,0,0);  
    new G4PVPlacement(0,               // no rotation
                      positionHCal, // at (x,y,z)
                      HCalLV,       // its logical volume
                      "HCAL",       // its name
                      worldLV,         // its mother  volume
                      false,           // no boolean operations
                      0,               // copy number
                      fCheckOverlaps); // checking overlaps 
    G4cout << " (HCal)" << recoil_chamberLength + recChamberBeSpacing + recChamberAfSpacing + ecalLength 
           << ", " << recoil_chamberLength + recChamberBeSpacing + recChamberAfSpacing + ecalLength + HCalLength<< G4endl;
  
    G4String HCalSDname = "HCalSD";
    HCalSD = new CalorimeterSD(HCalSDname,"HCalHitsCollection", cellH_nx , cellH_ny , cellH_nz );
    G4SDManager::GetSDMpointer() -> AddNewDetector(HCalSD);
  
    HCalLV->SetVisAttributes(HCalVisAtt);
    //HCalLV->SetVisAttributes (G4VisAttributes::Invisible);
    
    G4double HCal_X = 0;
    G4double HCal_Y = 0;
    G4double HCal_Z = 0;
    const G4int cellH_total = cellH_nx * cellH_ny * cellH_nz;
    G4double rx,ry,rz,gx,gy,gz; //local temp variable
  
    rx = 0.5 * cellH_xlen;
    ry = 0.5 * cellH_ylen;
    rz = 0.5 * cellH_zlen;
    gx = 0.5 * cellgas_xlen;
    gy = 0.5 * cellgas_ylen;
    gz = 0.5 * cellgas_zlen;
  
    G4Box* chamberS_HCal
        = new G4Box("hCalchamber", rx, ry, rz);
    G4Box* chamberS_HCal_Gas
        = new G4Box("hCalgaschamber", gx, gy, gz);
    G4SubtractionSolid* HCal_RPC_Shell
        = new G4SubtractionSolid("hCalRPCShell", chamberS_HCal, chamberS_HCal_Gas);
  
    HCal_RPC_LV = new G4LogicalVolume*[cellH_total];
    HCal_RPC_GAS_LV = new G4LogicalVolume*[cellH_total];
    HCal_Z =  -0.5*( cellH_nz*cellH_zlen ) + 0.5*cellH_zlen;
    for (G4int zNo=0; zNo<cellH_nz; zNo++) {
      HCal_Y =  0.5*( cellH_ny*cellH_ylen ) - 0.5*cellH_ylen;
      for (G4int yNo=0; yNo<cellH_ny; yNo++) {
        HCal_X = -0.5*( cellH_nx*cellH_xlen ) + 0.5*cellH_xlen;
        for (G4int xNo=0; xNo<cellH_nx; xNo++) {
        
            G4int idx = zNo*cellH_nx*cellH_ny+yNo*cellH_nx+xNo;
            HCal_RPC_LV[idx] =
                new G4LogicalVolume(HCal_RPC_Shell, Iron ,"HCalRPCShell",0,0,0);
  
            HCal_RPC_GAS_LV[idx] =
                new G4LogicalVolume(chamberS_HCal_Gas, RPCGas ,"HCalRPCGasChamber",0,0,0);
  
            HCal_RPC_GAS_LV[idx]->SetSensitiveDetector( HCalSD );
            HCal_RPC_GAS_LV[idx]->SetVisAttributes(HCalVisAtt);
            HCal_RPC_LV[idx]->SetVisAttributes(chamberVisAtt);
            HCal_RPC_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
            if(!vis_HCal) HCal_RPC_GAS_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
            
            new G4PVPlacement(0,                  // no rotation
              	  G4ThreeVector( HCal_X, HCal_Y, HCal_Z ), // at (x,y,z)
              	  HCal_RPC_LV[idx],      // its logical volume
              	  "HCALRPCShell",                    // its name
              	  HCalLV,                    // its mother  volume
              	  false,                        // no boolean operations
              	  idx,                       // copy number
              	  fCheckOverlaps);              // checking overlaps
  
            new G4PVPlacement(0,                  // no rotation
              	  G4ThreeVector( HCal_X, HCal_Y, HCal_Z ), // at (x,y,z)
              	  HCal_RPC_GAS_LV[idx],      // its logical volume
              	  "HCALRPCGas",                    // its name
              	  HCalLV,                    // its mother  volume
              	  false,                        // no boolean operations
              	  idx,                       // copy number
              	  fCheckOverlaps);              // checking overlaps
  
            HCal_X += cellH_xlen;
        }
        HCal_Y -= cellH_ylen;
      }
      HCal_Z += cellH_zlen;
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineHCE(G4bool RealConstruct = false)
{
    //////////////////////////////////////////////////////////////////////
    //NEED:
    //positionECal
    //outHCal_xlen
    //outHCal_ylen
    //ecalradius
    //ecalLength
    //SET:
    //NULL
    //////////////////////////////////////////////////////////////////////
    // HCal Edge 
    //here this part has no global dependency geometry parameter. 
    //so directlly return if not realconstruct
    if(!RealConstruct) return; 

    //here: the var depend on HCal is cellH_xlen*cellH_nx 
    //not cellH_xlen and cellH_xlen indivadul,so
    //G4double outHCE_xlen = cellH_xlen*cellH_nx; 
    //G4double outHCE_ylen = cellH_ylen*cellH_ny; 
    G4double outHCE_xlen = outHCal_xlen; 
    G4double outHCE_ylen = outHCal_ylen; 
    G4double outHCE_zlen = ecalLength + 0.1*cm;
    
    G4Material* vacuum  = G4Material::GetMaterial("vacuum");
    G4Material* Iron  = G4Material::GetMaterial("G4_Fe");
    G4Material* RPCGas = G4Material::GetMaterial("RPCGas");
    
    G4ThreeVector positionHCE = positionECal;
    G4Box* HCalES
      = new G4Box("hcale", 0.5 * outHCE_xlen, 0.5 * outHCE_ylen, 0.5 * outHCE_zlen); //only use once so local
    G4SubtractionSolid* HCal_Edge
      = new G4SubtractionSolid("HCalEdge", HCalES, ECalS);
    
    HCalELV
      = new G4LogicalVolume(HCal_Edge, vacuum, "HCalE",0,0,0);  
    new G4PVPlacement(0,               // no rotation
                      positionHCE, // at (x,y,z)
                      HCalELV,       // its logical volume
                      "HCALE",       // its name
                      worldLV,         // its mother  volume
                      false,           // no boolean operations
                      0,               // copy number
                      fCheckOverlaps); // checking overlaps 
    
    HCalELV->SetVisAttributes(HCalVisAtt);
    HCalELV->SetVisAttributes(G4VisAttributes::Invisible);
    
    // HCE Cap
    const G4int capHCE_nx = 3; // cell number along cap HCE
    const G4int capHCE_ny = 50;
    const G4int capHCE_nz = 1;
    G4double capHCE_xlen = outHCE_xlen; // cap length
    //G4double capHCE_ylen = 0.5*(outHCE_ylen - cell_nb*cell_length); // cap length
    G4double capHCE_ylen = 0.5*(outHCE_ylen - 2.*ecalradius); // cap length
    G4double capHCE_zlen = outHCE_zlen;
    G4double capCell_xlen = capHCE_xlen/capHCE_nx;
    G4double capCell_ylen = capHCE_ylen/capHCE_ny;
    G4double capCell_zlen = capHCE_zlen/capHCE_nz;
    
    G4double HCECap_xthick = 1.0*mm; // the thickness of the outer shell
    G4double HCECap_ythick = 7.5*mm;
    G4double HCECap_zthick = 1.0*mm;
    G4double capgas_xlen = capCell_xlen - 2.0*HCECap_xthick; // the thickness of the outer shell
    G4double capgas_ylen = capCell_ylen - 2.0*HCECap_ythick;
    G4double capgas_zlen = capCell_zlen - 2.0*HCECap_zthick;
    
    G4String HCECapSDname = "HCECapSD";
    HCECapSD = new CalorimeterSD(HCECapSDname, "HCECapHitsCollection", capHCE_nx, 2 * capHCE_ny, capHCE_nz);
    G4SDManager::GetSDMpointer()->AddNewDetector(HCECapSD);
    
    G4double HCECap_X = 0;
    G4double HCECap_Y = 0;
    G4double HCECap_Z = 0;
    const G4int HCECap_total = 2 * capHCE_nx * capHCE_ny * capHCE_nz;
    
    G4double rx,ry,rz,gx,gy,gz; //temp var, so local
    rx = 0.5 * capCell_xlen;
    ry = 0.5 * capCell_ylen;
    rz = 0.5 * capCell_zlen;
    
    gx = 0.5 * capgas_xlen;
    gy = 0.5 * capgas_ylen;
    gz = 0.5 * capgas_zlen;
    
    G4Box* chamberS_HCalCap
    	= new G4Box("hCalcapchamber", rx, ry, rz);
    G4Box* chamberS_HCalCap_Gas
    	= new G4Box("hCalcapgaschamber", gx, gy, gz);
    G4SubtractionSolid* HCalCap_RPC_Shell
    	= new G4SubtractionSolid("hCalcapRPCShell",
    		chamberS_HCalCap, chamberS_HCalCap_Gas);
    
    HCECap_RPC_LV = new G4LogicalVolume*[HCECap_total];
    HCECap_RPC_GAS_LV = new G4LogicalVolume*[HCECap_total];
    // Loop on up cap first
    HCECap_Z = -0.5*(capHCE_zlen)+0.5*capCell_zlen;
    for (G4int zNo = 0; zNo<capHCE_nz; zNo++) {
    	HCECap_Y = 0.5*(outHCE_ylen)-0.5*capCell_ylen;
    	for (G4int yNo = 0; yNo<capHCE_ny; yNo++) {
    		HCECap_X = -0.5*(capHCE_xlen)+0.5*capCell_xlen;
    		for (G4int xNo = 0; xNo<capHCE_nx; xNo++) {
    
    			G4int idx = zNo * capHCE_nx*capHCE_ny + yNo * capHCE_nx + xNo;
    			HCECap_RPC_LV[idx] =
    				new G4LogicalVolume(HCalCap_RPC_Shell, Iron, "HCECapRPCShell", 0, 0, 0);
    
    			HCECap_RPC_GAS_LV[idx] =
    				new G4LogicalVolume(chamberS_HCalCap_Gas, RPCGas, "HCECapRPCGasChamber", 0, 0, 0);
    
    			HCECap_RPC_GAS_LV[idx]->SetSensitiveDetector(HCECapSD);
    			HCECap_RPC_GAS_LV[idx]->SetVisAttributes(HCalVisAtt);
    			HCECap_RPC_GAS_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
    			HCECap_RPC_LV[idx]->SetVisAttributes(chamberVisAtt);
    			if (!vis_HCECap) HCECap_RPC_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
    
    			new G4PVPlacement(0,                  // no rotation
    				G4ThreeVector(HCECap_X, HCECap_Y, HCECap_Z), // at (x,y,z)
    				HCECap_RPC_LV[idx],      // its logical volume
    				"HCECapRPCShell",                    // its name
    				HCalELV,                    // its mother  volume
    				false,                        // no boolean operations
    				idx,                       // copy number
    				fCheckOverlaps);              // checking overlaps
    
    			new G4PVPlacement(0,                  // no rotation
    				G4ThreeVector(HCECap_X, HCECap_Y, HCECap_Z), // at (x,y,z)
    				HCECap_RPC_GAS_LV[idx],      // its logical volume
    				"HCECapRPCGas",                    // its name
    				HCalELV,                    // its mother  volume
    				false,                        // no boolean operations
    				idx,                       // copy number
    				fCheckOverlaps);              // checking overlaps
    
    			HCECap_X += capCell_xlen;
    		}
    		HCECap_Y -= capCell_ylen;
    	}
    	HCECap_Z += capCell_zlen;
    }
    // Loop on low cap then
    HCECap_Z = -0.5*(capHCE_zlen)+0.5*capCell_zlen;
    for (G4int zNo = 0; zNo<capHCE_nz; zNo++) {
    	//HCECap_Y = -0.5*(cell_nb*cell_length) - 0.5*capCell_ylen;
                HCECap_Y = -ecalradius - 0.5*capCell_ylen;
    	for (G4int yNo = 0; yNo<capHCE_ny; yNo++) {
    		HCECap_X = -0.5*(capHCE_xlen)+0.5*capCell_xlen;
    		for (G4int xNo = 0; xNo<capHCE_nx; xNo++) {
    
    			G4int idx = capHCE_nx * capHCE_ny*capHCE_nz
    				+ zNo * capHCE_nx*capHCE_ny + yNo * capHCE_nx + xNo;
    			HCECap_RPC_LV[idx] =
    				new G4LogicalVolume(HCalCap_RPC_Shell, Iron, "HCECapRPCShell", 0, 0, 0);
    
    			HCECap_RPC_GAS_LV[idx] =
    				new G4LogicalVolume(chamberS_HCalCap_Gas, RPCGas, "HCECapRPCGasChamber", 0, 0, 0);
    
    			HCECap_RPC_GAS_LV[idx]->SetSensitiveDetector(HCECapSD);
    			HCECap_RPC_GAS_LV[idx]->SetVisAttributes(HCalVisAtt);
    			HCECap_RPC_GAS_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
    			HCECap_RPC_LV[idx]->SetVisAttributes(chamberVisAtt);
    			if (!vis_HCECap) HCECap_RPC_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
    
    			new G4PVPlacement(0,                  // no rotation
    				G4ThreeVector(HCECap_X, HCECap_Y, HCECap_Z), // at (x,y,z)
    				HCECap_RPC_LV[idx],      // its logical volume
    				"HCECapRPCShell",                    // its name
    				HCalELV,                    // its mother  volume
    				false,                        // no boolean operations
    				idx,                       // copy number
    				fCheckOverlaps);              // checking overlaps
    
    			new G4PVPlacement(0,                  // no rotation
    				G4ThreeVector(HCECap_X, HCECap_Y, HCECap_Z), // at (x,y,z)
    				HCECap_RPC_GAS_LV[idx],      // its logical volume
    				"HCECapRPCGas",                    // its name
    				HCalELV,                    // its mother  volume
    				false,                        // no boolean operations
    				idx,                       // copy number
    				fCheckOverlaps);              // checking overlaps
    
    			HCECap_X += capCell_xlen;
    		}
    		HCECap_Y -= capCell_ylen;
    	}
    	HCECap_Z += capCell_zlen;
    }
    
     // HCE Side
    const G4int sideHCE_nx = 50; // cell number along side HCE
    const G4int sideHCE_ny = 1;
    const G4int sideHCE_nz = 1;
    //G4double sideHCE_xlen = 0.5*(outHCE_xlen - cell_nb*cell_length);
    G4double sideHCE_xlen = 0.5*(outHCE_xlen - 2.*ecalradius);
    //G4double sideHCE_ylen = cell_nb*cell_length; // side length
    G4double sideHCE_ylen = 2.*ecalradius; // side length
    G4double sideHCE_zlen = outHCE_zlen;
    G4double sideCell_xlen = sideHCE_xlen/sideHCE_nx;
    G4double sideCell_ylen = sideHCE_ylen/sideHCE_ny;
    G4double sideCell_zlen = sideHCE_zlen/sideHCE_nz;
    
    G4double HCESide_xthick = 1.0*mm; // the thickness of the outer shell
    G4double HCESide_ythick = 7.5*mm;
    G4double HCESide_zthick = 1.0*mm;
    G4double sidegas_xlen = sideCell_xlen - 2.0*HCESide_xthick; // the thickness of the outer shell
    G4double sidegas_ylen = sideCell_ylen - 2.0*HCESide_ythick;
    G4double sidegas_zlen = sideCell_zlen - 2.0*HCESide_zthick; 
    
    G4String HCESideSDname = "HCESideSD";
    HCESideSD = new CalorimeterSD(HCESideSDname, "HCESideHitsCollection", sideHCE_nx, 2 * sideHCE_ny, sideHCE_nz);
    G4SDManager::GetSDMpointer()->AddNewDetector(HCESideSD);
    
    G4double HCESide_X = 0;
    G4double HCESide_Y = 0;
    G4double HCESide_Z = 0;
    const G4int HCESide_total = 2 * sideHCE_nx * sideHCE_ny * sideHCE_nz;
    
    rx = 0.5 * sideCell_xlen;
    ry = 0.5 * sideCell_ylen;
    rz = 0.5 * sideCell_zlen;
    
    gx = 0.5 * sidegas_xlen;
    gy = 0.5 * sidegas_ylen;
    gz = 0.5 * sidegas_zlen;
    
    G4Box* chamberS_HCalSide
    	= new G4Box("hCalsidechamber", rx, ry, rz);
    G4Box* chamberS_HCalSide_Gas
    	= new G4Box("hCalsidegaschamber", gx, gy, gz);
    G4SubtractionSolid* HCalSide_RPC_Shell
    	= new G4SubtractionSolid("hCalsideRPCShell", chamberS_HCalSide, chamberS_HCalSide_Gas);
    
    HCESide_RPC_LV = new G4LogicalVolume*[HCESide_total];
    HCESide_RPC_GAS_LV = new G4LogicalVolume*[HCESide_total];
    // Loop on up side first
    HCESide_Z = -0.5*(sideHCE_zlen)+0.5*sideCell_zlen;
    for (G4int zNo = 0; zNo<sideHCE_nz; zNo++) {
    	HCESide_Y = 0.5*(sideHCE_ylen)-0.5*sideCell_ylen;
    	for (G4int yNo = 0; yNo<sideHCE_ny; yNo++) {
    		HCESide_X = -0.5*(outHCE_xlen)+0.5*sideCell_xlen;
    		for (G4int xNo = 0; xNo<sideHCE_nx; xNo++) {
    
    			G4int idx = zNo * sideHCE_nx*sideHCE_ny + yNo * sideHCE_nx + xNo;
    			HCESide_RPC_LV[idx] =
    				new G4LogicalVolume(HCalSide_RPC_Shell, Iron, "HCESideRPCShell", 0, 0, 0);
    
    			HCESide_RPC_GAS_LV[idx] =
    				new G4LogicalVolume(chamberS_HCalSide_Gas, RPCGas, "HCESideRPCGasChamber", 0, 0, 0);
    
    			HCESide_RPC_GAS_LV[idx]->SetSensitiveDetector(HCESideSD);
    			HCESide_RPC_GAS_LV[idx]->SetVisAttributes(HCalVisAtt);
    			HCESide_RPC_GAS_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
    			HCESide_RPC_LV[idx]->SetVisAttributes(chamberVisAtt);
    			if (!vis_HCESide) HCESide_RPC_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
    
    			new G4PVPlacement(0,                  // no rotation
    				G4ThreeVector(HCESide_X, HCESide_Y, HCESide_Z), // at (x,y,z)
    				HCESide_RPC_LV[idx],      // its logical volume
    				"HCESideRPCShell",                    // its name
    				HCalELV,                    // its mother  volume
    				false,                        // no boolean operations
    				idx,                       // copy number
    				fCheckOverlaps);              // checking overlaps
    
    			new G4PVPlacement(0,                  // no rotation
    				G4ThreeVector(HCESide_X, HCESide_Y, HCESide_Z), // at (x,y,z)
    				HCESide_RPC_GAS_LV[idx],      // its logical volume
    				"HCESideRPCGas",                    // its name
    				HCalELV,                    // its mother  volume
    				false,                        // no boolean operations
    				idx,                       // copy number
    				fCheckOverlaps);              // checking overlaps
    
    			HCESide_X += sideCell_xlen;
    		}
    		HCESide_Y -= sideCell_ylen;
    	}
    	HCESide_Z += sideCell_zlen;
    }
    // Loop on low side then
    HCESide_Z = -0.5*(sideHCE_zlen)+0.5*sideCell_zlen;
    for (G4int zNo = 0; zNo<sideHCE_nz; zNo++) {
    	HCESide_Y = 0.5*(sideHCE_ylen)-0.5*sideCell_ylen;
    	for (G4int yNo = 0; yNo<sideHCE_ny; yNo++) {
    		//HCESide_X = 0.5*(cell_nb*cell_length) + 0.5*sideCell_xlen;
    		HCESide_X = ecalradius + 0.5*sideCell_xlen;
    		for (G4int xNo = 0; xNo<sideHCE_nx; xNo++) {
    
    			G4int idx = sideHCE_nx * sideHCE_ny*sideHCE_nz
    				+ zNo * sideHCE_nx*sideHCE_ny + yNo * sideHCE_nx + xNo;
    			HCESide_RPC_LV[idx] =
    				new G4LogicalVolume(HCalSide_RPC_Shell, Iron, "HCESideRPCShell", 0, 0, 0);
    
    			HCESide_RPC_GAS_LV[idx] =
    				new G4LogicalVolume(chamberS_HCalSide_Gas, RPCGas, "HCESideRPCGasChamber", 0, 0, 0);
    
    			HCESide_RPC_GAS_LV[idx]->SetSensitiveDetector(HCESideSD);
    			HCESide_RPC_GAS_LV[idx]->SetVisAttributes(HCalVisAtt);
    			HCESide_RPC_GAS_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
    			HCESide_RPC_LV[idx]->SetVisAttributes(chamberVisAtt);
    			if (!vis_HCESide) HCESide_RPC_LV[idx]->SetVisAttributes(G4VisAttributes::Invisible);
    
    			new G4PVPlacement(0,                  // no rotation
    				G4ThreeVector(HCESide_X, HCESide_Y, HCESide_Z), // at (x,y,z)
    				HCESide_RPC_LV[idx],      // its logical volume
    				"HCESideRPCShell",                    // its name
    				HCalELV,                    // its mother  volume
    				false,                        // no boolean operations
    				idx,                       // copy number
    				fCheckOverlaps);              // checking overlaps
    
    			new G4PVPlacement(0,                  // no rotation
    				G4ThreeVector(HCESide_X, HCESide_Y, HCESide_Z), // at (x,y,z)
    				HCESide_RPC_GAS_LV[idx],      // its logical volume
    				"HCESideRPCGas",                    // its name
    				HCalELV,                    // its mother  volume
    				false,                        // no boolean operations
    				idx,                       // copy number
    				fCheckOverlaps);              // checking overlaps
    
    			HCESide_X += sideCell_xlen;
    		}
    		HCESide_Y -= sideCell_ylen;
    	}
    	HCESide_Z += sideCell_zlen;
    }
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineHCECap(G4bool RealConstruct = false)
{
	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineHCESide(G4bool RealConstruct = false)
{
	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTargetMaterial(G4String materialName)
{
  G4NistManager* nistManager = G4NistManager::Instance();
  G4bool fromIsotopes = false;

  G4Material* pttoMaterial =
              nistManager->FindOrBuildMaterial(materialName, fromIsotopes);

  if (fTargetMaterial != pttoMaterial) {
     if ( pttoMaterial ) {
        fTargetMaterial = pttoMaterial;
        if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
        G4cout << "\n----> The target is made of " << materialName << G4endl;
     } else {
        G4cout << "\n-->  WARNING from SetTargetMaterial : "
               << materialName << " not found" << G4endl;
     }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void DetectorConstruction::DefineWorld()
{
    //WorldLength and Material
    G4double worldLength = 4 * (recoil_chamberLength + recChamberBeSpacing + recChamberAfSpacing + ecalradius + HCalLength);

    G4Material* vacuum = G4Material::GetMaterial("vacuum");
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);
    
    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
           << " mm" << G4endl;
    //worldbox and Logical Volume
    G4Box* worldS
      = new G4Box("world",                                    //its name
                  worldLength/2,worldLength/2,worldLength/2); //its size
    worldLV
      = new G4LogicalVolume(
                   worldS,   //its solid
                   vacuum,      //its material
                   "World"); //its name
   
    // Place the World Physical volume unrotated at (0,0,0).
    worldPV
        = new G4PVPlacement(
                 0,               // no rotation
                 G4ThreeVector(), // at (0,0,0)
                 worldLV,         // its logical volume
                 "World",         // its name
                 0,               // its mother  volume
                 false,           // no boolean operations
                 0,               // copy number
                 fCheckOverlaps); // checking overlaps 

    worldLV->SetVisAttributes(boxVisAtt);
    if(!vis_World) worldLV->SetVisAttributes(G4VisAttributes::Invisible);
    G4cout << "-W: " << worldLength/2 ;
}

void DetectorConstruction::SetMagField(G4double fieldValue)
{
  fMagField->SetMagFieldValue(fieldValue);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetMaxStep(G4double maxStep)
{
  if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::SetTTrkMagField(G4double magField)
{
    fTTrkMagField = magField;
    G4UniformMagField* magFieldT
	    = new G4UniformMagField(G4ThreeVector(fTTrkMagField*tesla,0.,0.)); // Tagging Tracker
    G4FieldManager* localFieldMgrT=
	    new G4FieldManager(magFieldT);
    trackerTLV->SetFieldManager(localFieldMgrT, true);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

void DetectorConstruction::SetRTrkMagField(G4double magField)
{
/*
note: G4FieldManager can be reuse, but seems we do not need set multiple field for the tracker.
so all of these ptrs is local.
Now: ONE partial field of each tracker(R/T) and ONE global field is avliable
*/

    fRTrkMagField = magField;//this global used to get current field value,  for simple.
    G4UniformMagField* magFieldR
	    = new G4UniformMagField(G4ThreeVector(fRTrkMagField*tesla,0.,0.)); // Tagging Tracker
    G4FieldManager* localFieldMgrR=
	    new G4FieldManager(magFieldR);
    trackerRLV->SetFieldManager(localFieldMgrR, true);

    G4RunManager::GetRunManager()->GeometryHasBeenModified();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

