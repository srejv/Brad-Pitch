
#include "brad_delay.h"
#include "definebrad.h"
#include "brad_input.h"
#include "biquad.h"

void setupDelayParams(DelayParams *this, fixedp *buffer, Uint32 bufferSize) {
	this->wp = 0;		// inskrivningsindex
	this->rp = 0;		// avläsningsindex
	this->inc = Q1;		// inkrementeringsvärde
	this->fb = 48;

	this->useExternalFeedback = 0;
	this->externalFbSample = 0;
	this->currentFbSample = 0;
	
	this->wet = Q0_5;
	this->dry = Q0_5;

	this->delayInSamples = 0;
	this->buffer = buffer;
	this->bufferSize = bufferSize;
	
	memset(buffer, 0, sizeof(fixedp)*bufferSize);
}

//	Utför processering för en delay
void process_delay(DelayParams *this, fixedp *process, Uint32 processSize) {
	int i;
	fixedp out, in;
	int rpi; 		// integer part
	fixedp frac; // fractional part
	fixedp next; // next value in case of wrapping
	fixedp result;
	
	//fixedp feedbackSample;

	for(i = 0; i < processSize; i++) {
		in = process[i];

		// Get the sample from buffer
 		rpi = (int)qipart(this->rp);
		frac = qfpart(this->rp);
		next = rpi + 1 != this->bufferSize ? this->buffer[rpi+1] : this->buffer[0];
		
		out = this->buffer[rpi];
		
		// out = out + frac * (next - out) (linear interp)
		result = qadd(out, qmul(frac, qsub(next, out))); 
		
		
		// in i buffern (in + ut*feedback)
		if(this->useExternalFeedback == 0) {
			this->buffer[this->wp] = qadd(in, qmul(result, this->fb));
		}
		else {
			this->buffer[this->wp] = qadd(in, qmul(this->externalFbSample, this->fb)); // + delayP.fb*result ;
		}
		
		
		// Summera och släng ut! (ut * wet + in * dry)
		process[i] = qadd(qmul(result,this->wet), qmul(in,this->dry));
		
		// Increase read pointer and wrap around
		this->rp = qadd(this->rp, this->inc); 
		rpi = (int)qipart(this->rp);
		if(rpi >= this->bufferSize) this->rp = qsub(this->rp, this->bufferSize << FIXED_FRACBITS);

		// Increase write pointer and wrap around
		this->wp += 1;
		if(this->wp >= this->bufferSize) this->wp = 0;
	}
	return;
}

void process_filteredDelay(DelayParams *this, BiQuad *filter, fixedp *process, Uint32 processSize)  {
	
	int i;
	fixedp out, in;
	int rpi; // integer part
	fixedp frac; // fractional part
	fixedp next; // next value in case of wrapping
	fixedp result;
	
	//fixedp feedbackSample;

	for(i = 0; i < processSize; i++) {
		in = process[i];
		
		rpi = (int)qipart(this->rp);
		frac = qfpart(this->rp);
		next = rpi + 1 != this->bufferSize ? this->buffer[rpi+1] : this->buffer[0];
		
		out = this->buffer[rpi];
		
		// out = out + frac * (next - out) (linear interp)
		result = qadd(out, qmul(frac, qsub(next, out))); 
		
		// in i buffern (in + ut*feedback)
		this->buffer[this->wp] = qadd(in, qmul(BiQuad_do(filter, result), this->fb));
		

		// Summera och släng ut! (ut * wet + in * dry)
		process[i] = qadd(qmul(result,this->wet), qmul(in,this->dry));
		
		// Increase read pointer and wrap around
		this->rp = qadd(this->rp, this->inc); 
		rpi = (int)qipart(this->rp);
		if(rpi >= this->bufferSize) this->rp = qsub(this->rp, this->bufferSize << FIXED_FRACBITS);

		// Increase write pointer and wrap around
		this->wp += 1;
		if(this->wp >= this->bufferSize) this->wp = 0;
	}
	return;
}


void delay_setParam(DelayParams *this, BiQuad *filter, Uint32 param, int val) {
	switch(param) {
	case DELAY_TIME:
		delay_setDelayTime(this, val * 48);
		break;
	case DELAY_MIX:
		delay_setMix(this, val);
		break;
	case DELAY_FDB:
		delay_setFb(this, val);
		break;
	case DELAY_FILTER_TYPE:
	case DELAY_FILTER_FC:
	case DELAY_FILTER_Q:
		
		// update filter
		switch(*((int*)DELAY_FILTER_TYPE)) {
			case FILTER_LOWPASS:
				BiQuad_calculateLowPass(filter, *((int*)DELAY_FILTER_FC));
				break;
			case FILTER_HIGHPASS:
				BiQuad_calculateHighPass(filter, *((int*)DELAY_FILTER_FC));
				break;
			case FILTER_BANDPASS:
				BiQuad_calculateBPF(filter, *((int*)DELAY_FILTER_FC), *((int*)DELAY_FILTER_Q));
				break;
			case FILTER_BANDSTOP:
				BiQuad_calculateBSF(filter, *((int*)DELAY_FILTER_FC), *((int*)DELAY_FILTER_Q));
				break;
		}
		break;
	}
}

void delay_setDelayTime(DelayParams *this, int delayInSamples) {
	// subtract to make read index
	this->rp = int2q(this->wp - delayInSamples); // cast as int!

	// check and wrap BACKWARDS if the index is negative
	if( this->rp < 0 )
		this->rp += int2q(this->bufferSize);
}

void delay_setMix(DelayParams *this, int mix) {
	this->wet = mix;
	this->dry = Q1 - mix;
}

void delay_setFb(DelayParams *this, int fb) {

	this->fb = fb;
	if(this->fb > Q1) this->fb = Q1;
	else if(this->fb < -Q1) this->fb = -Q1;

}
