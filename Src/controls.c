/*!
@file	  control.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     14/09/2022
@brief    This source file 
______________________________________________________________________*/

#include <stdbool.h>

#include "cprocessing.h"
#include "grid.h"
#include "controls.h"
#include "vectors.h"
#include "utils.h"


// Display settings
#define EDGE_BUFFER 50.0f
#define MAX_VECTORS_LIST 7

// Color settings
#define PANEL_COLOR CP_Color_Create(0, 0, 0, 255)

#define I_HAT_COLOR CP_Color_Create(146, 208, 80, 255)
#define J_HAT_COLOR CP_Color_Create(255, 105, 105, 255)
#define V1_COLOR CP_Color_Create(249, 220, 1, 255)

// Size settings
#define VECTOR_SIZE 6.0f
#define MATRIX_BOX_HEIGHT 80.0f
#define MATRIX_BOX_WIDTH 60.0f
#define MATRIX_BOX_ARROW_OFFSET 20.0f

// Bounds for the panel background
CP_Vector top_left, size;

// Base vectors panel
Text text_base_v[4];

// Vectors panel
Matrix v_list[MAX_VECTORS_LIST];
CP_Vector v_panel_top_left;
int v_count = 0;

// Vectors transformed panel
Matrix v_transformed_list[MAX_VECTORS_LIST];
CP_Vector v_t_panel_top_left;


void panel_init(void) {
	//----------------------------------------------------------------------------------------------------------------
	// Panel background init
	top_left = CP_Vector_Set(grid_get_size(), 0);
	size = CP_Vector_Set((float)CP_System_GetWindowWidth() - top_left.x, (float)CP_System_GetWindowHeight());

	//----------------------------------------------------------------------------------------------------------------
	// Vectors panel init
	v_panel_top_left = CP_Vector_Set(top_left.x + EDGE_BUFFER, (float)CP_System_GetWindowHeight() / 2 - 160.0f);
	float spacing = (size.x - EDGE_BUFFER) / MAX_VECTORS_LIST;

	// First two vectors are i-hat and j-hat
	v_list[0] = panel_create_vector_empty(v_panel_top_left.x + spacing * 0, v_panel_top_left.y, I_HAT_COLOR);
	v_list[1] = panel_create_vector_empty(v_panel_top_left.x + spacing * 1, v_panel_top_left.y, J_HAT_COLOR);
	// The rest are generic
	v_list[2] = panel_create_vector_empty(v_panel_top_left.x + spacing * 2, v_panel_top_left.y, V1_COLOR);
	v_list[3] = panel_create_vector_empty(v_panel_top_left.x + spacing * 3, v_panel_top_left.y, V1_COLOR);
	v_list[4] = panel_create_vector_empty(v_panel_top_left.x + spacing * 4, v_panel_top_left.y, V1_COLOR);
	v_list[5] = panel_create_vector_empty(v_panel_top_left.x + spacing * 5, v_panel_top_left.y, V1_COLOR);
	v_list[6] = panel_create_vector_empty(v_panel_top_left.x + spacing * 6, v_panel_top_left.y, V1_COLOR);

	//----------------------------------------------------------------------------------------------------------------
	// Vectors transformed panel init
	v_t_panel_top_left = CP_Vector_Set(top_left.x + EDGE_BUFFER, (float)CP_System_GetWindowHeight() / 2);

	// First two vectors are i-hat and j-hat
	v_transformed_list[0] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 0, v_t_panel_top_left.y, I_HAT_COLOR);
	v_transformed_list[1] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 1, v_t_panel_top_left.y, J_HAT_COLOR);
	// The rest are generic
	v_transformed_list[2] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 2, v_t_panel_top_left.y, V1_COLOR);
	v_transformed_list[3] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 3, v_t_panel_top_left.y, V1_COLOR);
	v_transformed_list[4] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 4, v_t_panel_top_left.y, V1_COLOR);
	v_transformed_list[5] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 5, v_t_panel_top_left.y, V1_COLOR);
	v_transformed_list[6] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 6, v_t_panel_top_left.y, V1_COLOR);

	// First two vectors are reserved for i-hat and j-hat
	panel_input_new_base_vector(1, 0);
	panel_input_new_base_vector(0, 1);

	//----------------------------------------------------------------------------------------------------------------
	// Base vectors panel init

	// Initialize how text are drawn
	CP_Settings_TextAlignment(CP_TEXT_ALIGN_H_CENTER, CP_TEXT_ALIGN_V_MIDDLE);
}

void panel_render(void) {
	//----------------------------------------------------------------------------------------------------------------
	// Panel background
	draw_rect_noborder(top_left.x, top_left.y, size.x, size.y, PANEL_COLOR);

	//----------------------------------------------------------------------------------------------------------------
	// Vectors & Transformed vectors panel
	Matrix *current_v, *current_t_v;
	for (int index = 0; index < MAX_VECTORS_LIST; ++index) {
		// Vectors panel
		current_v = &v_list[index];
		draw_matrix_box(current_v->pos.x, current_v->pos.y, current_v->value_x, current_v->value_y, MATRIX_BOX_HEIGHT, MATRIX_BOX_WIDTH, current_v->color, false);

		// Transformed vectors panel
		current_t_v = &v_transformed_list[index];

		bool draw_text = v_list[index].value_x + v_list[index].value_y != 0;
		if (draw_text) { // If there's an output draw an arrow between them
			draw_line_pointed(current_v->pos.x + MATRIX_BOX_WIDTH / 2, current_v->pos.y + MATRIX_BOX_HEIGHT + MATRIX_BOX_ARROW_OFFSET, current_t_v->pos.x + MATRIX_BOX_WIDTH / 2, 
							  current_t_v->pos.y - MATRIX_BOX_ARROW_OFFSET, current_v->color, VECTOR_SIZE);
		}

		draw_matrix_box(current_t_v->pos.x, current_t_v->pos.y, current_t_v->value_x, current_t_v->value_y, MATRIX_BOX_HEIGHT, MATRIX_BOX_WIDTH, current_t_v->color, draw_text);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Base vectors panel

}

Matrix panel_create_vector(float pos_x, float pos_y, int value_x, int value_y, CP_Color color) {
	Matrix new_matrix = { .pos.x = pos_x, .pos.y = pos_y, .value_x = value_x, .value_y = value_y, .color = color };
	return new_matrix;
}

Matrix panel_create_vector_empty(float pos_x, float pos_y, CP_Color color) {
	return panel_create_vector(pos_x, pos_y, 0, 0, color);
}

void panel_input_new_vector_at(int value_x, int value_y, int index) {
	// Fill default vector matrix
	Matrix* current = &v_list[index];
	current->value_x = value_x;
	current->value_y = value_y;

	// Fill transformed vector matrix
	panel_apply_transformation(index);

	// Initialize the vector for the grid
	vector_create((float)value_x, (float)value_y, current->color);

	++v_count;
}

void panel_input_new_vector(int value_x, int value_y) {
	panel_input_new_vector_at(value_x, value_y, v_count);
}

void panel_input_new_base_vector(int value_x, int value_y) {
	// Fill default vector matrix
	Matrix* current = &v_list[v_count];
	current->value_x = value_x;
	current->value_y = value_y;

	// Fill transformed vector matrix
	Matrix* transformed = &v_transformed_list[v_count];
	transformed->value_x = value_x;
	transformed->value_y = value_y;

	// Initialize the vector for the grid
	vector_create((float)value_x, (float)value_y, current->color);

	++v_count;
}

void panel_apply_transformation(int index) {
	Matrix *current = &v_list[index];
	Matrix *transformed = &v_transformed_list[index];

	transformed->value_x = current->value_x * v_transformed_list[0].value_x + current->value_y * v_transformed_list[1].value_x;
	transformed->value_y = current->value_x * v_transformed_list[0].value_y + current->value_y * v_transformed_list[1].value_y;
}

void panel_apply_all_transformations(void) {
	for (int index = 2; index < v_count; ++index) {
		panel_apply_transformation(index);
	}
}

void panel_ihat_transformation(CP_Vector transformed_ihat) {
	v_transformed_list[0].value_x = (int)transformed_ihat.x;
	v_transformed_list[0].value_y = (int)transformed_ihat.y;

	panel_apply_all_transformations();

	grid_scale(transformed_ihat, CP_Vector_Set((float)v_transformed_list[1].value_x, (float)v_transformed_list[1].value_y));
}

void panel_jhat_transformation(CP_Vector transformed_jhat) {
	v_transformed_list[1].value_x = (int)transformed_jhat.x;
	v_transformed_list[1].value_y = (int)transformed_jhat.y;

	panel_apply_all_transformations();

	grid_scale(CP_Vector_Set((float)v_transformed_list[0].value_x, (float)v_transformed_list[0].value_y), transformed_jhat);
}

void panel_check_clicked(int mouse_x, int mouse_y) {

}
