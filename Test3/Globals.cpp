
#include "Globals.h"

u64 GetMilliseconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
u64 GetMicroseconds()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
u64 GetNanoseconds()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

double OppCos(double x)
{
	return (1-sin(fmod(x,PI)))*(abs(fmod(x,TAU)-PI) >= PI/2.0 ? 1 : -1);
}

double RedVal(double val)
{
	//return (0.0 <= val && val < 1.0/3.0 ? (1.0-3.0*val) : 0) + (2.0/3.0 < val && val <= 1.0 ? (3.0*val-2.0) : 0);
	//return 0.0 <= val && val <= 1.0/3.0 ? (OppCos(3.0*PI*val)+1.0)/2.0 : (2.0/3.0 <= val && val <= 1.0 ? (OppCos(3.0*PI*(val+1.0))+1.0)/2.0 : 0);
	//return (0.0/6.0 <= val && val < 1.0/6.0 ? 1 : 0) + (1.0/6.0 <= val && val < 2.0/6.0 ? -6.0*val+2.0 : 0) + (4.0/6.0 <= val && val < 5.0/6.0 ? 6.0*val-4.0 : 0) + (5.0/6.0 <= val && val < 6.0/6.0 ? 1 : 0);
	return val = 2.0-abs(6.0*(0.5-abs(val-0.5))), val=MIN(val,1.0), MAX(val,0.0);
}

double GreenVal(double val)
{
	//return (0.0 <= val && val < 1.0/3.0 ? (3.0*val) : 0) + (1.0/3.0 < val && val <= 2.0/3.0 ? (2.0-3.0*val) : 0);
	//return 0.0 <= val && val <= 2.0/3.0 ? (OppCos(3.0*PI*(val+1.0/3.0))+1.0)/2.0 : 0;
	//return (0.0/6.0 <= val && val < 1.0/6.0 ? 6.0*val : 0) + (1.0/6.0 <= val && val < 3.0/6.0 ? 1 : 0) + (3.0/6.0 <= val && val < 4.0/6.0 ? -6.0*val+4.0 : 0);
	return val = 2.0-abs(6.0*val-2.0), val=MIN(val,1.0), MAX(val,0.0);
}

double BlueVal(double val)
{
	//return (1.0/3.0 <= val && val < 2.0/3.0 ? (3.0*val-1.0) : 0)	+ (2.0/3.0 < val && val <= 1.0 ? (3.0-3.0*val) : 0);
	//return 1.0/3.0 <= val && val <= 1.0 ? (OppCos(3.0*PI*(val+2.0/3.0))+1.0)/2.0 : 0;
	//return (2.0/6.0 <= val && val < 3.0/6.0 ? 6.0*val-2.0 : 0) + (3.0/6.0 <= val && val < 5.0/6.0 ? 1 : 0) + (5.0/6.0 <= val && val < 6.0/6.0 ? -6.0*val+6.0 : 0);
	return val = 2.0-abs(6.0*val-4.0), val=MIN(val,1.0), MAX(val,0.0);
}

RGB ColourVal(double val)
{
	/*return RGB {
		(u8)(255.0*((0.0 <= val && val < 1.0/3.0 ? (1.0-3.0*val) : 0)		+ (2.0/3.0 < val && val <= 1.0 ? (3.0*val-2.0) : 0))),
		(u8)(255.0*((0.0 <= val && val < 1.0/3.0 ? (3.0*val) : 0)			+ (1.0/3.0 < val && val <= 2.0/3.0 ? (2.0-3.0*val) : 0))),
		(u8)(255.0*((1.0/3.0 <= val && val < 2.0/3.0 ? (3.0*val-1.0) : 0)	+ (2.0/3.0 < val && val <= 1.0 ? (3.0-3.0*val) : 0)))};*/
		
	/*return RGB {
		(u8)(255.0*(0.0 <= val && val <= 1.0/3.0 ? (OppCos(3.0*PI*val)+1.0)/2.0 : (2.0/3.0 <= val && val <= 1.0 ? (OppCos(3.0*PI*(val+1.0))+1.0)/2.0 : 0))),
		(u8)(255.0*(0.0 <= val && val <= 2.0/3.0 ? (OppCos(3.0*PI*(val+1.0/3.0))+1.0)/2.0 : 0)),
		(u8)(255.0*(1.0/3.0 <= val && val <= 1.0 ? (OppCos(3.0*PI*(val+2.0/3.0))+1.0)/2.0 : 0))};*/
		
	return RGB{(u8)(255.0*RedVal(val)),(u8)(255.0*GreenVal(val)),(u8)(255.0*BlueVal(val))};
}

RGB ColourScale(RGB rgb, double scale)
{
	return RGB{ (u8)(scale*rgb.R), (u8)(scale*rgb.G), (u8)(scale*rgb.B) };
}

double mmod(double a, double m)
{
	return fmod(a, m) + (a<0?m:0);
}
