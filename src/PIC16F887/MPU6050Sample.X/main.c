
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

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "i2c.h"
#include <pic16f887.h>

#define _XTAL_FREQ 4000000
#define MPU6050_ADDRESS 0xD2
#define WHO_I_AM_REG 0x75
#define SMPLRT_DIV 0x19
#define PWR_MGMT_1 0x6B
#define CONFIG 0x1A
#define ACCEL_CONFIG 0x1C
#define GYRO_CONFIG 0x1B
#define INT_ENABLE 0x38
#define ACCEL_XOUT_H 0x3B

unsigned char data = 0;
uint8_t error = 0;

void MPU6050_SetRegister(uint8_t reg, uint8_t value);
void MPU6050_Init(void);
unsigned char MPU6050_ReadRegister(uint8_t reg);

void main(void) {
    __delay_ms(1000);
    I2C_Init(I2C_MASTER_MODE);
    MPU6050_Init();
    
    while (1) {
        error = I2C_Start(MPU6050_ADDRESS);
        I2C_Write(WHO_I_AM_REG);
        I2C_Stop();
        
        __delay_ms(10);
        error = I2C_Start(MPU6050_ADDRESS);
        
        if (error != 0) {
            NOP();
        }
        
        data = I2C_Read(ACK);
        I2C_Stop();
        
        __delay_ms(1000);
        data = MPU6050_ReadRegister(GYRO_CONFIG);
        __delay_ms(500);
    }
}

void MPU6050_Init(void) {
    MPU6050_SetRegister(SMPLRT_DIV, 0x07);
    MPU6050_SetRegister(PWR_MGMT_1, 0x01);
    MPU6050_SetRegister(CONFIG, 0x00);
    MPU6050_SetRegister(ACCEL_CONFIG, 0x00);
    MPU6050_SetRegister(GYRO_CONFIG, 0x18);
    MPU6050_SetRegister(INT_ENABLE, 0x01);
}

void MPU6050_SetRegister(uint8_t reg, uint8_t value) {
    error = I2C_Start(MPU6050_ADDRESS);
    I2C_Write(reg);
    I2C_Write(value);
    I2C_Stop();
}

unsigned char MPU6050_ReadRegister(uint8_t reg) {
    unsigned char result = 0;
    error = 0;
    error = I2C_Start(MPU6050_ADDRESS);
    I2C_Write(reg);
    I2C_Stop();
    
    error = I2C_Start(MPU6050_ADDRESS);
    result = I2C_Read(ACK);
    I2C_Stop();
    return result;
}