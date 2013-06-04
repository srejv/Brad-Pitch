/*
 * display.c
 *
 * Created: 3/19/2013 9:58:17 AM
 *  Author: Martin Andersson, Martin Viktorsson, Fredrik Kuitunen.
 */ 

#include "defines.h"	/* Gemensamma definitioner. */

#include <stdio.h>
#include <avr/pgmspace.h>

#include "display.h"
#include "lcd.h"
#include "text.h"		/* Definierad menytext. */
#include "parameters.h" /* Lagrar parametervärden. */

void init_text(void)
{
	;
	
}

void display_write(uint8_t effect, uint8_t parameters[])
{
	char buffer[22];
	uint8_t display_parameter[3];
	
	for (uint8_t i=LCD_FIRST_ROW; i<LCD_FOURTH_ROW; i++)
		display_parameter[i] = effect*7+((parameters[i]-1));
	
	/* Hoppar till angiven rad och skriver där efter ut. */
	lcd_row(0);	/* Rad 1. */
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(text_effect_table[effect])));
	if (parameters_get(effect, 0))
	{
		fprintf(stdout, buffer, "on");	/* Alltid parameter 0. */
	}
	else{
		fprintf(stdout, buffer, "off");	/* Alltid parameter 0. */
	}
	
	lcd_row(1);	/* Rad 2. */
	/* fprintf(stdout, text_parameters[effect][parameter], parameters_get(effect, (parameter+1))); */
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(text_parameter_table[display_parameter[0]])));
	fprintf(stdout, buffer, parameters_get(effect, parameters[0]));
	
	lcd_row(2);	/* Rad 3. */
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(text_parameter_table[display_parameter[1]])));
	fprintf(stdout, buffer, parameters_get(effect, parameters[1]));
	
	lcd_row(3);	/* Rad 4. */
	strcpy_P(buffer, (PGM_P)pgm_read_word(&(text_parameter_table[display_parameter[2]])));
	fprintf(stdout, buffer, parameters_get(effect, parameters[2]));
}	