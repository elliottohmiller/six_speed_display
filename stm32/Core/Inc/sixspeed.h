/*
 * sixspeed.h
 *
 *  Created on: Aug 4, 2024
 *      Author: elliottmiller
 */

#ifndef SIXSPEED_H
#define SIXSPEED_H

#include <stdint.h>
#include "accelerometer.h"

/****array indexes for accelerometer axis data****/

#define X_ARR_POS 0
#define Y_ARR_POS 1
#define Z_ARR_POS 2

uint8_t get_position(lis3dh_t* ref, lis3dh_t* shift);
const char* get_gear_text(uint8_t position);
int sixspeed_process();

#endif /* SIXSPEED_H */
