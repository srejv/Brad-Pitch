
#include "wt.h"
#include "qmath.h"

#include "brad_sin.table"
//#include "brad_sawtooth.table"
#include "brad_triangle.table"

#define qN short2q(WT_SIZE)

void WaveTable_doOscillate(WaveTable *this, fixedp *pYn, fixedp *pYqn) {
	
	fixedp outSample = 0;
	fixedp quadOutSample = 0;

	// get int part
	int readIndex = qipart(this->readPointer);
	int quadPhaseReadIndex = qipart(this->quadPhaseReadIndex);

	// get frac part
	fixedp frac = qfpart(this->readPointer);

	// check for next, might wrap around
	int readNextIndex = readIndex + 1 > WT_SIZE - 1 ? 0 : readIndex + 1;
	int quadReadNextIndex = quadPhaseReadIndex + 1 > WT_SIZE - 1 ? 0 : quadPhaseReadIndex + 1;

	//printf("This: %ld, Next: %ld, Frac: %ld \n", this->table[readIndex], this->table[readNextIndex], frac);
	outSample = dLinTerp(0, Q1, this->table[readIndex], this->table[readNextIndex], frac);
	quadOutSample = dLinTerp(0, Q1, this->table[quadPhaseReadIndex], this->table[quadReadNextIndex], frac);
	// add increment
	this->readPointer = qadd(this->readPointer, this->mInc);
	this->quadPhaseReadIndex = qadd(this->quadPhaseReadIndex, this->mInc);

	// check for wrap around
	if(this->readPointer >= qN) {
		this->readPointer = qsub(this->readPointer, qN);
	}

	if(this->quadPhaseReadIndex >= qN) {
		this->quadPhaseReadIndex = qsub(this->quadPhaseReadIndex, qN);
	}

	// output
	*pYn = outSample;
	*pYqn = quadOutSample;

	if(this->invert) {
		*pYn *= -1;
		*pYqn *= -1;
	}

	if(this->unipolar) {
		*pYn = qmul(*pYn, Q0_5);
		*pYn = qadd(*pYn, Q0_5);

		*pYqn = qmul(*pYqn, Q0_5);
		*pYqn = qadd(*pYqn, Q0_5);
	}
}

/*
void WaveTable_generateTables() {
	

	// Variables
	fixedp fi, mt1, bt1, mt2, bt2, mtf2, btf2;
	fixedp ms1, bs1, ms2, bs2;
	fixedp oneOverN;
	fixedp squareMin, squareMax;
	int i;

	// Useful variables
	oneOverSR = _qinv(48000, 0, 24);	// 1/SR (1/8000)

	oneOverN = short2q(WT_SIZE);		// N (we want 1/1024)
	oneOverN = qinv(oneOverN);			// 1/N (1/1024)


	// Triangle
	// rising edge1:
	mt1 = qinv(short2q(256));
	bt1 = 0;

	// rising edge 2:
	mt2 = qinv(short2q(256));
	bt2 = short2q(-1);

	// valling edge:
	mtf2 = qinv(short2q(512));
	mtf2 = qmul(short2q(-2), mtf2);
	btf2 = short2q(1);

	// Sawtooth
	// rising edge1:
	ms1 = qinv(short2q(512));
	//ms1 = qinv(ms1);
	bs1 = 0;

	// rising edge2:
	ms2 = qinv(short2q(512));
	//ms2 = qinv(ms2);
	bs2 = short2q(-1) + 1;

	// square
	squareMax = qmul(short2q(1), float2q(0.70795f));
	squareMin = qmul(short2q(-1), float2q(0.70795f));

	// create SineTable
	for(i = 0; i < WT_SIZE; i++) {
		fi = short2q(i);

		// Sin table	( sin( (fi*1/n)*2pi ) * 0.70795f) (-3dB)
		SinTable[i] = qmul( qsin( qmul( qmul( fi, oneOverN ), Q2PI )), float2q(0.70795f) );	

		// Square table
		if(i > 0)
			SquareTable[i] = i < 512 ? squareMax : squareMin;
		else 
			SquareTable[i] = 0;

		// Sawtooth table
		SawtoothTable[i] = i < 512 ? qadd( qmul( ms1, fi ), bs1 ) : qadd( qmul( ms2, short2q( i-511 ) ), bs2 );
		SawtoothTable[i] = qmul( SawtoothTable[i], float2q(0.70795f));

		// Triangle table
		if( i < 256 )
			TriangleTable[i] = qadd( qmul( mt1, fi ), bt1 ); // mx + b; rising edge 1
		else if(i < 768)
			TriangleTable[i] = qadd( qmul( mtf2, short2q( i-256 ) ), btf2 ); // mx + b; falling edge
		else 
			TriangleTable[i] = qadd( qmul( mt2, short2q( i-768 ) ), bt2 ); // mx + b; rising edge 2c
		
		TriangleTable[i] = qmul( TriangleTable[i], float2q(0.70795f) );
	}
}*/

void WaveTable_cookFrequency(WaveTable *this, fixedp frequency) {
	fixedp tmp;
	tmp = _qmul(frequency, WT_SIZE, FIXED_FRACBITS, 0, FIXED_FRACBITS);
	tmp = _qdiv(tmp, 48000, FIXED_FRACBITS, 0, FIXED_FRACBITS);

	this->mInc = tmp;
}

void WaveTable_reset(WaveTable *this) {
	this->readPointer = 0;
	this->quadPhaseReadIndex = -QPIO2; // sätt detta rätt. 
}
