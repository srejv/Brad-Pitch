/*
 * lcd.c
 *
 *  Author: Martin Andersson
 */ 

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>

#include "hd44780.h"
#include "lcd.h"

/* #include "debug.h" */

/* Skickar en bokstav till LCD-modulen. */
int lcd_putchar(char c, FILE *unused)
{
	static uint8_t row = 0;		/* Håller reda på vilken rad skärmen senast skrev till. */
	
	if (c == '\n')				/* Tittar efter new line. */
	{
		row++;
		if (row > LCD_FOURTH_ROW)			/* Om radnumret row överstiger 3 */
			row = LCD_FIRST_ROW;			/* hoppar vi tillbaka till radnummer 0. */
		lcd_row(row);			/* Ändrar rad hos LCD-modulen. */
		return 0;				/* Då inget ska skrivas vid new line avslutas funktionen. */
	}
  
	hd44780_wait_ready();		/* Väntar tills LCD-modulens interna instruktion är klar. */
	hd44780_outdata(c);			/* Skickar data till LCD-modulen. */

	return 0;
}

void lcd_row(uint8_t n)
{	
	/* Ändrar DDRAM-addres till första position på första raden. */
	if (n == LCD_FIRST_ROW)	
	{
		hd44780_wait_ready();
		hd44780_outcmd(HD44780_AddressRow0);	
	}
	
	/* Ändrar DDRAM-addres till första position på andra raden. */
	else if (n == LCD_SECOND_ROW)
	{
		hd44780_wait_ready();
		hd44780_outcmd(HD44780_AddressRow1);
	}
	
	/* Ändrar DDRAM-addres till första position på tredje raden. */
	else if (n == LCD_THIRD_ROW)
	{
		hd44780_wait_ready();
		hd44780_outcmd(HD44780_AddressRow2);
	}
	
	/* Ändrar DDRAM-addres till första position på fjärde raden. */
	else if (n == LCD_FOURTH_ROW)
	{
		hd44780_wait_ready();
		hd44780_outcmd(HD44780_AddressRow3);
	}
	
	/* Avslutar endast funktionen ifall parametern skulle vara olämplig. */	
	else
		;
}

/* Rensar skärmen */
void lcd_clear(void)
{
	hd44780_wait_ready();
	hd44780_outcmd(HD44780_ClearDisplay);
}