/*
 * SPMiniMaxNode.h
 *
 *  Created on: May 20, 2017
 *      Author: nitzan
 */

#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include "SPChessGame.h"
#define PRINTNUM 20000000
#define nodestimeout 270000
#define maxminarrlen 4
#define allmovesmax 300
#define maxdefenders 10

/*
 * compute piece score spetial scoring function for level 5 the idea is that
 * not all places on board are eqaly good for a piece to be positioned in.
 * so we compute a different value for each position.
 * @param color the piece color
 * @param type the piece type
 * @param i piece y coordinate
 * @param j piece x coordinate
 * @ret score for piece.
 */
int pieceScorelvl5(COLOR color, PIECE type, int i, int j);

/**
 * computes best move for player A in case of pawn promotion
 * @param game game board
 * @param BestMove command to hold best move
 * @param alpha pruning value
 * @param beta pruning value
 * @param recursionDepth current recursion depth
 * @param player which player is playerB(white or black)
 * @param maxDepth max recursion depth
 * @param promote pointer and is set to value of
 * pawn promotion in case it occurs
 * @return min score
 */
int promotePlayB(SPChessGame* game,SPCommand* BestMove,
				 int alpha,int beta,int recursionDepth,
				 COLOR player,int maxDepth,int i,int j, char* promote, bool expert, int *numofnodes);
/**
 * computes best move for player B in case of pawn promotion
 * @param game game board
 * @param BestMove command to hold best move
 * @param alpha pruning value
 * @param beta pruning value
 * @param recursionDepth current recursion depth
 * @param player which player is playerB(white or black)
 * @param maxDepth max recursion depth
 * @param promote pointer and is set to value of
 * pawn promotion in case it occurs
 * @return min score
 */
int promotePlayA(SPChessGame* game,SPCommand* BestMove,
				 int alpha,int beta,int recursionDepth,
				 COLOR player,int maxDepth,int i,int j,char* promote, bool expert, int *numofnodes);

/**
 * computes best move for player B
 * @param game game board
 * @param BestMove command to hold best move
 * @param alpha pruning value
 * @param beta pruning value
 * @param recursionDepth current recursion depth
 * @param player which player is playerB(white or black)
 * @param maxDepth max recursion depth
 * @param promote pointer and is set to value of
 * pawn promotion in case it occurs
 * @return min score
 */
int computePlayB(SPChessGame* game, SPCommand* BestMove, int alpha, int beta, int recursionDepth,
                 COLOR player,int maxDepth, char* promote, bool expert, int *numofnodes);
/**
* computes best move for player A
* @param game game board
* @param BestMove command to hold best move
* @param alpha pruning value
* @param beta pruning value
* @param recursionDepth current recursion depth
* @param player which player is playerB(white or black)
* @param maxDepth max recursion depth
* @param promote pointer and is set to value of
* pawn promotion in case it occurs

* @return max score
*/
int computePlayA(SPChessGame* game, SPCommand* BestMove,int alpha, int beta, int recursionDepth,
                 COLOR player, int maxDepth, char* promote, bool expert, int *numofnodes);

/**
 * computes best move for player A in case of pawn promotion
 * @param game game board
 * @param BestMove command to hold best move
 * @param alpha pruning value
 * @param beta pruning value
 * @param recursionDepth current recursion depth
 * @param player which player is playerB(white or black)
 * @param maxDepth max recursion depth
 * @param promote pointer and is set to value of
 * pawn promotion in case it occurs
 * @return min score
 */
int promotePlayBlvl5(SPChessGame* game,SPCommand* BestMove,
                 int alpha,int beta,int recursionDepth,
                 COLOR player,int maxDepth,int i,int j, char* promote, bool expert, int *numofnodes);
/**
 * computes best move for player B in case of pawn promotion
 * @param game game board
 * @param BestMove command to hold best move
 * @param alpha pruning value
 * @param beta pruning value
 * @param recursionDepth current recursion depth
 * @param player which player is playerB(white or black)
 * @param maxDepth max recursion depth
 * @param promote pointer and is set to value of
 * pawn promotion in case it occurs
 * @return min score
 */
int promotePlayAlvl5(SPChessGame* game,SPCommand* BestMove,
                 int alpha,int beta,int recursionDepth,
                 COLOR player,int maxDepth,int i,int j,char* promote, bool expert, int *numofnodes);

/**
 * computes best move for player B
 * @param game game board
 * @param BestMove command to hold best move
 * @param alpha pruning value
 * @param beta pruning value
 * @param recursionDepth current recursion depth
 * @param player which player is playerB(white or black)
 * @param maxDepth max recursion depth
 * @param promote pointer and is set to value of
 * pawn promotion in case it occurs
 * @return min score
 */
int computePlayBlvl5(SPChessGame* game, SPCommand* BestMove, int alpha, int beta, int
recursionDepth,
                 COLOR player,int maxDepth, char* promote, bool expert, int *numofnodes);
/**
* computes best move for player A
* @param game game board
* @param BestMove command to hold best move
* @param alpha pruning value
* @param beta pruning value
* @param recursionDepth current recursion depth
* @param player which player is playerB(white or black)
* @param maxDepth max recursion depth
* @param promote pointer and is set to value of
* pawn promotion in case it occurs

* @return max score
*/
int computePlayAlvl5(SPChessGame* game, SPCommand* BestMove,int alpha, int beta, int recursionDepth,
                 COLOR player, int maxDepth, char* promote, bool expert, int *numofnodes);



/**
 * undoes a move
 * @param game game
 * @param i point 1
 * @param j point 1
 * @param x point 2
 * @param y point 2
 * @param cptured piece to restore
 */
void undo_move_min_max(SPChessGame* game,int i,int j,int x,int y,char cptured);
/**
 * returns a score for the board
 * @param game game
 * @param player players color
 * @return the score
 */
int ScoringFunction(SPChessGame* game, COLOR player);
/**
 * returns a score for the board in level 5 mode
 * @param game game
 * @param player players color
 * @return the score
 */
int ScoringFunction_lvl5(SPChessGame* game, int player);
/**
 * gets the best computer move
 * @param game the game
 * @param recursionDepth legal recursion depth
 * @param player player color
 * @return command describing move
 */
SPCommand get_computer_move(SPChessGame* game, int recursionDepth, COLOR player, char* promotion);

/**
 * quicksort an array
 * @param L array to sort
 * @param start begin
 * @param end end index
 */
void quicksort(int L[][maxminarrlen], int start, int end);
/**
 * recursive qsort function
 * @param L array
 * @param left left end
 * @param right right end
 * @param greaterorder highest first or last
 * @return new index
 */
int partition(int L[][maxminarrlen], int left, int right, bool greaterorder);
/**
 * swap between two locations in array
 * @param L array
 * @param left first index
 * @param right second index
 */
void swap(int L[][maxminarrlen],int left, int right);
 /**
  * get piece value
  * @param type type
  * @return int
  */
int pieceval(PIECE type);
/**
 * get piece location value
 * @param color color
 * @param type type of piece
 * @param i x
 * @param j y
 * @return int
 */
int piecelocval(COLOR color, PIECE type, int i,int j);
/**
 * get all legal moves
 * @param game game
 * @param player color
 * @param allmoves moves array
 * @param arrlen max length
 * @param positiveonly only store positive results-disabled currently
 * @return num of legal moves
 */
int get_all_moves(SPChessGame *game, int player, int allmoves[][maxminarrlen], int arrlen, int
positiveonly);
#endif /* SPMINIMAXNODE_H_ */
