/*!
@file	  vectors.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     23/10/2022
@brief    This source file 
______________________________________________________________________*/

#include "cprocessing.h"
#include "vectors.h"
#include "grid.h"
#include "utils.h"

// Size of vectors array, acts as buffer
#define MAX_VECTORS 10

// Size settings
#define VECTOR_SIZE 6.0f

// Storing all vectors information
Vector vectors_list[MAX_VECTORS];

// Current vector count
int vectors_count = 0;

// Screen position of origin
CP_Vector origin;

void vectors_init(void) {
	// Get posiiton of origin
	origin = grid_get_screenpos_origin();
}

void vectors_render(void) {
	Vector *current;
	CP_Vector screen_pos;
	for (int index = 0; index < vectors_count; ++index) {
		current = &vectors_list[index];
		screen_pos = grid_get_screenpos((int)current->pos.x, (int)current->pos.y);

		draw_line_pointed(origin.x, origin.y, screen_pos.x, screen_pos.y, current->color, VECTOR_SIZE);
	}
}

// Vector will be replaced based on the color given
void vector_replace(float x, float y, CP_Color color) {
	Vector *current;
	for (int index = 0; index < vectors_count; ++index) {
		current = &vectors_list[index];

		if (current->color.r == color.r && current->color.g == color.g && current->color.b == color.b) {
			current->pos.x = x;
			current->pos.y = y;
		}
	}
}

void vector_create(float x, float y, CP_Color color) {
	Vector new_vector = { .pos.x = x, .pos.y = y, .color = color };
	vectors_list[vectors_count] = new_vector;

	++vectors_count;
}
