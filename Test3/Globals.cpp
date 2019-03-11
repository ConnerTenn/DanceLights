
#include "Globals.h"

i64 GetMilliseconds()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
i64 GetMicroseconds()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}
i64 GetNanoseconds()
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

i64 StartTime = GetMicroseconds();


RGB HSLA::operator()()
{
	double c = (1.0-abs(2*Lightness-1))*Saturation;
	double h = Hue*6.0;
	double x = c*(1-abs(fmod(h,2)-1));
	double r=0, g=0, b=0;
	     
	if		(0.0 <= h && h <= 1.0) { r=c; g=x; b=0; }
	else if	(1.0 <  h && h <= 2.0) { r=x; g=c; b=0; }
	else if	(2.0 <  h && h <= 3.0) { r=0; g=c; b=x; }
	else if	(3.0 <  h && h <= 4.0) { r=0; g=x; b=c; }
	else if	(4.0 <  h && h <= 5.0) { r=x; g=0; b=c; }
	else if	(5.0 <  h && h <= 6.0) { r=c; g=0; b=x; }
	double m = Lightness-c/2.0;
	return {(u8)(255.0*(r+m)*Amplitude), (u8)(255.0*(g+m)*Amplitude), (u8)(255.0*(b+m)*Amplitude)};
}


/*double RedVal(double val)
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
}*/