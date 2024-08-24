#ifndef ACCELEROMETER_H
#define ACCELEROMETER_H

#include "lis3dh_reg.h"
#include "stm32g4xx_hal.h"
#include "spi.h"
#include "stdint.h"
#include <math.h>

#define ACC_SAMPLES 10000 //noticeable improvements in data filtering at 10000; slight screen latency tradeoff
#define PI 3.14159265 //for degree conversion
#define ONE_G 1000.0 //measurements are returned in mG

typedef struct
{
    stmdev_ctx_t dev_ctx;
    int16_t data_raw_acceleration[3];
    float acceleration_mg[3];
    lis3dh_reg_t reg;
    float offset[3];
    uint8_t position;

} lis3dh_t;

void LIS3DH_Config(lis3dh_t* lis3dh, void* hspi, uint16_t cs_pin);

void Accelerometer_Init(lis3dh_t* acc);

void read_accelerometer_data(lis3dh_t* accel);

uint8_t SPIx_WriteRead(uint8_t Byte, SPI_HandleTypeDef* handle);

int32_t acc_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len, uint16_t cs_pin);

int32_t acc_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len, uint16_t cs_pin);

void platform_delay(uint32_t ms);

const float* calibrate_offset(lis3dh_t* acc);

int* read_degree_difference(lis3dh_t* const ref, lis3dh_t* const shift, int* avg_degrees);

#endif
