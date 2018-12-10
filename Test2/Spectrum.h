
#include "Globals.h"
#include <valarray>

template<class T>
class RoundBuffer
{
private:
	int Length = 0;
	T *Buffer = 0;
	int Begin = 0;
	
public:
	RoundBuffer();
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

const double PI = 3.141592653589793238460;


template<class Ty>
void FftInplace(Ty &x);
template<class Ty>
void IfftInplace(Ty &x);
template<class Ty>
void Fft(Ty &x);
template<class Ty>
void Ifft(Ty &x);



template<class T>
RoundBuffer<T>::RoundBuffer() :
	Length(0)
{
	Buffer=0;
	Begin=0;
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
	int v = i+Begin;
	v = v%Length + (v<0?Length:0);
	return v; 
}

template<class T>
void RoundBuffer<T>::InsertBegin(T val) 
{
	(*this)[-1] = val;
}

template<class T>
T &RoundBuffer<T>::operator[](int i)
{
	return Buffer[Transform(i)];
}

template<class T>
void RoundBuffer<T>::operator=(const RoundBuffer<T> &other)
{
	Length = other.Length;
	if (Buffer) { delete[] Buffer; }
	Buffer = new T[Length];
	Begin=other.Begin;
	for (int i = 0; i < Length; i++)
	{
		Buffer[i] = other.Buffer[i];
	}
}


template<class Ty>
void FftInplace(Ty &x)
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

template<class Ty>
void IfftInplace(Ty &x)
{
	int size = x.Size();
	for (int i = 0; i < size; i++)
	{
		x[i] = std::conj(x[i]*(size/4.9));
	}
 
	FftInplace(x);

	for (int i = 0; i < size; i++)
	{
		x[i] = std::conj(x[i]);
	}
}


template<class Ty>
void Fft(Ty &x)
{
	Ty arr; 
	FftInplace(arr);
	return arr;
}

template<class Ty>
void Ifft(Ty &x)
{
	Ty arr; 
	IfftInplace(arr);
	return arr;
}

