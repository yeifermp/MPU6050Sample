#include <xc.h>
#include <string.h>
#include "i2c.h"

#define MPU6050_ADDRESS_R 0xD3
#define MPU6050_ADDRESS_W 0xD2
#define SMPLRT_DIV 0x19
#define PWR_MGMT_1 0x6B
#define CONFIG 0x1A
#define ACCEL_CONFIG 0x1C
#define GYRO_CONFIG 0x1B
#define INT_ENABLE 0x38
#define ACCEL_XOUT 0x3B
#define ACCEL_YOUT 0x3D
#define ACCEL_ZOUT 0x3F
#define TEMP_OUT 0x41
#define GYRO_XOUT 0x43
#define GYRO_YOUT 0x45
#define GYRO_ZOUT 0x47
#define SF_ACCE 16384.0
#define SF_GYRO 131.0

#define BUFFER_LENGTH 2
unsigned char buffer[BUFFER_LENGTH];

void MPU6050_SetRegister(uint8_t reg, uint8_t value);
void MPU6050_Init(void);
void MPU6050_ReadRegister(uint8_t reg, short num_bytes, unsigned char * buffer, uint8_t buffer_length);
double MPU6050_GetTemp();
double MPU6050_GetGyroZ();
double MPU6050_GetGyroY();
double MPU6050_GetGyroX();
double MPU6050_GetAccelZ();
double MPU6050_GetAccelY();
double MPU6050_GetAccelX();
double MPU6050_Get16BitRegister(unsigned char reg);

void MPU6050_Init(void) {
    MPU6050_SetRegister(SMPLRT_DIV, 0x07);
    MPU6050_SetRegister(PWR_MGMT_1, 0x01);
    MPU6050_SetRegister(CONFIG, 0x00);
    MPU6050_SetRegister(ACCEL_CONFIG, 0x00);
    MPU6050_SetRegister(GYRO_CONFIG, 0x18);
    MPU6050_SetRegister(INT_ENABLE, 0x01);
}

void MPU6050_SetRegister(uint8_t reg, uint8_t value) {
    AcknowledgmentMode ackMode = 0;
    
    ackMode = I2C_Start(MPU6050_ADDRESS_W);
    
    if (ackMode == ACK) {
        ackMode = I2C_Write(reg);
        
        if (ackMode == ACK) {
            ackMode = I2C_Write(value);
        }
    }
    
    I2C_Stop();
}

void MPU6050_ReadRegister(uint8_t reg, short num_bytes, unsigned char * buffer, uint8_t buffer_length) {
    memset(buffer, 0, buffer_length);
    AcknowledgmentMode ackMode = 0;
    
    ackMode = I2C_Start(MPU6050_ADDRESS_W);
        
    if (ackMode == ACK) {
        ackMode = I2C_Write(reg);
        
        if (ackMode == ACK){
            ackMode = I2C_RepeatedStart(MPU6050_ADDRESS_R);
            
            if (ackMode == ACK && num_bytes == 1) {
                buffer[0] = I2C_Read(NACK); 
            } else if (ackMode == ACK && num_bytes > 1) {
                uint8_t index = 0;
                
                do {
                    buffer[index] = I2C_Read(ACK); 
                    index++;
                } while (index < num_bytes - 1);
                
                buffer[index] = I2C_Read(NACK); 

            }
        }
    }
    
    I2C_Stop();
}

double MPU6050_GetAccelX() {
    MPU6050_ReadRegister(ACCEL_XOUT, 2, buffer, BUFFER_LENGTH);
    double raw_value = buffer[0] << 8 | buffer[1];
    return raw_value / SF_ACCE;
}

double MPU6050_GetAccelY() {
    MPU6050_ReadRegister(ACCEL_YOUT, 2, buffer, BUFFER_LENGTH);
    double raw_value = buffer[0] << 8 | buffer[1];
    return raw_value / SF_ACCE;
}

double MPU6050_GetAccelZ() {
    MPU6050_ReadRegister(ACCEL_ZOUT, 2, buffer, BUFFER_LENGTH);
    double raw_value = buffer[0] << 8 | buffer[1];
    return raw_value / SF_ACCE;
}

double MPU6050_GetGyroX() {
    MPU6050_ReadRegister(GYRO_XOUT, 2, buffer, BUFFER_LENGTH);
    double raw_value = buffer[0] << 8 | buffer[1];
    return raw_value / SF_GYRO;
}

double MPU6050_GetGyroY() {
    double raw_value = MPU6050_Get16BitRegister(GYRO_YOUT);
    return raw_value / SF_GYRO;
}

double MPU6050_GetGyroZ() {
    double raw_value = MPU6050_Get16BitRegister(GYRO_ZOUT);
    return raw_value / SF_GYRO;
}

double MPU6050_GetTemp() {
    double raw_value = MPU6050_Get16BitRegister(TEMP_OUT);
    double value = (raw_value / 340.00) + 36.53;
    return value;
}

double MPU6050_Get16BitRegister(unsigned char reg) {
    MPU6050_ReadRegister(reg, 2, buffer, BUFFER_LENGTH);
    return buffer[0] << 8 | buffer[1];
}