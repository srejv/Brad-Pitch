
#pragma once 

#include "qmath.h"
#include "tistdtypes.h"

#define PSHIFT_BUFSIZE 2160

#define PSHIFT_TAPS 2 

extern Int32 env[];
extern fixedp pitch_delay[];
extern Int32 pitch_table_cent[];

typedef struct PShift_t {
	fixedp gain;

	fixedp qdsize;	// dsize i Qx.15
	int dsize;		// delay size in samples

	int pitch; 		// pitch +-24
	int cent;		// cent +-100

	fixedp fdb;		// feedback +-100
	
	fixedp rp;
	Uint32 wp;
	
	Uint32 wet;
	Uint32 dry;
} PShift;

// Pitch Shift - Tidsplanet.
void process_pitchshift(PShift *this, fixedp *process);

void PShift_setupPitchParams(PShift *this);

void PShift_setPitchStep(PShift *this, short step);

void pitcht_setParam(PShift *t, Uint32 param, int val);
