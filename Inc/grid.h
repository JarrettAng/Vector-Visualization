/*
@file	  grid.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     27/09/2022
@brief    This header file
______________________________________________________________________*/

#pragma once

typedef struct {
	int x, y;
	float screen_x, screen_y;
} Point;

void grid_init(void);

void grid_render(void);
void grid_point_render(void);

void grid_scale(CP_Vector transformed_ihat, CP_Vector transformed_jhat);

void grid_animate(void);

float grid_get_size(void);

CP_Vector grid_get_screenpos(int grid_x, int grid_y);
CP_Vector grid_get_screenpos_origin(void);
