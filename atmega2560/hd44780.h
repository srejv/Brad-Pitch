/*
 * Send byte b to the LCD.  HD44780_RS is the RS signal (register select), 0
 * selects instruction register, 1 selects the data register.
 */
void	hd44780_outbyte(uint8_t b, uint8_t RS);

/*
 * Read one byte from the LCD controller.  HD44780_RS is the RS signal, 0
 * selects busy flag (bit 7) and address counter, 1 selects the data
 * register.
 */
uint8_t	hd44780_inbyte(uint8_t RS);

/*
 * Wait for the busy flag to clear.
 */
void	hd44780_wait_ready(void);

/*
 * Initialize the LCD controller hardware.
 */
void	hd44780_init(void);

/*
 * Prepare the LCD controller pins for powerdown.
 */
void	hd44780_powerdown(void);


/* Send a command to the LCD controller. */
#define hd44780_outcmd(n)	hd44780_outbyte((n), 0)

/* Send a data byte to the LCD controller. */
#define hd44780_outdata(n)	hd44780_outbyte((n), 1)

/* Read the address counter and busy flag from the LCD. */
#define hd44780_incmd()		hd44780_inbyte(0)

/* Read the current data byte from the LCD. */
#define hd44780_indata()	hd44780_inbyte(1)

/* Instruktioner */
#define HD44780_FunctionSet		0b00111000	/* 8-bitar databuss, 16:1 mux */
#define HD44780_ClearDisplay	0b00000001	/* Rensar skärm. */
#define HD44780_EnterMode		0b00000110	/* Stegvis öknin, ingen "shift". */
#define HD44780_DisplayOnOff	0b00001100	/* Aktiverar skärm utan makör och blikning. */
#define HD44780_AddressRow0		0x80		/* 8-bit DDRAM-adress för första bokstaven på rad 0. */
#define HD44780_AddressRow1		0xC0		/* rad 1. */
#define HD44780_AddressRow2		0x94		/* rad 2. */
#define HD44780_AddressRow3		0xD4		/* rad 3. */