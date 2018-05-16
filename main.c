//in parser
#include <stdio.h>
#include <string.h>
#include "SPGUImanager.h"
#include "ConsoleMode.h"
int main(int argc, char** argv) {
	if((argc==2 && strcmp(argv[1],"-g")==0)){
		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			printf("ERROR: unable to init SDL: %s\n", SDL_GetError());
			return 1;
		}
		SPGuiManager* manager = spManagerCreate();
		if (manager == NULL ) {
			SDL_Quit();
			return 0;
		}
		SDL_Event event;
		while (1) {
			if (SDL_WaitEvent(&event)<0){
				printf("Error in SDL_WaitEvent\n");
				break;
			}
			if (spManagerHandleEvent(manager, &event) == SP_MANAGER_QUTT) {
				break;
			}
			spManagerDraw(manager);
		}
		spManagerDestroy(manager);
		SDL_Quit();
		return 0;
	}

	int cond = 1;
	GAME_STATE mode = SETTING;
	SPCommand command;
	SPChessGame* game = game_create();
	printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
	consuleMainLoop(&mode, &cond, game, &command);
	game_destroy(game);
	return 0;
}
