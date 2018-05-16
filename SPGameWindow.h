/*
 * SPGameWindow.h
 *
 *  Created on: Aug 10, 2017
 *      Author: nitzan
 */

#ifndef SPGAMEWINDOW_H_
#define SPGAMEWINDOW_H_
#include <SDL.h>
#include <SDL_video.h>
#include "active_window.h"
#include "SPChessGame.h"
#include "SPWindow.h"
#include "Widget.h"
#include "SimpleChessPiece.h"
#include "LoadStore.h"
#include <stdbool.h>

typedef struct {
	int x;
	int y;
	int var;
}SP_GAME_PRESS;


typedef struct {
	SDL_Window* gameWindow;
	SDL_Renderer* gameRenderer;
	Widget** widgets;
	int numOfWidgets;
	SPChessGame* game;
	SP_GAME_PRESS press;
	SP_GAME_PRESS movingPiece;
	SDL_Texture* board;
	Setting* setting;
	bool save;
	bool restart;
} SPGameWin;


/*
 * draw castling moves
 * @param src the game window
 * @param x row of musebuttondown event
 * @param y column of musebottundown event
 */
void drawCastelingMoves(SPGameWin* src, int x, int y);
/*
 * handle pawn promotion
 * @param game
 * @param x is the x coordinate of the pawn we promote
 * @param y is the y coordinate of the pawn we promote
 */
void pawnPromotion(SPChessGame* game, int x, int y);
/*
 * Display pawn promotion Dialog.
 * @ret 0 user promote to pawn
 * @ret 1 user promote to bishop
 * @ret 2 user promote to knight
 * @ret 3 user promote to rook
 * ret 4 user promote to knight
 */
int pawnPromotionDialog();

/**
 * handles a game event(click, drag, etc)
 * @param src the game window
 * @param event event
 * @return VALID_MOVE/TIE/black or white MATE
 */
MOVE_RET handleGameEvenet(SPGameWin* src, SDL_Event* event);
/**
 * draws the game window
 * @param gameWin game
 */
void drawGameBoardWindow(SPGameWin* gameWin);
/**
 * creates a game window
 * @return the game window
 */
SPWindow* spGameWindowCreate();
/**
 * frees the game window
 * @param src game window
 */
void destroyGameWindow(SPWindow* src);
/**
 * handle event game
 * @param src window
 * @param event event
 */
void handleEvenetGameWindow(SPWindow* src, SDL_Event* event);
/**
 * draw the game
 * @param src game window
 */
void drawGameWindow(SPWindow* src);
/**
 * displays exit dialog
 * @return 0 if failed to display
 */
int showExitDialog();
/**
 * draw get possible moves
 * @param gameWin game
 * @param x x of piece
 * @param y y of piece
 */
void drawmoves(SPGameWin* gameWin, int x, int y);
#endif /* SPGAMEWINDOW_H_ */
