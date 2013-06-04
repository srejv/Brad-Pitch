/*
 * hpi.h
 *
 * Created: 4/10/2013 2:46:18 PM
 *  Author: Administrator
 */ 

void hpi_init(void);
void hpi_test(void);
void hpi_write(int32_t parameter_val, uint8_t parameter_num, uint8_t effect);

static float HPI_scale[NUMBER_OF_EFFECTS][NUMBER_OF_PARAMETERS]={ //Minvärde för varje parameter
	{1 ,327.68f ,1 ,327.68f ,0 ,0 ,0 ,0},
	{1 ,327.68f ,327.68f ,327.68f ,327.68f ,327.68f ,1 ,0},
	{1 ,327.68f ,327.68f ,327.68f ,327.68f ,0 ,0 ,0},
	{1 ,327.68f ,327.68f ,327.68f ,327.68f ,0 ,0 ,0},
	{1 ,327.68f ,327.68f ,327.68f ,327.68f ,327.68f ,0 ,0},
	{1 ,327.68f ,1 , 1,327.68f ,327.68f ,0 ,0}, //Pitch shift
	{1 ,327.68f ,327.68f ,327.68f ,327.68f ,1 ,1 ,1000},
	{1 ,327.68f ,327.68f ,327.68f ,327.68f ,0 ,0 ,0},
	{1 ,327.68f ,327.68f ,327.68f ,327.68f ,0 ,0 ,0},
};