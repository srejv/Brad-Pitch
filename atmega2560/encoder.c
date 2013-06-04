/*
 * encoder.c
 *
 */ 

#include "defines.h"

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "encoder.h"
#include "uart.h"		/* Drivrutin för UART-kommunikation */

/* Deklarationer */
static uint8_t old_state[6], old_state2;

void encoder_init()
{
	uint8_t n;
		
	/* Konfigurerar ingångar. */
	DDRK = 0;				/* DDRC = 0; */
	DDRJ &= 0b11100000;		/* DDRD = DDRD & 0b11000001; */
	
	/* Stätter på pull-up resistorer. */
	PORTK = 0xff;				/* PORTC = 0; */
	PORTJ |= 0b00011111;	/* PORTD = PORTD & 0b11000001; */
	
	/* Aktiverar pull-up-resistorer. */
	PORTJ |= (1 << enc12);
	
	/* Läser in initiala värden till funktionen encoder_change2(). */
	old_state2 = encoder_signals;

	#ifdef debug
		fprintf(stderr, "Initialt varde fran pulsgivare: 0x%x\n" , encoder_signals);
	#endif
	
	for(n = 1; n < 7; n++)
	{
		old_state[n] = (encoder_signals & (3 << (2 * (n-1)))) >> (2 * (n-1));
		#ifdef debug
			fprintf(stderr, "Initialt pulsgivarvarde %d: %d\n", n, old_state[n]);
		#endif
	}
	
	/* Väljer vilka ben som ska ge avbrott. */
	/* PCMSK2 */
	PCMSK2 = 0xff;				/* Samtliga på port F. */
	/* PCMSK3 */
	PCMSK1 = 0x1f;
				//_BV(enc8) |		/* PK0 */
				//_BV(enc9) |		/* PK1 */
				//_BV(enc10)|		/* PK2 */
				//_BV(enc11);		/* PK3 */
				//_BV(enc12);	*/	/* PK4 */
	
	/* Aktiverar avbrott vid ändring. */
	PCICR =		_BV(PCIE2) |	/* PCIE2 */ /* Port C */
				_BV(PCIE1);		/* PCIE3 */	/* Port D */
	
}

void encoder_debounce(void)
{
	_delay_us(bounce_time);	
}

uint8_t encoder_direction(uint8_t n, unsigned int state)
{	
	/* Läser ut aktuella signaler. */
	state = ( (state & (3 << (2*(n-1))) ) >> (2*(n-1)) );
	
	/* Tillståndsmaskin. */
	switch(old_state[n])
	{
		case 0:						/* Tillstånd ba = 00. */
		if(state == 1)			/* Om nytt tillstånd är ba == 01 */
		{
			old_state[n] = state;	/* Hoppar till ett nytt tillstånd. */
			return CW;			/* Returnerar riktningen medurs. */
		}
		else if(state == 2)		/* Om nytt tillstånd är ba = 10 */
		{
			old_state[n] = state;	/* Hoppar till ett nytt tillstånd. */
			return CCW;			/* Returnerar riktningen moturs. */
		}
		
		case 1:						/* Tillstånd ba = 01 */
		if(state == 0)			/* Om nytt tillstånd är ba == 00 */
		{
			old_state[n] = state;	/* Hoppar till ett nytt tillstånd. */
			return NC;			/* Returnerar inget. */
		}
		else if(state == 3)		/* Om nytt tillstånd är ba == 11 */
		{
			old_state[n] = state;	/* Hoppar till ett nytt tillstånd. */
			return NC;			/* Returnerar inget. */
		}
		
		case 2:						/* Tillstånd ba = 10. */
		if(state == 0)			/* Om nytt tillstånd är ba == 00 */
		{
			old_state[n] = state;	/* Hoppar till ett nytt tillstånd. */
			return NC;			/* Returnerar inget. */
		}
		else if(state == 3)		/* Om nytt tillstånd är ba = 11 */
		{
			old_state[n] = state;	/* Hoppar till ett nytt tillstånd. */
			return NC;			/* Returnerar inget. */
		}
		
		case 3:						/* Tillstånd ba = 11 */
		if(state == 2)			/* Om nytt tillstånd är ba == 10 */
		{
			old_state[n] = state;	/* Hoppar till ett nytt tillstånd. */
			return CW;			/* Returnerar medurs. */
		}
		else if(state == 1)		/* Om nytt tillstånd är ba == 01 */
		{
			old_state[n] = state;	/* Hoppar till ett nytt tillstånd. */
			return CCW;			/* Returnerar moturs. */
		}
	}
	
	return NC;	
}

/* Funktioner kollar igenom varje tillståndsmaskin för pulsgivarna 
 * och returnerar nummer och riktning från den första som upptäcks. */
struct encoder_output encoder_change(uint8_t start, uint8_t stop)
{
	struct encoder_output out;
	unsigned int state;
	uint8_t direction;
	
	/* Läser in nytt tillstånd. */
	state = encoder_signals;
	
	for (start; start < stop+1; start++)
	{
		out.number = start;
		direction	= encoder_direction(start, state);
		if (direction == CW) {
			out.direction = CW;
			return out;
		}
		else if (direction == CCW) {
			out.direction = CCW;
			return out;
		}
	}	
		
	out.number = 0;
	out.direction = NC;
	return out;		
}

/* Funktionen kollar först vilka signaler som har ändrats och då vilken pulsgivare
 * för att sedan beräkna vilken riktning. Borde vara snabbare än encoder_change(). */
struct encoder_output encoder_change2(void)
{
	struct encoder_output out;
	int state;
	
	/* Läser in nytt tillstånd. */
	state = encoder_signals;
	
	/* Vilken signal från pulsgivarna har ändrats? */
	switch (state ^ old_state2)		/* Bitvis XOR. */
	{
		case 1:	/* Signal a från pulsgivare 1. */
			out.number = 1;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(1, state);
			return out;
			
		case 2:	/* Signal b från pulsgivare 1. */
			out.number = 1;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(1, state);
			return out;
		
		case 4:	/* Signal a från pulsgivare 2. */
			out.number = 2;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(2, state);
			return out;

		case 8:	/* Signal b från pulsgivare 2. */
			out.number = 2;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(2, state);
			return out;

		case 16:	/* Signal a från pulsgivare 3. */
			out.number = 3;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(3, state);
			return out;
	
		case 32:	/* Signal b från pulsgivare 3. */
			out.number = 3;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(3, state);
			return out;
	
		case 64:	/* Signal a från pulsgivare 4. */
			out.number = 4;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(4, state);
			return out;

		case 128:	/* Signal b från pulsgivare 4. */
			out.number = 4;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(4, state);
			return out;

		case 256:	/* Signal a från pulsgivare 5. */
			out.number = 5;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(5, state);
			return out;

		case 512:	/* Signal b från pulsgivare 5. */
			out.number = 6;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(6, state);
			return out;

		case 1024:	/* Signal a från pulsgivare 6. */
			out.number = 6;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(6, state);
			return out;

		case 2048:	/* Signal b från pulsgivare 6. */
			out.number = 6;
			state = ((state & (3 << 0)) >> 0);
			out.direction = encoder_direction(6, state);
			return out;
	}
	old_state2 = state;
	
	out.number = 0;
	out.direction = NC;
	return out;
}