
#include "Globals.h"
#include "TextScreen.h"
#include "DanceController.h"
#include "InputController.h"

//DanceController Dance;
//InputController Input;
u64 StartTime = GetMilliseconds();
TerminalBuffer TermBuffer(10, 900, 30);

class FOO
{
public:
	static int gg;
};

void InputProgram(char *name)
{
	std::cout << "Beginning Init Program\n";
	if (name) { InputController::InputFromFile(name); }
	else { InputController::InputFromMic(); }
	//InputController::InputFromMath();
	std::cout << "Exit Init Program\n";
}

void DanceProgram()
{
	std::cout << "Beginning Dance Program\n";
	DanceController::UpdateLoop();
	
	std::cout << "Exit Dance Program\n";
}

int main(int argc, char **argv)
{
	set_conio_terminal_mode();
	
	PixelTerm::Init();
	//if (argc != 2) { std::cout << "ERROR: Incorrect Arguments\n"; return 1; } 
	
	std::cout << "Starting threads\n";
	std::thread danceThread(DanceProgram);
	std::thread inputThread(InputProgram, argc==2?argv[1]:0);
	
	
	while(!kbhit()) { usleep(1000); }
	getch();
	DanceController::Run = false;
	InputController::Run = false;
	
	std::cout << "Stopping...\n";
	
	danceThread.join();
	inputThread.join();
	
	std::cout << "Done\n";
	
	PixelTerm::Close();
	reset_terminal_mode();
	return 0;
}

int main2()
{
	std::cout << sizeof(float) << " " << sizeof(double) << "\n";
	RoundBuffer<int> buff(10);
	
	for (int i = 0; i < 30; i++)
	{
		buff.InsertBegin(i);
		
		for (int j = 0; j < 10; j++)
		{
			std::cout << buff[j] << " ";
		}
		std::cout << "\n";
	}
	
	return 0;
}

