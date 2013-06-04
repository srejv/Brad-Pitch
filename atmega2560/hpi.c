/*
 * hpi.c
 *
 * Created: 4/10/2013 2:39:39 PM
 *  Author: Administrator
 */ 

#include <avr/io.h>
#include "defines.h"
#include <util/delay.h>

void hpi_hstrobe(void)
{
	CLR(PORT, HDS2);	/* Fallande flank hos HSTROBE l�ser kontrollsignaler. */
	_delay_ms(1);
	SET(PORT, HDS2);		/* Stigande flank hos HSTROBE l�ser datasignalerna. */
}

void hpi_init(void)
{
	
	/* Aktiverar utg�ngar till HPI. */
	HD_LSB_DDR = 0xff;
	HD_MSB_DDR = 0xff;
	SET(DDR, HDS2);
	SET(DDR, HAS);
	SET(DDR, HDS1);
	SET(DDR, HCNTL0);
	SET(DDR, HCS);
	SET(DDR, HHWIL);
	SET(DDR, HCNTL1);
	SET(DDR, HRW);
	SET(DDR, PARAM_INT1); //Interruptutg�ng
	
	
	/* Definierar konstanta v�rden. */
	SET(PORT, HAS);		/* Ska st� konstant h�g. */
	CLR(PORT, HCS);		/* Ska st� konstant l�g. */
	SET(PORT, HDS1);	/* Ska st� konstant h�g. */
	CLR(PORT, HRW);		/* Skrivl�ge. */
	
	/* Initiera HPIC.*/
	CLR(PORT, HRW);		// Skrivl�ge.

	/* Skriver till HPIC-kontrollregister. */
	CLR(PORT, HCNTL1);
	CLR(PORT, HCNTL0);
	
	CLR(PORT, HHWIL);	// F�rsta delen.
	
	HD_MSB = 0;			// Utdata p� f�rsta 8-bitarsporten.
	HD_LSB = 0;			// Utdata p� andra 8-bitarsporten.
	
	hpi_hstrobe();
	
	SET(PORT, HHWIL);	// Andra delen.
	HD_MSB = 0;
	HD_LSB = 0;
	
	hpi_hstrobe();
	
	/* Initiera HPIA-adressregister. */
	
	/* Skriver till HPIA-adressregister. */
	CLR(PORT, HCNTL1);
	SET(PORT, HCNTL0);
	CLR(PORT, HHWIL);	// F�rsta delen.
	HD_MSB = 0x00;
	HD_LSB = 0x00;
	
	hpi_hstrobe();
	
	SET(PORT, HHWIL);	// Andra delen.
	HD_MSB = 0x00;
	HD_LSB = 0x00;
	
	hpi_hstrobe();		// Skrivit in adress 80001000h.
	
	/* Skriver till HPIA-adressregister. */
	CLR(PORT, HCNTL1);
	SET(PORT, HCNTL0);
	CLR(PORT, HHWIL);	// F�rsta delen.
	HD_MSB = 0x80;
	HD_LSB = 0x00;

	hpi_hstrobe();
	
	SET(PORT, HHWIL);	// Andra delen.
	HD_MSB = 0x10;
	HD_LSB = 0x00;
		
	hpi_hstrobe();		// Skrivit in adress 80001000h.
	
	for ( uint8_t effect=0; effect < NUMBER_OF_EFFECTS; effect++) //Nollst�ll minnesrymd f�r HPI
	{
 		for(int32_t parameter=0; parameter < NUMBER_OF_PARAMETERS; parameter++){
			hpi_write(0,parameter,effect);	
		}
	}
	

}

void hpi_test(void)
{
		/* Skriver till HPID-dataregister. */
		SET(PORT, HCNTL1);
		SET(PORT, HCNTL0);
		CLR(PORT, HHWIL);	// F�rsta delen.
		HD_MSB = 0x12;
		HD_LSB = 0x34;

		hpi_hstrobe();
		
		SET(PORT, HHWIL);	// Andra delen.
		HD_MSB = 0x56;
		HD_LSB = 0x00;
		
		hpi_hstrobe();		// Skrivit in adress 80001000h.
		
		//for(;;);
}


void hpi_test2(void)
{	
		/* Skriver till HPIA-adressregister. */
		CLR(PORT, HCNTL1);
		SET(PORT, HCNTL0);
		CLR(PORT, HHWIL);	// F�rsta delen.
		HD_MSB = 0x80;
		HD_LSB = 0x00;
		
		hpi_hstrobe();
		
		SET(PORT, HHWIL);	// Andra delen.
		HD_MSB = 0x10;
		HD_LSB = 0x10;
		
		hpi_hstrobe();		// Skrivit in adress 80001000h.
		
		/* Skriver till HPID-dataregister med fast adress. */
		SET(PORT, HCNTL1);
		SET(PORT, HCNTL0);
		CLR(PORT, HHWIL);	// F�rsta delen.
		HD_MSB = 0xff;
		HD_LSB = 0xff;
		
		hpi_hstrobe();
		
		SET(PORT, HHWIL);	// Andra delen.
		HD_MSB = 0xff;
		HD_LSB = 0xff;
		
		hpi_hstrobe();		// Skrivit in adress 80001000h.
}

void hpi_write(int32_t parameter_val, uint8_t parameter_num, uint8_t effect )
{	
		/* Skriver till HPIA-adressregister. */
		CLR(PORT, HCNTL1);
		SET(PORT, HCNTL0);
		CLR(PORT, HHWIL);	// F�rsta delen.
		HD_MSB = 0x80;
		HD_LSB = 0x00;
		
		
		hpi_hstrobe();
		
		SET(PORT, HHWIL);	// Andra delen.
		// adress 0x80001XYY
		// X=nummer p� parameter (0-F)
		// YY=minnesarea f�r parametrar om totalt 16*16bytes. Varje parameter har en uppl�sning p� 32bitar
		// vilket ger max 64 parametrar innan man skriver �ver tidigare v�rden.
		HD_MSB = 0x10 + effect;
		HD_LSB = 0x00 + (parameter_num<<2); //multiplicera med 4
		
		
		hpi_hstrobe();		// Skrivit in adress 80001XYYh.
		
		/* Skriver till HPID-dataregister med fast adress. */
		SET(PORT, HCNTL1);
		SET(PORT, HCNTL0);
		CLR(PORT, HHWIL);	// F�rsta delen.
		
		HD_MSB = (parameter_val & 0xff000000UL) >> 24; //Maska ut de tv� mest signifikanta byten
		HD_LSB = (parameter_val & 0x00ff0000UL) >> 16;

		hpi_hstrobe();
		
		SET(PORT, HHWIL);	// Andra delen.	
		HD_MSB = (parameter_val & 0x0000ff00UL) >>  8;
		HD_LSB = (parameter_val & 0x000000ffUL)      ;
		
		hpi_hstrobe();		// Skrivit in data till adress 0x80001XYY.
		CLR(PORT,PARAM_INT1);
		SET(PORT,PARAM_INT1);		
}