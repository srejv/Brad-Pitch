
/*
 * lcd_test.h
 *
 *  Author: Martin Andersson
 */ 

/* Skickar en bokstav till skärmen. */
int	lcd_putchar(char c, FILE *stream);

/* Ändrar rad för utskrift, n = {0, 1, 2, 3}. */
void lcd_row(uint8_t n);

/* Rensar skärmen */
void lcd_clear(void);