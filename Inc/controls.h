/*
@file	  grid.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     23/10/2022
@brief    This header file
______________________________________________________________________*/

#pragma once

typedef struct {
	CP_Vector pos;
	int value_x;
	int value_y;
	CP_Color color;
} Matrix;

void panel_init(void);

void panel_render(void);

Matrix panel_create_vector(float pos_x, float pos_y, int value_x, int value_y, CP_Color color);
Matrix panel_create_vector_empty(float pos_x, float pos_y, CP_Color color);

void panel_input_replace_vector_at(int value_x, int value_y, int index);
void panel_input_new_vector_at(int value_x, int value_y, int index);
void panel_input_new_vector(int value_x, int value_y);
void panel_input_new_base_vector(int value_x, int value_y);

void panel_apply_transformation(int index);
void panel_apply_all_transformations(void);

void panel_ihat_transformation(CP_Vector transformed_ihat);
void panel_jhat_transformation(CP_Vector transformed_jhat);

void panel_add_clickable(float pos_x, float pos_y, float size_x, float size_y, int max_count);
void panel_mouse_clicked(float mouse_x, float mouse_y);
void panel_number_keyed(char number);
void panel_space_keyed(void);
void panel_clear_input(void);

void panel_input_basevector_init(void);
void panel_input_basevector_update(char number);
void panel_input_basevector_space(void);
void panel_input_basevector_reset(void);
void panel_input_basevector_matrix_reset(void);
void panel_input_change_vector_init(void);
void panel_input_vector_update(char number);
void panel_input_vector_space(void);
void panel_input_vector_reset(void);
