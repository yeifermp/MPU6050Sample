#include <string.h>
#include <xc.h>
#include <pic16f887.h>

#define DATA PORTD
#define DATA_DIR TRISD

#define E PORTEbits.RE2
#define RS PORTEbits.RE0
#define RW PORTEbits.RE1
#define PBS PORTAbits.RA5
#define RST PORTAbits.RA4
#define GLCD_FIRST_LINE 0x80
#define GLCD_SECOND_LINE 0xB0
#define GLCD_THIRD_LINE 0x98
#define GLCD_FOURTH_LINE 0xA8

#define _XTAL_FREQ 4000000

void GLCD_Init (void);
void GLCD_SendCommand (char cmd);
void GLCD_SendData (char data);
void GLCD_SendBuffer (char * buffer);
void GLCD_FourthLine (void);
void GLCD_ThirdLine (void);
void GLCD_SecondLine (void);
void GLCD_FirstLine (void);
void GLCD_SetDDRAM (uint8_t address);
void GLCD_Clear (void);
uint8_t GLCD_ReadBusyFlag(void);
uint8_t GLCD_ReadDDRAM (void);
void GLCD_ExtendedFunc (void);
void GLCD_SetGDRAM (uint8_t data);

void GLCD_Init(void) {
    ANSEL = ANSELH = 0;
    TRISD = 0x0;
    TRISE = 0x0;
    TRISA = 0x0;
    
    PBS = 1;
    RST = 1;
    __delay_ms(500);
    GLCD_SendCommand(0x30);
    GLCD_SendCommand(0x30);        
    GLCD_SendCommand(0xf);
    GLCD_SendCommand(0x1);
    GLCD_SendCommand(0x10);
    //GLCD_ExtendedFunc();
    //GLCD_SendCommand(0x6);
    //GLCD_SendCommand(0x3);
}

void GLCD_SendCommand (char cmd) {
    DATA_DIR = 0x0;
    RS = 0;
    RW = 0;
    DATA = cmd;
    E = 1;
    __delay_ms(10);
    E = 0;
}

void GLCD_SendData (char data) {
    DATA_DIR = 0x0;
    RS = 1;
    RW = 0;
    DATA = data;
    E = 1;
    __delay_ms(10);
    E = 0;
}

void GLCD_SendBuffer (char * buffer) {
    for(uint8_t index = 0; index < strlen(buffer); index++) {
        GLCD_SendData(buffer[index]);
    }
}

void GLCD_SetDDRAM (uint8_t address) {
    GLCD_SendCommand(address);
}

void GLCD_FirstLine (void) {
    GLCD_SetDDRAM(GLCD_FIRST_LINE);
}

void GLCD_SecondLine (void) {
    GLCD_SetDDRAM(GLCD_SECOND_LINE);
} 

void GLCD_ThirdLine (void) {
    GLCD_SetDDRAM(GLCD_THIRD_LINE);
} 

void GLCD_FourthLine (void) {
    GLCD_SetDDRAM(0x0);
} 

void GLCD_Clear (void) {
    GLCD_SendCommand(0x1);
}

uint8_t GLCD_ReadBusyFlag (void) {
    uint8_t value = 0x0;
    DATA = 0x0;
    DATA_DIR = 0xff;
    RW = 1;
    RS = 0;
    E = 1;
    value = DATA;
    __delay_us(10);
    E = 0;
    
    return value;
}

uint8_t GLCD_ReadDDRAM (void) {
    uint8_t ddram_data = 0x0;
    DATA = 0x0;
    DATA_DIR = 0xff;
    RW = 1;
    RS = 1;
    E = 1;
    __delay_us(80);
    ddram_data = DATA;
    E = 0;    
    return ddram_data;
}

void GLCD_ExtendedFunc (void) {
    GLCD_SendCommand(0x36);
}

void GLCD_SetGDRAM (uint8_t data) {
    uint8_t cmd = 0x80 | data;
    GLCD_SendCommand(cmd);
}