
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = XT        // Oscillator Selection bits (XT oscillator: Crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = ON        // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO = ON        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = ON         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <pic16f887.h>
#include "MPU6050.h"

#define _XTAL_FREQ 4000000

double accelX = 0;    
    double accelY = 0;
    double accelZ = 0;
    double temp = 0;
    double gyroX = 0;
    double gyroY = 0;
    double gyroZ = 0;

void main(void) {
    
    
    I2C_Init(I2C_MASTER_MODE);
    MPU6050_Init();
    
    while (1) { 
        temp = MPU6050_GetTemp();
        gyroZ = MPU6050_GetGyroZ();
        gyroY = MPU6050_GetGyroY();
        gyroX = MPU6050_GetGyroX();
        accelZ = MPU6050_GetAccelZ();
        accelY = MPU6050_GetAccelY();
        accelX = MPU6050_GetAccelX();
        
        __delay_ms(500);
    }
}