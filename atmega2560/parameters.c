/*
 * parameters.c
 *
 * Created: 3/22/2013 12:13:38 PM
 *  Author: Martin Andersson.
 *  Vid tilläg av parametrar skall följande ändras HPI_scale (HPI.h), PAR_MAX, PAR_MIN samt antalet aparmetrar i test.c
 */

#include "defines.h"
#include <stdint.h>
#include "hpi.h"

static int parameters[NUMBER_OF_EFFECTS][NUMBER_OF_PARAMETERS];

/* Returnerar önskat parametervärde.
   n = effekt
   m = parameter */
int parameters_get(uint8_t n, uint8_t m)
{
	return parameters[n][m];
}

/* Lägger in ett nytt parametervärde. */
void parameters_store(uint8_t n, uint8_t m, int parameter)
{
	int32_t tmp;
	parameters[n][m] = parameter; //skriv till MC-minne
	// Skalning av parameter för att få högre upplösning till DSP
	tmp = (int32_t)(parameter*HPI_scale[n][m]);
	hpi_write(tmp, m,n); //skriv parameter till DSP med HPI
}

/* Sparar alla parametrar till eeprom. */
void parameters_save(void)
{
	;
}

/* Läser in alla parametrar från önskad plats i eeporom */
void parameters_lod(uint8_t n)
{
	;
}