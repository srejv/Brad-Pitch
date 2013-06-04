/*
	BiQuad struct
*/

#ifndef BIQUAD_H_
#define BIQUAD_H_

// Typ
#define FLT_EPSILON_PLUS	 1		// smallest such that 1.0+FLT_EPSILON != 1.0 
#define FLT_EPSILON_MINUS	-1		// smallest such that 1.0-FLT_EPSILON != 1.0
#define FLT_MIN_PLUS		 1		// min positive value
#define FLT_MIN_MINUS		-1		// min negative values

#include "qmath.h"
#include "tistdtypes.h"

typedef struct BiQuad_t
{
	// Coefficients
	fixedp a0, a1, a2, b1, b2;
	fixedp c0, d0; // Not used atm (c0 = dry, d0 = wet)

	// Delays
	fixedp xz1, xz2;
	fixedp yz1, yz2;
} BiQuad;


BiQuad* BiQuad_create();
void BiQuad_FlushDelays(BiQuad* filter);
fixedp BiQuad_do(BiQuad* filter, fixedp xn);

void BiQuad_calculateLowPass(BiQuad *t, Uint16 fc);
void BiQuad_calculateHighPass(BiQuad *t, Uint16 fc);
void BiQuad_calculateBPF(BiQuad *t, Uint16 fc, Int32 Q);
void BiQuad_calculateBSF(BiQuad *t, Uint16 fc, Int32 Q);

#endif 
