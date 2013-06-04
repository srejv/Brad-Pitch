/*
 * defines.h
 *
 * Created: 3/2/2013 2:29:51 PM
 */

/* Antal effekter och parametrar. */

#define NUMBER_OF_EFFECTS 9
#define NUMBER_OF_PARAMETERS 8

#define LCD_FIRST_ROW 0
#define LCD_SECOND_ROW 1
#define LCD_THIRD_ROW 2
#define LCD_FOURTH_ROW 3

/* Alusning */
#define debug			/* Används till #ifdef-makro */
/* #define debug_extreme */	/* -"- som kan påverka känsliga tider. */

/* Klockfrekvens */
#define	F_CPU 1000000UL		/* Ordinarie frekvens för intern RC-oscillator */

/* HD44780 LCD-modul koppling */
#define HD44780_RS		D, 5	/* LCD(1) */
#define HD44780_RW		D, 6	/* LCD(2) */
#define HD44780_E		D, 7	/* LCD(3) */
#define HD44780_DB_OUT	PORTC	/* LCD(4:11) */
#define HD44780_DB_IN	PINC	/* LCD(4:11) */
#define HD44780_DB_DDR	DDRC	/* datariktningsregister. */

// Interrupt för prameteruppdatering

#define PARAM_INT1	H, 1 //AUX4 H ,0 kass!

/* Lysdiod */
/* #define LED	B, 3 */

/* Inställningar för pulsgivare */
/* #define encoder_signals (PINC | ((PIND & 0b01111100) << 6)) */

/* encoder(0:7) är kopplat till PK(0:7).
 * encoder(8:12) är kopplat till PJ(0:4) */
#define encoder_signals (PINK | ((PINJ & 0b00011111) << 8) )	/* pulsgivarnas koppling. */
#define enc0	0			/* PK0, encoder(0), S1-a  */
#define enc1	1			/* PK1, encoder(1), S1-b */
#define enc2	2			/* PK2, encoder(2), S2-a */
#define enc3	3			/* PK3, encoder(3), S2-b */
#define enc4	4			/* PK4, encoder(4), S3-a */
#define enc5	5			/* PK5, encoder(5), S3-b */
#define enc6	6			/* PK6, encoder(6), S4-a */
#define enc7	7			/* PK7, encoder(7), S4-b */
#define enc8	1			/* PJ0, encoder(8), S5-a */
#define enc9	2			/* PJ1, encoder(9), S5-b */
#define enc10	3			/* PJ2, encoder(10), S6-a */
#define enc11	4			/* PJ3, encoder(11), S6-ab */
#define enc12	5			/* PJ4, encoder(12), gemensamm signal från tryckknappar. */
#define bounce_time	100		/* [µs] Kontaktstudstid */
							/* Riktning från pulsgivare */
#define NC	0				/* Ingen ändring. */
#define CW	1				/* Medurs. */
#define CCW	2				/* Moturs. */

/* HPI */
#define HD_LSB		PORTA
#define HD_LSB_DDR	DDRA
#define HD_MSB		PORTF
#define HD_MSB_DDR	DDRF
#define HDS2		L, 0
#define HAS			D, 0		/* L, 1 Sönder? */
#define HDS1		L, 2
#define HCNTL0		L, 3
#define HCS			L, 4
#define HHWIL		L, 5
#define HCNTL1		L, 6
#define HRW			L, 7


/* ¨Makron för att ställa bitar */
#define _BV(bit) (1 << (bit)) 
#define GLUE(a, b)     a##b
#define SET_(what, p, m) GLUE(what, p) |= (1 << (m))
#define CLR_(what, p, m) GLUE(what, p) &= ~(1 << (m))
#define GET_(/* PIN, */ p, m) GLUE(PIN, p) & (1 << (m))
#define SET(what, x) SET_(what, x)
#define CLR(what, x) CLR_(what, x)
#define GET(/* PIN, */ x) GET_(x)