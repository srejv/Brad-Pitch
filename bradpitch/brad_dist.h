
#pragma once

#include "qmath.h"
#include "tistdtypes.h"


typedef struct Distortion_t {
	fixedp gain;	// Gain 
	fixedp fdb;		// Feedback
	fixedp lvl1;	// Övre klippningsnivå
	fixedp lvl2;	// Nedre klippningsnivå 
					//   ^-- denna är positiv, men 
					//		 görs om till negativ i algoritmen
	fixedp prev;	// Sampelvärdet ett sampel tillbaka i tiden 
} Distortion;

void process_dist(Distortion *t, fixedp *x);

void process_fuzz(Distortion *t, fixedp *x);
void process_thunderFuzz(Distortion *t, fixedp *x);
void process_overdrive(Distortion *t, fixedp *x);
void process_squareDist(Distortion *t, fixedp *x);

void dist_setParam(Distortion *t, Uint32 param, int val);
