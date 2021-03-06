
#include "Globals.h"
#include <array>


template<class Ty>
void FftInplace(Ty &x, u32 N);
template<class Ty>
void IfftInplace(Ty &x, u32 size);
template<class Ty>
void Fft(Ty &x, u32 size);
template<class Ty>
void Ifft(Ty &x, u32 size);


/*
Definitions
*/

/*
template<class T>
RoundBuffer<T>::RoundBuffer() :
	Length(0)
{
	Buffer=0;
	Begin=0;
}*/




template<class Ty>
void FftInplace(Ty &x, u32 N)
{
	// DFT
	u32 k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	ComplexD phiT = ComplexD(cos(thetaT), -sin(thetaT)), T;
	while (k > 1)
	{
		n = k;
		k >>= 1;
		phiT = phiT * phiT;
		T = 1.0L;
		for (unsigned int l = 0; l < k; l++)
		{
			for (unsigned int a = l; a < N; a += n)
			{
				unsigned int b = a + k;
				ComplexD t = x[a] - x[b];
				x[a] += x[b];
				x[b] = t * T;
			}
			T *= phiT;
		}
	}
	// Decimate
	unsigned int m = (unsigned int)log2(N);
	for (unsigned int a = 0; a < N; a++)
	{
		unsigned int b = a;
		// Reverse bits
		b = (((b & 0xaaaaaaaa) >> 1) | ((b & 0x55555555) << 1));
		b = (((b & 0xcccccccc) >> 2) | ((b & 0x33333333) << 2));
		b = (((b & 0xf0f0f0f0) >> 4) | ((b & 0x0f0f0f0f) << 4));
		b = (((b & 0xff00ff00) >> 8) | ((b & 0x00ff00ff) << 8));
		b = ((b >> 16) | (b << 16)) >> (32 - m);
		if (b > a)
		{
			ComplexD t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//ComplexD f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
	
	ComplexD f = 2.0/N; 
	for (unsigned int i = 0; i < N; i++) { x[i] *= f; }
}

template<class Ty>
void IfftInplace(Ty &x, u32 size)
{
	for (u32 i = 0; i < size; i++)
	{
		x[i] = std::conj(x[i]*(size/4.9));
	}
 
	FftInplace(x);

	for (u32 i = 0; i < size; i++)
	{
		x[i] = std::conj(x[i]);
	}
}


template<class Ty>
void Fft(Ty &x, u32 size)
{
	Ty arr;
	//for (u32 i = 0; i < size; i++) { arr[i] = x}
	arr = x;
	FftInplace(arr, size);
	return arr;
}

template<class Ty>
void Ifft(Ty &x, u32 size)
{
	Ty arr;
	arr = x;
	IfftInplace(arr, size);
	return arr;
}

