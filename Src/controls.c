/*!
@file	  control.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     23/10/2022
@brief    This source file 
______________________________________________________________________*/

// For input
#include <stdio.h>
#include <stdlib.h>

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
#define HATS_COMBINED_COLOR CP_Color_Create(201, 157, 93, 255)
#define V1_COLOR CP_Color_Create(250,233,231, 255)
#define V2_COLOR CP_Color_Create(250,175,160, 255)
#define V3_COLOR CP_Color_Create(255,228,137, 255)
#define V4_COLOR CP_Color_Create(206,221,129, 255)
#define V5_COLOR CP_Color_Create(176,109,129, 255)

// Size settings
#define VECTOR_SIZE 6.0f
#define MATRIX_BOX_HEIGHT 80.0f
#define MATRIX_BOX_WIDTH 60.0f
#define MATRIX_BOX_ARROW_OFFSET 5.0f
#define MAX_CLICKABLE_LIST 20

// Text settings
#define TEXT_TITLE_COUNT 2
#define TEXT_BASE_V_COUNT 10
#define TEXT_BASE_V_INPUT_COUNT 8
#define TEXT_TRANSFORMED_V_COUNT 6
#define TEXT_TRANSFORMED_V_INPUT_COUNT 2
#define TEXT_V_COUNT 1
#define DEFAULT_HEADER_TEXT_SIZE 48.0f
#define DEFAULT_HINT_TEXT_SIZE 24.0f
#define HEADER_DEFAULT_COLOR CP_Color_Create(255, 255, 255, 255)
#define HINT_DEFAULT_COLOR CP_Color_Create(200, 200, 200, 255)

// Bounds for the panel background
CP_Vector top_left, size;

// Program title
Text text_title[TEXT_TITLE_COUNT];
CP_Vector title_panel_top_left;

// Base vectors panel
Text text_base_v[TEXT_BASE_V_COUNT];
Text text_base_v_input[TEXT_BASE_V_INPUT_COUNT];
CP_Vector b_v_panel_top_left;
Matrix b_v_matrix;

// Vectors panel
Text text_v[TEXT_V_COUNT];
Matrix v_list[MAX_VECTORS_LIST];
CP_Vector v_panel_top_left;
int v_count = 0;

// Vectors transformed panel
Text text_transformed_v[TEXT_TRANSFORMED_V_COUNT];
Text text_transformed_v_input[TEXT_TRANSFORMED_V_INPUT_COUNT];
Matrix v_transformed_list[MAX_VECTORS_LIST];
CP_Vector v_t_panel_top_left;

// List of clickable objects
Collider clickables[MAX_CLICKABLE_LIST];
int clickable_count = 0;
// Input
#define MAX_INPUT_LIST 3
#define MAX_INPUTS_TOTAL_LIST 4
char input_text_storage[MAX_CLICKABLE_LIST][MAX_INPUTS_TOTAL_LIST][MAX_INPUT_LIST + 1] = { '\0' }; // The last array in this 3D array +1 because the last character needs to be the terminator '\0'
char input_text[MAX_INPUT_LIST] = { '\0' };
int input_text_index = 0;
int input_space_count = 0;
int input_max_count = 0;
int clicked_index = -1;
int input_vector_to_transform; // Index of clicked vector in the vector transformation panel

void panel_init(void) {
	//----------------------------------------------------------------------------------------------------------------
	// Panel background init
	top_left = CP_Vector_Set(grid_get_size(), 0);
	size = CP_Vector_Set((float)CP_System_GetWindowWidth() - top_left.x, (float)CP_System_GetWindowHeight());

	//----------------------------------------------------------------------------------------------------------------
	// Title panel init
	title_panel_top_left = CP_Vector_Set(top_left.x + EDGE_BUFFER, 10.0f);
	text_title[0] = text_create(title_panel_top_left.x, title_panel_top_left.y, "Vector Visualization!", HEADER_DEFAULT_COLOR, DEFAULT_HEADER_TEXT_SIZE);
	text_title[1] = text_create(title_panel_top_left.x, text_title[0].pos.y + DEFAULT_HEADER_TEXT_SIZE, "[Made by Jarrett using CProcessing] Visualize transformations to a 30x30 grid.", HINT_DEFAULT_COLOR, DEFAULT_HINT_TEXT_SIZE);

	//----------------------------------------------------------------------------------------------------------------
	// Base vectors panel init
	// CAUTION: Positions are semi-hardcoded
	b_v_panel_top_left = CP_Vector_Set(top_left.x + EDGE_BUFFER, (float)CP_System_GetWindowHeight() / 2 - 430.0f);
	text_base_v[0] = text_create(b_v_panel_top_left.x, b_v_panel_top_left.y, "Change base vectors", HEADER_DEFAULT_COLOR, DEFAULT_HEADER_TEXT_SIZE);

	text_base_v[1] = text_create(b_v_panel_top_left.x, b_v_panel_top_left.y + DEFAULT_HEADER_TEXT_SIZE + 10.0f, "Input new i-hat:", I_HAT_COLOR, TEXT_SIZE_DEFAULT);
	text_base_v[2] = text_create(b_v_panel_top_left.x + 200.0f, b_v_panel_top_left.y + DEFAULT_HEADER_TEXT_SIZE + 10.0f, "(         ,         )", I_HAT_COLOR, TEXT_SIZE_DEFAULT);
	panel_add_clickable(text_base_v[2].pos.x, text_base_v[2].pos.y - 5.0f, 130.0f, TEXT_SIZE_DEFAULT + 10.0f, 2); // Add text to clickables (click size hard-coded) CLICK_INDEX 0

	text_base_v[3] = text_create(b_v_panel_top_left.x, text_base_v[1].pos.y + TEXT_SIZE_DEFAULT + 20.0f, "Input new j-hat:", J_HAT_COLOR, TEXT_SIZE_DEFAULT);
	text_base_v[4] = text_create(b_v_panel_top_left.x + 200.0f, text_base_v[1].pos.y + TEXT_SIZE_DEFAULT + 20.0f, "(         ,         )", J_HAT_COLOR, TEXT_SIZE_DEFAULT);
	panel_add_clickable(text_base_v[4].pos.x, text_base_v[4].pos.y - 5.0f, 130.0f, TEXT_SIZE_DEFAULT + 10.0f, 2); // Add text to clickables (click size hard-coded) CLICK_INDEX 1

	text_base_v[5] = text_create(b_v_panel_top_left.x + 400.0f, b_v_panel_top_left.y + DEFAULT_HEADER_TEXT_SIZE + 10.0f, "Input new", HATS_COMBINED_COLOR, TEXT_SIZE_DEFAULT);
	text_base_v[6] = text_create(b_v_panel_top_left.x + 400.0f, text_base_v[5].pos.y + TEXT_SIZE_DEFAULT, "i/j-hat matrix:", HATS_COMBINED_COLOR, TEXT_SIZE_DEFAULT);
	b_v_matrix = panel_create_vector_empty(b_v_panel_top_left.x + 585.0f, b_v_panel_top_left.y + DEFAULT_HEADER_TEXT_SIZE + 15.0f, HATS_COMBINED_COLOR);
	panel_add_clickable(b_v_matrix.pos.x, b_v_matrix.pos.y - 5.0f, 160.0f, 100.0f, 4); // Add text to clickables (click size hard-coded) CLICK_INDEX 2

	text_base_v[7] = text_create(b_v_panel_top_left.x, text_base_v[3].pos.y + 50.0f, "(How To Use) Click on any bracket, type any number, press enter to advance to next.", HINT_DEFAULT_COLOR, DEFAULT_HINT_TEXT_SIZE);
	text_base_v[8] = text_create(b_v_panel_top_left.x, text_base_v[7].pos.y + DEFAULT_HINT_TEXT_SIZE, "Once all numbers typed, press enter to confirm. You can't backspace yet :(", HINT_DEFAULT_COLOR, DEFAULT_HINT_TEXT_SIZE);
	text_base_v[9] = text_create(b_v_panel_top_left.x, text_base_v[8].pos.y + DEFAULT_HINT_TEXT_SIZE, "For the matrix, i-hat will be inputted first then j-hat.", HATS_COMBINED_COLOR, DEFAULT_HINT_TEXT_SIZE);

	// Numbers to draw in the i-hat and j-hat brackets
	text_base_v_input[0] = text_create(text_base_v[2].pos.x + 35.0f, text_base_v[2].pos.y + TEXT_SIZE_DEFAULT / 2, "", I_HAT_COLOR, TEXT_SIZE_DEFAULT);
	text_base_v_input[1] = text_create(text_base_v[2].pos.x + 95.0f, text_base_v[2].pos.y + TEXT_SIZE_DEFAULT / 2, "", I_HAT_COLOR, TEXT_SIZE_DEFAULT);
	text_base_v_input[2] = text_create(text_base_v[4].pos.x + 35.0f, text_base_v[4].pos.y + TEXT_SIZE_DEFAULT / 2, "", J_HAT_COLOR, TEXT_SIZE_DEFAULT);
	text_base_v_input[3] = text_create(text_base_v[4].pos.x + 95.0f, text_base_v[4].pos.y + TEXT_SIZE_DEFAULT / 2, "", J_HAT_COLOR, TEXT_SIZE_DEFAULT);
	// Numbers to draw in the i/j-hat matrix
	text_base_v_input[4] = text_create(b_v_matrix.pos.x + 45.0f, b_v_matrix.pos.y + TEXT_SIZE_DEFAULT / 2 + 2.0f, "", I_HAT_COLOR, TEXT_SIZE_DEFAULT);
	text_base_v_input[5] = text_create(b_v_matrix.pos.x + 45.0f, b_v_matrix.pos.y + TEXT_SIZE_DEFAULT / 2 + 52.0f, "", I_HAT_COLOR, TEXT_SIZE_DEFAULT);
	text_base_v_input[6] = text_create(b_v_matrix.pos.x + 105.0f, b_v_matrix.pos.y + TEXT_SIZE_DEFAULT / 2 + 2.0f, "", J_HAT_COLOR, TEXT_SIZE_DEFAULT);
	text_base_v_input[7] = text_create(b_v_matrix.pos.x + 105.0f, b_v_matrix.pos.y + TEXT_SIZE_DEFAULT / 2 + 52.0f, "", J_HAT_COLOR, TEXT_SIZE_DEFAULT);

	//----------------------------------------------------------------------------------------------------------------
	// Vectors panel init
	v_panel_top_left = CP_Vector_Set(top_left.x + EDGE_BUFFER, (float)CP_System_GetWindowHeight() / 2 - 100.0f);
	// The text part
	text_v[0] = text_create(v_panel_top_left.x, v_panel_top_left.y - 70.0f, "Vector Transformations", HEADER_DEFAULT_COLOR, DEFAULT_HEADER_TEXT_SIZE);

	// The panel part
	float spacing = (size.x - EDGE_BUFFER) / MAX_VECTORS_LIST;

	// First two vectors are i-hat and j-hat
	v_list[0] = panel_create_vector_empty(v_panel_top_left.x + spacing * 0, v_panel_top_left.y, I_HAT_COLOR);
	v_list[1] = panel_create_vector_empty(v_panel_top_left.x + spacing * 1, v_panel_top_left.y, J_HAT_COLOR);
	// The rest are generic
	v_list[2] = panel_create_vector_empty(v_panel_top_left.x + spacing * 2, v_panel_top_left.y, V1_COLOR);
	v_list[3] = panel_create_vector_empty(v_panel_top_left.x + spacing * 3, v_panel_top_left.y, V2_COLOR);
	v_list[4] = panel_create_vector_empty(v_panel_top_left.x + spacing * 4, v_panel_top_left.y, V3_COLOR);
	v_list[5] = panel_create_vector_empty(v_panel_top_left.x + spacing * 5, v_panel_top_left.y, V4_COLOR);
	v_list[6] = panel_create_vector_empty(v_panel_top_left.x + spacing * 6, v_panel_top_left.y, V5_COLOR);

	// Add pre-transformed vectors text to clickables (click size hard-coded) CLICK_INDEX 3-9
	Matrix *v_click_current;
	for (int v_click_index = 0; v_click_index < MAX_VECTORS_LIST; ++v_click_index) {
		v_click_current = &v_list[v_click_index];
		panel_add_clickable(v_click_current->pos.x, v_click_current->pos.y - 5, 65.0f, 100.0f, 2);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Vectors transformed panel init
	v_t_panel_top_left = CP_Vector_Set(top_left.x + EDGE_BUFFER, v_panel_top_left.y + 130.0f);

	// First two vectors are i-hat and j-hat
	v_transformed_list[0] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 0, v_t_panel_top_left.y, I_HAT_COLOR);
	v_transformed_list[1] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 1, v_t_panel_top_left.y, J_HAT_COLOR);
	// The rest are generic
	v_transformed_list[2] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 2, v_t_panel_top_left.y, V1_COLOR);
	v_transformed_list[3] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 3, v_t_panel_top_left.y, V2_COLOR);
	v_transformed_list[4] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 4, v_t_panel_top_left.y, V3_COLOR);
	v_transformed_list[5] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 5, v_t_panel_top_left.y, V4_COLOR);
	v_transformed_list[6] = panel_create_vector_empty(v_t_panel_top_left.x + spacing * 6, v_t_panel_top_left.y, V5_COLOR);

	// Create the texts
	text_transformed_v[0] = text_create(v_t_panel_top_left.x, v_t_panel_top_left.y + DEFAULT_HEADER_TEXT_SIZE + 60.0f, "Input new vector:", HEADER_DEFAULT_COLOR, TEXT_SIZE_DEFAULT);
	text_transformed_v[1] = text_create(v_t_panel_top_left.x + 215.0f, text_transformed_v[0].pos.y, "(         ,         )", HEADER_DEFAULT_COLOR, TEXT_SIZE_DEFAULT);
	text_transformed_v[2] = text_create(v_t_panel_top_left.x, text_transformed_v[1].pos.y + 40.0f, "(How To Use) Before inputting a vector, click on any of the matrices above to select it, ", HINT_DEFAULT_COLOR, DEFAULT_HINT_TEXT_SIZE);
	text_transformed_v[3] = text_create(v_t_panel_top_left.x, text_transformed_v[2].pos.y + DEFAULT_HINT_TEXT_SIZE, "Click the \"Input new vector\" bracket and type input like in \"Change Base Vectors\".", HINT_DEFAULT_COLOR, DEFAULT_HINT_TEXT_SIZE);
	text_transformed_v[4] = text_create(v_t_panel_top_left.x, text_transformed_v[3].pos.y + DEFAULT_HINT_TEXT_SIZE, "You may directly input to the post-transformed brackets, but if no pre-transform", HATS_COMBINED_COLOR, DEFAULT_HINT_TEXT_SIZE);
	text_transformed_v[5] = text_create(v_t_panel_top_left.x, text_transformed_v[4].pos.y + DEFAULT_HINT_TEXT_SIZE, "vector can be calculated to match, the input will round off to the nearest point.", HATS_COMBINED_COLOR, DEFAULT_HINT_TEXT_SIZE);
	
	// Numbers to draw in input vector brackets
	text_transformed_v_input[0] = text_create(text_transformed_v[1].pos.x + 37.5f, text_transformed_v[1].pos.y + TEXT_SIZE_DEFAULT / 2, "", HEADER_DEFAULT_COLOR, TEXT_SIZE_DEFAULT);
	text_transformed_v_input[1] = text_create(text_transformed_v[1].pos.x + 97.5f, text_transformed_v[1].pos.y + TEXT_SIZE_DEFAULT / 2, "", HEADER_DEFAULT_COLOR, TEXT_SIZE_DEFAULT);

	// Add post-transformed vectors text to clickables (click size hard-coded) CLICK_INDEX 10-16
	for (int v_click_index = 0; v_click_index < MAX_VECTORS_LIST; ++v_click_index) {
		v_click_current = &v_transformed_list[v_click_index];
		panel_add_clickable(v_click_current->pos.x, v_click_current->pos.y - 5, 65.0f, 100.0f, 2);
	}
	panel_add_clickable(text_transformed_v[1].pos.x, text_transformed_v[1].pos.y - 5.0f, 130.0f, TEXT_SIZE_DEFAULT + 10.0f, 2); // Add input new vector text to clickables (click size hard-coded) CLICK_INDEX 17

	//----------------------------------------------------------------------------------------------------------------
	// Others init
	// First two vectors are reserved for i-hat and j-hat
	panel_input_new_base_vector(1, 0);
	panel_input_new_base_vector(0, 1);

	clicked_index = 5; // Index of the 3rd transform vector panel
	panel_input_change_vector_init(); // Simulate click of that vector, so "Input new vector" works and changes that vector
}

void panel_render(void) {
	//----------------------------------------------------------------------------------------------------------------
	// Panel background
	draw_rect_noborder(top_left.x, top_left.y, size.x, size.y, PANEL_COLOR);

	//----------------------------------------------------------------------------------------------------------------
	// Title panel
	Text *current_title_text;
	for (int index = 0; index < TEXT_TITLE_COUNT; ++index) {
		current_title_text = &text_title[index];
		draw_text_top_left(current_title_text->text, current_title_text->pos.x, current_title_text->pos.y, current_title_text->color, current_title_text->size);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Vectors & Transformed vectors panel
	Text *current_v_text;
	for (int index = 0; index < TEXT_V_COUNT; ++index) {
		current_v_text = &text_v[index];
		draw_text_top_left(current_v_text->text, current_v_text->pos.x, current_v_text->pos.y, current_v_text->color, current_v_text->size);
	}

	Matrix *current_v, *current_t_v;
	for (int index = 0; index < MAX_VECTORS_LIST; ++index) {
		// Vectors panel
		current_v = &v_list[index];
		draw_matrix_box(current_v->pos.x, current_v->pos.y, current_v->value_x, current_v->value_y, MATRIX_BOX_HEIGHT, MATRIX_BOX_WIDTH, current_v->color, FALSE);

		// Transformed vectors panel
		current_t_v = &v_transformed_list[index];

		BOOL draw_text = v_list[index].value_x + v_list[index].value_y != 0;
		if (draw_text) { // If there's an output draw an arrow between them
			draw_line_pointed(current_v->pos.x + MATRIX_BOX_WIDTH / 2, current_v->pos.y + MATRIX_BOX_HEIGHT + MATRIX_BOX_ARROW_OFFSET, current_t_v->pos.x + MATRIX_BOX_WIDTH / 2, 
							  current_t_v->pos.y - MATRIX_BOX_ARROW_OFFSET, current_v->color, VECTOR_SIZE);
		}

		draw_matrix_box(current_t_v->pos.x, current_t_v->pos.y, current_t_v->value_x, current_t_v->value_y, MATRIX_BOX_HEIGHT, MATRIX_BOX_WIDTH, current_t_v->color, draw_text);
	}

	// The text part
	for (int index = 0; index < TEXT_TRANSFORMED_V_COUNT; ++index) {
		current_v_text = &text_transformed_v[index];
		draw_text_top_left(current_v_text->text, current_v_text->pos.x, current_v_text->pos.y, current_v_text->color, current_v_text->size);
	}
	for (int index = 0; index < TEXT_TRANSFORMED_V_INPUT_COUNT; ++index) {
		current_v_text = &text_transformed_v_input[index];
		draw_text_centre(current_v_text->text, current_v_text->pos.x, current_v_text->pos.y, current_v_text->color, current_v_text->size);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Base vectors panel
	Text *current_b_v_text;
	for (int index = 0; index < TEXT_BASE_V_COUNT; ++index) {
		current_b_v_text = &text_base_v[index];
		draw_text_top_left(current_b_v_text->text, current_b_v_text->pos.x, current_b_v_text->pos.y, current_b_v_text->color, current_b_v_text->size);
	}
	draw_matrix_box(b_v_matrix.pos.x, b_v_matrix.pos.y, b_v_matrix.value_x, b_v_matrix.value_y, MATRIX_BOX_HEIGHT + 9.0f, MATRIX_BOX_WIDTH * 2.5f, b_v_matrix.color, FALSE);
	for (int index = 0; index < TEXT_BASE_V_INPUT_COUNT; ++index) {
		current_b_v_text = &text_base_v_input[index];
		draw_text_centre(current_b_v_text->text, current_b_v_text->pos.x, current_b_v_text->pos.y, current_b_v_text->color, current_b_v_text->size);
	}
}

Matrix panel_create_vector(float pos_x, float pos_y, int value_x, int value_y, CP_Color color) {
	Matrix new_matrix = { .pos.x = pos_x, .pos.y = pos_y, .value_x = value_x, .value_y = value_y, .color = color };
	return new_matrix;
}

Matrix panel_create_vector_empty(float pos_x, float pos_y, CP_Color color) {
	return panel_create_vector(pos_x, pos_y, 0, 0, color);
}

//----------------------------------------------------------------------------------------------------------------
// Creating new vectors

void panel_input_replace_vector_at(int value_x, int value_y, int index) {
	// Fill default vector matrix
	Matrix* current = &v_list[index];
	current->value_x = value_x;
	current->value_y = value_y;

	// Fill transformed vector matrix
	panel_apply_transformation(index);

	// Replace the vector for the grid
	vector_replace((float)value_x, (float)value_y, current->color);

	++v_count;
}

void panel_input_new_vector_at(int value_x, int value_y, int index) {
	// Fill default vector matrix
	Matrix *current = &v_list[index];
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

//----------------------------------------------------------------------------------------------------------------
// Transformations
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

//----------------------------------------------------------------------------------------------------------------
// Input
void panel_add_clickable(float pos_x, float pos_y, float size_x, float size_y, int max_count) {
	clickables[clickable_count] = collider_create(pos_x, pos_y, size_x, size_y, max_count);
	++clickable_count;
}

void panel_mouse_clicked(float mouse_x, float mouse_y) {
	if (clicked_index >= 0) return;

	for (int index = 0; index < clickable_count; ++index) {
		BOOL check = is_clicked(mouse_x, mouse_y, &clickables[index]);
		if (check) {
			clicked_index = index;
			break;
		}
	}

	// Initialize depending on what was clicked
	if (clicked_index <= 2) {
		panel_input_basevector_init();
	}
	else if (clicked_index <= 16) { // 3-9 is pre-transform vector, 10-16 is post-transform vector
		panel_input_change_vector_init();
	}
	else if (clicked_index == 17) { // 17 is input new vector
		panel_input_basevector_init();
	}
}

void panel_number_keyed(char number) {
	if (clicked_index < 0 || input_text_index >= MAX_INPUT_LIST) return;
	if (number == '-' && input_text_index != 0) return;

	// Do something depending on what was clicked
	if (clicked_index <= 2) { //0-2 is base vector
		panel_input_basevector_update(number);
	}
	else if (clicked_index == 17) { // 17 is input new vector
		panel_input_vector_update(number);
	}

	++input_text_index;
}

void panel_space_keyed(void) {
	if (clicked_index < 0) return;

	// Do something depending on what was clicked
	if (clicked_index <= 2) {
		panel_input_basevector_space();
	}
	else if (clicked_index == 17) { // 17 is input new vector
		panel_input_vector_space();
	}

	// Reset the input read pointer and increase space counter
	input_text_index = 0;
	for (int index = 0; index < MAX_INPUT_LIST; ++index) {
		input_text[index] = '\0';
	}

	++input_space_count;

	if (input_space_count >= input_max_count) {
		panel_clear_input();
	}
}

void panel_clear_input(void) {
	// Reset based on what was clicked
	if (clicked_index <= 1) {
		panel_input_basevector_reset();
	}
	else if (clicked_index == 2) { // 2 is i/j-hat input matrix
		panel_input_basevector_matrix_reset();
	}
	else if (clicked_index == 17) { // 17 is input new vector
		panel_input_vector_reset();
	}

	// Reset the input read pointer and increase space counter
	input_text_index = 0;
	for (int index = 0; index < MAX_INPUT_LIST; ++index) {
		input_text[index] = '\0';
	}

	clicked_index = -1;
	input_space_count = 0;
	input_max_count = 0;
}

//----------------------------------------------------------------------------------------------------------------
// Different clicked cases

// Case 0-2: Change base i-hat or j-hat clicked & Case 2: i/j-hat matrix clicked & Case 17 input new vector clicked
void panel_input_basevector_init(void) {
	input_max_count = clickables[clicked_index].max_input_count;
}

void panel_input_basevector_update(char number) {
	input_text[input_text_index] = number;

	// Update visually
	int index = clicked_index * 2 + input_space_count;
	text_base_v_input[index].text = input_text;
}

void panel_input_basevector_space(void) {
	int index = clicked_index * 2 + input_space_count;

	// Force a 0 is blank or just a '-'
	if (!input_text_index || (input_text[0] == '-' && (input_text_index == 1 || input_text[1] == '0'))) {
		input_text[0] = '0';
		for (int input_index = 1; input_index < MAX_INPUT_LIST; ++input_index) {
			input_text[input_index] = '\0';
		}
	}

	char *input_done = input_text_storage[clicked_index][input_space_count];
	for (int count = 0; count < MAX_INPUT_LIST; ++count) {
		input_done[count] = input_text[count];
	}

	// Update visually
	text_base_v_input[index].text = input_done;
}

// Case 0-1 exit
void panel_input_basevector_reset(void) {
	// Convert char array into a float
	CP_Vector input_converted;
	sscanf_s(input_text_storage[clicked_index][0], "%f", &input_converted.x);
	sscanf_s(input_text_storage[clicked_index][1], "%f", &input_converted.y);

	// Apply the transformation
	clicked_index == 0 ? panel_ihat_transformation(input_converted) : panel_jhat_transformation(input_converted);

	// Clear the base vector bracket visually
	int index = clicked_index * 2;
	text_base_v_input[index].text = "";
	text_base_v_input[index + 1].text = "";
}

// Case 2 exit
void panel_input_basevector_matrix_reset(void) {
	// Convert char array into a float
	CP_Vector input_converted_ihat, input_converted_jhat;
	sscanf_s(input_text_storage[clicked_index][0], "%f", &input_converted_ihat.x);
	sscanf_s(input_text_storage[clicked_index][1], "%f", &input_converted_ihat.y);

	sscanf_s(input_text_storage[clicked_index][2], "%f", &input_converted_jhat.x);
	sscanf_s(input_text_storage[clicked_index][3], "%f", &input_converted_jhat.y);

	// Apply the transformation
	panel_ihat_transformation(input_converted_ihat);
	panel_jhat_transformation(input_converted_jhat);

	// Clear the base vector bracket visually
	int index = 4;
	text_base_v_input[index].text = "";
	text_base_v_input[index + 1].text = "";
	text_base_v_input[index + 2].text = "";
	text_base_v_input[index + 3].text = "";
}

// Case 3-9 is pre-transformed vector clicked & Case 10-16 is post-transformed vector clicked
void panel_input_change_vector_init(void) {
	int index = (clicked_index - 3) % 7;

	// Prevent editting of i-hat and j-hat for the time being 
	if (index <= 1) {
		clicked_index = -1; // Reset the click index because we are done with this
		return;
	}

	input_vector_to_transform = clicked_index;

	// Update the "Input new vector" color
	CP_Color *new_color = clicked_index <= 9 ? &v_list[index].color : &v_transformed_list[index].color;

	text_transformed_v[0].color = *new_color;
	text_transformed_v[1].color = *new_color;
	text_transformed_v_input[0].color = *new_color;
	text_transformed_v_input[1].color = *new_color;

	clicked_index = -1; // Reset the click index because we are done with this
}

// Case 17 is input new vector is clicked
void panel_input_vector_update(char number) {
	input_text[input_text_index] = number;

	// Update visually
	int index = input_space_count;
	text_transformed_v_input[index].text = input_text;
}

void panel_input_vector_space(void) {
	int index = input_space_count;

	// Force a 0 is blank or just a '-'
	if (!input_text_index || (input_text[0] == '-' && (input_text_index == 1 || input_text[1] == '0'))) {
		input_text[0] = '0';
		for (int input_index = 1; input_index < MAX_INPUT_LIST; ++input_index) {
			input_text[input_index] = '\0';
		}
	}

	char* input_done = input_text_storage[clicked_index][input_space_count];
	for (int count = 0; count < MAX_INPUT_LIST; ++count) {
		input_done[count] = input_text[count];
	}

	// Update visually
	text_transformed_v_input[index].text = input_done;
}

void panel_input_vector_reset(void) {
	// Convert char array into a float
	int input_converted_x, input_converted_y;
	sscanf_s(input_text_storage[clicked_index][0], "%d", &input_converted_x);
	sscanf_s(input_text_storage[clicked_index][1], "%d", &input_converted_y);

	// TODO refactor to allow editting of transformed directly
	// TODO refactor to allow editting of transformed directly
	// TODO refactor to allow editting of transformed directly

	int index = (input_vector_to_transform - 3) % 7;
	if (input_vector_to_transform - 3 > 7) { // If the transformed vector is selected
		// Find the pre-transformed vector using Cramer's rule
		int i_x_exponent = v_transformed_list[0].value_x, j_x_exponent = v_transformed_list[1].value_x;
		int i_y_exponent = v_transformed_list[0].value_y, j_y_exponent = v_transformed_list[1].value_y;

		float pre_x = (float)(input_converted_x * j_y_exponent - input_converted_y * j_x_exponent) / (float)(i_x_exponent * j_y_exponent - i_y_exponent * j_x_exponent);
		float pre_y = (float)(i_x_exponent * input_converted_y - i_y_exponent * input_converted_x) / (float)(i_x_exponent * j_y_exponent - i_y_exponent * j_x_exponent);

		input_converted_x = (int)pre_x;
		input_converted_y = (int)pre_y;
	}

	// If vector already exist, replace instead of writing a new one
	Matrix *selected = &v_list[index];

	(selected->value_x == 0 && selected->value_y == 0) ? panel_input_new_vector_at(input_converted_x, input_converted_y, index) : panel_input_replace_vector_at(input_converted_x, input_converted_y, index);

	// Clear the base vector bracket visually;
	text_transformed_v_input[0].text = "";
	text_transformed_v_input[1].text = "";
}
