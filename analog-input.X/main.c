/*
 Timer based on this code: https://microcontroladores-c.blogspot.com/2014/09/timer0-com-pic12f675.html
 * AD converter based on: https://forum.microchip.com/s/topic/a5C3l000000MRbeEAG/t336178
 * AD code also from here: https://saeedsolutions.blogspot.com/2012/07/pic12f675-adc-code-proteus-simulation.html
 */
#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <math.h>
#define _XTAL_FREQ 4000000

#define LED1 GP0
#define LED2 GP1
/////////////////////////////////////////////////////////configuraçôes//////////////////////////////////////////////////
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-Up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF         // Data Code Protection bit (Data memory code protection is enabled)


volatile uint8_t counter= 0;
volatile int reading= 0;
volatile unsigned int voltage= 0;
volatile uint8_t readAndDecide= 1;
volatile int tens= 0;
volatile int units= 0;
volatile uint8_t executeTens= 0;
volatile uint8_t executeUnits= 0;

void __interrupt() isr()//interrupt vector
{   
    counter++;
    if(counter == 4){
        counter= 0;
        
        if(readAndDecide == 1){
            readAndDecide= 0;
            executeTens= 1;
            executeUnits= 0;
            
            GO_nDONE= 1;        
            while(GO_nDONE);
            reading = ((ADRESH<<8)+ADRESL); 
            // 5V supply temperature equation= (-19.784*V)+75.304
            // 3V supply temperature equation= (-32.974*V)+75.304
            voltage = (-20*((reading * 5) / 1024))+75; // if you want to show voltage, substitute 5 by 50
            
            
            if(voltage >= 40){
                tens= 4;
                units= voltage - 40;
            }else if(voltage >= 30){
                tens= 3;
                units= voltage - 30;
            }else if(voltage >= 20){
                tens= 2;
                units= voltage - 20;
            }else if(voltage >= 10){
                tens= 1;
                units= voltage - 10;
            }else{
                tens= 0;
                units= voltage;
            }
            tens= tens * 2;
            units= units * 2;
        }else if(executeTens == 1){
            tens--;
            LED2= 0;
            if(LED1){
                LED1= 0;
            }else{
                LED1= 1;
            }
            if(tens < 1){
                executeTens= 0;
                executeUnits= 1;
            }
            
        }else if(executeUnits == 1){
            LED1= 0;
            units--;
            if(LED2){
                LED2= 0;
            }else{
                LED2= 1;
            }
            if(units < 1){
                executeTens= 0;
                executeUnits= 0;
                readAndDecide= 1;
            }
            
        }
        
        
        
        
    }
    
    TMR0IF = 0;//  clear timer0 interrupt flag
    TMR0 = 0;// zeroes timer 0 counting, so that it couts from 256 down to 0 again
}


//////////////////////////////////////////////////////Main routine///////////////////////////////////////////////////////////////
void main(void) {
    TRISIO = 0b010100;// GP2 and GP4 analog inputs
    CMCON = 7;// disable comparators
    ANSEL = 0b00011100;// AN2 and AN3 as analog inputs  
    ADCON0=0b10001001; // AN2, right justified, VDD as ref
    //ADCON0=0b10001101; // AN3, right justified, VDD as ref
    ADON=1; // Enable ADC
    WPU = 0X00;// pull ups disabled
    TMR0 = 0;
    OSCCAL = 0b11111111;// internal oscillator to max frequency
    OPTION_REG = 0b10000111;
    INTCON = 0b11100000;
    
    
    
   
    
    while(1)
    {
        
        
    }//infinite loop

}