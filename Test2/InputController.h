
class InputController;

#ifndef _INPUT_CONTROLLER_H_
#define _INPUT_CONTROLLER_H_

#include "mad.h"
#include <fstream>
#include "Globals.h"
#include "DanceTypes.h"
#include "DanceController.h"

class InputController
{
public:
	static bool Run;
	static void Callback(int *samples, int length);

	static void InputFromFile(std::string filename);
};

#endif
