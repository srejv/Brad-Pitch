
#include "brad_tremolo.h"
#include "brad_input.h"

void setupTremoloParams(TremoloParam *this) {
	// LFO
	this->LFO.invert = 0;
	this->LFO.table = TriangleTable;
	this->LFO.unipolar = 0;
	this->LFO.mInc = 0;
	this->LFO.readPointer = 0;
	
	// Set tremolo
	this->depth = float2q(0.75f);
	this->dry = 0;
	this->wet = Q1;
	this->fdb = 0;
	this->g1 = Q1 - qmul(this->depth, Q0_5);
	this->g2 = qmul(this->depth, Q0_5);


	WaveTable_reset(&this->LFO);
	WaveTable_cookFrequency(&this->LFO, float2q(3.43f));
}

void tremolo_setParam(TremoloParam *this, Uint32 adr, int val) {
	switch(adr) {
	case TREMOLO_FEEDBACK:
		this->fdb = val;
		break;
	case TREMOLO_MIX:
		this->wet = val;
		this->dry = Q1 - this->wet;
		break;
	case TREMOLO_MOD_DEPTH:
		this->depth = val;
		this->g1 = Q1 - qmul(val, Q0_5);
		this->g2 = qmul(val, Q0_5);
		break;
	case TREMOLO_MOD_FREQ:
		WaveTable_cookFrequency(&this->LFO, val);
		break;
	}
}

// Tremolo - Ändrar amplituden mellan [0,1].
// Ehm. just ingen wet/dry
void process_tremolo(TremoloParam *this, fixedp *buffer) {
	int i;
	fixedp y, yq, result;
	for(i = 0; i < PROCESS_SIZE; i++) {
		WaveTable_doOscillate(&this->LFO, &y, &yq);
		//buffer[i] = qadd(qmul(qadd(result), this->wet), qmul(buffer[i], this->dry));
		// y(n) = x(n)(g1 + LFO(n)g2)
		result = this->g1 + qmul(y, this->g2); // buffer[i]
		buffer[i] = qmul(result, buffer[i]); 
	}
	return;
}
