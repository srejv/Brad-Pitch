/*
 * parameters.h
 *
 * Created: 3/22/2013 12:13:56 PM
 *  Author: Martin Andersson
 */ 

/* Returnerar �nskat parameterv�rde. */
int parameters_get(uint8_t n, uint8_t m);

/* L�gger in ett nytt parameterv�rde. */
void parameters_store(uint8_t n, uint8_t m, int parameter);

/* Sparar alla parametrar till eeprom. */
void parameters_save(void);

/* L�ser in alla parametrar fr�n �nskad plats i eeporom */
void parameters_lod(uint8_t n);

static uint8_t param_scale[NUMBER_OF_EFFECTS][NUMBER_OF_PARAMETERS]={ //Skalning p� steg f�r LCD-display
	{0, 5, 1, 5, 1, 10, 5, 0},
	{0, 5, 20, 5, 5, 5, 1, 0 },
	{0, 5, 5, 1, 5, 0, 0, 0 },
	{0, 5, 5, 1, 5, 0, 0, 0 },
	{0, 5, 5, 1, 5, 5, 0, 0 },
	{0, 5, 1, 1, 5, 5, 0, 0 }, //Pitch shift
	{0, 5, 5, 5, 5, 10, 100, 1 },
	{0, 5, 5, 5, 5, 0, 0, 0 },
	{0, 5, 5, 50, 1, 0, 0, 0 }
};
	
static int32_t param_max[NUMBER_OF_EFFECTS][NUMBER_OF_PARAMETERS]={ //Maxv�rde f�r varje parameter (active hanteras i egen funktion d�rav v�rde 0
	{0, 100, 1000, 100, 4, 5000, 200, 0 },
	{0, 100, 1000, 100, 100, 100, 3, 0 },
	{0, 100, 500, 100, 100, 0, 0, 0 },
	{0, 50, 500, 100, 100, 0, 0, 0 },
	{0, 50, 500, 100, 100, 100, 0, 0 },
	{0, 100, 24, 100, 100, 100, 100, 0 },//Pitchshift
	{0, 100, 200, 200, 200, 800, 8000, 48 },
	{0, 100, 500, 100, 100, 0, 0, 0 },
	{0, 100, 100, 5000, 500, 0, 0, 0 },
};
	
static int32_t param_min[NUMBER_OF_EFFECTS][NUMBER_OF_PARAMETERS]={ //Minv�rde f�r varje parameter
	{0, 0, 0, -100, 0, 0, 0, 0 },
	{0, 0, 0, 0, 0, -100, 0, 0 },
	{0, 100, 0, 0, -100, 0, 0, 0 },
	{0, 50, 0, 0, -100, 0, 0, 0 },
	{0, 50, 0, 0, -100, 0, 0, 0 },
	{0, 0, -24, -100, -100, 0, 0, 0 },
	{0, 0, 0, 0, 0, 0, 0, 0 },
	{0, 0, 0, 0, -100, 0, 0, 0 },
	{0, 0, -100, 0, 0, 0, 0, 0 },
};

