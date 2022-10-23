/*!
@file	  main.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     14/09/2022
@brief    This source file 
______________________________________________________________________*/

#include "cprocessing.h"
#include "grid.h"
#include "controls.h"
#include "vectors.h"

#define BLACK CP_Color_Create(0, 0, 0, 255)
#define WHITE CP_Color_Create(255, 255, 255, 255)

void game_init(void) {
	//CP_System_SetWindowSize(CP_System_GetDisplayWidth(), CP_System_GetDisplayHeight() - 100);
	CP_System_Fullscreen();

	grid_init();
	vectors_init();
	panel_init();
}


void game_update(void) {
	if (CP_Input_KeyTriggered(KEY_W)) {
		panel_ihat_transformation(CP_Vector_Set(3, 1));
	}
	if (CP_Input_KeyTriggered(KEY_E)) {
		panel_jhat_transformation(CP_Vector_Set(0, -2));
	}
	if (CP_Input_KeyTriggered(KEY_R)) {
		panel_input_new_vector_at(2, 3, 2);
	}

	CP_Graphics_ClearBackground(BLACK);
	grid_render();
	grid_animate();
	// grid_point_render(); // For debugging
	vectors_render();
	panel_render();
}

void game_exit(void) {

}

int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}
