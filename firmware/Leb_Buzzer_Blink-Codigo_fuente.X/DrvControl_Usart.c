//
// Atenea series mainboard software V3.1
// Creado por: Rafael Gonzalez
// Compilador: XC8
// Fecha de Actualizacion: Noviembre 2020
//

//#include <system.h>
#include <xc.h>
//#pragma CLOCK_FREQ 16384000
#define _XTAL_FREQ 16384000UL

#include "DrvControl_Usart.h"
long baudios = 19200;

void Usart_Initialize(void)
{
	//set_bit(trisc, 7);									// Pin configuration.
	//set_bit(trisc, 6);
	TRISCbits.TRISC7 = 1;									// Pin configuration.
	TRISCbits.TRISC6 = 1;
    //spbrg = 0x6B;										// 9600 bauds at 16.384 Mhz clock.
	//spbrg = 0x34;										// 19200 bauds at 16.384 Mhz clock.
	SPBRG = 0x34;										// 19200 bauds at 16.384 Mhz clock.
	//spbrg = 0x1A;										// 38400 bauds at 16.384 Mhz clock.
    
    //SPBRG = 51;
	
	
    //txsta = 0x06;										// Transmision & reception disabled. 
	//rcsta  = 0x80;
	//set_bit(intcon, GIEL);
	TXSTA = 0x06;										// Transmision & reception disabled. 
	RCSTA  = 0x80;
	INTCONbits.GIEL = 1;								// Global enable of low priority interrupts.
}

//void Usart_RxEnabled(bool action)
void Usart_RxEnabled(unsigned char action)
{
	//if (action) set_bit(rcsta, CREN);
	//else clear_bit(rcsta, CREN);
    if (action) RCSTAbits.CREN = 1;
	else RCSTAbits.CREN = 0;
}

//void Usart_TxEnabled(bool action)
void Usart_TxEnabled(unsigned char action)
{
	//if (action)	set_bit(txsta, TXEN);
	//else clear_bit(txsta, TXEN);
    if (action)	TXSTAbits.TXEN = 1;
	else TXSTAbits.TXEN = 0;
}

unsigned char Usart_OnByteReception(void)
{
	// Clear reception interrupt flag. Then get character from register
	// and place it in reception buffer.
	//clear_bit(pir1, RCIF);
	//return rcreg;
    PIR1bits.RCIF = 0;
	//return rcreg;
    return RCREG;
}

void Usart_SendByte(unsigned char c)
{
	// Wait until transmision register is empty. Then place character to
	// be sent in transmision register.
	//while (!test_bit(txsta, TRMT)) continue;
    while (TXSTAbits.TRMT == 0) continue;
	//txreg = c;
	TXREG = c;
}

void Usart_Putc(unsigned char c) 
{
	// Wait until transmision register is empty. Then place character to
	// be sent in transmision register.
	//while (!test_bit(txsta, TRMT)) continue;
    while (TXSTAbits.TRMT == 0) continue;
	//txreg = c;
	TXREG = c;
}

void Usart_SendString(const char *strc) 
{
	// Send a char until find 0(null) char.
	while (*strc != 0) Usart_Putc(*strc++);
}

void Usart_Putcr(void) 
{
	// Send 0x0D(carriage return).
	Usart_Putc (0x0D);
}