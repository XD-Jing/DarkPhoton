
#ifndef EventMessenger_h
#define EventMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "RootManager.hh"

class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;
class RootManager;


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class EventMessenger: public G4UImessenger
{
  public:

	  EventMessenger(RootManager* rootMng);
	  virtual ~EventMessenger();
	  virtual void SetNewValue(G4UIcommand*, G4String);

  private:

	  RootManager*             froot;
	  G4UIdirectory*           fEventDirectory;
      G4UIcmdWithAnInteger*    fEvtJobCmd;
      G4UIcmdWithAnInteger*    fJobNbCmd;
	
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


