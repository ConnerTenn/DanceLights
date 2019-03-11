
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <iostream>
#include <string.h>
#include <math.h>
#include <complex>
#include <initializer_list>

#include <unistd.h>
#include <vector>
//#include <list>
#include <chrono>
//#include <cstring>
//#include <thread>

//#include <PixelTerm.h>

#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))
#define ABS(a) ((a)<0?-(a):(a))
#define PI M_PIl
#define TAU (2.0*PI)
#define MOD(a,b) ((a)%(b)+((a)<0?(b):0))

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;


i64 GetMilliseconds();
i64 GetMicroseconds();
i64 GetNanoseconds();

extern i64 StartTime;
extern i64 Time1, Time2;

inline double mmod(double a, double m)
{
	return fmod(a, m) + (a<0?m:0);
}

struct RGB
{
	u_int8_t R;
	u_int8_t G;
	u_int8_t B;
	u_int64_t XColour();
};

struct ColourVal
{
	double Colour;
	double Scale;
};

struct HSLA
{
	double Hue;
	double Saturation;
	double Lightness;
	double Amplitude;
	RGB operator()();
};

#define RED (0.0)
#define YELLOW (1.0/6.0)
#define GREEN (2.0/6.0)
#define CYAN (3.0/6.0)
#define BLUE (4.0/6.0)
#define MAGENTA (5.0/6.0)

//double OppCos(double x);
inline double RedVal(double val)
{
	return val = 2.0-abs(6.0*(0.5-abs(val-0.5))), val=MIN(val,1.0), MAX(val,0.0);
}
inline double GreenVal(double val)
{
	return val = 2.0-abs(6.0*val-2.0), val=MIN(val,1.0), MAX(val,0.0);
}
inline double BlueVal(double val)
{
	return val = 2.0-abs(6.0*val-4.0), val=MIN(val,1.0), MAX(val,0.0);
}
inline RGB RGBVal(ColourVal val)
{		
	return RGB{(u8)(255.0*RedVal(val.Colour)*val.Scale),(u8)(255.0*GreenVal(val.Colour)*val.Scale),(u8)(255.0*BlueVal(val.Colour)*val.Scale)};
}
inline RGB RGBScale(RGB rgb, double scale)
{
	return RGB{ (u8)(scale*rgb.R), (u8)(scale*rgb.G), (u8)(scale*rgb.B) };
}
inline RGB ColourMix(RGB a, RGB b, double w)
{
	double iw = 1.0-w;
	return {(u8)(a.R*iw + b.R*w), (u8)(a.G*iw + b.G*w), (u8)(a.B*iw + b.B*w)};
}
inline RGB ColourMixInt(RGB a, RGB b, i32 w)
{
	double iw = 1000-w;
	return {(u8)((a.R*iw + b.R*w)/1000), (u8)((a.G*iw + b.G*w)/1000), (u8)((a.B*iw + b.B*w)/1000)};
}
inline double Bistable(double x, double a)
{
	return x<0 ? 0 : (x<a && a!=0 ? x/a : 1);
}

inline RGB RGBVal2(HSLA val)
{
	double c = (1.0-abs(2*val.Lightness-1))*val.Saturation;
	double h = val.Hue*6.0;
	double x = c*(1-abs(fmod(h,2)-1));
	double a = val.Amplitude*255.0;
	double r, g, b;
	double m = val.Lightness-c/2.0;
	     
	if		(h <= 1.0) { r=c; g=x; b=0; }
	else if	(h <= 2.0) { r=x; g=c; b=0; }
	else if	(h <= 3.0) { r=0; g=c; b=x; }
	else if	(h <= 4.0) { r=0; g=x; b=c; }
	else if	(h <= 5.0) { r=x; g=0; b=c; }
	else if	(h <= 6.0) { r=c; g=0; b=x; }
	
	return {(u8)((r+m)*a), (u8)((g+m)*a), (u8)((b+m)*a)};
}

template<class T, int N>
struct Array
{
	T Values[N];
	
	Array();
	Array(std::initializer_list<T> list);
	Array(Array<T, N> &other);
	inline int size();
	T &operator[](int i);
	void Copy(Array<T, N> &other);
};

template<class T>
class DynamicArray
{
public:
	T *Values;
	int Length;
	
public:
	DynamicArray(int len);
	DynamicArray(std::initializer_list<T> list);
	DynamicArray(const DynamicArray<T> &other);
	~DynamicArray();
	void Resize(int len);
	inline int size();
	T &operator[](int i);
	void operator=(const DynamicArray<T> &other);
	void Copy(DynamicArray<T> &other);
};

template<class T>
class RoundBuffer
{
private:
	int Length = 0;
	T *Buffer = 0;
	int Begin = 0;
	
public:
	//RoundBuffer();
	RoundBuffer(int len);
	RoundBuffer(const RoundBuffer<T> &other);
	~RoundBuffer();
	
private:
	int Transform(int i);
	
public:
	inline int size();
	void InsertBegin(T val);
	
	T &operator[](int i);
	void operator=(const RoundBuffer<T> &other);
};






template<class T, int N>
Array<T,N>::Array() { }

template<class T, int N>
Array<T,N>::Array(std::initializer_list<T> list)
{
	for (int i = 0; i < (int)list.size(); i++) { Values[i] = list.begin()[i]; }
}

template<class T, int N>
Array<T,N>::Array(Array<T, N> &other)
{
	for (int i = 0; i < N; i++) { Values[i] = other[i]; }
}

template<class T, int N>
int Array<T,N>::size() { return N; }

template<class T, int N>
T &Array<T,N>::operator[](int i) { return Values[i]; }

template<class T, int N>
void Array<T,N>::Copy(Array<T, N> &other)
{
	for (int i = 0; i < N; i++) { Values[i] = other[i]; }
}




template<class T>
DynamicArray<T>::DynamicArray(int len) :
	Length(len)
{
	//std::cout << "DynamicArray:New Len" << len << "\n";
	Values = new T[len];
}

template<class T>
DynamicArray<T>::DynamicArray(std::initializer_list<T> list)
{
	DynamicArray(list.size());
	for (int i = 0; i < (int)list.size(); i++) { Values[i] = list.begin()[i]; }
}

template<class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T> &other)
{
	//std::cout << "DynamicArray:Copy\n";
	DynamicArray(other.size());
	for (int i = 0; i < Length; i++) { Values[i] = other[i]; }
}
template<class T>
DynamicArray<T>::~DynamicArray()
{
	//std::cout<<"DynamicArray:Delete "<<Length<<"\n";
	delete[] Values;
}
template<class T>
void DynamicArray<T>::Resize(int len)
{
	delete[] Values;
	Values = new T[len];
}

template<class T>
int DynamicArray<T>::size() { return Length; }

template<class T>
T &DynamicArray<T>::operator[](int i) { return Values[i]; }

template<class T>
void DynamicArray<T>::operator=(const DynamicArray<T> &other)
{
	Length = other.Length;
	Resize(Length);
	for (int i = 0; i < Length; i++) { Values[i] = other.Values[i]; }
}

template<class T>
void DynamicArray<T>::Copy(DynamicArray<T> &other)
{
	DynamicArray(other.size());
	for (int i = 0; i < Length; i++) { Values[i] = other[i]; }
}



template<class T>
RoundBuffer<T>::RoundBuffer(int len) :
	Length(len)
{
	Buffer = new T[Length];
	Begin=0;
}

template<class T>
RoundBuffer<T>::RoundBuffer(const RoundBuffer<T> &other)
{
	(*this) = other;
}

template<class T>
RoundBuffer<T>::~RoundBuffer()
{
	delete[] Buffer;
}

template<class T>
int RoundBuffer<T>::size() { return Length; }

template<class T>
int RoundBuffer<T>::Transform(int i) 
{
	if (Length==0) { throw std::domain_error("div/0"); }
	int v = i+Begin;
	v = v%Length + (v<0?Length:0);
	return v;  
}

template<class T>
void RoundBuffer<T>::InsertBegin(T val) 
{
	Begin = Transform(-1);
	(*this)[0] = val;
}

template<class T>
T &RoundBuffer<T>::operator[](int i)
{
	//std::cout << "operator["<<i<<"]\n";
	return Buffer[Transform(i)];
}

template<class T>
void RoundBuffer<T>::operator=(const RoundBuffer<T> &other)
{
	//std::cout << "operator=\n";
	Length = other.Length;
	if (Buffer) { delete[] Buffer; }
	Buffer = new T[Length];
	Begin=other.Begin;
	for (int i = 0; i < Length; i++)
	{
		Buffer[i] = other.Buffer[i];
	}
}

#endif

