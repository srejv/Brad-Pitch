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

/* S�nder en puls till HD44780_E (enable). */
static inline uint8_t
hd44780_pulse_e(bool readback)
{
  uint8_t x;

  SET(PORT, HD44780_E);
  
  if (readback)
	{
		__asm__ volatile("nop");	/* V�ntar en klockpuls. */
		HD44780_DB_DDR = 0;					/* Konfigurerar databuss DB till ing�ng */
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

/*  S�nder en byte till LCD-modulen. */
void hd44780_outbyte(uint8_t b, uint8_t RS)
{
	HD44780_DB_DDR = 0xff;					/* Databuss DB konfigureras till utg�ng. */
	CLR(PORT, HD44780_RW);					/* Skrivl�ge. */
	if (RS)							/* Data. */
		SET(PORT, HD44780_RS);
	else							/* Instruktion. */
		CLR(PORT, HD44780_RS);				
	HD44780_DB_OUT = b;							/* L�gger ut datan p� bussen DB. */
	(void)hd44780_pulse_e(false);	/* S�nder en puls till E. */
}

/* L�ser en byte fr�n LCD-modulen. */
uint8_t hd44780_inbyte(uint8_t RS)
{
  uint8_t x;

  SET(PORT, HD44780_RW);					/* L�sl�ge. */
  HD44780_DB_DDR = 0x00;					/* Databus DB konfigureras till ing�ng */
  if (RS)
  {
	  SET(PORT, HD44780_RS);
  }  
  else
  {
	  CLR(PORT, HD44780_RS);
  }  
  x = hd44780_pulse_e(true);
  CLR(PORT, HD44780_RW);					/* �terg�r till skrivl�ge */

  return x;
}

/* V�ntar tills busy flag har slocknat. */
void hd44780_wait_ready(void)
{
  while (hd44780_incmd() & HD44780_BUSYFLAG) ;
}

/* F�rbereder LCD-modulens ben f�r avst�ngning. */
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
	
	/* Testar skr�m. */
	SET(PORT, HD44780_RS);			/* �ndrar till datal�ge. */
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
	
	/* Konfigurerar utg�ngar till LCD-modul. */
	SET(DDR, HD44780_RS);	/* S�tter HD44780_RS till utg�ng */
	SET(DDR, HD44780_RW);	/* S�tter HD44780_RW till utg�ng */
	SET(DDR, HD44780_E);	/* S�tter E till utg�ng */
	HD44780_DB_DDR = 0xff;	/* S�tter hela DB till utg�ng. */
	
	/* St�ller utg�ngarna till noll. */
	CLR(PORT, HD44780_RS);					/* Instruktionsl�ge. */
	CLR(PORT, HD44780_RW);					/* Skrivning. */
	CLR(PORT, HD44780_E);					/* Nollst�ller E. */
	HD44780_DB_OUT = 0;							/* Nollst�ller databussen DB. */
	
	/* V�nta 15 ms. */
	_delay_ms(15);
	
	/* St�ller in funktion */
	SET(PORT, HD44780_E);
	HD44780_DB_OUT = HD44780_FunctionSet;
	_delay_us(0.14);
	CLR(PORT, HD44780_E);
	
	/* V�nta 5 ms */
	_delay_ms(5);

	/* Ger ut E-puls tv� g�nger till */
	for(i=0; i<2; i++)	
	{
		SET(PORT, HD44780_E);
		_delay_us(0.14);
		CLR(PORT, HD44780_E);
		_delay_us(100);	
	}

	/* Rensar sk�rm och dess minne (DD RAM). */
	SET(PORT, HD44780_E);
	HD44780_DB_OUT = HD44780_ClearDisplay;
	_delay_us(0.14);
	CLR(PORT, HD44780_E);
	_delay_ms(16);
	
	/* St�ller in enter-l�ge. */
	SET(PORT, HD44780_E);
	HD44780_DB_OUT = HD44780_EnterMode;
	_delay_us(0.14);
	CLR(PORT, HD44780_E);
	_delay_us(40);
	
	/* Aktiverar skr�m. */
	SET(PORT, HD44780_E);
	HD44780_DB_OUT = HD44780_DisplayOnOff;
	_delay_us(0.14);
	CLR(PORT, HD44780_E);
	_delay_us(40);
		
	/* hd44780_test(); */
}