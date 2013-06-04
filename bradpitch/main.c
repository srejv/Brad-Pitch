#include <std.h>
#include <swi.h>
#include "DSK6416_AIC23.h"
Uint32 fs = DSK6416_AIC23_FREQ_48KHZ;
#define DSK6416_AIC23_INPUT_MIC 0x0015
#define DSK6416_AIC23_INPUT_LINE 0x0011
Uint16 inputsource = DSK6416_AIC23_INPUT_LINE;

// HPI header
#include "qmath.h"
#include "wt.h"
#include "definebrad.h"

// Interrupts
extern far SWI_Obj SWI_process_isr;

// Tripple buffer
#pragma DATA_ALIGN (A, 8)
fixedp A[PROCESS_SIZE];
#pragma DATA_ALIGN (B, 8)
fixedp B[PROCESS_SIZE];
#pragma DATA_ALIGN (C, 8)
fixedp C[PROCESS_SIZE];
// Tripple buffer pointers for buffer switching
fixedp *input;
fixedp *output;
fixedp *process;
fixedp *processOut;
fixedp *tmp;

Uint32 bufferindex;
char bufferflag;

#include "brad_delay.h"

// Delay
fixedp delay[48000];
unsigned int delayp;
DelayParams fxDelay;
#include "biquad.h"
BiQuad delayFilter;

#include "brad_pitch_instrumental.h"
PShift fxPitch;

#include "brad_moddelay.h"
ModDelayParams fxChorus, fxVibrato, fxFlanger;

#include "brad_3bandeq.h"
EQSTATE fxEQ;

#include "brad_dist.h"
Distortion fxDist;

#include "brad_tremolo.h"
TremoloParam fxTremolo;

#include "brad_reverb.h"
ReverbParams fxReverb;

#include "brad_input.h"
#include "brad_buffermap.h"


// Initiera variablar och effekter
void setup_effects() {
	// setup buffers
	memset(A, 0, PROCESS_SIZE*sizeof(fixedp));
	memset(B, 0, PROCESS_SIZE*sizeof(fixedp));
	memset(C, 0, PROCESS_SIZE*sizeof(fixedp));
	bufferindex = 0;
	bufferflag = 0;
	
	input = A;
	process = B;
	output = C;
	
	// Setup Tremolo
	setupTremoloParams(&fxTremolo);
	
	
	// Setup Delay
	delayp = 0;
	//delay = ((fixedp*)DELAY_BUFFER_START);
	setupDelayParams(&fxDelay, &delay[0], DELAY_BUFFER_SIZE);
	//fxDelay.delayInSamples = 24000;
	delay_setDelayTime(&fxDelay, 20*48);

	BiQuad_FlushDelays(&delayFilter);
	BiQuad_calculateLowPass(&delayFilter, 440);
	
	
	// Setup Pitch Shift
	PShift_setupPitchParams(&fxPitch);
	//memset(delay, 0, sizeof(fixedp)*48000);	
	
	// Setup Flanger
	setFlangerSettings(&fxFlanger, float2q(2.0f), float2q(0.8f), 0);

	// Setup Vibrato
	setVibratoSettings(&fxVibrato, float2q(0.7f), float2q(0.9f));

	// Setup Chorus
	setChorusSettings(&fxChorus, float2q(0.6f), float2q(0.4f), 0, float2q(0.010));

	// Setup Equalizer
	init_3band_state(&fxEQ,880,5000,48000);
	
	fxEQ.hg = float2q(1.5f);
	fxEQ.mg = Q1;
	fxEQ.lg = float2q(0.5f);

	// Setup Distortion
	fxDist.fdb = 0;
	fxDist.gain = float2q(15.0f);
	fxDist.lvl1 = 30617;
	fxDist.lvl2 = 7767;
	fxDist.prev = 0;
	
	reverb_setupParams(&fxReverb);
	
	*((int*)DELAY_ACTIVE) = 0;
	*((int*)DISTORTION_ACTIVE) = 0;
	*((int*)FLANGER_ACTIVE) = 0;
	*((int*)VIBRATO_ACTIVE) = 0;
	*((int*)CHORUS_ACTIVE) = 0;
	*((int*)PITCHT_ACTIVE) = 0;
	*((int*)EQ_ACTIVE) = 0;
	*((int*)TREMOLO_ACTIVE) = 0;
	*((int*)REVERB_ACTIVE) = 0;	// Reverb har problem med filtret. blir för stora värden tror jag. 
	*((int*)DELAY_FILTER_TYPE) = 0;
	
	return;
}



// HW Interrupt 11, audio input/output
void c_int11(void)
{	
	input[bufferindex] = (fixedp)input_left_sample();	
	output_left_sample(output[bufferindex]);
	output_right_sample(output[bufferindex]);
	if(++bufferindex >= PROCESS_SIZE) {
		bufferindex = 0;
		bufferflag = 1;
		
		// buffer switching: input -> process, process -> output, output -> input
		tmp = output;
		output = process;
		process = input;
		input = tmp;

		SWI_post(&SWI_process_isr);
	}
	return;
}

void c_inpinterrupt(void) {
	Int32 p;
	Uint32 fx, adress;
	static int flag = 0;
	if( flag == 0) {
		DSK6416_LED_on(3);
		flag = 1;
	} else {
		DSK6416_LED_off(3);
		flag = 0;
	}
	
	adress = *((int*)(0x01880004)); // ta adressen från HPIAW
	p = *((int*)adress);
	fx = (adress) & 0x00000F00;
	
	switch(fx) {
	case FX_DELAY:
		// Säg att det är delay
		delay_setParam( &fxDelay, &delayFilter, adress, p );
		break;
	case FX_FLANGER:
		flanger_setParam( &fxFlanger, adress, p );
		break;
	case FX_CHORUS:
		chorus_setParam( &fxChorus, adress, p );
		break;
	case FX_VIBRATO:
		vibrato_setParam( &fxVibrato, adress, p );
		break;
	case FX_PITCHT:
		pitcht_setParam( &fxPitch, adress, p );
		break;
	case FX_DIST:
		dist_setParam( &fxDist, adress, p );
		break;
	case FX_EQ:
		eq_setParam( &fxEQ, adress, p );
		break;
	case FX_TREMOLO:
		tremolo_setParam( &fxTremolo, adress, p );
		break;
	case FX_REVERB:
		reverb_setParam( &fxReverb, adress, p );
		break;

	}
	return;
}

void process_eq() {
	Uint32 n;
	for(n = 0; n < PROCESS_SIZE; n++) {
		process[n] = do_3band(&fxEQ, process[n]);
	}

	return;
}

// Processval för ljudsignalen, skiftar vår 3-buffer. 
void process_isr(void) 
{	
	// Do processing
	if(*((int*)PITCHT_ACTIVE)) {
		process_pitchshift(&fxPitch, process);
	}

	if(*((int*)VIBRATO_ACTIVE)) {
		process_ModDelay(&fxVibrato, process);
	}	

	if(*((int*)FLANGER_ACTIVE)) {
		process_ModDelay(&fxFlanger, process);
	}

	if(*((int*)CHORUS_ACTIVE)) {
		process_ModDelay(&fxChorus, process);
	}
	
	if(*((int*)TREMOLO_ACTIVE)) {
		process_tremolo(&fxTremolo, process);
	}
	
	if(*((int*)DISTORTION_ACTIVE)) {
		process_dist(&fxDist, process);
	}
	
	if(*((int*)DELAY_ACTIVE)) {
		if(*((int*)DELAY_FILTER_TYPE)) {
			process_filteredDelay(&fxDelay, &delayFilter, process, PROCESS_SIZE);
		} else {
			process_delay(&fxDelay, process, PROCESS_SIZE);
		}
	}

	if(*((int*)EQ_ACTIVE)) {
		process_eq();
	}

	if(*((int*)REVERB_ACTIVE)) {
		process_reverb(&fxReverb, process);
	}

	return;
}

void main()
{
	int super;
	super = sizeof(fixedp);
	
	setup_effects();
	DSK6416_LED_init();
	
	// Initialize interrupts
	comm_intr();
	DSK6416_LED_on(0);
}
