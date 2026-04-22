/*
 * File:   main.c
 * Author: local
 *
 * Created on April 22, 2026, 10:05 AM
 */


// PIC24FJ128GA010 Configuration Bit Settings

// 'C' source line config statements

// CONFIG2
#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF             // JTAG Port Enable (JTAG port is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include "xc.h"
#include "libpic30.h"
#include "stdio.h"
#include "adc.h"

void init(void){
    ADC_SetConfiguration(ADC_CONFIGURATION_DEFAULT);
    ADC_ChannelEnable(ADC_CHANNEL_POTENTIOMETER); //konfig potencjometru
    TRISA = 0x0000; // port A na wyj?cie - diody
    TRISD |= (1<<6);   // RD6 przycisk na wej?cie
    TRISD |= (1<<13);  // RD13 przycisk na wej?cie
}


int obslugaPrzyciskow(int value) {
    static char prev6 = 1;
    static char prev13 = 1;

    char current6 = PORTDbits.RD6;
    char current13 = PORTDbits.RD13;

    if(prev6 == 1 && current6 == 0) {
        __delay32(100000);
        if(PORTDbits.RD6 == 0) {
            value++;
            if(value > 2) {
                value = 1;
            }
        }
    }

    if(prev13 == 1 && current13 == 0) {
        __delay32(100000);
        if(PORTDbits.RD13 == 0) {
            value--;
            if(value < 1) {
                value = 2;
            }
        }
    }

    prev6 = current6;
    prev13 = current13;

    return value;
}


int main(void){
    
    init();
    
    unsigned long adc_value;
    int value = 1;
    unsigned long opoznienie = 40000;
    
    unsigned int snake;

    int count = 0;
    int max = 128;
    int prev_val = 0;
    
    while(1){

        value = obslugaPrzyciskow(value);

        if (value == 1){
            for(int i = 0; i <= 5; i++) {
                snake = 0x0007 << i;
                LATA = snake;

                adc_value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
                if (adc_value != 0xFFFF){
                    opoznienie = 40000 + (adc_value * 4000);
                }

                __delay32(opoznienie);

                int newValue = obslugaPrzyciskow(value);
                if(newValue != value) {
                    value = newValue;
                    break;
                }
            }

            if(value == 1) {
                for(int i = 4; i >= 1; i--) {
                    snake = 0x0007 << i;
                    LATA = snake;

                    adc_value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
                    if (adc_value != 0xFFFF){
                        opoznienie = 40000 + (adc_value * 4000);
                    }

                    __delay32(opoznienie);

                    int newValue = obslugaPrzyciskow(value);
                    if(newValue != value) {
                        value = newValue;
                        break;
                    }
                }
            }
        }else {
            adc_value = ADC_Read10bit(ADC_CHANNEL_POTENTIOMETER);
            if (adc_value != 0xFFFF){
                opoznienie = 40000 + (adc_value * 4000);
            }

            if (prev_val == 0) {
                prev_val = 1;
                LATA = count + prev_val;
            } else if (prev_val != max) {
                LATA = count + (prev_val * 2);
                prev_val = prev_val * 2;
            } else {
                max = max / 2;
                count += prev_val;
                LATA = count;
                prev_val = 0;
            }

            if (count >= 255) {
                count = 0;
                max = 128;
                prev_val = 0;
            }

            __delay32(opoznienie);
        }
    }
}
