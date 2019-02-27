
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


double mmod(double a, double m)
{
	return fmod(a, m) + (a<0?m:0);
}


double RedVal(double val)
{
	//return (0.0/6.0 <= val && val < 1.0/6.0 ? 1 : 0) + (1.0/6.0 <= val && val < 2.0/6.0 ? -6.0*val+2.0 : 0) + (4.0/6.0 <= val && val < 5.0/6.0 ? 6.0*val-4.0 : 0) + (5.0/6.0 <= val && val < 6.0/6.0 ? 1 : 0);
	return val = 2.0-abs(6.0*(0.5-abs(val-0.5))), val=MIN(val,1.0), MAX(val,0.0);
}
double GreenVal(double val)
{
	//return (0.0/6.0 <= val && val < 1.0/6.0 ? 6.0*val : 0) + (1.0/6.0 <= val && val < 3.0/6.0 ? 1 : 0) + (3.0/6.0 <= val && val < 4.0/6.0 ? -6.0*val+4.0 : 0);
	return val = 2.0-abs(6.0*val-2.0), val=MIN(val,1.0), MAX(val,0.0);
}
double BlueVal(double val)
{
	//return (2.0/6.0 <= val && val < 3.0/6.0 ? 6.0*val-2.0 : 0) + (3.0/6.0 <= val && val < 5.0/6.0 ? 1 : 0) + (5.0/6.0 <= val && val < 6.0/6.0 ? -6.0*val+6.0 : 0);
	return val = 2.0-abs(6.0*val-4.0), val=MIN(val,1.0), MAX(val,0.0);
}

RGB RGBVal(ColourVal val)
{		
	return RGB{(u8)(255.0*RedVal(val.Colour)*val.Scale),(u8)(255.0*GreenVal(val.Colour)*val.Scale),(u8)(255.0*BlueVal(val.Colour)*val.Scale)};
}
RGB RGBScale(RGB rgb, double scale)
{
	return RGB{ (u8)(scale*rgb.R), (u8)(scale*rgb.G), (u8)(scale*rgb.B) };
}
RGB ColourMix(RGB a, RGB b, double w)
{
	return {(u8)(a.R*(1.0-w) + b.R*(w)), (u8)(a.G*(1.0-w) + b.G*(w)), (u8)(a.B*(1.0-w) + b.B*(w))};
}