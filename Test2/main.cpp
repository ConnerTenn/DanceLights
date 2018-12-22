
#include "Globals.h"
#include "DanceController.h"
#include "InputController.h"

DanceController Dance;
InputController Input;

void InputProgram()
{
	std::cout << "Beginning Init Program\n";
	Input.InputFromFile("audio.mp3");
}

void DanceProgram()
{
	std::cout << "Beginning Dance Program\n";
	Dance.UpdateLoop();
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
	Dance.Run = false;
	Input.Run = false;
	
	danceThread.join();
	inputThread.join();
	
	std::cout << "Done\n";
	reset_terminal_mode();
	return 0;
}