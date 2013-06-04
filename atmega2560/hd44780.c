/*
 * hd44780.c
 *
 *  Author: Martin Andersson
 */ 

#include "defines.h"

#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#include "hd44780.h"

#define HD44780_BUSYFLAG 0x80

/* Sänder en puls till HD44780_E (enable). */
static inline uint8_t
hd44780_pulse_e(bool readback)
{
  uint8_t x;

  SET(PORT, HD44780_E);
  
  if (readback)
	{
		__asm__ volatile("nop");	/* Väntar en klockpuls. */
		HD44780_DB_DDR = 0;					/* Konfigurerar databuss DB till ingång */
		x = HD44780_DB_IN;
		/*
		_delay_us(37);
		x = 0;
		*/
	}
    
  else
    x = 0;
  CLR(PORT, HD44780_E);

  return x;
}

/*  Sänder en byte till LCD-modulen. */
void hd44780_outbyte(uint8_t b, uint8_t RS)
{
	HD44780_DB_DDR = 0xff;					/* Databuss DB konfigureras till utgång. */
	CLR(PORT, HD44780_RW);					/* Skrivläge. */
	if (RS)							/* Data. */
		SET(PORT, HD44780_RS);
	else							/* Instruktion. */
		CLR(PORT, HD44780_RS);				
	HD44780_DB_OUT = b;							/* Lägger ut datan på bussen DB. */
	(void)hd44780_pulse_e(false);	/* Sänder en puls till E. */
}

/* Läser en byte från LCD-modulen. */
uint8_t hd44780_inbyte(uint8_t RS)
{
  uint8_t x;

  SET(PORT, HD44780_RW);					/* Läsläge. */
  HD44780_DB_DDR = 0x00;					/* Databus DB konfigureras till ingång */
  if (RS)
  {
	  SET(PORT, HD44780_RS);
  }  
  else
  {
	  CLR(PORT, HD44780_RS);
  }  
  x = hd44780_pulse_e(true);
  CLR(PORT, HD44780_RW);					/* Återgår till skrivläge */

  return x;
}

/* Väntar tills busy flag har slocknat. */
void hd44780_wait_ready(void)
{
  while (hd44780_incmd() & HD44780_BUSYFLAG) ;
}

/* Förbereder LCD-modulens ben för avstängning. */
void
hd44780_powerdown(void)
{
	PORTA = 0;	/* ASSIGN(PORT, DB0, 0); */
	CLR(PORT, HD44780_RS);
	CLR(PORT, HD44780_RW);
	CLR(PORT, HD44780_E);
}

void hd44780_test(void)
{
	uint8_t i;
	
	/* Testar skräm. */
	SET(PORT, HD44780_RS);			/* Ändrar till dataläge. */
	for(i=0; i<30; i++)
	{
		SET(PORT, HD44780_E);
		HD44780_DB_OUT = 'A';
		_delay_us(0.14);
		CLR(PORT, HD44780_E);
		_delay_us(40);
	}	
}	

/* Initierar LCD-modulen. */
void hd44780_init(void)
{
	uint8_t i;
	
	/* Konfigurerar utgångar till LCD-modul. */
	SET(DDR, HD44780_RS);	/* Sätter HD44780_RS till utgång */
	SET(DDR, HD44780_RW);	/* Sätter HD44780_RW till utgång */
	SET(DDR, HD44780_E);	/* Sätter E till utgång */
	HD44780_DB_DDR = 0xff;	/* Sätter hela DB till utgång. */
	
	/* Ställer utgångarna till noll. */
	CLR(PORT, HD44780_RS);					/* Instruktionsläge. */
	CLR(PORT, HD44780_RW);					/* Skrivning. */
	CLR(PORT, HD44780_E);					/* Nollställer E. */
	HD44780_DB_OUT = 0;							/* Nollställer databussen DB. */
	
	/* Vänta 15 ms. */
	_delay_ms(15);
	
	/* Ställer in funktion */
	SET(PORT, HD44780_E);
	HD44780_DB_OUT = HD44780_FunctionSet;
	_delay_us(0.14);
	CLR(PORT, HD44780_E);
	
	/* Vänta 5 ms */
	_delay_ms(5);

	/* Ger ut E-puls två gånger till */
	for(i=0; i<2; i++)	
	{
		SET(PORT, HD44780_E);
		_delay_us(0.14);
		CLR(PORT, HD44780_E);
		_delay_us(100);	
	}

	/* Rensar skärm och dess minne (DD RAM). */
	SET(PORT, HD44780_E);
	HD44780_DB_OUT = HD44780_ClearDisplay;
	_delay_us(0.14);
	CLR(PORT, HD44780_E);
	_delay_ms(16);
	
	/* Ställer in enter-läge. */
	SET(PORT, HD44780_E);
	HD44780_DB_OUT = HD44780_EnterMode;
	_delay_us(0.14);
	CLR(PORT, HD44780_E);
	_delay_us(40);
	
	/* Aktiverar skräm. */
	SET(PORT, HD44780_E);
	HD44780_DB_OUT = HD44780_DisplayOnOff;
	_delay_us(0.14);
	CLR(PORT, HD44780_E);
	_delay_us(40);
		
	/* hd44780_test(); */
}