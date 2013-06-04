

#pragma once

#include "qmath.h"
#include "tistdtypes.h"
#include "wt.h"
#include "brad_delay.h"

// functions to update the member objects
void ModDelay_updateLFO();
void ModDelay_updateDDL();

// cooking function for mod type
void ModDelay_cookModType();

// convert a LFO value to a delay offset value
fixedp ModDelay_calculateDelayOffset(fixedp fLFOSample);

enum {Flanger,Vibrato,Chorus}; 	// Effect type
enum {Tri, Sine};		  		// LFO Type
enum {normal,quad,invert}; 		// LFO Phase

// these will depend on the type of mod
typedef struct ModDelayParams_t {
	fixedp fMinDelay_mSec;		// Minsta delaytiden i ms
	fixedp fMaxDelay_mSec;		// Högsta delaytiden i ms
	
	fixedp fModDepth_pct;		// Moduleringsdjup i %
	fixedp fModFrequency_Hz;	// Moduleringsfrekvens i Hz

	fixedp fFeedback_pct;		// Feedback i %
	fixedp fChorusOffset;		// Offset för chorus i ms

	WaveTable LFO;				// Lågfrekevsoscillator

	Uint32 uLFOPhase;			// Vilken fas på oscillator 
								// (vanlig, förskjuten pi/4 eller invert)
	Uint32 uModType;			// Typ av våg (Triangel eller sinus)
	DelayParams delay;			// Delayparametrar (även buffer)
} ModDelayParams;


void setFlangerSettings(ModDelayParams *this, fixedp depth, fixedp modhz, fixedp feedback);

void setVibratoSettings(ModDelayParams *this, fixedp depth, fixedp modhz);

void setChorusSettings(ModDelayParams *this, fixedp depth, fixedp modhz, fixedp feedback, fixedp chorusOffset);

void process_ModDelay(ModDelayParams *this, fixedp *process);

