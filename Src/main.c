/*!
@file	  main.c
@author	  Ang Jiawei Jarrett (a.jiaweijarrett)
@date     23/10/2022
@brief    This source file 
______________________________________________________________________*/

#include "cprocessing.h"
#include "grid.h"
#include "controls.h"
#include "vectors.h"

#define BLACK CP_Color_Create(0, 0, 0, 255)
#define WHITE CP_Color_Create(255, 255, 255, 255)

// Foward declarations
void process_input(void);

void game_init(void) {
	CP_System_SetWindowSize(1920, 1080);


	grid_init();
	vectors_init();
	panel_init();
}

void game_update(void) {
	process_input();

	CP_Graphics_ClearBackground(BLACK);
	grid_render();
	grid_animate();
	// grid_point_render(); // For debugging
	vectors_render();
	panel_render();
}

void process_input(void) {
	// When clicked
	if (CP_Input_MouseTriggered(MOUSE_BUTTON_1)) {
		panel_mouse_clicked(CP_Input_GetMouseX(), CP_Input_GetMouseY());
	}

	// When any number is pressed
	if (CP_Input_KeyTriggered(KEY_0)) panel_number_keyed('0');
	else if (CP_Input_KeyTriggered(KEY_1)) panel_number_keyed('1');
	else if (CP_Input_KeyTriggered(KEY_2)) panel_number_keyed('2');
	else if (CP_Input_KeyTriggered(KEY_3)) panel_number_keyed('3');
	else if (CP_Input_KeyTriggered(KEY_4)) panel_number_keyed('4');
	else if (CP_Input_KeyTriggered(KEY_5)) panel_number_keyed('5');
	else if (CP_Input_KeyTriggered(KEY_6)) panel_number_keyed('6');
	else if (CP_Input_KeyTriggered(KEY_7)) panel_number_keyed('7');
	else if (CP_Input_KeyTriggered(KEY_8)) panel_number_keyed('8');
	else if (CP_Input_KeyTriggered(KEY_9)) panel_number_keyed('9');
	else if (CP_Input_KeyTriggered(KEY_MINUS)) panel_number_keyed('-');

	// When space is pressed
	if (CP_Input_KeyTriggered(KEY_ENTER)) {
		panel_space_keyed();
	}
}

void game_exit(void) {

}

int main(void)
{
	CP_Engine_SetNextGameState(game_init, game_update, game_exit);
	CP_Engine_Run();
	return 0;
}
