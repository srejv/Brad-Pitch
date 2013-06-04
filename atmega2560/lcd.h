
/*
 * lcd_test.h
 *
 *  Author: Martin Andersson
 */ 

/* Skickar en bokstav till sk�rmen. */
int	lcd_putchar(char c, FILE *stream);

/* �ndrar rad f�r utskrift, n = {0, 1, 2, 3}. */
void lcd_row(uint8_t n);

/* Rensar sk�rmen */
void lcd_clear(void);