
#include "Globals.h"
#include "Screen.h"
#include "DanceLight.h"
#include "DanceTypes.h"

#include "InputController.h"


class DanceController
{
public:
	BeatCycle PrimaryBeat;
	BeatCycle OffBeat;
	bool Run = true;
	
	time_t LastUpdate;
	
	std::vector<DanceLight> DanceLightList;
	
	//Asynchronous
	void Input(BeatCycle primayBeat, BeatCycle secondaryBeat, Melody mainMelody);
	
	void UpdateLoop();
};

