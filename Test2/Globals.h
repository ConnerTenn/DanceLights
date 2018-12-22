
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <iostream>
#include <string.h>
#include <math.h>
#include <complex>
#include <initializer_list>

#include <unistd.h>
#include <vector>
#include <chrono>
#include <cstring>
#include <thread>

#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))
#define PI M_PIl
#define TAU (2.0*PI)

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef std::complex<double> Complex;


u64 GetMilliseconds();
u64 GetMicroseconds();
u64 GetNanoseconds();

double Mag(std::complex<double> val);

struct RGB
{
	u8 R = 0;
	u8 G = 0;
	u8 B = 0;

	u8 operator[](int i);
};


template<class T, int N>
struct Array
{
	T Values[N];
	
	Array();
	Array(std::initializer_list<T> list);
	Array(const Array<T, N> &other);
	int Size();
	T &operator[](int i);
	void Copy(const Array<T, N> &other);
};



template<class T, int N>
Array<T,N>::Array(std::initializer_list<T> list)
{
	for (int i = 0; i < (int)list.size(); i++) { Values[i] = list.begin()[i]; }
}

template<class T, int N>
Array<T,N>::Array(const Array<T, N> &other)
{
	for (int i = 0; i < N; i++) { Values[i] = other[i]; }
}

template<class T, int N>
int Array<T,N>::Size() { return N; }

template<class T, int N>
T &Array<T,N>::operator[](int i) { return Values[i]; }

template<class T, int N>
void Array<T,N>::Copy(const Array<T, N> &other)
{
	for (int i = 0; i < N; i++) { Values[i] = other[i]; }
}

#endif

