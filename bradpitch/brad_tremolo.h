/*
	Tremolo
	
	Amplitude variation.  
	Created by Oscar Dragén 27-05-2013 
*/

#pragma once

#include "qmath.h"
#include "wt.h"


typedef struct TremoloParam_t {
	fixedp depth;
	fixedp oscVal;
	fixedp wet;
	fixedp dry;
	fixedp fdb;
	
	fixedp g1,g2;

	WaveTable LFO;
} TremoloParam;

void setupTremoloParams(TremoloParam *this);

void tremolo_setParam(TremoloParam *this, Uint32 adr, int val);

void process_tremolo(TremoloParam *this, fixedp *buffer);
