/*!
@file	  utils.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     23/10/2022
@brief    This source file 
______________________________________________________________________*/

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

//----------------------------------------------------------------------------------------------------------------
// Basic shapes
void draw_rect(float left, float top, float length, float height, CP_Color color, float border) {
	CP_Settings_StrokeWeight(border);
	CP_Settings_Fill(color);
	CP_Graphics_DrawRect(left, top, length, height);
}

void draw_rect_noborder(float left, float top, float length, float height, CP_Color color) {
	draw_rect(left, top, length, height, color, .0f);
}

void draw_line(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness) {
	CP_Settings_Stroke(color);
	CP_Settings_StrokeWeight(thickness);
	CP_Graphics_DrawLine(point_1_x, point_1_y, point_2_x, point_2_y);
}

void draw_line_pointed(float point_1_x, float point_1_y, float point_2_x, float point_2_y, CP_Color color, float thickness) {
	CP_Vector line = CP_Vector_Set(point_1_x - point_2_x, point_1_y - point_2_y);
	line = CP_Vector_Normalize(line);
	line = CP_Vector_Scale(line, 12.0f); // Second parameter is size of triangle at the end of the arrow

	// Draw pointed part
	draw_vector_arrow(point_2_x, point_2_y, line, color);

	// Draw line part

	CP_Settings_Stroke(color);
	CP_Settings_StrokeWeight(thickness);
	CP_Graphics_DrawLine(point_1_x, point_1_y, point_2_x + line.x, point_2_y + line.y);
}

void draw_circle(float point_x, float point_y, float diameter, CP_Color color, float border) {
	CP_Settings_StrokeWeight(border);
	CP_Settings_Fill(color);
	CP_Graphics_DrawCircle(point_x, point_y, diameter);
}

void draw_circle_noborder(float point_x, float point_y, float diameter, CP_Color color) {
	draw_circle(point_x, point_y, diameter, color, .0f);
}

//----------------------------------------------------------------------------------------------------------------
// Complex shapes
void draw_vector_arrow(float point_x, float point_y, CP_Vector distance, CP_Color color) {
	CP_Vector normal = CP_Vector_Set(-distance.y, distance.x);

	CP_Settings_StrokeWeight(.0f);
	CP_Settings_Fill(color);
	// The 1.5f bias makes the arrow longer than wider
	CP_Graphics_DrawTriangle(point_x, point_y, point_x + distance.x * 1.5f + normal.x, point_y + distance.y * 1.5f + normal.y, point_x + distance.x * 1.5f - normal.x, point_y + distance.y * 1.5f - normal.y);
}

void draw_matrix_box(float left, float top, int value_x, int value_y, float box_height, float box_width, CP_Color color, BOOL force_draw_text) {
	float box_thickness = 3.0f, bracket_stub = box_width / 6.0f;

	// Left bracket [
	draw_rect_noborder(left, top, box_thickness, box_height, color);
	draw_rect_noborder(left, top, bracket_stub, box_thickness, color);
	draw_rect_noborder(left, top + box_height - box_thickness, bracket_stub, box_thickness, color);

	// Coordinates
	if (force_draw_text || value_x + value_y != 0) {
		char buffer[10] = { 0 };

		sprintf_s(buffer, _countof(buffer), "%d", value_x);
		draw_text(buffer, left + box_width / 2, top + box_height / 4, color);

		sprintf_s(buffer, _countof(buffer), "%d", value_y);
		draw_text(buffer, left + box_width / 2, top + box_height * 3 / 4, color);
	}

	// Right bracket ]
	draw_rect_noborder(left + box_width - box_thickness, top, box_thickness, box_height, color);
	draw_rect_noborder(left + box_width - box_thickness - bracket_stub, top, bracket_stub, box_thickness, color);
	draw_rect_noborder(left + box_width - box_thickness - bracket_stub, top + box_height - box_thickness, bracket_stub, box_thickness, color);
}

//----------------------------------------------------------------------------------------------------------------
// Text

Text text_create(float pos_x, float pos_y, char *text, CP_Color color, float size) {
	Text new_text = { .pos.x = pos_x, .pos.y = pos_y, .text = text, .color = color, .size = size };
	return new_text;
}

void draw_text_full(char *text, float pos_x, float pos_y, CP_Color color, CP_TEXT_ALIGN_HORIZONTAL h_align, CP_TEXT_ALIGN_VERTICAL v_align, float size) {
	CP_Settings_TextAlignment(h_align, v_align);
	CP_Settings_TextSize(size);
	CP_Settings_Fill(color);
	CP_Font_DrawText(text, pos_x, pos_y);
}

void draw_text(char* text, float pos_x, float pos_y, CP_Color color) {
	draw_text_full(text, pos_x, pos_y, color, CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE, TEXT_SIZE_DEFAULT);
}

void draw_text_top_left(char* text, float pos_x, float pos_y, CP_Color color, float size) {
	draw_text_full(text, pos_x, pos_y, color, CP_TEXT_ALIGN_H_LEFT, CP_TEXT_ALIGN_V_TOP, size);
}

//----------------------------------------------------------------------------------------------------------------
// Colliders
Collider collider_create(float pos_x, float pos_y, float size_x, float size_y, int max_count) {
	Collider new_collider = { .pos.x = pos_x, .pos.y = pos_y, .size.x = size_x, .size.y = size_y , .max_input_count = max_count };
	return new_collider;
}

BOOL is_clicked(float mouse_x, float mouse_y, const Collider *collider) {
	if (mouse_x >= collider->pos.x && mouse_x <= collider->pos.x + collider->size.x) {
		if (mouse_y >= collider->pos.y && mouse_y <= collider->pos.y + collider->size.y) {
			return TRUE;
		}
	}
	return FALSE;
}
