/*
 * encoder.h
 *
 *  Author: Martin Andersson
 */

/* Typer */
struct encoder_output {
	uint8_t number;
	uint8_t direction;
};

void encoder_init(void);

void encoder_debounce(void);

/* Läser in en pulsgivares värde och anger riktning. */
uint8_t encoder_direction(uint8_t n, unsigned int state);

/* Vilken pulsgivare har ändrats? */
struct encoder_output encoder_change(uint8_t start, uint8_t stop);

/* Vilken pulsgivare har ändrats? */
struct encoder_output encoder_change2(void);