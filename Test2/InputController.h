
#ifndef _INPUT_CONTROLLER_H_
#define _INPUT_CONTROLLER_H_

#include "Globals.h"
#include "DanceTypes.h"
#include "mad.h"
#include <fstream>

class InputController
{
public:
	bool Run = true;
	void Callback(int *samples, int length);

	void InputFromFile(std::string filename);
};

#endif
