
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "Globals.h"
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>

#define CSI(a,c) "\033[" a c

#define RESET   CSI("0","m")
#define RED     CSI("1;31","m")
#define GREEN   CSI("1;32","m")
#define BLUE    CSI("1;34","m")
#define MAGENTA CSI("1;35","m")
#define YELLOW  CSI("1;33","m")
#define CYAN    CSI("1;36","m")

#define CLRLINE CSI("0","K")

#define MOVETO(x,y) CSI(<<(y)<<";"<<(x)<<, "H")
#define FCOLOUR(r,g,b) CSI("38;2;"<<(r)<<";"<<(g)<<";"<<(b)<<,"m")
#define BCOLOUR(r,g,b) CSI("48;2;"<<(r)<<";"<<(g)<<";"<<(b)<<,"m")
#define PRINT(s) std::cout<<s<<std::flush
#define PRINTF(s) std::cout<<s

extern struct termios orig_termios;
void reset_terminal_mode();
void set_conio_terminal_mode();
int kbhit();
int getch();

struct ColourChar
{
	RGB Foreground = {0,0,0};
	RGB Background = {0,0,0};
	char Character = 0;
};

class Screen
{
	int Width = 0;
	int Height = 0;
	ColourChar **ScreenBuffer;
public:
	Screen();
	Screen(int height);
	Screen(int width, int height);
	~Screen();

	void Clear();
	Array<int,2> GetDimensions();

	void Draw(int x, int y, RGB foreground, RGB background, char character);
	void DrawToBuffer(int x, int y, RGB foreground, RGB background, char character);
	void ClearBuffer();
	void DisplayBuffer();
	int GetWidth();
	int GetHeight();

private:
	void SavePosition();
	void RecoverPosition();
};

RGB RGBFade(double val);

#endif
