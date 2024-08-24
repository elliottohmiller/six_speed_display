#include "accelerometer.h"
#include <stm32g431xx.h>

#define CS_GPIO_PORT GPIOA

void LIS3DH_Config(lis3dh_t* lis3dh, void* hspi, uint16_t cs_pin)
{

    lis3dh->dev_ctx.write_reg = acc_write;
    lis3dh->dev_ctx.read_reg = acc_read;
    lis3dh->dev_ctx.mdelay = platform_delay;
    lis3dh->dev_ctx.handle = hspi;
    lis3dh->dev_ctx.cs_pin = cs_pin;

    for (int i = 0; i < 3; ++i)
    {
  	  lis3dh->offset[i] = 0;
    }

}


void Accelerometer_Init(lis3dh_t* acc)
{

    /* Enable Block Data Update. */
    lis3dh_block_data_update_set(&acc->dev_ctx, PROPERTY_ENABLE);
    /* Set Output Data Rate to 1Hz. */
    lis3dh_data_rate_set(&acc->dev_ctx, LIS3DH_ODR_100Hz);
    /* Set full scale to 2g. */
    lis3dh_full_scale_set(&acc->dev_ctx, LIS3DH_2g);
    /* Set device in continuous mode with 12 bit resol. */
    lis3dh_operating_mode_set(&acc->dev_ctx, LIS3DH_HR_12bit);

    calibrate_offset(acc);

}


void read_accelerometer_data(lis3dh_t* accel)
{
    lis3dh_xl_data_ready_get(&accel->dev_ctx, &accel->reg.byte);

if (accel->reg.byte) {
      // Read accelerometer data
      memset(accel->data_raw_acceleration, 0x00, 3 * sizeof(int16_t));
      lis3dh_acceleration_raw_get(&accel->dev_ctx, (int16_t*)&accel->data_raw_acceleration);

      accel->acceleration_mg[0] =
        lis3dh_from_fs2_hr_to_mg(accel->data_raw_acceleration[0]);
      accel->acceleration_mg[1] =
        lis3dh_from_fs2_hr_to_mg(accel->data_raw_acceleration[1]);
      accel->acceleration_mg[2] =
        lis3dh_from_fs2_hr_to_mg(accel->data_raw_acceleration[2]);
}

}

uint8_t SPIx_WriteRead(uint8_t Byte, SPI_HandleTypeDef* hspi)
{
  uint8_t receivedbyte = 0;

  /* Send a Byte through the SPI peripheral */
  /* Read byte from the SPI bus */
  HAL_SPI_TransmitReceive(hspi, (uint8_t*) &Byte, (uint8_t*) &receivedbyte, 1, 1000);

  return receivedbyte;
}

int32_t acc_read(void *handle, uint8_t reg, uint8_t *bufp, uint16_t len, uint16_t cs_pin)
{

	  if(len > 0x01)
	  {
	    reg |= (uint8_t)(0x80 | 0x40);
	  }
	  else
	  {
	    reg |= 0x80;
	  }

  uint8_t dummy = 0x00;

  HAL_GPIO_WritePin(CS_GPIO_PORT, cs_pin, GPIO_PIN_RESET);
  SPIx_WriteRead(reg, handle);

  while(len > 0x00)
  {
    /* Send dummy byte (0x00) to generate the SPI clock to ACCELEROMETER (Slave device) */
    *bufp = SPIx_WriteRead(dummy, handle);
    len--;
    bufp++;
  }

  HAL_GPIO_WritePin(CS_GPIO_PORT, cs_pin, GPIO_PIN_SET);

  return 0;
}

int32_t acc_write(void *handle, uint8_t reg, const uint8_t *bufp, uint16_t len, uint16_t cs_pin)
{

  if(len > 0x01)
	{
	  reg |= 0x40;
	}

  HAL_GPIO_WritePin(CS_GPIO_PORT, cs_pin, GPIO_PIN_RESET);

  SPIx_WriteRead(reg, handle);

  while(len >= 0x01)
  {
	SPIx_WriteRead(*bufp, handle);
    len--;
    bufp++;
  }
  HAL_GPIO_WritePin(CS_GPIO_PORT, cs_pin, GPIO_PIN_SET);

  return 0;
}

void platform_delay(uint32_t ms)
{
  HAL_Delay(ms);
}

const float* calibrate_offset(lis3dh_t* acc)
{

	for (int j = 0; j <= (ACC_SAMPLES - 1); ++j)
	{
		read_accelerometer_data(acc);

		for (int i = 0; i <= 2; ++i)
		{
			acc->offset[i] += (acc->acceleration_mg[i]);
		}
	}

  	for (int i = 0; i <= 2; ++i)
  	{
  		acc->offset[i] = (acc->offset[i] / ((float)ACC_SAMPLES));
  	}

  	return acc->offset;
}

int* read_degree_difference(lis3dh_t* const ref, lis3dh_t* const shift, int* avg_degrees)
  {
	float avg_acceleration[3] = {0.0, 0.0, 0.0};

	for (int j = 0; j <= (ACC_SAMPLES - 1); ++j)
	{
		read_accelerometer_data(shift);
		read_accelerometer_data(ref);

		for (int i = 0; i <= 2; ++i)
		{
			avg_acceleration[i] += (((shift->acceleration_mg[i] - shift->offset[i]) * (1 - ((ref->offset[i] - shift->offset[i]) / 1000)))) - (ref->acceleration_mg[i] - ref->offset[i]);
		}
	}

	for (int i = 0; i <= 2; ++i)
	{
		avg_acceleration[i] = (avg_acceleration[i] / ((float)ACC_SAMPLES));
	}


	for (int i = 0; i <= 2; ++i)
	{
		avg_degrees[i] = (asin((avg_acceleration[i] / ONE_G)) * (180 / PI));
	}

	return avg_degrees;

  }
