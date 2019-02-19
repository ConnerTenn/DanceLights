
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

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;


u64 GetMilliseconds();
u64 GetMicroseconds();
u64 GetNanoseconds();

extern u64 StartTime;

struct RGB
{
	u_int8_t R;
	u_int8_t G;
	u_int8_t B;
	u_int64_t XColour();
};

template<class T, int N>
struct Array
{
	T Values[N];
	
	Array();
	Array(std::initializer_list<T> list);
	Array(Array<T, N> &other);
	int Size();
	T &operator[](int i);
	void Copy(Array<T, N> &other);
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
	int Size();
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
int Array<T,N>::Size() { return N; }

template<class T, int N>
T &Array<T,N>::operator[](int i) { return Values[i]; }

template<class T, int N>
void Array<T,N>::Copy(Array<T, N> &other)
{
	for (int i = 0; i < N; i++) { Values[i] = other[i]; }
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
int RoundBuffer<T>::Size() { return Length; }

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

