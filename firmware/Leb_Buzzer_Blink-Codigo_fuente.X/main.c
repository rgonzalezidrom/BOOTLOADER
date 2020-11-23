/*
 * File:   main.c
 * Author: rafae
 *
 * Created on 8 de noviembre de 2020, 23:39
 */

// PIC18F4620 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1H
#pragma config OSC = HS         // Oscillator Selection bits (HS oscillator)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bits (Brown-out Reset enabled and controlled by software (SBOREN is enabled))
#pragma config BORV = 2         // Brown Out Reset Voltage bits ()

// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<4:0> pins are configured as digital I/O on Reset)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = OFF        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "DrvControl_Usart.h"
#include <stdio.h>
#define _XTAL_FREQ 16384000L

#define LED_A LATDbits.LATD5
#define LED_B LATDbits.LATD6
#define LED_C LATDbits.LATD7

#define BEGIN_LED_A TRISDbits.TRISD5 = 0
#define BEGIN_LED_B TRISDbits.TRISD6 = 0
#define BEGIN_LED_C TRISDbits.TRISD7 = 0

#define BUZZER LATAbits.LA4
#define BEGIN_BUZZER TRISAbits.TRISA4 = 0


volatile unsigned char flagLEd = 0;
volatile unsigned char contFlagBuzzer = 0;
#define CONT_FLAG_BUZZER 4

void __interrupt(high_priority) isrh(void)
{
	
	if (TMR0IF)
	{
		flagLEd=1;
        contFlagBuzzer++;
        TMR0H = 0XF0;
        TMR0L = 0XBD;
        INTCONbits.TMR0IF = 0;
	}

}

void main(void) {
    
    BEGIN_LED_A;
    LED_A = 0;
    BEGIN_LED_B;
    LED_B = 1;
    BEGIN_LED_C;
    LED_C = 0;
    
    BEGIN_BUZZER;
    BUZZER = 0;
    
    OSCCONbits.SCS = 0b00;//0b01110011;
    
    //TIMER
    T0CON = 0b10010111;
    
    RCONbits.IPEN = 1;
    INTCONbits.GIE=1;
    INTCONbits.PEIE = 1;
    INTCON2bits.TMR0IP = 1;
    INTCONbits.TMR0IE =  1;
    INTCONbits.TMR0IF =  0;
    
    TMR0H = 0XF0;
    TMR0L = 0XBD;
        
    Usart_Initialize();
    Usart_TxEnabled(1);
    Usart_SendString("Bootloader - V2.1 - ATENEA IDROM-LT ... \n\r");
    
    
    while(1){
        //char string[20];
        //sprintf(string,"flag : %d \n",flagLEd);
        //Usart_SendString(string);
        if(flagLEd){
            flagLEd=0;
            LED_A = ~LED_A;
            LED_B = ~LED_B;
            LED_C = ~LED_C;
            if(contFlagBuzzer>=CONT_FLAG_BUZZER){
                BUZZER = 1;
                __delay_ms(50);
                BUZZER = 0;
                __delay_ms(50);
                contFlagBuzzer=0;
            }
            
        }
    }
}
