#ifndef SPChessGAME_H_
#define SPChessGAME_H_
#include <stdbool.h>
#include "SPArrayList.h"

/**
 * SPChessGame Summary:
 *
 * A container that represents a classic connect-4 game, a two players 6 by 7
 * board game (rows X columns). The container supports the following functions.
 *
 * spChessGameCreate           - Creates a new game board
 * spChessGameCopy             - Copies a game board
 * spChessGameDestroy          - Frees all memory resources associated with a game
 * spChessGameSetMove          - Sets a move on a game board
 * spChessGameIsValidMove      - Checks if a move is valid
 * spChessGameUndoPrevMove     - Undoes previous move made by the last player
 * spChessGamePrintBoard       - Prints the current board
 * spChessGameGetCurrentPlayer - Returns the current player
 *
 */

#define MAXNUMKEGALMOVES 50
#define SP_Chess_GAME_DIM 8
#define SP_Chess_GAME_EMPTY_ENTRY '_'
#define wknight 'n'
#define bknight 'N'
#define wking 'k'
#define bking 'K'
#define wqueen 'q'
#define bqueen 'Q'
#define wbishop 'b'
#define bbishop 'B'
#define wpawn 'm'
#define bpawn 'M'
#define wrook 'r'
#define brook 'R'
#define colors(game) (game->currentPlayer==WHITE)?"white":"black"
typedef enum {
	ONE_PLAYER = 1,
	TWO_PLAYERS = 2
} MODE;

typedef enum color {
    BLACK=-1,
    WHITE=1,
    GREY=0,
} COLOR;

typedef enum {
    INVALID_POSITION,  PIECE_NOT_HERE, ILLEAGEL_MOVE, VALID_MOVE, BLACK_MATE, WHITE_MATE, TIE
} MOVE_RET;


typedef struct sp_Chess_game_t {
    //gameBoard[0][0] means lower left corner
	char gameBoard[SP_Chess_GAME_DIM][SP_Chess_GAME_DIM];
	MODE mode;
	int diff;
	COLOR UserCol;
	COLOR currentPlayer; //whose turn to play
	SPArrayList* hist;
	bool casteling[6];
	int GameSaved;
} SPChessGame;

/**
 * Type used for returning error codes from game functions
 */
typedef enum sp_Chess_game_message_t {
	SP_Chess_GAME_INVALID_ARGUMENT,
	SP_Chess_GAME_SUCCESS,
//You may add any message you like
} SP_Chess_GAME_MESSAGE;

#define max(a,b)  a<b ? b : a

typedef enum gamepiece {
    ROOK=0,
    KNIGHT=1,
    KING=2,
    QUEEN=3,
    PAWN=4,
    BISHOP=5,
    NOT=6
} PIECE;


/**
 * sets casteling flags in game struct
 * @param game the game being played
 * @param com the casteling command that has been played
 */
void setCasteling(SPChessGame* game, SPCommand* com);
/**
 * perform casteling move if it is leagle set flags for future attepts to castle
 * @param game the game being played.
 * @param i row of rook or king in casteling move.
 * @param j column of rook or king in casteling move.
 * @ret WHITE_MATE if white king is in check mate
 * @ret BLACK_MATE if black king is in check mate
 * @ret TIE if game is tie after move
 * @ret LEAGLE_MOVE if none of the above had happened
 */
MOVE_RET SPCastelingMove(SPChessGame* game, int i, int j);
/**
 * check if casteling command is leagle according to chess rules.
 * @param game the game being played.
 * @param i row of rook or king in casteling move.
 * @param j column of rook or king in casteling move.
 * @ret true if command is legal.
 * @ret false if command is not legal.
 */
bool isLeagleCastelingMove(SPChessGame* game, int i, int j, COLOR color);
/**
 * perform casteling move assume casteling move is legal.
 * @param game the game being played
 * @param i row in of rook or king in casteling move
 * @param j column of rook or king in casteling move
 */
void castelingMove(SPChessGame* game, int i, int j);

/**
 * prints computer's pawn promotion
 * @param promotion indicating what piece computer took
 * @param x origin first coordinate
 * @param y origin second coordinate
 * @param z destination first coordinate
 * @param w destination second coordinate
 * @param compPice the piece computer has moved if this function is
 * called this should be pawn always.
 */
void printPromotion(char promotion,int x, int y, int z, int w, char* compPiece);
/**
 * returns 1 if after moving piece in x,y to i,j piece is threatened zero else
 * @param game SPChessGame
 * @param x,y origin position
 * @param i,j destination position
 * @param color
 */
int try_move_threatened(SPChessGame *game, int x, int y, int i, int j);


/**
 * returns the color of the given char piece
 * @param piece piece to return color of
 * @return black/white/grey(no piece)
 */
COLOR color(char piece);
/**
 * creates and inits a game
 * @return game created
 */
SPChessGame* game_create();
/**
 * destorys a game and frees all resources regarding it
 * @param game game to destroy
 */
void game_destroy(SPChessGame *game);
/**
 * loads possible moves into array moves
 * @param game game
 * @param x x position of piece
 * @param y y position of piece
 * @param moves array to fill
 * @return length of get_moves
 */
int get_moves(SPChessGame *game, int x, int y, int* moves);
/**
 * get the type of given piece at given location
 * @param game game board
 * @param i x position
 * @param j y position
 * @return piece type e.g king or queen or any other
 */
PIECE get_piece_type(SPChessGame *game, int i, int j);
/**
 * returns the color of the piece at given location
 * @param game game board
 * @param i x position
 * @param j y position
 * @return color of piece
 */
int getcolor(SPChessGame* game, int i,int j);
/**
 * returns location of piece in 8*i+j format. if there is more than one, returns the first one
 * @param game game board
 * @param color piece color
 * @return location of piece
 */
int getPiecePos(SPChessGame *game, COLOR color, PIECE type);
/**
 * get the piece type
 * @param piece given piece
 * @return king or queen or other piece type
 */
PIECE getpiece(char piece);
/**
 * sets the game board to initial position
 * @param game game to init
 */
void initboard(SPChessGame* game);
/**
 * if x and y are legal positions on board e.g between 0 and 7 inc.
 * @param x x coordinate
 * @param y y coordinate
 * @return 1 if true else 0
 */
int isDestInBoard(int x, int y);
/**
 * is path from x1,y1 to x2,y2 clear when moving on a diagonal(assuming it is one)
 * @param game game object
 * @param x1 point1 x
 * @param y1 point1 y
 * @param x2 point2 x
 * @param y2 point2 y
 * @param colorSource color of piece in point 1
 * @param colorDest color of piece in point 2
 * @return 1 if yes and otherwise 0
 */
int isDiagClear(SPChessGame *game, int x1, int y1, int x2, int y2, COLOR colorSource,COLOR colorDest);
/**
 * return if bishop move is legal
 * @param game game object
 * @param com command how to move bishop
 * @param colorSource color of bishop
 * @param colorDest color of destination piece
 * @return 1 if possible else 0
 */
int isLeagelMoveBishop(SPChessGame *game, SPCommand *com, COLOR colorSource, COLOR colorDest);
/**
 * return if knight move is legal
 * @param com command how to move piece
 * @param colorSource color of piece
 * @param colorDest color of destination piece
 * @return 1 if possible else 0
 */
int isLeagelMoveKinght(SPCommand *com, COLOR colorSource, COLOR colorDest);
/**
 * return if king move is legal
 * @param game game object
 * @param com command how to move piece
 * @param colorSource color of piece
 * @param colorDest color of destination piece
 * @return 1 if possible else 0
 */
int isLeagelMoveKing(SPCommand *com, COLOR colorSource, COLOR colorDest);
/**
 * return if pawn move is legal
 * @param game game object
 * @param com command how to move piece
 * @param colorSource color of piece
 * @param colorDest color of destination piece
 * @return 1 if possible else 0
 */
int isLeagelMovePawn(SPChessGame *game, SPCommand *com, COLOR colorSource, COLOR colorDest);
/**
 * return if queen move is legal
 * @param game game object
 * @param com command how to move piece
 * @param colorSource color of piece
 * @param colorDest color of destination piece
 * @return 1 if possible else 0
 */
int isLeagelMoveQueen(SPChessGame *game, SPCommand *com, COLOR colorSource, COLOR colorDest);
/**
 * return if rook move is legal
 * @param game game object
 * @param com command how to move piece
 * @param colorSource color of piece
 * @param colorDest color of destination piece
 * @return 1 if possible else 0
 */
int isLeagelMoveRook(SPChessGame *game, SPCommand *com, COLOR colorSource, COLOR colorDest);
/**
 * is path from x1,y1 to x2,y2 clear when moving on a row/column(assuming it is one)
 * @param game game object
 * @param x1 point1 x
 * @param y1 point1 y
 * @param x2 point2 x
 * @param y2 point2 y
 * @param colorSource color of piece in point 1
 * @param colorDest color of piece in point 2
 * @return 1 if yes and otherwise 0
 */
int isLineClear(SPChessGame *game, int x1, int y1, int x2, int y2, COLOR colorSource,COLOR colorDest);
/**
 * returns true(1) if positio is legal, i.e between 0 and 7 inc. for all arguments
 * @param com
 * @return 1 if true else 0
 */
int isOnBoard(SPCommand *com);
/**
 * checks if there is a checkmate
 * @param game gme object
 * @return TIE if tie, else black/white mate, or valid move if none of the above
 */
MOVE_RET isThereCheckMate(SPChessGame *game);
/**
 * computes if piece is threatened by pawn
 * @param com command describing pawn move
 * @param colorSource color of pawn
 * @param colorDest color of piece threatened
 * @return 1 if true else 0
 */
int isThreatenByPawn(SPCommand *com, COLOR colorSource, COLOR colorDest);
/**
 * computes if king is threatened
 * @param colorDest color of king threatened
 * @return 1 if true else 0
 */
int KingThreatened(SPChessGame *game, COLOR colorDest);
/**
 * moves piece from point a,b to point c,d
 * @param game gam baord
 * @param a x coord of first point
 * @param b y coord of first point
 * @param c x coord of second point
 * @param d y coord of second point
 * @return overriddenpiece of point moved to
 */
char move_piece_c(SPChessGame *game, int a, int b, int c, int d);
/**
 * moves a piece as described in the command
 * @param game game board
 * @param com command
 * @return overridden char of point moved to
 */
char move_piece(SPChessGame *game, SPCommand *com);
/**
 * is king move legal direction wise(not checking for mates etc)
 * @param com command
 * @return
 */
int nonRecursiveLeagleMoveKing(SPCommand *com);
/**
 * will move to x,y leave piece threatened by another piece
 * @param game game
 * @param x first coord of piece
 * @param y second coord of piece
 * @param colorDest color of piece
 * @return 1 if true else 0
 */
int PieceThreatened(SPChessGame *game, int x, int y,COLOR colorDest);
/**
 * prints the game board
 * @param game game to print
 * @return SP_Chess_GAME_SUCCESS if ok, else SP_Chess_GAME_SUCCESS if game is null
 */
SP_Chess_GAME_MESSAGE print_board(SPChessGame *game);
/**
 * prints the legal move for the piece
 * @param game game
 * @param com command, contains the piece location
 */
void SPChessGetMoves(SPChessGame *game,SPCommand* com);
/**
 * checks if command is legal
 * @param game game
 * @param com command
 * @return 1 if true else 0
 */
int SPChessIsLeagelMove(SPChessGame *game, SPCommand *com);
/**
 * attempts to move the piece as described in the command
 * @para, guiMode: true iff function has been invoked from gui mode
 * @param game game
 * @param com command
 * @return INVALID_POSITION/PIECE_NOT_HERE/VALID_MOVE/ILLEAGEL_MOVE
 */
MOVE_RET SPChessMovePiece(SPChessGame* game, SPCommand* com, bool guiMode);
/**
 * restarts game and cleans board
 * @param game game to restart
 */
void SPChessRestart(SPChessGame* game);
/**
 * undoes a players move
 * @param game game
 */
int SPChessUndoMove(SPChessGame* game);
/**
 * plays compute move, get move from min max
 * @param guiMode true iff function has been invoked from gui mode
 * @param game game board
 * @return 1 if success else 0
 */
MOVE_RET SPComputerMove(SPChessGame* game, bool guiMode);
/**
 * return 1 if king is not threatened after move, else 0
 * @param game game
 * @param x of first point
 * @param y of first point
 * @param i of second point
 * @param j of second point
 * @param color
 * @return 1/0
 */
int try_move(SPChessGame *game, int x, int y, int i, int j, COLOR color);
/**
 * returns 1 if piece has at least 1 legal move else 0
 * @param game game
 * @param x x of piece
 * @param y y of piece
 * @param color piece color
 * @return 1/0
 */
int try_moves(SPChessGame *game, int x, int y, COLOR color);
/**
 * returns 1 if is a pawn promotion command
 * @param game game
 * @param command command
 * @return int
 */
int ispawnpromotioncommand(SPChessGame* game, SPCommand* command);
/**
 * in case we are in gui mode this function show simple message box indicating check
 * @param color the player which we are checking if is in check.
 */
void showCheckMateDialog(COLOR color);
/**
 * moves the pieces on board to undo
 * @param game game
 * @param com command to undo
 */
void undomoveboard(SPChessGame* game, SPCommand* com);
#endif
