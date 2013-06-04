/*
 * test.c
 *
 */ 

#include "defines.h"	/* Gemensamma definitioner. */

#include <ctype.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

#include "uart.h"			/* Drivrutin f�r UART-kommunikation */
#include "lcd.h"			/* H�gniv� drivrutin f�r LCD-modul. */
#include "hd44780.h"		/* L�gniv� drivrutin f�r LCD-modul. */
#include "encoder.h"		/* Drivrutin f�r pulsgivarna. */
/* #include "debug.h" */	/* Hj�lpmedel vid avlusning. */
#include "display.h"		/* Funktioner f�r mey. */
#include "parameters.h"		/* Lagrar parameterv�rden. */
#include "hpi.h"

/* Deklarationer */
#define FIRST_PARAMETER 0
#define SECOND_PARAMETER 1
#define THIRD_PARAMETER 2

static uint8_t effect=0, parameters[] = {1,2,3};
static uint8_t number_of_parameters[] = {7,6,4,4,5,5,7,4,4}; //Antal parametrar f�r var effekt

/* Konfigurerar en str�m uart_srt kopplad till funktionen uart_putchar. */
FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

/* Konfigurerar en str�m lcd_srt kopplad till funktionen lcd_putchar. */
FILE lcd_str = FDEV_SETUP_STREAM(lcd_putchar, NULL, _FDEV_SETUP_WRITE);

static void ioinit(void)
{
	/* Initierar UART. */
	uart_init();
	#ifdef debug
		fprintf(stderr, "UART-kommunikation initierad.\n");
	#endif
	
	/* Initierar LCD-modul. */
	hd44780_init();
	#ifdef debug
		fprintf(stderr, "LCD-modul initierad.\n");
	#endif
	
	/* Initierar pulsgivarna. */	
	encoder_init();	
	#ifdef debug
		fprintf(stderr, "Pulsgivare initierade.\n");
	#endif
	
	/* Initierar HPI. */
	hpi_init();
	#ifdef debug
		fprintf(stderr, "HPI initierade.\n");
	#endif
}

/* Anropas n�r pulsgivare S1, S2, S3 eller S4. */
ISR(PCINT2_vect)	/* PCINT2_vect */
{	
	struct encoder_output in;
	int tmp16;
	
	/* Startar tidtagare f�r att undvika kontaktstuds. */
	encoder_debounce();
	
	/* Kontrollerar vilken pulsgivare som har �ndrats
	 * och i vilken riktning. */
	in = encoder_change(1, 4);
	
	/* Uppdaterar parametrar. */
	switch (in.number)
	{
		case 1:
			parameters[FIRST_PARAMETER] = 1;					/* Parametermenyn b�rjar fr�n b�rjan. */
			parameters[SECOND_PARAMETER] = 2;
			parameters[THIRD_PARAMETER] = 3;
			if (in.direction == CW)
			{
				effect++;
				if(effect >= NUMBER_OF_EFFECTS)
					effect = 0;
			}
			else if (in.direction == CCW)
			{
				if(effect == 0)
					effect = NUMBER_OF_EFFECTS-1;
				else
					effect--;
				
			}				
			break;
			
		case 2:		
			parameters[FIRST_PARAMETER] -= 1;
			if (in.direction == CW) {
				parameters[FIRST_PARAMETER]++;
				if(parameters[FIRST_PARAMETER] >= number_of_parameters[effect]) {
					parameters[FIRST_PARAMETER] = 1;
				}
			}
			else if (in.direction == CCW) {
				parameters[FIRST_PARAMETER]--;
				if(parameters[FIRST_PARAMETER] < 1) {
					parameters[FIRST_PARAMETER] = number_of_parameters[effect]-1;
				}
			}
			for (uint8_t i = FIRST_PARAMETER; i <= THIRD_PARAMETER; i++) 
			{
				parameters[i] = (parameters[FIRST_PARAMETER]+i) % number_of_parameters[effect];
			}
			for (uint8_t i = FIRST_PARAMETER; i <= THIRD_PARAMETER; i++)
			{
				parameters[i] += 1;
			}								
			break;
			
		case 3:
			tmp16 = parameters_get(effect, 0);
			if (tmp16)
			{
				tmp16=0;
			}
			else
				tmp16++;
			parameters_store(effect, 0, tmp16);
			break;
			
		case 4:
			tmp16 = parameters_get(effect, parameters[0]);
			if (in.direction == CW) {
				if ((tmp16+param_scale[effect][parameters[0]])>= param_max[effect][parameters[0]])
				{
					tmp16=param_max[effect][parameters[0]];
				}
				else{
					tmp16=tmp16+param_scale[effect][parameters[0]];
				}
			}
			else if (in.direction == CCW) {
				if ((tmp16-param_scale[effect][parameters[0]]) <= param_min[effect][parameters[0]])
				{
					tmp16=param_min[effect][parameters[0]];
				}
				else{
					tmp16=tmp16-param_scale[effect][parameters[0]];
				}
			}	
			parameters_store(effect, parameters[0], tmp16);
			break;
	}
	
	/* St�nger flaggan f�r  */
	PCIFR = _BV(PCIF2);		/* PCIF2 */
	
	/* Aktivierar avbrott */
	sei();
}

/* Anropas n�r pulsgivare S5 och S6 �ndras. */
ISR(PCINT1_vect)	/* PCINT3_vect */
{
	struct encoder_output in;
	int tmp16;
	
	/* Startar tidtagare f�r att undvika kontaktstuds. */
	encoder_debounce();
	
	/* Kontrollerar vilken pulsgivare som har �ndrats
	 * och i vilken riktning. */
	in = encoder_change(5, 6);
	
	/* Uppdaterar parametrar. */
	switch (in.number)
	{		
		case 5:
			tmp16 = parameters_get(effect, parameters[1]);
			if (in.direction == CW) {
				if ((tmp16+param_scale[effect][parameters[1]])>= param_max[effect][parameters[1]])
				{
					tmp16=param_max[effect][parameters[1]];
				}
				//tmp16++;
				else{
					tmp16=tmp16+param_scale[effect][parameters[1]];
				}				
			}					
			else if (in.direction == CCW) {
				if ((tmp16-param_scale[effect][parameters[1]]) <= param_min[effect][parameters[1]])
				{
					tmp16=param_min[effect][parameters[1]];
				}
				else{
					tmp16=tmp16-param_scale[effect][parameters[1]];
				}//tmp16--;	
			}					
					
			parameters_store(effect, parameters[1], tmp16);
			break;
			
		case 6:
			tmp16 = parameters_get(effect, parameters[2]);
			if (in.direction == CW) {
				if ((tmp16+param_scale[effect][parameters[2]])>= param_max[effect][parameters[2]])
				{
					tmp16=param_max[effect][parameters[2]];
				}
				//tmp16++;
				else{
					tmp16=tmp16+param_scale[effect][parameters[2]];
				}
			}
			else if (in.direction == CCW) {
				if ((tmp16-param_scale[effect][parameters[2]]) <= param_min[effect][parameters[2]])
				{
					tmp16=param_min[effect][parameters[2]];
				}
				else{
					tmp16=tmp16-param_scale[effect][parameters[2]];
				}//tmp16--;
			}
			parameters_store(effect, parameters[2], tmp16);
			break;
	}
	
	/* St�nger flaggan f�r  */
	PCIFR = _BV(PCIF1);		/* PCIF3 */
	
	/* Aktivierar avbrott */
	sei();
}	

/* H�r startar programmet. */
int main(void)
{	
	/* Konfiguration av str�mmar. */
	stderr = stdin = &uart_str;			/* Kopplar error output till uart_str. */
	stdout = &lcd_str;					/* Kopplar standard output till lcd_str. */
	SET(PORT,PARAM_INT1);					//S�tt interruptutg�ng h�g
	
	/* Initiering av in- och utg�ngar samt inkopplade enheter. */
	ioinit();
	
	/* Skriver ut effektmeny 0. */
	display_write(0, parameters);
	
	/* Aktiverar avbrott. */
	sei();
	#ifdef debug
		fprintf(stderr, "Avbrott aktiverat.\n");
	#endif
	
	/* Testar HPI */
	//hpi_test();
	
	
	/* Programslinga som snurrar i det o�ndliga och avbryts av avbrott. */
	for(;;)
	{
		/* Rensar skr�men. */
		lcd_clear();
		
		/* Skriver ut p� sk�rmen. */
		display_write(effect, parameters);
				
		/* V�ntar f�r att undvika flimmer p� sk�rmen. */
		_delay_ms(200);
		
		fprintf(stderr, ".");
	}
}
