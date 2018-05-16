/*
 * SPmanager.h
 *
 *  Created on: Aug 9, 2017
 *      Author: nitzan
 */

#ifndef SPMANAGER_H_
#define SPMANAGER_H_
#include <SDL.h>
#include <SDL_video.h>
#include "SPMainWindow.h"
#include "SPLoadWindow.h"
#include "GameModeWindow.h"
#include "SPDifficultyWindow.h"
#include "SPColorWindow.h"
#include "SPGameWindow.h"
#include "SPChessGame.h"
#include "LoadStore.h"
#include <stdbool.h>

typedef enum{
	SP_MANAGER_QUTT,
	SP_MANAGER_NONE,
}SP_MANAGER_EVENET;



typedef struct {
	SPWindow* gameWin;
	SPWindow* mainWin;
	SPWindow* loadWin;
	SPWindow* modeWin;
	SPWindow* diffWin;
	SPWindow* colWin;
	bool firstMove;
	int GameMode;
	int GameCol;
	int GameDiff;
	int numOfsavedGames;
	ACTIVE_WINDOW activeWin;
} SPGuiManager;




void moveSave(SPGuiManager* src);
/**
 * create a gui manager
 * @return the gui manager
 */
SPGuiManager* spManagerCreate();
/**
 * frees the gui manager and destroys it
 * @param src gui manager to be destroyed
 */
void spManagerDestroy(SPGuiManager* src);
/**
 * draw the window and all its button
 * @param src the gui manager
 */
void spManagerDraw(SPGuiManager* src);
/**
 * handles a game event
 * @param src the manager
 * @param event the event
 * @return SP_MANAGER_QUTT if exit, else SP_MANAGER_NONE
 */
SP_MANAGER_EVENET spManagerHandleEvent(SPGuiManager* src, SDL_Event* event);
/**
 * handles a game event
 * @param src gui manager
 * @param event sdl event
 */
void handleGameEvent(SPGuiManager* src, SDL_Event* event);
/**
 * handles a load event
 * @param src gui manager
 * @param event the event
 */
void handleLoadEvent(SPGuiManager* src, SDL_Event* event);

#endif /* SPMANAGER_H_ */
