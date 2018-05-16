/*
 * SPGUImanager.c
 *
 *  Created on: Aug 12, 2017
 *      Author: nitzan
 */
#include "SPGUImanager.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SPGuiManager* spManagerCreate(){
	SPGuiManager* res = (SPGuiManager*) malloc(sizeof(SPGuiManager));
	if (res == NULL ) {
		printf("Error in malloc\n");
		spManagerDestroy(res);
		activeWin = SP_EXIT;
		return NULL ;
	}
	res->mainWin = spMainWindowCreate();
	if (res->mainWin == NULL ) {
		spManagerDestroy(res);
		free(res);
		return NULL ;
	}
	res->firstMove = true;
	res->GameMode = 1;
	res->GameDiff = 2;
	res->GameCol = 1;
	res->numOfsavedGames = 0;
	for (int i = 1; i < 6; i++){
		char slot[30];
		sprintf(slot,"slot%d",i);
		FILE *fp=fopen(slot,"r");
		if (fp != NULL){
			res->numOfsavedGames = i;
			fclose(fp);
		}
	}
	res->diffWin = NULL;
	res->colWin = NULL;
	res->gameWin = NULL;
	res->modeWin= NULL;
	res->loadWin = NULL;
	activeWin = SP_MAIN_WINDOW_ACTIVE;
	prevWin = SP_MAIN_WINDOW_ACTIVE;
	return res;
}

void spManagerDestroy(SPGuiManager* src){
	if (src == NULL) {
		return;
	}
	if (src->modeWin != NULL) {
		destroyModeWindow(src->modeWin);
	}
	if (src->diffWin != NULL) {
		destroyDiffWindow(src->diffWin);
	}
	if (src->gameWin != NULL) {
		destroyGameWindow(src->gameWin);
	}
	if (src->colWin != NULL) {
		destroyColWindow(src->colWin);
	}
	if (src->loadWin != NULL) {
		destroyLoadWindow(src->loadWin);
	}
	if (src->mainWin != NULL) {
		destroyMainWindow(src->mainWin);
	}
	free(src);
}

void spManagerDraw(SPGuiManager* src){
	if (!src) {
		return;
	}
	if (activeWin == SP_MAIN_WINDOW_ACTIVE) {
		if (src->mainWin == NULL){
			src->mainWin = spMainWindowCreate();
		}
		drawMainWindow(src->mainWin);
	}
	if (activeWin == SP_MODE_WINDOW_ACTIVE) {
		if (src->modeWin == NULL){
			src->modeWin = spModeWindowCreate();
		}
		drawModeWindow(src->modeWin);
	}
	if (activeWin == SP_DIFFICULTY_WINDOW_ACTIVE) {
		if (src->diffWin == NULL){
			src->diffWin = spDiffWindowCreate();
		}
		drawDiffWindow(src->diffWin);
	}
	if (activeWin == SP_COLOR_WINDOW_ACTIVE) {
		if (src->colWin == NULL){
			src->colWin = spColWindowCreate();
		}
		drawColWindow(src->colWin);
	}
	if (activeWin == SP_LOAD_WINDOW_ACTIVE) {
		if (src->loadWin == NULL){
			src->loadWin = spLoadWindowCreate();
			SPLoadWin* set = (SPLoadWin*)(src->loadWin->data);
			set->numOfsavedGames = src->numOfsavedGames;
		}
		drawLoadWindow(src->loadWin);
	}
	if  (activeWin == SP_GAME_WINDOW_ACTIVE) {
		if (src->gameWin == NULL){
			src->gameWin = spGameWindowCreate();
			SPGameWin* gameWin = (SPGameWin*)(src->gameWin->data);
			gameWin->game->diff = src->GameDiff;
			gameWin->game->mode = src->GameMode;
			gameWin->game->UserCol = src->GameCol;
			if (gameWin->game->mode == ONE_PLAYER && gameWin->game->UserCol == BLACK && src->firstMove
					&& gameWin->game->currentPlayer == WHITE){
				SPComputerMove(gameWin->game,true);
				src->firstMove = false;
			}
		}
		drawGameWindow(src->gameWin);
	}
}


SP_MANAGER_EVENET spManagerHandleEvent(SPGuiManager* src, SDL_Event* event) {
	if (src == NULL || event == NULL ) {
		return SP_MANAGER_NONE;
	}else if (activeWin == SP_MAIN_WINDOW_ACTIVE) {
		if (src->mainWin == NULL){
			src->mainWin = spMainWindowCreate();
		}
		handleEvenetMainWindow(src->mainWin, event);
		if (activeWin != SP_MAIN_WINDOW_ACTIVE) {
			destroyMainWindow(src->mainWin); src->mainWin = NULL;
		}
	}else if (activeWin == SP_MODE_WINDOW_ACTIVE) {
		if (src->modeWin == NULL){
			src->modeWin = spModeWindowCreate();
		}
		handleEvenetModeWindow(src->modeWin, event);
		SPModeWin* mode = (SPModeWin*)(src->modeWin->data);
		src->GameMode = mode->onePlayer;
		if (activeWin != SP_MODE_WINDOW_ACTIVE) {
			destroyModeWindow(src->modeWin); src->modeWin = NULL;
		}
	}else if (activeWin == SP_GAME_WINDOW_ACTIVE) {
		handleGameEvent(src,event);
	}else if (activeWin == SP_DIFFICULTY_WINDOW_ACTIVE) {
		if (src->diffWin == NULL){
			src->diffWin = spDiffWindowCreate();
		}
		handleEvenetDiffWindow(src->diffWin,event);
		SPDiffWin* diff = (SPDiffWin*)(src->diffWin->data);
		src->GameDiff = diff->diff;
		if (activeWin != SP_DIFFICULTY_WINDOW_ACTIVE) {
			destroyDiffWindow(src->diffWin); src->diffWin = NULL;
		}
	}else if (activeWin == SP_COLOR_WINDOW_ACTIVE) {
		if (src->colWin == NULL){
			src->colWin = spColWindowCreate();
		}
		handleEvenetColWindow(src->colWin,event);
		SPColWin* col = (SPColWin*)(src->colWin->data);
		src->GameCol = col->UserCol;
		if (activeWin != SP_COLOR_WINDOW_ACTIVE) {
			destroyColWindow(src->colWin); src->colWin = NULL;
		}
	}
	else if (activeWin == SP_LOAD_WINDOW_ACTIVE) {
		handleLoadEvent(src, event);
	}else if (activeWin == SP_EXIT){
		return SP_MANAGER_QUTT;}
	return SP_MANAGER_NONE;
}

void handleGameEvent(SPGuiManager* src, SDL_Event* event){
	SPGameWin* gameWin;
	if (src->gameWin == NULL){
		src->gameWin = spGameWindowCreate();
		if (src->gameWin == NULL ) {
			return;
		}
	}
	gameWin = (SPGameWin*)(src->gameWin->data);
	gameWin->game->diff = src->GameDiff;
	gameWin->game->mode = src->GameMode;
	gameWin->game->UserCol = src->GameCol;
	if (gameWin->game->mode == ONE_PLAYER && gameWin->game->UserCol == BLACK && src->firstMove
			&& gameWin->game->currentPlayer == WHITE){
		SPComputerMove(gameWin->game,true);
		src->firstMove = false;
	}
	handleEvenetGameWindow(src->gameWin, event);
	if (gameWin->save){
		moveSave(src);
		char slot[30];
		sprintf(slot,"slot%d",1);
		SPCommand com;
		strcpy(com.file,slot);
		save(&com,gameWin->game);
		if (src->numOfsavedGames < 5){
			src->numOfsavedGames++;
		}
		gameWin->save = false;
	}
	if (gameWin->restart){
		src->firstMove = true;
		gameWin->restart = false;
	}
	if (activeWin == SP_LOAD_WINDOW_ACTIVE){
		SPCommand comSave;
		prevWin = SP_GAME_WINDOW_ACTIVE;
		strcpy(comSave.file,"backToGame");
		save(&comSave,gameWin->game);
	}
	if (activeWin != SP_GAME_WINDOW_ACTIVE) {
		destroyGameWindow(src->gameWin);
		src->gameWin = NULL;
		src->firstMove = true;
	}
}


void moveSave(SPGuiManager* src){
	for (int i = src->numOfsavedGames; i >= 1; i--){
		if (i == 5){
			continue;
		}
		char slotin[30];
		char slotout[30];
		sprintf(slotin,"slot%d",i);
		SPCommand com;
		SPChessGame game;
		strcpy(com.file,slotin);
		load(&com,&game);
		sprintf(slotout,"slot%d",i+1);
		strcpy(com.file,slotout);
		save(&com,&game);
	}
}

void handleLoadEvent(SPGuiManager* src, SDL_Event* event){
	if (src->loadWin == NULL){
		src->loadWin = spGameWindowCreate();
		if (src->loadWin == NULL ) {
			return;
		}
		SPLoadWin* set = (SPLoadWin*)(src->loadWin->data);
		set->numOfsavedGames = src->numOfsavedGames;
	}
	SPLoadWin* loadWin = (SPLoadWin*)(src->loadWin->data);
	handleEvenetLoadWindow(src->loadWin,event);
	if (loadWin->LoadActiveted){
		if (src->gameWin == NULL){
			src->gameWin = spGameWindowCreate();
			if (src->gameWin == NULL ) {
				return;
			}
		}
		SPGameWin* gameWin = (SPGameWin*)(src->gameWin->data);
		char slot[30];
		sprintf(slot,"slot%d",loadWin->slotPressed);
		SPCommand com;
		strcpy(com.file,slot);
		load(&com,gameWin->game);
		src->GameCol = gameWin->game->UserCol;
		src->GameDiff = gameWin->game->diff;
		src->GameMode = gameWin->game->mode;
		activeWin = SP_GAME_WINDOW_ACTIVE;
		loadWin->slotPressed = 0;
		loadWin->LoadActiveted = 0;
	}
	if (loadWin->backToGame){
		if (src->gameWin == NULL){
			src->gameWin = spGameWindowCreate();
			if (src->gameWin == NULL ) {
				return;
			}
		}
		SPCommand com;
		SPGameWin* gameWin = (SPGameWin*)(src->gameWin->data);
		strcpy(com.file,"backToGame");
		load(&com,gameWin->game);
		src->GameCol = gameWin->game->UserCol;
		src->GameDiff = gameWin->game->diff;
		src->GameMode = gameWin->game->mode;
		loadWin->slotPressed = 0;
		loadWin->LoadActiveted = 0;
	}
	if (activeWin != SP_LOAD_WINDOW_ACTIVE) {
		src->numOfsavedGames = loadWin->numOfsavedGames;
		destroyLoadWindow(src->loadWin); src->loadWin = NULL;
	}
}

