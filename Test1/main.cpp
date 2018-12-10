
#include "DanceLight.h"
#include "Spectrum.h"

void DrawGraph(Screen &screen, int x, int y, CArray &data, RGB colour, double scale)
{
	for (int i = 0; i <= 18; i++) 
	{
		screen.Draw(x, i+y, colour, {0,0,0}, abs(i-9)+'0');
	}
	for (int i = 0; i < (int)data.size(); ++i)
	{
		screen.Draw(i+1+x, 9+y, colour, {0,0,0}, i+'0');
		int dat = (int)(data[i].real()*scale);
		for (int j = MIN(dat, 0); j <= MAX(dat, 0); j++)
		{
			if (j == 0) { screen.Draw(i+1+x, 9-j+y, {0,0,0}, colour, (i<10?i+'0':'-')); }
			else { screen.Draw(i+1+x, 9-j+y, colour, colour, ' '); }
		}
		
	}
}

void DrawDBGraph(Screen &screen, int x, int y, CArray &data, RGB colour, double scale)
{
	for (int i = 0; i <= 18; i++) 
	{
		screen.Draw(x, i+y, colour, {0,0,0}, abs(i-9)+'0');
	}
	for (int i = 0; i < (int)data.size()/2; ++i)
	{
		screen.Draw(i+1+x, 9+y, colour, {0,0,0}, i+'0');
		
		int real = data[i].real()*scale, imag = data[i].imag()*scale;
		screen.Draw(i+1+x, 9-real+y, colour, {0,0,0}, 'R');
		screen.Draw(i+1+x, 9-imag+y, colour, {0,0,0}, 'I');
		
		int dat = (int)(scale*Mag(data[i]));
		for (int j = MIN(dat, 0); j <= MAX(dat, 0); j++)
		{
			if (j == 0) { screen.Draw(i+1+x, 9-j+y, {0,0,0}, colour, (i<10?i+'0':'-')); }
			else if (j==real) { screen.Draw(i+1+x, 9-j+y, {0,0,0}, colour, 'R'); }
			else if (j==imag) { screen.Draw(i+1+x, 9-j+y, {0,0,0}, colour, 'I'); }
			else { screen.Draw(i+1+x, 9-j+y, colour, colour, ' '); }
		}
		
	}
}

#define LEN 128
#define FREQ(f,t) 2*PI*((double)(t)*(f)/LEN)

int main()
{
	Screen screen(LEN+1, 20*3);
	srand(time(0));

	int i = 0;
	while (!kbhit())
	{
		screen.Clear();
		CArray data(LEN);
		for (int t = 0; t < LEN; t++)
		{
			data[t] = 5*sin(FREQ(2,t-i));//exp(-pow((1.0/10.0)*(t-30),2))*10*cos(FREQ(40,t-i)) + exp(-pow((1.0/10.0)*(t-90),2))*10*sin(FREQ(6,t-i));
		}

		DrawGraph(screen, 0,0, data, {255,0,0}, 1.0);

		// forward fft
		fft(data);

		//std::cout << "fft" << std::endl;
		DrawDBGraph(screen, 0,20, data, {0,255,0}, 1.0);
		/*for (int i = 0; i < (int)data.size(); ++i)
		{
			PRINT(Mag(data[i]) << " ");
		}*/


		// inverse fft
		ifft(data);

		//std::cout << std::endl << "ifft" << std::endl;
		DrawGraph(screen, 0,40, data, {0,100,255}, 1.0);
		PRINT("\n");
		
		PRINT("Press Enter To Continue");
		usleep(30000);
		i++;
	}
	getch();

	return 0;
}
