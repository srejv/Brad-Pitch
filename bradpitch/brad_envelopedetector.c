
#include "brad_envelopedetector.h"
#include "definebrad.h"

void EnvDetector_setupDetector(EnvelopeDetector *this) {

	EnvDetector_setAttackAndReleaseTime(this, Q0_5, Q0_5);
	this->envelope = 0;
		
	//this->attack_coef = exp(log(0.01)/(attack_in_ms * samplerate * 0.001));
	//this->release_coef = exp(log(0.01)/(release_in_ms * samplerate * 0.001));
	return;
}

void EnvDetector_setAttackAndReleaseTime(EnvelopeDetector *this, fixedp attackTimeInMs, fixedp releaseTimeInMs) {
	fixedp aims, rims, sampleRate;

	this->attack_in_ms = attackTimeInMs;
	this->release_in_ms = releaseTimeInMs;

	sampleRate = _qmul(SAMPLE_RATE << 4, Q_MSEC_TO_S, 4, FIXED_FRACBITS, FIXED_FRACBITS);
	// Detta är fel. Suck. Får fixa hemma eller nåt. :<
	aims = qinv(qmul(attackTimeInMs, sampleRate));
	rims = qinv(qmul(attackTimeInMs, sampleRate));

	this->attack_coef = qexp(qmul(Q_TC_DIGITAL, aims));
	this->release_coef = qexp(qmul(Q_TC_DIGITAL, rims));
	return;
}

fixedp EnvDetector_detect(EnvelopeDetector *this, fixedp *in) {
	fixedp tmp = abs(in[0]);
	if(tmp > this->envelope) {
		// attack
		this->envelope = qadd(qmul(this->attack_in_ms, qsub(this->envelope, in[0])), in[0]);
	}
	else {
		// release
		this->envelope = qadd(qmul(this->release_in_ms, qsub(this->envelope, in[0])), in[0]);
	}
	return this->envelope;
}

