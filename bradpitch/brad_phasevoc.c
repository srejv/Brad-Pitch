
#include "qmath.h"
#include <tistdtypes.h>
#include "definebrad.h"
#include <dsp_fft.h>
#include <dsp_fft32x32.h>
#include "twiddle.coef"

#include "brad_phasevoc.h"

void process_Segment(PhaseVocoder *t, COMPLEX *frame);


void phaseVocoder_setup(PhaseVocoder *t, int toneShift) {
	
	fixedp tmp;

	tmp = qpow(short2q(2), qdiv(short2q(toneShift),short2q(12)));

	//% Storleken p� mellanrummet mellan varje segment hos utsignalen.
	t->hopSizeOut = _qmul(tmp, HOP_SIZE, FIXED_FRACBITS, 0, 0);
	//% Korrigerar f�r avrundning i hopSizeOut. what?
	t->pShift = qdiv(int2q(t->hopSizeOut), int2q(HOP_SIZE));
	t->hopOutIndex = 0;
	t->rp = 0;
	t->wp = 0;

	return;
}


static fixedp INPUT_BUFFER_N[1024];
static fixedp OUTPUT_BUFFER_N[BUFFER_SIZE];


Uint32 wp;
Uint32 rp;

Uint32 segmentIndex;

//.align 1024
COMPLEX frank[FRAME_SIZE];

Int32 pPhi[FRAME_SIZE];
Int32 phiCulum[FRAME_SIZE];

//.align 1024
COMPLEX fftFrame[FRAME_SIZE];
//.align 1024
COMPLEX preOutputFrame[FRAME_SIZE];
//.align 1024
COMPLEX outputFrame[FRAME_SIZE];

#pragma DATA_ALIGN (frank, 8); 
#pragma DATA_ALIGN (fftFrame, 8); 
#pragma DATA_ALIGN (preOutputFrame, 8);
#pragma DATA_ALIGN (outputFrame, 8); 

void phaseVocoder_initialize() {
	memset(&pPhi, 0, sizeof(Uint32)*FRAME_SIZE);
	memset(&phiCulum, 0, sizeof(Uint32)*FRAME_SIZE);	
	memset(&fftFrame, 0, sizeof(COMPLEX)*FRAME_SIZE);	
	memset(&preOutputFrame, 0, sizeof(COMPLEX)*FRAME_SIZE);	
	memset(&outputFrame, 0, sizeof(COMPLEX)*FRAME_SIZE);	
	memset(&frank, 0, sizeof(COMPLEX)*FRAME_SIZE);

	memset(&INPUT_BUFFER_N, 0, sizeof(fixedp)*1024);
	memset(&OUTPUT_BUFFER_N, 0, sizeof(fixedp)*BUFFER_SIZE);
	return;
}

void phaseVocoder_process(PhaseVocoder *t, fixedp *x) {
	// L�s fr�n buffern
	Int32 n, q;
	Int32 rpi, start;
	
	start = segmentIndex * HOP_SIZE;
	for(n=start; n < HOP_SIZE; n++) {
		INPUT_BUFFER_N[start++] = x[n];
		if(start >= 1024) start = 0;
	}
	
	if(++segmentIndex >= 4) segmentIndex = 0;
	
	start = segmentIndex * HOP_SIZE;
	for(n=0; n < FRAME_SIZE; n++) {
		frank[n].r = qmul(INPUT_BUFFER_N[start], HANN[n]);
		frank[n].i = 0;
		start += 1;
		if(start >= 1024) start = 0;
	}
	
	// input buffern �r full, 
	process_Segment(t, frank);

	start = segmentIndex * HOP_SIZE;
	for(n = 0; n < 256; n++) {
		x[n] = frank[start+n].r;
		if(start >= 1024) start = 0;
	}

	/*
	for(n = 0; n < FRAME_SIZE; n++) {
		q = t->hopOutIndex+n;
		if( q > BUFFER_SIZE) {
			q -= BUFFER_SIZE;
		}

		if ( n < FRAME_SIZE-(t->hopSizeOut)) {
			OUTPUT_BUFFER_N[q] = qadd(OUTPUT_BUFFER_N[q], qmul(outputFrame[n].r, HANN[n]));
		}
		else {
			OUTPUT_BUFFER_N[q] = qmul(outputFrame[n].r, HANN[n]);  //, 31, FIXED_FRACBITS, FIXED_FRACBITS);
		}
	}

	t->hopOutIndex = t->hopOutIndex + t->hopSizeOut;
	if (t->hopOutIndex >= BUFFER_SIZE) t->hopOutIndex -= BUFFER_SIZE;
	
	rpi = qipart(t->rp);
	for(n=0;n<256;n++) {
		
		x[n] = OUTPUT_BUFFER_N[rpi];
		
		t->rp += t->pShift;
		rpi = qipart(t->rp);
		if(rpi >= 1024) { 
			t->rp -= int2q(1024);
		}
	}*/

	return;
}




	
void process_Segment(PhaseVocoder *t, COMPLEX *frame) {
	
	fixedp R, Phi, dPhi, dpPhi, dpmPhi;
	fixedp trueFreq;
	
	Int32 n;
	memset(fftFrame, 0, sizeof(COMPLEX)*FRAME_SIZE);
	memset(outputFrame, 0, sizeof(COMPLEX)*FRAME_SIZE);

	DSP_fft32x32(w, FRAME_SIZE, &(frame[0].r), &(fftFrame[0].r));

	
	for(n=0; n < FRAME_SIZE; n++) {
		// absolutbelopp och fasvinkel
		/*R = qsqrt(qadd(QSQUARE(fftFrame[n].r), QSQUARE(fftFrame[n].i)));
		Phi = qatan2(fftFrame[n].i, fftFrame[n].r);
		
		// skillnad i nuvarande fas med f�rra
		dPhi = Phi - pPhi[n];
		pPhi[n] = Phi;
		
		// bort med fasskillnad
		dpPhi = REMOVE_PHASE_DIFFERENCE(dPhi, n); // dPhi - HOP_SIZE*Q2PI*(n/2)/FRAME_SIZE
		dpmPhi = PHASE_MOD(dpPhi);
		
		// trueFreq = 2PI * (n/FRAMESIZE) + dpmPhi/HOP_SIZE 
		trueFreq = qmul(Q2PI, qdiv(n << FIXED_FRACBITS, FRAME_SIZE << FIXED_FRACBITS)) + qdiv(dpmPhi, HOP_SIZE << FIXED_FRACBITS);
		
		// sparas detta mellan frames?
		phiCulum[n] += _qmul(t->hopSizeOut, trueFreq, 0, FIXED_FRACBITS, FIXED_FRACBITS);

		preOutputFrame[n].r = qmul(R, qcos(phiCulum[n])); //R*exp(1i*phiCulm);
		preOutputFrame[n].i = qmul(R, qsin(phiCulum[n])); */
	}

	DSP_ifft32x32(w, FRAME_SIZE, &(fftFrame[0].r), &(frame[0].r));
	for(n = 0; n < FRAME_SIZE; n++) {
		outputFrame[n].r = frame[n].r >> 10;
		outputFrame[n].i = frame[n].i;
	}

	return;
}

