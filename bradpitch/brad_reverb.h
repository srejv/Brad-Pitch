

#include "qmath.h"
#include <tistdtypes.h>

typedef struct ReverbParams_t {
	Int32 feedback;			// Feedback i %
	fixedp cutOffFrequency;	//
	Uint32 revTime_samples;	// Revtime
	
	Int32 wp;			// Inskrivningspekare

	fixedp alfa;		// F�r l�gpass
	fixedp prev;		// Det tidigare utr�knade v�rdet
						// (s� att det beh�lls mellan blocken)

	fixedp wet;	
	fixedp dry; 
} ReverbParams;


void reverb_calculateFilter(ReverbParams *t);

void reverb_setupParams(ReverbParams *t);

void process_reverb(ReverbParams *t, fixedp *process);

void reverb_setParam(ReverbParams *t, Uint32 param, int val);

