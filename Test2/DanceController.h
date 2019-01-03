
class DanceController;

#ifndef _DANCE_CONTROLLER_H_
#define _DANCE_CONTROLLER_H_

#include "Globals.h"
#include "DanceLight.h"
#include "DanceTypes.h"

#include "InputController.h"


class DanceController
{
public:
	static BeatCycle PrimaryBeat;
	static BeatCycle OffBeat;
	static bool Run;
	
	static time_t LastUpdate;
	
	static std::vector<DanceLight> DanceLightList;
	
	//Asynchronous
	static void Input(BeatCycle primayBeat, BeatCycle secondaryBeat, Melody mainMelody);
	
	static void UpdateLoop();
};

#endif