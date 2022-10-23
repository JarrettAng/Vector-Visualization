/*
@file	  grid.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     27/09/2022
@brief    This header file
______________________________________________________________________*/

#pragma once

#include "cprocessing.h"

#define TEXT_MAX_BUFFER 50

typedef struct {
	CP_Vector pos;
	char text[TEXT_MAX_BUFFER];
} Text;

void draw_rect(float left, float top, float length, float height, CP_Color color, float border);
void draw_rect_noborder(float left, float top, float length, float height, CP_Color color);

void draw_line(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness);
void draw_line_pointed(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness);

void draw_circle(float point_x, float point_y, float diameter, CP_Color color, float border);
void draw_circle_noborder(float point_x, float point_y, float diameter, CP_Color color);

void draw_vector_arrow(float point_x, float point_y, CP_Vector distance, CP_Color color);
void draw_matrix_box(float left, float top, int value_x, int value_y, float box_height, float box_width, CP_Color color, BOOL force_draw_text);
