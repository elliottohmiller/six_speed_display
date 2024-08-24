/*
 * sixspeed.c
 *
 *  Created on: Aug 4, 2024
 *      Author: elliottmiller
 */
#include "sixspeed.h"
#include "st7789.h"
#include "fonts.h"

//#define DEBUG_TEXT

/****text representation****/

const char* undefined_gear = "?";
const char* neutral = "N";
const char* first = "1";
const char* second = "2";
const char* third = "3";
const char* fourth = "4";
const char* fifth = "5";
const char* sixth = "6";
const char* reverse = "R";


#ifdef  DEBUG_TEXT

#define

	const char* positive = "+";
	const char* negative = "-";
	const char* xval = "X = ";
	const char* yval = "Y = ";
	const char* zval = "Z = ";
	const char* trigger = "T";

#endif

/****indexing****/

enum grid_y
{
	left,
	center_y,
	right,
	right_rev,
	y_max,
};

enum grid_x
{
	up,
	center_x,
	down,

	x_max,
};

/****************for array indexing******************/

enum gear_num
{
	neutral_gear,
	first_gear,
	second_gear,
	third_gear,
	fourth_gear,
	fifth_gear,
	sixth_gear,
	reverse_gear,
	between_gear,

	max_gears,
};

uint8_t shift_grid[3][4] = { //y, x

		{first_gear,   third_gear,   fifth_gear,   between_gear},

		{neutral_gear, neutral_gear, neutral_gear, neutral_gear},

		{second_gear,  fourth_gear,  sixth_gear,   reverse_gear}
};
uint8_t get_position(lis3dh_t* ref, lis3dh_t* shift)
{

	int avg_degrees[3] = {0, 0, 0};

	read_degree_difference(ref, shift, &avg_degrees);

	int8_t y_square  = 0;
	int8_t x_square  = 0;

	if (avg_degrees[X_ARR_POS] >= 9)
		{x_square = up;}
	else if (avg_degrees[X_ARR_POS] <= -9)
		{x_square = down;}
	else
		{x_square = center_x;}

	if (-avg_degrees[Y_ARR_POS] > 8)
		{y_square = right_rev;}
	else if (-avg_degrees[Y_ARR_POS] >= 6 && -avg_degrees[Y_ARR_POS] <= 8)
		{y_square = right;}
	else if (-avg_degrees[Y_ARR_POS] <= -3)
		{y_square = left;}
	else
		{y_square = center_y;}

	return shift_grid[x_square][y_square];
}

const char* get_gear_text(uint8_t position)
{
	switch (position)
	{

	case neutral_gear: return neutral;
	case first_gear:   return first;
	case second_gear:  return second;
	case third_gear:   return third;
	case fourth_gear:  return fourth;
	case fifth_gear:   return fifth;
	case sixth_gear:   return sixth;
	case reverse_gear: return reverse;

	default: return undefined_gear;

	}

}


int sixspeed_process(lis3dh_t* ref, lis3dh_t* shift)
{

	while(1)
	{

		shift->position = get_position(ref, shift);

		ST7789_WriteScaledChar(96, 16, *(get_gear_text(shift->position)), Font_16x26, MYGREEN, BLACK, 8);
	}

}

