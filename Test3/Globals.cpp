
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

RGB ColourVal(double val)
{
	return RGB {
		(u8)(255.0*((0.0 <= val && val < 1.0/3.0 ? (1.0-3.0*val) : 0)		+ (2.0/3.0 < val && val <= 1.0 ? (3.0*val-2.0) : 0))),
		(u8)(255.0*((0.0 <= val && val < 1.0/3.0 ? (3.0*val) : 0)			+ (1.0/3.0 < val && val <= 2.0/3.0 ? (2.0-3.0*val) : 0))),
		(u8)(255.0*((1.0/3.0 <= val && val < 2.0/3.0 ? (3.0*val-1.0) : 0)	+ (2.0/3.0 < val && val <= 1.0 ? (3.0-3.0*val) : 0)))};
}
