/*
@file	  vectors.h
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     23/10/2022
@brief    This header file
______________________________________________________________________*/

#pragma once

typedef struct {
	CP_Vector pos;
	CP_Color color;
} Vector;

void vectors_init(void);

void vectors_render(void);

void vector_replace(float x, float y, CP_Color color);
void vector_create(float x, float y, CP_Color color);
