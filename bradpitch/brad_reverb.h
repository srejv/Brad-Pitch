

#include "qmath.h"
#include <tistdtypes.h>

typedef struct ReverbParams_t {
	Int32 feedback;			// Feedback i %
	fixedp cutOffFrequency;	//
	Uint32 revTime_samples;	// Revtime
	
	Int32 wp;			// Inskrivningspekare

	fixedp alfa;		// För lågpass
	fixedp prev;		// Det tidigare uträknade värdet
						// (så att det behålls mellan blocken)

	fixedp wet;	
	fixedp dry; 
} ReverbParams;


void reverb_calculateFilter(ReverbParams *t);

void reverb_setupParams(ReverbParams *t);

void process_reverb(ReverbParams *t, fixedp *process);

void reverb_setParam(ReverbParams *t, Uint32 param, int val);

