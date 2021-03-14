#include <stdint.h>
#include <pic16f887.h>

typedef enum { I2C_MASTER_MODE, I2C_SLAVE_MODE} I2CMode;
typedef enum { ACK, NACK } AcknowledgmentMode;

void I2C_Init(I2CMode mode);
uint8_t I2C_Start(unsigned char address);
void I2C_Wait(void);
uint8_t I2C_Write(unsigned char data);
uint8_t I2C_Stop (void);
unsigned char I2C_Read (AcknowledgmentMode mode);
void I2C_Ack (void);
void I2C_Nack (void);

void I2C_Init(I2CMode mode) {
    TRISCbits.TRISC3 = 1;   
    TRISCbits.TRISC4 = 1;
    SSPCONbits.SSPEN = 1;
    SSPSTAT = 0;
    SSPSTATbits.SMP = 1;
    
    if(mode == I2C_MASTER_MODE) {
        SSPCONbits.SSPM = 0x8;
    } else {
        SSPCONbits.SSPM = 0x4;
    }
    
    SSPCON2 = 0;
    SSPADD = 0x9;
    SSPIF=0;
    SSPIE=1;
}

uint8_t I2C_Start(unsigned char address) {
    SSPCON2bits.SEN = 1;    
    while (SSPCON2bits.SEN);
    SSPIF = 0;
    
    if(!SSPSTATbits.S)
        return 1;
    
    return I2C_Write(address);
}

uint8_t I2C_Write(unsigned char data) {
    SSPBUF = data;
    I2C_Wait();
    
    if(ACKSTAT) 
        return 1;
    else 
        return 0;
}

void I2C_Wait(void) {
    while (!SSPIF);
    SSPIF = 0;
}

uint8_t I2C_Stop (void) {
    SSPCON2bits.PEN = 1;    
    while(SSPCON2bits.PEN);
    SSPIF = 0;
    if (SSPSTATbits.P)
        return 1;
    else
        return 0;
}

unsigned char I2C_Read (AcknowledgmentMode mode) {
    unsigned char buffer = 0;
    SSPCON2bits.RCEN = 1;
    
    while (!SSPSTATbits.BF);
    buffer = SSPBUF;
    
    if(mode == ACK) {
        I2C_Ack();
    } else {
        I2C_Nack();
    }
    
    return buffer;
}

void I2C_Ack (void) {
    SSPCON2bits.ACKDT = 0;
    SSPCON2bits.ACKEN = 1;
    while(SSPCON2bits.ACKEN);
}

void I2C_Nack (void) {
    SSPCON2bits.ACKDT = 1;
    SSPCON2bits.ACKEN = 1;
    while(SSPCON2bits.ACKEN);
}