

#include "brad_reverb.h"
#include "definebrad.h"
#include "brad_input.h"

// 24000 sampel = halva samplingsfrekvensen
// vilket innebär en halv sekunds max reverbtid.
#define REVERB_BUFFER_SIZE 24000
#define REVERB_BUFFER_START 0x8060000
fixedp reverbBuffer[REVERB_BUFFER_SIZE];

#define Q31 31

// t = this, standard inom objektorientering
// jävligt dålig kod. 
// dåliga kommentarer. fuck. pinsamt. 

void reverb_setupParams(ReverbParams *t) {
	t->alfa = 0;
	t->cutOffFrequency = short2q(500);
	t->feedback = 32765;
	t->prev = 0;
	t->revTime_samples = 960;
	t->wp = 0;
	
	reverb_calculateFilter(t);
	memset(reverbBuffer, 0, sizeof(fixedp)*24000);
} 

void process_reverb(ReverbParams *t, fixedp *process) {
	Int32 i, rp;
	fixedp revtmp;

	rp = t->wp - t->revTime_samples;
	if(rp < 0) rp += 24000;
	for(i = 0; i < PROCESS_SIZE; i++) {
		// Feedback * reverb[now - delayinsamples];
		revtmp = qmul(t->feedback, reverbBuffer[rp]);

		// current value = alfa*revtmp + (1-alfa)*prevReverbVal
		t->prev = qmul(t->alfa, revtmp) + qmul(Q1 - t->alfa, t->prev);

		// insert current value to buffer
		reverbBuffer[t->wp] = process[i] + t->prev;
		
		// new value
		process[i] = qadd( qmul(t->wet, reverbBuffer[t->wp]), qmul(t->dry, process[i]) );
		
		// increment and wrap write and read pointer
		t->wp++;
		if(t->wp > 24000) t->wp = 0;
		rp++;
		if(rp >= 24000) rp = 0;
	}  
	
	return;
}

void reverb_calculateFilter(ReverbParams *t) {
	
	fixedp dt, rc;
	
	//% Lågpass-parametrar! --------------------------
	
	dt = _float2q(1.0f/SAMPLE_RATE, Q31);
	rc = _qinv(_qmul(q2q(Q2PI,FIXED_FRACBITS,Q31), q2q(t->cutOffFrequency,FIXED_FRACBITS,Q31), Q31, Q31, Q31), Q31, Q31);
	
	t->alfa = _qdiv(dt, rc + dt, Q31,Q31,Q31);
	t->alfa = q2q(t->alfa,Q31,FIXED_FRACBITS);
	return;
}

void reverb_setParam(ReverbParams *t, Uint32 param, int val) {
	switch(param) {
	case REVERB_FEEDBACK:
		t->feedback = val;
		break;
	case REVERB_CUTOFFFREQ:
		t->cutOffFrequency = int2q(val/10);
		reverb_calculateFilter(t);
		break;
	case REVERB_MIX:
		t->wet = val;
		t->dry = Q1 - val;
		break;
	case REVERB_TIME:
		t->revTime_samples = val*48;
		break;
	}
}	
