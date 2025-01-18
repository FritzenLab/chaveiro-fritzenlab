/*
 Based on this code: https://microcontroladores-c.blogspot.com/2014/09/timer0-com-pic12f675.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#define _XTAL_FREQ 4000000

#define LED GP5
#define Buzzer GP2
/////////////////////////////////////////////////////////configuraçôes//////////////////////////////////////////////////
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-Up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF         // Data Code Protection bit (Data memory code protection is enabled)


volatile uint8_t counter= 0;

void __interrupt() isr()//interrupt vector
{   
    counter++;
    if(counter == 8){
        counter= 0;
        LED = ~LED;
        Buzzer= ~Buzzer;    
        
    }
    
    TMR0IF = 0;//  clear timer0 interrupt flag
    TMR0 = 0;// zeroes timer 0 counting, so that it couts from 256 down to 0 again
}


//////////////////////////////////////////////////////Main routine///////////////////////////////////////////////////////////////
void main(void) {
    TRISIO = 0b000000;// all outputs
    CMCON = 7;// disable comparators
    ANSEL = 0;//no analog ports
    WPU = 0X00;// pull ups disabled
    TMR0 = 0;
    OSCCAL = 0b11111111;// internal oscillator to max frequency
    OPTION_REG = 0b10000111;
    INTCON = 0b11100000;
    
    
    Buzzer= 1;
    
    while(1)
    {
        
        
    }//infinite loop

}