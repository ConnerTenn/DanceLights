
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
	void SetInternal(T val, int i);
	
public:
	int Size();
	int Transform(int i);
	void Set(T val, int i);
	void Append(T val);
	
	//T operator[](int i);
	T &operator[](int i);
	void operator=(const RoundBuffer<T> &other);
};

const double PI = 3.141592653589793238460;


void fft(CArray &x);
void ifft(CArray &x);
void fft2(RoundBuffer<Complex> &x);
void ifft2(RoundBuffer<Complex> &x);
//template<int N=32>
void fft3(Array<Complex, 16> &x);
void fft3(Array<Complex, 32> &x);
void fft3(Array<Complex, 64> &x);
RoundBuffer<Complex> beat(RoundBuffer<RoundBuffer<Complex>> &x);



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
void RoundBuffer<T>::SetInternal(T val, int i) 
{
	Buffer[i%Length] = val;
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
void RoundBuffer<T>::Set(T val, int i) 
{
	SetInternal(val, Transform(i));
}

template<class T>
void RoundBuffer<T>::Append(T val) 
{
	Begin = Transform(-1);
	SetInternal(val, Begin);
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

