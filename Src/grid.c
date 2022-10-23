/*!
@file	  grid.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     23/10/2022
@brief    This source file 
______________________________________________________________________*/

#include "cprocessing.h"
#include "grid.h"
#include "utils.h"

// Grid settings
#define SIZE_X 31
#define SIZE_Y 31
#define MAJOR_GAP 5 // Every X lines, draw a major line

// Size settings
#define POINT_SIZE 10.0f
#define BG_LINE_SIZE 1.0f
#define LINE_SIZE 2.0f
#define MAJOR_LINE_SIZE 3.0f

// Animation settings
#define TRANSITION_TIME 2.0f // How long should the scaling animation take?

// Color settings
#define ORIG_GRID_COLOR CP_Color_Create(15, 103, 153, 255)
#define ORIG_MAJOR_COLOR CP_Color_Create(20, 132, 193, 255)
#define ORIG_AXIS_COLOR CP_Color_Create(255, 255, 255, 255)
#define BG_GRID_COLOR CP_Color_Create(127, 127, 127, 255)

// The bounds of the grid (Currently set to top left of window to centre of window)
#define EDGE_BUFFER 0 // Padding against the corners of the screen
CP_Vector top_left, bot_right;

// Storing the actual grids
Point grid[SIZE_X][SIZE_Y];
Point bg_grid[SIZE_X][SIZE_Y];
CP_Vector scale_start_grid[SIZE_X][SIZE_Y];
CP_Vector scale_end_grid[SIZE_X][SIZE_Y];

// The index of the centre line of the grid
int centre_x, centre_y;
CP_Vector centre_screen_pos;

// The spacing between each point
float spacing;

// Flag for playing transformation animation
BOOL animation;
float elapsed_time = 0.0f;
float lerp_factor;

void grid_init() {
	// The spacing between each point (The whole length is the height of the screen minus the 2 buffers at the top and bottom) divided by the number of points 
	spacing = (float)(CP_System_GetWindowHeight() - EDGE_BUFFER * 2) / (SIZE_Y - 1);
	// The screen position of the top left corner of the grid, it's just the buffer.
	top_left = CP_Vector_Set(EDGE_BUFFER, EDGE_BUFFER);

	// Set the centre line indexes for both axis
	centre_x = SIZE_X / 2;
	centre_y = SIZE_Y / 2;

	Point current_point;
	for (int index_x = 0; index_x < SIZE_X; ++index_x) {
		for (int index_y = 0; index_y < SIZE_Y; ++index_y) {
			current_point.x = -(centre_x - index_x);
			current_point.y = centre_y - index_y;
			current_point.screen_x = top_left.x + index_x * spacing;
			current_point.screen_y = top_left.y + index_y * spacing;

			grid[index_x][index_y] = current_point;
			bg_grid[index_x][index_y] = current_point;
		}
	}

	centre_screen_pos = CP_Vector_Set(grid[centre_x][centre_y].screen_x, grid[centre_x][centre_y].screen_y);
}

// Draws the grid lines
void grid_render(void) {
	Point *current_top, *current_bot;
	CP_Color draw_color;
	float draw_thickness;

	//----------------------------------------------------------------------------------------------------------------
	// Draw BG grid
	for (int index_x = 0; index_x < SIZE_X; ++index_x) {
		current_top = &bg_grid[index_x][0];
		current_bot = &bg_grid[index_x][SIZE_Y - 1];
		
		draw_thickness = (index_x % MAJOR_GAP) ? BG_LINE_SIZE : LINE_SIZE;
		draw_line(current_top->screen_x, current_top->screen_y, current_bot->screen_x, current_bot->screen_y, BG_GRID_COLOR, draw_thickness);
	}

	for (int index_y = 0; index_y < SIZE_Y; ++index_y) {
		current_top = &bg_grid[0][index_y];
		current_bot = &bg_grid[SIZE_X - 1][index_y];

		draw_thickness = (index_y % MAJOR_GAP) ? BG_LINE_SIZE : LINE_SIZE;
		draw_line(current_top->screen_x, current_top->screen_y, current_bot->screen_x, current_bot->screen_y, BG_GRID_COLOR, draw_thickness);
	}

	//----------------------------------------------------------------------------------------------------------------
	// Draw orig grid
	for (int index_x = 0; index_x < SIZE_X; ++index_x) {
		current_top = &grid[index_x][0];
		current_bot = &grid[index_x][SIZE_Y - 1];

		draw_color = (index_x == centre_x) ? ORIG_AXIS_COLOR : (index_x % MAJOR_GAP) ? ORIG_GRID_COLOR : ORIG_MAJOR_COLOR;
		draw_thickness = (index_x % MAJOR_GAP) ? LINE_SIZE : MAJOR_LINE_SIZE;
		draw_line(current_top->screen_x, current_top->screen_y, current_bot->screen_x, current_bot->screen_y, draw_color, draw_thickness);
	}

	for (int index_y = 0; index_y < SIZE_Y; ++index_y) {
		current_top = &grid[0][index_y];
		current_bot = &grid[SIZE_X - 1][index_y];

		draw_color = (index_y == centre_y) ? ORIG_AXIS_COLOR : (index_y % MAJOR_GAP) ? ORIG_GRID_COLOR : ORIG_MAJOR_COLOR;
		draw_thickness = (index_y % MAJOR_GAP) ? LINE_SIZE : MAJOR_LINE_SIZE;
		draw_line(current_top->screen_x, current_top->screen_y, current_bot->screen_x, current_bot->screen_y, draw_color, draw_thickness);
	}
}

// Draws the grid points, mainly for debugging
void grid_point_render(void) {
	Point* current_point;

	for (int index_x = 0; index_x < SIZE_X; ++index_x) {
		for (int index_y = 0; index_y < SIZE_Y; ++index_y) {

			current_point = &grid[index_x][index_y];

			CP_Settings_NoStroke();

			(index_x == centre_x && index_y == centre_y-1) ? CP_Settings_Fill(CP_Color_Create(255, 0, 0, 255)) : CP_Settings_Fill(ORIG_GRID_COLOR);
 			CP_Graphics_DrawCircle(current_point->screen_x, current_point->screen_y, POINT_SIZE);

			//char buffer[50] = { 0 };
			//sprintf_s(buffer, _countof(buffer), "%d,%d", current_point->x, current_point->y);
			//char buffer2[50] = { 0 };
			//sprintf_s(buffer2, _countof(buffer), "%.0f,%.0f", current_point->screen_x, current_point->screen_y);

			CP_Settings_Fill(ORIG_AXIS_COLOR);
			CP_Settings_TextSize(15.0f);
			//CP_Font_DrawText(buffer, current_point->screen_x, current_point->screen_y);
			//CP_Font_DrawText(buffer2, current_point->screen_x, current_point->screen_y + 15);
		}
	}
}

void grid_scale(CP_Vector transformed_ihat, CP_Vector transformed_jhat) {
	Point *current_point, *original_point;
	
	for (int index_x = 0; index_x < SIZE_X; ++index_x) {
		for (int index_y = 0; index_y < SIZE_Y; ++index_y) {
			current_point = &grid[index_x][index_y];
			original_point = &bg_grid[index_x][index_y];

			// Save the point positions before transformation to the transformation animation grid
			scale_start_grid[index_x][index_y].x = current_point->screen_x;
			scale_start_grid[index_x][index_y].y = current_point->screen_y;

			current_point->x = original_point->x * (int)transformed_ihat.x + original_point->y * (int)transformed_jhat.x;
			current_point->y = original_point->x * (int)transformed_ihat.y + original_point->y * (int)transformed_jhat.y;

			// Save the final result to the transformation animation grid
			scale_end_grid[index_x][index_y].x = centre_screen_pos.x + current_point->x * spacing;
			scale_end_grid[index_x][index_y].y = centre_screen_pos.y + -current_point->y * spacing;
		}
	}

	// Start the transformation animation
	animation = TRUE;
}

// Animates the transformation rotation using lerps
void grid_animate(void) {
	if (!animation) return;

	elapsed_time += CP_System_GetDt();

	lerp_factor = elapsed_time / TRANSITION_TIME;
	lerp_factor = lerp_factor * lerp_factor * (3.0f - 2.0f * lerp_factor);

	Point *current_point;
	CP_Vector *start_position, *end_position;
	for (int index_x = 0; index_x < SIZE_X; ++index_x) {
		for (int index_y = 0; index_y < SIZE_Y; ++index_y) {
			current_point = &grid[index_x][index_y];
			start_position = &scale_start_grid[index_x][index_y];
			end_position = &scale_end_grid[index_x][index_y];

			current_point->screen_x = CP_Math_LerpFloat(start_position->x, end_position->x, lerp_factor);
			current_point->screen_y = CP_Math_LerpFloat(start_position->y, end_position->y, lerp_factor);
		}
	}

	if (elapsed_time > TRANSITION_TIME) {
		elapsed_time = 0.0f;
		animation = FALSE;
	}
}

float grid_get_size(void) {
	return (float)CP_System_GetWindowHeight() - EDGE_BUFFER * 2.0f;
}

CP_Vector grid_get_screenpos(int grid_x, int grid_y) {
	Point *current_point = &grid[centre_x + grid_x + 1][-grid_y - centre_y - 1];

	CP_Vector screen_pos = {.x = current_point->screen_x, .y = current_point->screen_y};
	return screen_pos;
}

CP_Vector grid_get_screenpos_origin(void) {
	return grid_get_screenpos(0, 0);
}
