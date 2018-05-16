/*
 * ConsoleMode.h
 *
 *  Created on: Aug 14, 2017
 *      Author: nitzan
 */

#ifndef CONSOLEMODE_H_
#define CONSOLEMODE_H_

#include <stdio.h>
#include "SPFIARParser.h"
#include "SPChessGame.h"
#include "LoadStore.h"
#define MAXLEN 620
typedef enum STATE{
    GAME,SETTING
} GAME_STATE;

/**
 * populates the given str with input from command line
 * @param str
 */
void readline(char str[MAXLEN+1]);
/**
 * mainloop for terminal mode
 * @param prevgame print new game message boolean
 * @param mode pregame or in game
 * @param cond break condition
 * @param game gameboard
 * @param command command to play
 */
void consuleMainLoop(GAME_STATE *mode, int *cond, SPChessGame* game, SPCommand *command);
/**
 * plays command that in pre game mode
 * @param cond break condition
 * @param game gameboard
 * @param mode pregame or in game
 * @param command command to play
 */
void inoutCommandFromUserMode(int* cond, SPChessGame* game, GAME_STATE* mode, SPCommand *command);
/**
 * plays command that in in-game mode
 * @param cond break condition
 * @param game gameboard
 * @param mode pregame or in game
 * @param command command to play
 */
void inoutCommandFromUserGame(int* cond, SPChessGame* game, SPCommand *command, GAME_STATE* mode);
/**
 * choose weather to play the in-game or pre-game option
 * @param prevgame print new game message boolean
 * @param mode pregame or in game
 * @param cond break condition
 * @param game gameboard
 * @param command command to play
 */
void gameplay(GAME_STATE *mode, int *cond, SPChessGame* game, SPCommand *command);
/**
 * print game message and board
 * @param game game board
 */
void printgame(SPChessGame *game);
/**
 * print next move message
 * @param game game board
 */
void printUserInput(SPChessGame *game);
/**
 * pawn promotion for console mode
 * @param game game
 * @param command command
 */
void pawnpromotion(SPChessGame* game, SPCommand* command);
/**
 * on START actions
 * @param cond condition for exit
 * @param game game
 * @param mode mode
 */
void onstart(int* cond, SPChessGame* game, GAME_STATE* mode);

/**
 * on move actions
 * @param cond if to exit
 * @param game game
 * @param command command
 */
void onmove(int* cond, SPChessGame* game, SPCommand *command);

/**
 * on print command
 * @param game game board
 */
void onprint(SPChessGame* game);
/**
 * on move actions
 * @param cond if to exit
 * @param game game
 * @param command command
 */
void oncastle(int* cond, SPChessGame* game, SPCommand *command);


#endif /* CONSOLEMODE_H_ */
