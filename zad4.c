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
#include "buttons.h"
#include "lcd.h"
#include "string.h"

void system_init(void) {
    LCD_Initialize();
}


void pokazReklame(void) {
    char tekst[] = "                \xF6\xFF WESOLEGO DNIA MIESZKANCY OLSZTYNA HOHO!!! \xFF\xF6               ";
    char ekran[17];

    int dlugosc = strlen(tekst);
    int i;
    int j;

    while(1) {
        for(i = 0; i <= dlugosc - 16; i++) {

            for(j = 0; j < 16; j++) {
                ekran[j] = tekst[i + j];
            }

            ekran[16] = '\0';

            LCD_ClearScreen();
            
            LCD_PutString(" ", 1);
            LCD_PutChar((char)246);
            LCD_PutChar((char)247);
            LCD_PutString(" ZYCZENIA ", 10);
            LCD_PutChar((char)247);
            LCD_PutChar((char)246);

            LCD_PutChar('\n');

            LCD_PutString(ekran, 16);

            __delay32(1500000);
        }
    }
}


int main(void) {
    system_init();

    pokazReklame();

    while(1) {

    }

    return 0;
}
