
#pragma once

#include "qmath.h"
#include "tistdtypes.h"


typedef struct Distortion_t {
	fixedp gain;	// Gain 
	fixedp fdb;		// Feedback
	fixedp lvl1;	// �vre klippningsniv�
	fixedp lvl2;	// Nedre klippningsniv� 
					//   ^-- denna �r positiv, men 
					//		 g�rs om till negativ i algoritmen
	fixedp prev;	// Sampelv�rdet ett sampel tillbaka i tiden 
} Distortion;

void process_dist(Distortion *t, fixedp *x);

void process_fuzz(Distortion *t, fixedp *x);
void process_thunderFuzz(Distortion *t, fixedp *x);
void process_overdrive(Distortion *t, fixedp *x);
void process_squareDist(Distortion *t, fixedp *x);

void dist_setParam(Distortion *t, Uint32 param, int val);
