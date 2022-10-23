/*
@file	  grid.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     23/10/2022
@brief    This header file
______________________________________________________________________*/

#pragma once

#include "cprocessing.h"

typedef struct {
	CP_Vector pos;
	char *text;
	CP_Color color;
	float size;
} Text;

typedef struct {
	CP_Vector pos;
	CP_Vector size;
	int max_input_count; // How many inputs can be sent before pressing space counts as quit (e.g. vector is 2 (x, y), a 2x2 matrix is 4 (a, b, c, d)
} Collider;

void draw_rect(float left, float top, float length, float height, CP_Color color, float border);
void draw_rect_noborder(float left, float top, float length, float height, CP_Color color);

void draw_line(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness);
void draw_line_pointed(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness);

void draw_circle(float point_x, float point_y, float diameter, CP_Color color, float border);
void draw_circle_noborder(float point_x, float point_y, float diameter, CP_Color color);

void draw_vector_arrow(float point_x, float point_y, CP_Vector distance, CP_Color color);
void draw_matrix_box(float left, float top, int value_x, int value_y, float box_height, float box_width, CP_Color color, BOOL force_draw_text);

#define TEXT_SIZE_DEFAULT 32.0f

Text text_create(float pos_x, float pos_y, char *text, CP_Color color, float size);
void draw_text_full(char* text, float pos_x, float pos_y, CP_Color color, CP_TEXT_ALIGN_HORIZONTAL h_align, CP_TEXT_ALIGN_VERTICAL v_align, float size);
void draw_text(char *text, float pos_x, float pos_y, CP_Color color);
void draw_text_top_left(char* text, float pos_x, float pos_y, CP_Color color, float size);

Collider collider_create(float pos_x, float pos_y, float size_x, float size_y, int max_count);
BOOL is_clicked(float mouse_x, float mouse_y, const Collider *collider);