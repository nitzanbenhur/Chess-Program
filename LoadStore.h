/*
 * LoadStore.h
 *
 *  Created on: Aug 27, 2017
 *      Author: nitzan
 */

#ifndef LOADSTORE_H_
#define LOADSTORE_H_
#include "SPChessGame.h"
#include <stdlib.h>
#include <stdio.h>
#define numofs 4
/**
 * loades values into game, called after file is loaded
 * @param values values of players color, game mode and misc values.
 * @param game game object
 * @param tempboard loaded gameboard
 */
void setvaluesfromloadedfile(int *values, SPChessGame *game,
							 char tempboard[SP_Chess_GAME_DIM][SP_Chess_GAME_DIM]);
/**
 * save to file function
 * @param com current command
 * @param game current game
 */
void save(SPCommand *com, SPChessGame *game);

/**
 * load command from file function
 * @param com command to update
 * @param setting setting uo update(from file)
 * @param game game board
 * @return 1 is succeded else 0 is file is in bad format
 */
int load(SPCommand *com, SPChessGame* game);

#endif /* LOADSTORE_H_ */
