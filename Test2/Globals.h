
#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <iostream>
#include <string.h>
#include <math.h>
#include <complex>

#include <unistd.h>
#include <vector>
#include <chrono>
#include <cstring>

#define MIN(a,b) ((a)<=(b)?(a):(b))
#define MAX(a,b) ((a)>=(b)?(a):(b))

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;


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
	//Array(const Array<T, N> &other);
	int Size();
	T &operator[](int i);
	//void operator=(Array);
};

/*template<class T, int N>
Array<T,N>::Array(const Array<T, N> &other)
{
	for (int i = 0; i < N; i++) { Values[i] = other[i]; }
}*/

template<class T, int N>
int Array<T,N>::Size() { return N; }

template<class T, int N>
T &Array<T,N>::operator[](int i) { return Values[i]; }

#endif

