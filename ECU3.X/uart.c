#include <xc.h>
#include "uart.h"

void init_uart(void)
{
	/* Serial initialization */
	RX_PIN = 1;
	TX_PIN = 0;

	/* TXSTA:- Transmitor Status and control Register */
	/* 9bit TX enable or disable bit */ 
	TXSTAbits.TX9 = 0;
	/* UART Tarsmition enable bit */
	TXSTAbits.TXEN = 1;
	/* Synchronous or Asynchronous mode selection */
	/* Asynchronous */
	TXSTAbits.SYNC = 0;
	/* Low or High baud rate selection bit */
	/* High Baud Rate */
	TXSTAbits.BRGH = 1;

	/* RCSTA :- Recepition Status and control Register */
	/* TX/RC7 and RX/RC6 act as serial port */ 
	RCSTAbits.SPEN = 1;
	/* 9bit RX enable or disable bit */
	RCSTAbits.RX9 = 0;
	/* Continous reception enable or disable */ 
	RCSTAbits.CREN = 1;

	/* BAUDCTL:- Baud rate control register */

	/* Baud Rate Setting Register */
	/* Set to 10 for 115200, 64 for 19200 and 129 for 9600 */
	SPBRG = 129;


	/* TX interrupt flag bit */
	TXIF = 0;

	/* RX interrupt enable bit */
	RCIF = 0;
}

void putch(unsigned char byte)
{
	/* Output one byte */
	/* Set when register is empty */
	while(!TXIF)
	{
		continue;
	}
	TXIF = 0;
	TXREG = byte;
}

int puts(const char *s)
{
	while(*s)
	{
		putch(*s++);
	}
	return 0;
}

unsigned char getch(void)
{
	/* Retrieve one byte */
	/* Set when register is not empty */
	while(!RCIF)
	{
		continue;
	}
	RCIF = 0;
	return RCREG;
}

unsigned char getche(void)
{
	unsigned char c;

	putch(c = getch());

	return (c);
}