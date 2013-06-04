
#include "brad_dist.h"
#include "qmath.h"
#include "definebrad.h"
#include "brad_input.h"
#include "tistdtypes.h"


void process_dist(Distortion *t, fixedp *x) {
	
	int type;
	type = *((int*)DISTORTION_TYPE);
	
	switch(type) {
		case 0:
			process_fuzz(t, x);
			break;
		case 1:
			process_thunderFuzz(t, x);
			break;
		case 2:
			process_squareDist(t, x);
			break;
		case 3:
			process_overdrive(t, x);
			break;
	}
	return;
}
void process_fuzz(Distortion *t, fixedp *x) { 
	Uint32 n;
	
	if(t->fdb != 0) {
	
		for(n = 0; n < PROCESS_SIZE; n++) {
			t->prev = qadd(qmul(x[n], t->gain), qmul(t->prev, t->fdb));

			if (t->prev > t->lvl1)
				t->prev = t->lvl1;
			else if (t->prev < -t->lvl2)
				t->prev = -t->lvl2;

			x[n] = t->prev;
		}

	}
	else {
		for(n = 0; n < PROCESS_SIZE; n++) {
			t->prev = qmul(x[n], t->gain);

			if (t->prev > t->lvl1)
				t->prev = t->lvl1;
			else if (t->prev < -t->lvl2)
				t->prev = -t->lvl2;

			x[n] = t->prev;
		}
	}
}

void process_squareDist(Distortion *t, fixedp *x) { 
	Uint32 n;
	
	if(t->fdb != 0) {
		for(n = 0; n < PROCESS_SIZE; n++) {
			t->prev = qadd(qmul(x[n], t->gain), qmul(t->prev, t->fdb));
			
			if (t->prev > 0)
				t->prev = t->lvl1;
			else if (t->prev < 0)
				t->prev = -t->lvl2;

			x[n] = t->prev;
		}

	}
	else {
		for(n = 0; n < PROCESS_SIZE; n++) {
			t->prev = qmul(x[n], t->gain);

			if (t->prev > 0)
				t->prev = t->lvl1;
			else if (t->prev < 0)
				t->prev = -t->lvl2;

			x[n] = t->prev;
		}
	}
}


void process_thunderFuzz( Distortion *t, fixedp *x ) {
	Uint32 n;
	
	// remember to keep sample from previous cycle. :) keep in structure. disttruc
	if(t->fdb) {
		for(n=0; n < PROCESS_SIZE; n++) {
			t->prev = qadd(qmul(x[n],t->gain), qmul(t->prev, t->fdb));
		
			if (t->prev > t->lvl1) 
				t->prev = t->lvl1;
			else if (t->prev < -t->lvl2)
				t->prev = -t->lvl2;

			x[n] = qabs(t->prev);
		}
	}
	else {
		for(n=0; n < PROCESS_SIZE; n++) {
			t->prev = qmul(x[n],t->gain);

			if (t->prev > t->lvl1) 
				t->prev = t->lvl1;
			else if (t->prev < -t->lvl2)
				t->prev = -t->lvl2;
			

			x[n] = qabs(t->prev);
		}
	}
}

void process_overdrive(Distortion *t, fixedp *x) {
	
	Uint32 n;
	fixedp denom, tmp, a, b, numerator, denom1, denom2, hurr;
	
	// kom ihåg förra processens sampel?
	/*if(t->fdb) {
		for(n = 0; n < PROCESS_SIZE; n++) {
			t->prev = qadd( qmul( x[n],t->gain ), qmul( qmul(t->prev, t->gain), t->fdb ) );
			
	
			// 6554 = 0.2
			numeratorLvl1 = qmul(t->lvl1, qexp(qmul(t->prev, qadd(Q1, qmul(6554, qsub(Q1, t->lvl1))))));
			numeratorLvl2 = qmul(t->lvl2, qexp(qmul(-t->prev, qadd(Q1, qmul(6554, qsub(Q1, t->lvl2))))));
			denom = qadd(qexp(t->prev), qexp(-t->prev));
			x[n] = qdiv(qsub(numeratorLvl1, numeratorLvl2), denom);
		}
	} 
	else {*/
	hurr = qinv(t->gain);
		for(n = 0; n < PROCESS_SIZE; n++) {

			t->prev = qmul(x[n],t->gain);
			
			a = qadd(Q1,qmul(hurr,qsub(Q1,t->lvl1)));
			b = qadd(Q1,qmul(hurr,qsub(Q1,t->lvl2)));
			tmp = qmul(-t->prev, qadd(a,b));
			
			if(tmp > short2q(32)) tmp = short2q(32);
			if(tmp < short2q(-32)) tmp = short2q(-32);
			
			tmp = qexp(tmp);
			numerator = qsub(t->lvl1, qmul(t->lvl2, tmp));
			
			tmp = qmul(t->prev, qsub(Q1, a));

			if(tmp > short2q(32)) tmp = short2q(32);
			if(tmp < short2q(-32)) tmp = short2q(-32);
			denom1 = qexp(tmp);

			tmp = qmul(-t->prev, qadd(Q1, a));

			if(tmp > short2q(32)) tmp = short2q(32);
			if(tmp < short2q(-32)) tmp = short2q(-32);
			denom2 = qexp(tmp);
			
			denom = qadd(denom1, denom2);
			tmp = qdiv(numerator, denom);
			
			if (tmp > AUDIOMAX) {
				// nu är tmp större än 32767
				tmp = AUDIOMAX;
			} else if (tmp < AUDIOMIN) {
				tmp = AUDIOMIN;
			}

			x[n] = tmp;
		}
	//}
}


void dist_setParam(Distortion *t, Uint32 param, int val) {
	switch(param) {

	case DISTORTION_FDB:
		t->fdb = val;
		break;
	case DISTORTION_GAIN:
		t->gain = val;
		break;
	case DISTORTION_LVL1:
		t->lvl1 = val;
		break;
	case DISTORTION_LVL2:
		t->lvl2 = val;
		break;
	}
}
