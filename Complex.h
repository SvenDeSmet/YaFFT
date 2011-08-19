#ifndef CCOMPLEX_H_
#define CCOMPLEX_H_

#include <stdlib.h>
#include "Math.h"

#define GlobalAlignLevel 5
template <class D> class AlignedArray {
private:
	unsigned char* mem;
	
public:
	D* alignedData;
	
	AlignedArray(int size, int alignLevel = GlobalAlignLevel) {
	    mem = new unsigned char[size * sizeof(D) + ((1 << alignLevel) - 1)];
		alignedData = (D*) ((((long int) mem + ((1 << alignLevel) - 1)) >> alignLevel) << alignLevel);
	}
	
	D& operator [] (int index) { return alignedData[index]; }
	const D operator [] (int index) const { return alignedData[index]; }
	
	~AlignedArray() { delete mem; }
};

#define ScaleFact 16
template <class R>
class Complex {
	template <class S> friend Complex<S> operator + (const Complex<S> a, const Complex<S> b);
	template <class S> friend Complex<S> operator - (const Complex<S> a, const Complex<S> b);
	template <class S> friend Complex<S> operator * (const Complex<S> a, const Complex<S> b);
	template <class S> friend Complex<S> operator >> (const Complex<S> a, int shift);
	template <class S> friend Complex<S> operator * (const S a, const Complex<S> b);
	template <class S> friend Complex<S> operator / (const Complex<S> b, const S a);
	template <class S> friend bool operator == (const Complex<S> a, const Complex<S> b);
private:
	R r, i;
public:
	Complex<R>(R iR, R iI) : r(iR), i(iI) { };
	Complex<R>() { };
	Complex<R>(R iR) : r(iR), i((R) 0.0) { };
	static Complex<R> unit(double iPhase) { return Complex<R>(cos(iPhase), sin(iPhase)); };
	void print() const { printf("(%f, %f)\n", r, i); };
	R getNorm() const { return sqrt(getNormSquared()); };
	R getNormSquared() const { return ((*this) * ((*this).getConjugate())).getReal(); };
	Complex<R> getNormalizedComplex() const { return (*this)/((R) sqrt(getNormSquared())); };
	Complex<R> getConjugate() const { return Complex<R>(r, -i); };
	
	R getReal() const { return r; }
	double getAngle() const { 
		Complex<R> n = getNormalizedComplex();
		return atan2(n.i, n.r); 
	};
};

template <class R> Complex<R> inline operator + (const Complex<R> a, const Complex<R> b) { return Complex<R>(a.r + b.r, a.i + b.i); }
template <class R> Complex<R> inline operator - (const Complex<R> a, const Complex<R> b) { return Complex<R>(a.r - b.r, a.i - b.i); }
template <class R> bool operator == (const Complex<R> a, const Complex<R> b) { return (a.r == b.r) && (a.i == b.i); }
template <class R> Complex<R> inline operator >> (const Complex<R> a, int shift) { return ((R) pow(0.5, shift)) * a; };

template <class R, class U, int L> R mul(R a, R b) { return (((U) a) * ((U) b)) >> L; };

template <class R> Complex<R> inline operator * (const R c, const Complex<R> b) { return Complex<R>(c * b.r, c * b.i); }
template <class R> Complex<R> inline operator * (const Complex<R> a, const Complex<R> b) {
	return Complex<R>(a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r);
}

short int inline imul16(const short int a, const short int b) {  return mul<short int, int, 16>(a, b); }

template <class R> R isub(R a, R b) { return ((a >> 1) - (b >> 1)); };
template <class R> R iadd(R a, R b) { return ((a >> 1) + (b >> 1)); };

short int inline isub16(const short int a, const short int b) { return isub<short int>(a, b); }
short int inline iadd16(const short int a, const short int b) { return iadd<short int>(a, b); }
short int inline isub32(const short int a, const short int b) { return isub<short int>(a, b); }
short int inline iadd32(const int a, const short int b) { return iadd<short int>(a, b); }

int inline imul16_32(const short int a, const short int b) { return mul<short int, int, 16>(a, b); }

template <> Complex<short int> inline operator * (const short int c, const Complex<short int> b) { 
	return Complex<short int>(imul16(c, b.r), imul16(c, b.i)); 
}

template <> Complex<short int> inline operator * (const Complex<short int> a, const Complex<short int> b) {
	return Complex<short int>((((int) a.r * (int) b.r - (int) a.i * (int) b.i)) >> 14, 
		                      (((int) a.r * (int) b.i + (int) a.i * (int) b.r)) >> 14);
}


template <class R> Complex<R> inline operator / (const Complex<R> b, const R c) { return (1/c) * b; }


#endif