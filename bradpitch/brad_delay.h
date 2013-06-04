

#pragma once 

#include "qmath.h"
#include "tistdtypes.h"
#include "biquad.h"
#include "brad_envelopedetector.h"

typedef struct DelayParams_t {
	Int32 wp;		// Inskrivningspekare
	fixedp rp;  	// Avläsningspekare
	fixedp inc; 	// Avläsningsinkrement (för att kunna variera hastigheten som avläsningen sker)

	fixedp fb;				// feedback i % [-1,1]
	Uint32 delayInSamples;	// Fördröjning i antal sampel
	Uint32 delayStartPosition; // Startpositio

	fixedp wet;		// Behandlad signal %
	fixedp dry;		// Obehandlar signal %
	
	// För vissa effekter vill en kunna koppla in extern feedback, främst för 
	// olika stereoeffekter.
	unsigned char useExternalFeedback;	// Ifall en vill använda extern feedback eller ej
	fixedp externalFbSample;	// Externt feedback sampel
	fixedp currentFbSample;		// Nuvarande feedback sampel

	//fixedp driveInGain;
	//fixedp attackThreshold;
	//EnvelopeDetector detector;
	
	Uint32 bufferSize;		// Bufferstorlek
	fixedp *buffer;			// Delaybuffer.
} DelayParams;

// Initierar en parameterstruct
void setupDelayParams(DelayParams *this, fixedp *buffer, Uint32 bufferSize);
void setDelay(DelayParams *this, int delayInSamples);

void process_delay(DelayParams *this, fixedp *process, Uint32 processSize);
void process_filteredDelay(DelayParams *this, BiQuad *filter, fixedp *process, Uint32 processSize);

void delay_setDelayTime(DelayParams *this, int delayInSamples);
void delay_setMix(DelayParams *this, int mix);
void delay_setFb(DelayParams *this, int fb);
void delay_setParam(DelayParams *this, BiQuad *filter, Uint32 param, int val);

