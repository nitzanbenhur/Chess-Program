/*
 * SPGameWindow.c
 *
 *  Created on: Aug 10, 2017
 *      Author: nitzan
 */

#include "SPGameWindow.h"
#include "SimpleButton.h"
#include "TwoStateButton.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void handleRestartButtonEvenet(Widget* src, SDL_Event* event, void* Game) {
	if (src == NULL || event == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SPGameWin* gameWin = (SPGameWin*) Game;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			 SPChessRestart(gameWin->game);
			 gameWin->restart = true;
		}
	} else {
		return;
	}
}

void handleSaveButtonEvenet(Widget* src, SDL_Event* event, void* Game) {
	if (src == NULL || event == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SPGameWin* gameWin = (SPGameWin*) Game;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			gameWin->save = true;
		}
	} else {
		return;
	}
}

void handleLoadButtonEvenetGame(Widget* src, SDL_Event* event, void* Game) {
	if (src == NULL || event == NULL || Game == NULL) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			activeWin = SP_LOAD_WINDOW_ACTIVE;
		}
	} else {
		return;
	}
}

void handleUndoButtonEvenet(Widget* src, SDL_Event* event, void* Game) {
	if (src == NULL || event == NULL ) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPGameWin* gameWin = (SPGameWin*) Game;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			SPChessUndoMove(gameWin->game);
		}
	} else {
		return;
	}
}

void handleMainMenuButtonEvenet(Widget* src, SDL_Event* event, void* Game) {
	if (src == NULL || event == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SPGameWin* gameWin = (SPGameWin*) Game;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			if (!gameWin->game->GameSaved){
				int exitStatus = showExitDialog();
				if (exitStatus == 0 || exitStatus == 2){
					return;
				}
				else if (exitStatus == 1){
					activeWin = SP_MAIN_WINDOW_ACTIVE;
					return;
				}
			}
			activeWin = SP_MAIN_WINDOW_ACTIVE;
			}
		} else {
			return;
		}
}

int showExitDialog(){
	const SDL_MessageBoxButtonData buttons[] = {
	        { /* .flags, .buttonid, .text */        0, 0, "no" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
	        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
	};
	const SDL_MessageBoxColorScheme colorScheme = {
    {{ 255,   0,   0 },{   0, 255,   0 },{ 255, 255,   0 },{   0,   0, 255 },{ 255,   0, 255 }}
	};
	const SDL_MessageBoxData messageboxdata = {
	SDL_MESSAGEBOX_INFORMATION,
    NULL,"","are you sure you want to exit without saving?", SDL_arraysize(buttons), buttons,&colorScheme };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
    	printf("SDL_ShowMessageBox failed: %s\n", SDL_GetError());
    	SDL_Log("ERROR: displaying message box");
    	activeWin = SP_EXIT;
	    return 0;
    }
    return buttons[buttonid].buttonid;
}

void handleExitButtonEvenetGame(Widget* src, SDL_Event* event, void* Game) {
	if (src == NULL || event == NULL ) {
		return; //Better to return an error value
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SPGameWin* gameWin = (SPGameWin*) Game;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			if (!gameWin->game->GameSaved){
				int exitStatus = showExitDialog();
				if (exitStatus == 0 || exitStatus == 2){
					return;
				}
				else if (exitStatus == 1){
					activeWin = SP_EXIT;
					return;
				}
			}
			activeWin = SP_EXIT;
		}
	} else {
		return;
	}
}


Widget** createGameWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*) * 18);
	if (widgets == NULL ) {
		printf("Error in malloc\n");
		activeWin = SP_EXIT;
		return NULL ;
	}
	SDL_Rect restartR = { .x = 10, .y = 10, .h = 60, .w = 80 };
	SDL_Rect saveR = { .x = 10, .y = 80, .h = 60, .w = 80 };
	SDL_Rect loadR = { .x = 10, .y = 150, .h = 60, .w = 80 };
	SDL_Rect UndoR = { .x = 10, .y = 220, .h = 60, .w = 80 };
	SDL_Rect MainMenuR = { .x = 10, .y = 290, .h = 60, .w = 80 };
	SDL_Rect exitR = { .x = 10, .y = 360, .h = 60, .w = 80 };
	widgets[0] = createSimpleButton(renderer, &restartR, "./images/button_restart.bmp");
	widgets[1] = createSimpleButton(renderer, &saveR, "./images/button_save.bmp");
	widgets[2] = createSimpleButton(renderer, &loadR, "./images/button_load.bmp");
	widgets[3] = createTwoStateButton(renderer, &UndoR, "./images/button_undo.bmp"
			,"./images/button_undoB.bmp",0);
	widgets[4] = createSimpleButton(renderer, &MainMenuR, "./images/button_main-menu.bmp");
	widgets[5] = createSimpleButton(renderer, &exitR, "./images/button_exit.bmp");
	widgets[6] = createSimpleChessPiece(renderer,"./images/Pawn-W.bmp",wpawn);
	widgets[7] = createSimpleChessPiece(renderer, "./images/Pawn-B.bmp",bpawn);
	widgets[8] = createSimpleChessPiece(renderer,"./images/Knight-W.bmp",wknight);
	widgets[9] = createSimpleChessPiece(renderer, "./images/Knight-B.bmp",bknight);
	widgets[10] = createSimpleChessPiece(renderer, "./images/Queen-W.bmp",wqueen);
	widgets[11] = createSimpleChessPiece(renderer, "./images/Queen-B.bmp",bqueen);
	widgets[12] = createSimpleChessPiece(renderer, "./images/Rook-W.bmp",wrook);
	widgets[13] = createSimpleChessPiece(renderer, "./images/Rook-B.bmp",brook);
	widgets[14] = createSimpleChessPiece(renderer, "./images/Bishop-W.bmp",wbishop);
	widgets[15] = createSimpleChessPiece(renderer, "./images/Bishop-B.bmp",bbishop);
	widgets[16] = createSimpleChessPiece(renderer, "./images/King-W.bmp",wking);
	widgets[17] = createSimpleChessPiece(renderer, "./images/King-B.bmp",bking);
	widgets[0]->handleEvent = handleRestartButtonEvenet;
	widgets[1]->handleEvent = handleSaveButtonEvenet;
	widgets[2]->handleEvent = handleLoadButtonEvenetGame;
	widgets[3]->handleEvent = handleUndoButtonEvenet;
	widgets[4]->handleEvent = handleMainMenuButtonEvenet;
	widgets[5]->handleEvent = handleExitButtonEvenetGame;
	return widgets;
}

SPWindow* spGameWindowCreate() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPGameWin* data = malloc(sizeof(SPGameWin));
	if (res == NULL || data == NULL){
		printf("Error in malloc\n");
	}
	SDL_Window* window = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 700, 600, SDL_WINDOW_OPENGL);
	if (window == NULL){
		printf("Error in SDL_CreateWindow\n");
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	if (renderer == NULL){
		printf("Error in SDL_CreateRenderer\n");
	}
	Widget** widgets = createGameWidgets(renderer);
	for (int i = 0; i < 18; i++){
		if (widgets[i] == NULL){
			destroyGameWindow(res);
			activeWin = SP_EXIT;
			return NULL;
		}

	}
	if (res == NULL || data == NULL || window == NULL || renderer == NULL
			|| widgets == NULL ) {
		free(res);
		free(data);
		free(widgets);
		//We first destroy the renderer
		SDL_DestroyRenderer(renderer); //NULL safe
		SDL_DestroyWindow(window); //NULL safe
		return NULL ;
	}
	SDL_Surface* loadingSurface = SDL_LoadBMP("./images/chessboard.bmp");
	if (loadingSurface == NULL ) {
		destroyGameWindow(res);
		printf("couldn't create chessboard.bmp surface\n");
		return NULL ;
	}
	data->board = SDL_CreateTextureFromSurface(renderer,loadingSurface);
	if (data->board == NULL ) {
		SDL_FreeSurface(loadingSurface);
		destroyGameWindow(res);
		printf("couldn't create chessboard.bmp texture\n");
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	data->save = false;
	data->restart = false;
	data->game = game_create();
	data->press.var = 0;
	data->movingPiece.var = 0;
	data->numOfWidgets = 18;
	data->gameWindow = window;
	data->gameRenderer = renderer;
	data->widgets = widgets;
	res->data = (void*) data;
	res->destroyWindow = destroyGameWindow;
	res->drawWindow = drawGameWindow;
	res->handleEventWindow = handleEvenetGameWindow;
	return res;
}


void destroyGameWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPGameWin* data = (SPGameWin*) src->data;
	int i = 0;
	for (; i < 18; i++) {
		destroyWidget(data->widgets[i]);
	}
	SDL_DestroyTexture(data->board);
	game_destroy(data->game);
	free(data->widgets);
	SDL_DestroyRenderer(data->gameRenderer);
	SDL_DestroyWindow(data->gameWindow);
	free(data);
	free(src);
}


void handleEvenetGameWindow(SPWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL){
		return;
	}
	if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
		activeWin = SP_EXIT;
		return;
	}
	SPGameWin* data = (SPGameWin*)src->data;
	if (event->motion.x >= 100){
		MOVE_RET ret = handleGameEvenet(data, event);
		drawGameWindow(src);
		if (ret == TIE ){
			if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
					"Game Over","The game is tied\n", NULL )<0){
				printf("Error in SDL_ShowSimpleMessageBox\n");
			}
			activeWin = SP_EXIT;
		}
		else if (ret == BLACK_MATE ){
			if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
					"Game Over","Checkmate! white player wins the game\n", NULL )<0){
				printf("Error in SDL_ShowSimpleMessageBox\n");
			}
			activeWin = SP_EXIT;
		}
		else if (ret == WHITE_MATE){
			if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
					"Game Over","Checkmate! black player wins the game\n", NULL )<0){
				printf("Error in SDL_ShowSimpleMessageBox\n");
			}
			activeWin = SP_EXIT;
		}
		return;
	}
	for(int i =0;i<6;i++){
		data->widgets[i]->handleEvent(data->widgets[i],event,data);
	}
	return;
}

MOVE_RET handleGameEvenet(SPGameWin* src, SDL_Event* event){
	switch(event->type){
	case SDL_MOUSEBUTTONDOWN:
	if (event->motion.x >= 100){
		src->press.var = 1;
		src->press.x = event->motion.x;
		src->press.y = event->motion.y;
	}
	break;
	case SDL_MOUSEBUTTONUP:
	if (src->press.var){
		SPCommand com;
		int x = 600/8;
		com.arg = 	(int) 7-src->press.y/x;
		com.arg2 =  (int) (src->press.x-100)/x;
		com.arg3 =  (int) 7-(event->button.y/x);
		com.arg4 =  (int) (event->button.x-100)/x;
		com.overriddenpiece = src->game->gameBoard[com.arg3][com.arg4];
		com.cmd = MOVE;
        MOVE_RET ret;
        if(getcolor(src->game,com.arg,com.arg2) == src->game->currentPlayer
        		&& getcolor(src->game,com.arg3,com.arg4) == src->game->currentPlayer){
        	if (get_piece_type(src->game,com.arg,com.arg2) == KING
        			&& get_piece_type(src->game,com.arg3,com.arg4) == ROOK &&
        			isLeagleCastelingMove(src->game,com.arg3,com.arg4,src->game->currentPlayer)){
        		ret=SPCastelingMove(src->game,com.arg3,com.arg4);
        	}
        	if (get_piece_type(src->game,com.arg,com.arg2) == ROOK
        			&& get_piece_type(src->game,com.arg3,com.arg4) == KING &&
        			isLeagleCastelingMove(src->game,com.arg,com.arg2,src->game->currentPlayer)){
        		ret=SPCastelingMove(src->game,com.arg,com.arg2);
        	}
        }else{
		    ret = SPChessMovePiece(src->game,&com,true);
        }
		switch(ret){
		case TIE:
			return TIE;
			break;
		case BLACK_MATE:
			return BLACK_MATE;
			break;
		case WHITE_MATE:
			return WHITE_MATE;
			break;
		default:
			break;
		}
		src->press.var = 0;
		src->movingPiece.var = 0;
	break;
	case SDL_MOUSEMOTION:
		if (src->press.var){
			src->movingPiece.x = event->motion.x;
			src->movingPiece.y = event->motion.y;
			src->movingPiece.var = 1;
		}
	break;
	}
	}
	return VALID_MOVE;
}

void pawnPromotion(SPChessGame* game, int x, int y){
	int piece = pawnPromotionDialog();
	switch(piece){
	case 0:
		if ((game->currentPlayer == BLACK && game->mode == ONE_PLAYER) ||
				(game->currentPlayer == WHITE && game->mode == TWO_PLAYERS)){
			game->gameBoard[x][y] = bpawn;
		}
		else {
			game->gameBoard[x][y] = wpawn;
		}
		break;
	case 1:
		if ((game->currentPlayer == BLACK && game->mode == ONE_PLAYER) ||
				(game->currentPlayer == WHITE && game->mode == TWO_PLAYERS)){
			game->gameBoard[x][y] = bbishop;
		}
		else {
			game->gameBoard[x][y] = wbishop;
		}
		break;
	case 2:
		if ((game->currentPlayer == BLACK && game->mode == ONE_PLAYER) ||
				(game->currentPlayer == WHITE && game->mode == TWO_PLAYERS)){
			game->gameBoard[x][y] = bknight;
		}
		else {
			game->gameBoard[x][y] = wknight;
		}
		break;
	case 3:
		if ((game->currentPlayer == BLACK && game->mode == ONE_PLAYER) ||
				(game->currentPlayer == WHITE && game->mode == TWO_PLAYERS)){
			game->gameBoard[x][y] = brook;
		}
		else {
			game->gameBoard[x][y] = wrook;
		}
		break;
	case 4:
		if ((game->currentPlayer == BLACK && game->mode == ONE_PLAYER) ||
				(game->currentPlayer == WHITE && game->mode == TWO_PLAYERS)){
			game->gameBoard[x][y] = bqueen;
		}
		else {
			game->gameBoard[x][y] = wqueen;
		}
		break;
	}
}


int pawnPromotionDialog(){
	const SDL_MessageBoxButtonData buttons[] = {
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "pawn" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "bishop" },
	        { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "knight" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 3, "rook" },
	        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 4, "queen" },
	};
	const SDL_MessageBoxColorScheme colorScheme = {
    {{ 255,   0,   0 },{   0, 255,   0 },{ 255, 255,   0 },{   0,   0, 255 },{ 255,   0, 255 }}
	};
	const SDL_MessageBoxData messageboxdata = {
	SDL_MESSAGEBOX_INFORMATION,
    NULL,"","Pawn promotion - Choose Piece", SDL_arraysize(buttons), buttons,&colorScheme };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
    	printf("SDL_ShowMessageBox failed: %s\n", SDL_GetError());
    	SDL_Log("error displaying message box");
    	activeWin = SP_EXIT;
	    return 0;
    }
    return buttons[buttonid].buttonid;
}

void drawGameWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SDL_Rect boardR = { .x = 100, .y = 0, .h = 600, .w = 600 };
	SPGameWin* data = (SPGameWin*) src->data;
	//Draw window
	if (SDL_SetRenderDrawColor(data->gameRenderer, 255, 255, 255, 255)<0){
		printf("Error in SDL_SetRenderDrawColor\n");
		activeWin = SP_EXIT;
		return;
	}
	if (SDL_RenderClear(data->gameRenderer)<0){
		printf("Error in SDL_RenderClear\n");
		activeWin = SP_EXIT;
		return;
	}
	if (SDL_RenderCopy(data->gameRenderer, data->board, NULL, &boardR)<0){
		printf("Error in SDL_RenderCopy\n");
		activeWin = SP_EXIT;
		return;
	}
	TwoStateButton* but = (TwoStateButton*) data->widgets[3]->data;
	if (data->game->mode == TWO_PLAYERS || spArrayListSize(data->game->hist) < 2){
		but->buttonOn = 0;
	}
	else {
		but->buttonOn = 1;
	}
	for (int i = 0; i < 6; i++) {
		(data->widgets[i])->drawWidget(data->widgets[i],NULL);
	}
	drawGameBoardWindow(data);
	SDL_RenderPresent(data->gameRenderer);
}


void drawGameBoardWindow(SPGameWin* gameWin) {
	int z = 600/8;
	int x = (int) 7-gameWin->press.y/z;
	int y =  (int) (gameWin->press.x-100)/z;
	if (gameWin->game->mode == ONE_PLAYER && gameWin->press.var &&
			getcolor(gameWin->game,x,y)==gameWin->game->currentPlayer
			&& (gameWin->game->diff < 3)){
		drawmoves(gameWin,x,y);
	}
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			for (int k = 6; k < 18; k++){
				Piece_Pos data;
				if (gameWin->press.var && gameWin->movingPiece.var && i == x && j == y){
					data.piece = gameWin->game->gameBoard[i][j];
					SDL_Rect rec = { .x = gameWin->movingPiece.x-z/2,
							.y = gameWin->movingPiece.y-z/2, .h = 600/8, .w = 600/8 };
					data.pos = rec;
					drawSimpleChessPiece(gameWin->widgets[k],&data);
				}
				else {
					data.piece = gameWin->game->gameBoard[i][j];
					SDL_Rect rec = { .x = 100 + (600/8)*j, .y = (600/8)*(7-i), .h = 600/8, .w = 600/8 };
					data.pos = rec;
					drawSimpleChessPiece(gameWin->widgets[k],&data);
				}
			}
		}
	}
}

void drawmoves(SPGameWin* gameWin, int x, int y){
	drawCastelingMoves(gameWin,x,y);
	int moves[64];
	get_moves(gameWin->game,x,y,moves);
	int index = 0;
	//SDL_Surface* loadingSurface;
	while (moves[index] != -1){
		int i = moves[index]%8;
		int j = moves[index]/8;
		SDL_Rect rec =  { .x = 100 + (600/8)*i, .y = (600/8)*(7-j), .h = 600/8, .w = 600/8 };
		// Set render color to blue ( rect will be rendered in this color )
		if (try_move_threatened(gameWin->game,x,y,j,i)){
			SDL_SetRenderDrawColor( gameWin->gameRenderer, 255, 0, 0, 255 );
		}
		else if (gameWin->game->currentPlayer != getcolor(gameWin->game,moves[index]/8,moves[index]%8)
			&& GREY != getcolor(gameWin->game,moves[index]/8,moves[index]%8)){
			SDL_SetRenderDrawColor( gameWin->gameRenderer, 0, 255, 0, 255 );
		}
		else {
			SDL_SetRenderDrawColor( gameWin->gameRenderer, 255, 255,0, 255 );
		}
		// Render rect
		SDL_RenderFillRect( gameWin->gameRenderer, &rec );
		index++;
	}
}

void drawCastelingMoves(SPGameWin* src, int x, int y){
	SDL_SetRenderDrawColor( src->gameRenderer, 128,0,128, 255 );
	if (get_piece_type(src->game,x,y) == ROOK &&
			isLeagleCastelingMove(src->game,x,y,src->game->currentPlayer)){
		int pos = getPiecePos(src->game,src->game->currentPlayer, KING);
		int i = pos/8;
		int j = pos%8;
		SDL_Rect rec =  { .x = 100 + (600/8)*j, .y = (600/8)*(7-i), .h = 600/8, .w = 600/8 };
		SDL_RenderFillRect( src->gameRenderer, &rec );
	}
	if (get_piece_type(src->game,x,y) == KING){
		if (getcolor(src->game,x,y) == BLACK){
			if (isLeagleCastelingMove(src->game,7,0,src->game->currentPlayer)){
				SDL_Rect rec =  { .x = 100 + (600/8)*0, .y = (600/8)*(7-7), .h = 600/8, .w = 600/8 };
				SDL_RenderFillRect( src->gameRenderer, &rec );
			}
			if (isLeagleCastelingMove(src->game,7,7,src->game->currentPlayer)){
				SDL_Rect rec =  { .x = 100 + (600/8)*7, .y = (600/8)*(7-7), .h = 600/8, .w = 600/8 };
				SDL_RenderFillRect( src->gameRenderer, &rec );
			}
		}
		else {
			if (isLeagleCastelingMove(src->game,0,0,src->game->currentPlayer)){
				SDL_Rect rec =  { .x = 100 + (600/8)*0, .y = (600/8)*(7-0), .h = 600/8, .w = 600/8 };
				SDL_RenderFillRect( src->gameRenderer, &rec );
			}
			if (isLeagleCastelingMove(src->game,0,7,src->game->currentPlayer)){
				SDL_Rect rec =  { .x = 100 + (600/8)*7, .y = (600/8)*(7-0), .h = 600/8, .w = 600/8 };
				SDL_RenderFillRect( src->gameRenderer, &rec );
			}
		}
	}

}

