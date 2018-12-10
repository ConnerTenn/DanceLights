
#include "Spectrum.h"



// Cooley-Tukey FFT (in-place, breadth-first, decimation-in-frequency)
// Better optimized but less intuitive
// !!! Warning : in some cases this code make result different from not optimased version above (need to fix bug)
// The bug is now fixed @2017/05/30 
void fft(CArray &x)
{
	// DFT
	unsigned int N = x.size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
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
				Complex t = x[a] - x[b];
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
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//Complex f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
	
	Complex f = 2.0/N; 
	for (unsigned int i = 0; i < N; i++) { x[i] *= f; }
}
 
// inverse fft (in-place)
void ifft(CArray& x)
{
	x*=x.size()/4.0;
	
    // conjugate the complex numbers
    x = x.apply(std::conj);
 
    // forward fft
    fft( x );
 
    // conjugate the complex numbers again
    x = x.apply(std::conj);
 
    // scale the numbers
    //x /= x.size();
}

template<class Ty>
void fft(Ty &x)
{
	// DFT
	u32 N = x.Size(), k = N, n;
	double thetaT = 3.14159265358979323846264338328L / N;
	Complex phiT = Complex(cos(thetaT), -sin(thetaT)), T;
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
				Complex t = x[a] - x[b];
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
			Complex t = x[a];
			x[a] = x[b];
			x[b] = t;
		}
	}
	//// Normalize (This section make it not working correctly)
	//Complex f = 1.0 / sqrt(N);
	//for (unsigned int i = 0; i < N; i++)
	//	x[i] *= f;
	
	Complex f = 2.0/N; 
	for (unsigned int i = 0; i < N; i++) { x[i] *= f; }
}


void fft2(RoundBuffer<Complex> &x)
{
	fft(x);
}

void ifft2(RoundBuffer<Complex> &x)
{
	int size = x.Size();
	for (int i = 0; i < size; i++)
	{
		x[i] = std::conj(x[i]*(size/4.9));
	}
 
	fft2(x);

	for (int i = 0; i < size; i++)
	{
		x[i] = std::conj(x[i]);
	}
}

void fft3(Array<Complex, 16> &x)
{
	fft(x);
}
void fft3(Array<Complex, 32> &x)
{
	fft(x);
}
void fft3(Array<Complex, 64> &x)
{
	fft(x);
}

RoundBuffer<Complex> beat(RoundBuffer<RoundBuffer<Complex>> &x)
{
	int len = x.Size();
	RoundBuffer<Complex> buff(len);
	
	for (int i = 0; i < len; i++)
	{
		fft2(x[i]);
		
		Complex max;
		for (int j = 0; j < x[i].Size(); j++)
		{
			if (Mag(x[i][j]) > Mag(max)) { max = x[i][j]; }
		}
		
		buff[i] = max;
	}
	
	return buff;
}
