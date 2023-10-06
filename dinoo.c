// PIC16F877A Configuration Bit Settings
// 'C' source line config statements
// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB3/PGM pin has digital I/O function; low-voltage programming disabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#include <htc.h>
#define RS RC0
#define RW RC1
#define EN RC2
#define SW RC3
#define _XTAL_FREQ 20000000

void fun();
void score();
int jump();
void stop();
int i = 0, k = 0, c = 0;

__CONFIG(0X1F7A);

void cmd(unsigned char command) {
    RS = 0;
    RW = 0;
    EN = 1;
    PORTD = command;
    __delay_ms(20);
    EN = 0;
}

void data(unsigned char data1) {
    RS = 1;
    RW = 0;
    EN = 1;
    PORTD = data1;
    __delay_ms(20);
    EN = 0;
}

void character(unsigned char *car, char location) {
    cmd(0x40 + location * 8);
    __delay_ms(20);
    for (int i = 0; i < 8; i++)
        data(car[i]);
    __delay_ms(20);
}

int main() {
    TRISC = 0;
    TRISD = 0;
    TRISC3 = 1;
    char arr[8] = {0X07, 0X05, 0X07, 0X16, 0X1F, 0X1E, 0X04, 0X06};
    character(&arr[0], 0);
    char arr1[8] ={0X07, 0X05, 0X07, 0X16, 0X1F, 0X1E, 0X04, 0X06};
    //char arr1[8] = {0x0E, 0x0E, 0x04, 0x1E, 0x04, 0x04, 0x1E, 0x00};
    character(&arr1[0], 1);
    char arr2[8] = {0X04, 0X05, 0X15, 0X15, 0X17, 0X1C, 0X04, 0X04};
    character(&arr2[0], 2);
    char arr3[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    character(&arr3[0], 3);
    cmd(0x01);
    cmd(0x0C);
    cmd(0x38);
    cmd(0xc0);
    data(0);
    cmd(0xcf);
    data(2);
    int i = 0;
    __delay_ms(200);
    while (1) {
        if (RC3 == 1) {
            jump();
        }
        score();
        data(3);
        cmd(0xcf - (unsigned char)c); // Cast c to unsigned char
        data(3);
        __delay_ms(100);
        data(3);
        cmd(0xcf - (unsigned char)c); // Cast c to unsigned char
        data(2);
        c++;
        if (c > 15 && RC3 == 0) {
            stop();
            cmd(0x01);
            while (1); // Remove the main() call
        }
        if (c > 15) {
            cmd(0xc0);
            data(3);
            c = 0;
            cmd(0xc0);
            data(0);
        }
        if (RC3 == 1)
            jump();
    }
}

void stop() {
    cmd(0x01);
    cmd(0x80);
    char *stop = "GAME OVER";
    for (int i = 0; i < 9; i++) {
        data(stop[i]);
        __delay_ms(300);
    }
    while (1);
}

void score() {
    char *num = "0123456789";
    char *name = "score : ";
    cmd(0x82);
    for (int j = 0; j < 7; j++)
        data(name[j]);
    cmd(0x8A);
    data(num[k]);
    cmd(0x8B);
    data(num[i]);
    __delay_ms(100);
    if (i > 10) {
        i = 0;
        k++;
        if (k == 9)
            k = 0;
    } else if (RC3 == 1)
        jump();
}

int jump() {
    cmd(0xc0);
    data(3);
    cmd(0x81);
    data(1);
    __delay_ms(200);
    cmd(0x81);
    data(3);
    cmd(0xc0);
    data(0);
    return (i++);
}


