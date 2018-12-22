
#include "Globals.h"
#include "DanceController.h"
#include "InputController.h"

//DanceController Dance;
//InputController Input;

class FOO
{
public:
	static int gg;
};

void InputProgram()
{
	std::cout << "Beginning Init Program\n";
	InputController::InputFromFile("audio.mp3");
}

void DanceProgram()
{
	std::cout << "Beginning Dance Program\n";
	DanceController::UpdateLoop();
}

int main(int argc, char **argv)
{
	set_conio_terminal_mode();
	
	//if (argc != 2) { std::cout << "ERROR: Incorrect Arguments\n"; return 1; } 
	
	std::cout << "Starting threads\n";
	std::thread danceThread(DanceProgram);
	std::thread inputThread(InputProgram);
	
	
	while(!kbhit()) { }
	getch();
	DanceController::Run = false;
	InputController::Run = false;
	
	std::cout << "Stopping...\n";
	
	danceThread.join();
	inputThread.join();
	
	std::cout << "Done\n";
	reset_terminal_mode();
	return 0;
}