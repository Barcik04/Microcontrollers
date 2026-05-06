/*
 * File:   main.c
 * Author: local
 *
 * Created on April 29, 2026, 10:03 AM
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
#include "lcd.h"
#include "string.h"
#include "stdio.h"
#include "libpic30.h"

#define CZAS_START 3

int czasGracz1 = CZAS_START;
int czasGracz2 = CZAS_START;

int aktywnyGracz = 0;

void system_init(void) {
    AD1PCFG = 0xFFFF;

    TRISDbits.TRISD6 = 1;     // RD6 jako wejście - gracz 1
    TRISDbits.TRISD13 = 1;    // RD13 jako wejście - gracz 2

    LCD_Initialize();
}

void pokazCzasy(void) {
    char linia1[17];
    char linia2[17];

    int min1 = czasGracz1 / 60;
    int sek1 = czasGracz1 % 60;

    int min2 = czasGracz2 / 60;
    int sek2 = czasGracz2 % 60;

    if(aktywnyGracz == 1) {
        sprintf(linia1, "G1 %02d:%02d *", min1, sek1);
    } else {
        sprintf(linia1, "G1 %02d:%02d  ", min1, sek1);
    }

    if(aktywnyGracz == 2) {
        sprintf(linia2, "G2 %02d:%02d *", min2, sek2);
    } else {
        sprintf(linia2, "G2 %02d:%02d  ", min2, sek2);
    }

    LCD_ClearScreen();
    LCD_PutString(linia1, strlen(linia1));
    LCD_PutChar('\n');
    LCD_PutString(linia2, strlen(linia2));
}

void pokazPrzegrana(int gracz) {
    LCD_ClearScreen();

    if(gracz == 1) {
        LCD_PutString("GRACZ 1", 7);
        LCD_PutChar('\n');
        LCD_PutString("PRZEGRAL", 8);
    } else {
        LCD_PutString("GRACZ 2", 7);
        LCD_PutChar('\n');
        LCD_PutString("PRZEGRAL", 8);
    }

    while(1) {

    }
}

int main(void) {
    system_init();

    int licznikTickow = 0;

    pokazCzasy();

    while(1) {

        if(PORTDbits.RD6 == 0) {
            aktywnyGracz = 2;
            pokazCzasy();

            __delay32(200000);         
            while(PORTDbits.RD6 == 0);  
        }

        if(PORTDbits.RD13 == 0) {
            aktywnyGracz = 1;
            pokazCzasy();

            __delay32(200000);           
            while(PORTDbits.RD13 == 0);
        }


        __delay32(533333);

        if(aktywnyGracz != 0) {
            licznikTickow++;
        }

        if(licznikTickow >= 10) {
            licznikTickow = 0;

            if(aktywnyGracz == 1) {
                czasGracz1--;

                if(czasGracz1 <= 0) {
                    czasGracz1 = 0;
                    pokazCzasy();
                    pokazPrzegrana(1);
                }

                pokazCzasy();
            }

            if(aktywnyGracz == 2) {
                czasGracz2--;

                if(czasGracz2 <= 0) {
                    czasGracz2 = 0;
                    pokazCzasy();
                    pokazPrzegrana(2);
                }

                pokazCzasy();
            }
        }
    }

    return 0;
}
