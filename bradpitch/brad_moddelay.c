

#include "brad_moddelay.h"
#include "wt.h"
#include "brad_delay.h"
#include "brad_input.h"

// 48000 * 0.1s = 4800
//#define FLANGER_BUFFER_SIZE 1200
//#define FLANGER_BUFFER_START 0x8070000
fixedp flangerDelayBuffer[1200];
fixedp vibratoDelayBuffer[1200];
fixedp chorusDelayBuffer[9600];

void setFlangerSettings(ModDelayParams *this, fixedp depth, fixedp modhz, fixedp feedback) {
	this->fChorusOffset = 0;
	this->fFeedback_pct = float2q(0.25); // mellan -1 - 1
	this->fMaxDelay_mSec = 48*7; // 7 msec
	this->fMinDelay_mSec = 0;
	this->fModDepth_pct = depth;
	this->fModFrequency_Hz = modhz;
	this->uModType = Flanger;
	this->uLFOPhase = normal;

	// Set up Delay Line
	setupDelayParams(&this->delay, flangerDelayBuffer, 4800);
	this->delay.wet = Q0_5;
	this->delay.dry = Q0_5;
	this->delay.fb = this->fFeedback_pct;
	delay_setDelayTime(&this->delay, 4800);

	// Set up LFO
	this->LFO.table = SinTable;
	this->LFO.invert = 0;
	this->LFO.unipolar = 1;
	this->LFO.mInc = 0;
	this->LFO.readPointer = 0;
	WaveTable_cookFrequency(&this->LFO, this->fModFrequency_Hz);
	WaveTable_reset(&this->LFO);
}

void setVibratoSettings(ModDelayParams *this, fixedp depth, fixedp modhz) {
	this->fChorusOffset = 0;
	this->fFeedback_pct = 0;
	this->fMaxDelay_mSec = float2q(0.007f);
	this->fMinDelay_mSec = 0;
	this->fModDepth_pct = depth;
	this->fModFrequency_Hz = modhz;
	this->uModType = Vibrato;
	this->uLFOPhase = normal;

	// Set up Delay Line
	setupDelayParams(&this->delay, vibratoDelayBuffer, 1200);
	this->delay.wet = Q1;
	this->delay.dry = 0;
	this->delay.fb = 0;

	// Set up LFO
	this->LFO.table = TriangleTable;
	this->LFO.invert = 0;
	this->LFO.unipolar = 1;
	this->LFO.mInc = 0;
	this->LFO.readPointer = 0;
	WaveTable_cookFrequency(&this->LFO, this->fModFrequency_Hz);
	WaveTable_reset(&this->LFO);
}

void setChorusSettings(ModDelayParams *this, fixedp depth, fixedp modhz, fixedp feedback, fixedp chorusOffset) {
	this->fChorusOffset = chorusOffset;
	this->fFeedback_pct = 0;
	this->fMaxDelay_mSec = float2q(0.030f);
	this->fMinDelay_mSec = float2q(0.005f);
	this->fModDepth_pct = depth;
	this->fModFrequency_Hz = modhz;
	this->uModType = Chorus;
	this->uLFOPhase = normal;

	// Set up Delay Line
	setupDelayParams(&this->delay, chorusDelayBuffer, 9600);
	this->delay.wet = Q0_5;
	this->delay.dry = Q0_5;
	this->delay.fb = 0;

	// Set up LFO
	this->LFO.table = TriangleTable;
	this->LFO.invert = 0;
	this->LFO.unipolar = 1;
	this->LFO.mInc = 0;
	this->LFO.readPointer = 0;
	WaveTable_cookFrequency(&this->LFO, this->fModFrequency_Hz);
	WaveTable_reset(&this->LFO);
}

// calculateDelayOffset():
/*
	fLFOSample: a value from 0.0 to 1.0 from the LFO object

	returns: the calculated delay time in mSec for each effect

	NOTES: - the range for the flanger/vibrato is simply mapped from min to max
			 starting at min: 
			 fLFOSample*(m_fMaxDelay_mSec - m_fMinDelay_mSec)) + m_fMinDelay_mSec;

		   - the range for the Chorus includes the starting offset
			 fStart = m_fMinDelay_mSec + m_fChorusOffset;
*/
fixedp temp;
fixedp calculateDelayOffset(ModDelayParams *this, fixedp fLFOSample)
{
	fixedp fStart, temp;
	if(this->uModType == Flanger || this->uModType == Vibrato) 
	{
		// flanger 0->1 gets mapped to 0->maxdelay
		// return (fModDepth_pct/100.0)*(fLFOSample*(m_fMaxDelay_mSec - m_fMinDelay_mSec)) + m_fMinDelay_mSec;
		temp = qsub(this->fMaxDelay_mSec, this->fMinDelay_mSec);
		temp = qmul(temp, fLFOSample);
		temp = qmul(temp, this->fModDepth_pct);
		return qadd(temp, this->fMinDelay_mSec); //qadd(qmul(this->fModDepth_pct,qmul(fLFOSample, qsub(this->fMaxDelay_mSec, this->fMinDelay_mSec))), this->fMinDelay_mSec);
	}
	else if(this->uModType == Chorus)
	{
		// Chorus adds starting offset to move delay range
		fStart = qadd(this->fMinDelay_mSec, this->fChorusOffset);
		temp = qsub(this->fMaxDelay_mSec, this->fMinDelay_mSec);
		temp = qmul(temp, fLFOSample);
		temp = qmul(temp, this->fModDepth_pct);		
		return qadd(temp, fStart);
	}

	return 0;
}

void process_ModDelay(ModDelayParams *this, fixedp *process) {
	int delayInSamples, i;
	fixedp fDelay, fYn, fYqn;
	fYn = 0, fYqn = 0;


	for(i = 0; i < PROCESS_SIZE; i++) {
		// 1. Get LFO Values, normal and quad phase
		WaveTable_doOscillate(&this->LFO, &fYn, &fYqn); // need quad phase addition

		// 2. calculate delay offset
		fDelay = 0;
		if(this->uLFOPhase == quad) {
			fDelay = calculateDelayOffset(this, fYqn); // Quadtrature
		}
		else {
			fDelay = calculateDelayOffset(this, fYn);	// normal
		}

		// 3. set the delay & cook
		delayInSamples = _qmul(fDelay, SAMPLE_RATE, FIXED_FRACBITS, 0, 0); // INTE RÄTT
		if(delayInSamples <= 0) {
			delayInSamples = 1;
		}
		if(delayInSamples > 9600) {
			delayInSamples = 9600;
		}
		delay_setDelayTime(&this->delay, delayInSamples); // Osäker på detta :\

		// 4. get the delay output one channel in
		process_delay(&this->delay, &process[i], 1);
		process[i] = qmul(process[i], Q0_707);

	}
}

void flanger_setParam(ModDelayParams *t, Uint32 param, int val) {
	switch(param) {
	case FLANGER_FEEDBACK:
		t->delay.fb = val;
		break;
	case FLANGER_MIX:
		//t->delay.wet = val;
		//t->delay.dry = Q1 - (val);
		break;
	case FLANGER_MOD_DEPTH:
		if(val < 0) break;

		t->fModDepth_pct = val;
		if(t->fModDepth_pct >= Q1) t->fModDepth_pct = Q1;
		break;
	case FLANGER_MOD_FREQ:
		if(val < 0) break;
		t->fModFrequency_Hz = val;
		WaveTable_cookFrequency(&t->LFO, t->fModFrequency_Hz);
		break;
	}
}

void vibrato_setParam(ModDelayParams *t, Uint32 param, int val) {
	switch(param) {
	case VIBRATO_FEEDBACK:
		t->delay.fb = val;
		break;
	case VIBRATO_MIX:
		//t->delay.wet = val;
		//t->delay.dry = Q1 - (val);
		break;
	case VIBRATO_MOD_DEPTH:
		t->fModDepth_pct = val;
		if(t->fModDepth_pct >= Q1) t->fModDepth_pct = Q1;
		break;
	case VIBRATO_MOD_FREQ:
		t->fModFrequency_Hz = val;
		WaveTable_cookFrequency(&t->LFO, t->fModFrequency_Hz);
		break;
	}
}

void chorus_setParam(ModDelayParams *t, Uint32 param, int val) {
	
	switch(param) {
	case CHORUS_FEEDBACK:
		t->delay.fb = val;
		break;
	case CHORUS_MIX:
		//t->delay.wet = val;
		//t->delay.dry = Q1 - (val);
		break;
	case CHORUS_MOD_DEPTH:
		t->fModDepth_pct = val;
		if(t->fModDepth_pct >= Q1) t->fModDepth_pct = Q1;
		break;
	case CHORUS_MOD_FREQ:
		t->fModFrequency_Hz = val;
		WaveTable_cookFrequency(&t->LFO, t->fModFrequency_Hz);
		break;
	case CHORUS_OFFSET:
		t->fChorusOffset = val;
		break;
	}
}
