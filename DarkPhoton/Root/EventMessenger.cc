#include "EventMessenger.hh"
#include "RootManager.hh"

#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//

EventMessenger::EventMessenger(RootManager* rootMng)
	 : G4UImessenger(), froot(rootMng)
{
  fEventDirectory = new G4UIdirectory("/DP/Event/");
  fEventDirectory->SetGuidance("Event operations");

  fEvtJobCmd = new G4UIcmdWithAnInteger("/DP/Event/StartID",this);
  fEvtJobCmd->SetGuidance("Set the Evt Start ID for this job.");
  fEvtJobCmd->SetParameterName("startid",false);
  fEvtJobCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

  fJobNbCmd = new G4UIcmdWithAnInteger("/DP/Event/NbEvent",this);
  fJobNbCmd->SetGuidance("Set the Evt Number for this job.");
  fJobNbCmd->SetParameterName("evtNb",false);
  fJobNbCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

}

EventMessenger::~EventMessenger()
{
  delete fEvtJobCmd;
  delete fJobNbCmd;
}

void EventMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  if( command == fEvtJobCmd )
  { 
	froot->SetStartID(fEvtJobCmd->GetNewIntValue(newValue));
  }

  if( command == fJobNbCmd )
  { 
	froot->SetNbEvent(fJobNbCmd->GetNewIntValue(newValue));
  }
}

