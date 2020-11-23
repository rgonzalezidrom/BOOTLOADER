//
// Atenea series mainboard software V3.1
// Creado por: Rafael Gonzalez
// Compilador: XC8
// Fecha de Actualizacion: Noviembre 2020
//

#ifndef _USART_H
#define _USART_H

//#include <system.h>
//#pragma CLOCK_FREQ 16384000
#define _XTAL_FREQ 16384000UL

	unsigned char Usart_OnByteReception(void);

	void Usart_Initialize(void);
	void Usart_RxEnabled(unsigned char action);
	void Usart_TxEnabled(unsigned char action);
	// void Usart_RxEnabled(bool action);
	// void Usart_TxEnabled(bool action);
	void Usart_ResetReception(void);
	void Usart_SendByte(unsigned char c);
	void Usart_Putc(unsigned char c);
	void Usart_Putcr(void);
	void Usart_SendString(const char *strc);


#endif //_USART_H
