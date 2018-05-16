#include <stdlib.h>
#include <stdio.h>
#include "SPChessGame.h"
#include <assert.h>
#include <string.h>
#include "SPMiniMax.h"
#include "SPGameWindow.h"
#include "ConsoleMode.h"
#include <SDL_video.h>
#include <SDL.h>

SPChessGame* game_create(){
    SPChessGame* game;
    game = malloc(sizeof(SPChessGame));
    if(game == NULL){
        printf("Error: malloc has failed");
        return NULL;
    }
    game->hist = spArrayListCreate();
    game->currentPlayer = WHITE;
    if(game->hist == NULL){
        return NULL;
    }
	for (int i = 0; i < 6; i++){
    	game->casteling[i] = true;
    }
    game->GameSaved = 0;
    game->mode = ONE_PLAYER;
    game->diff = 2;
    game->UserCol = WHITE;
    initboard(game);
    return game;
}
void game_destroy(SPChessGame *game){
    if(game != NULL) {
        if(game->hist!=NULL) {
            spArrayListDestroy(game->hist);
        }
        free(game);
        game = NULL;
    }
}
void initboard(SPChessGame* game){
    for(int j=0;j<SP_Chess_GAME_DIM;j++){
        for(int i=0;i<SP_Chess_GAME_DIM;i++){
            if(i>1 && i<6) {
                game->gameBoard[i][j] = SP_Chess_GAME_EMPTY_ENTRY;
            }else if(i==6){
                game->gameBoard[i][j]=  bpawn;
            }else if(i==1){
                game->gameBoard[i][j]= wpawn;
            }else if(i==0){
                if(j==0 || j==7){
                    game->gameBoard[i][j]=wrook;
                }else if(j==1 || j==6){
                    game->gameBoard[i][j]= wknight;
                }else if(j==2 || j==5){
                    game->gameBoard[i][j]= wbishop;
                }else if(j==3){
                    game->gameBoard[i][j]= wqueen;
                }else if(j==4){
                    game->gameBoard[i][j]=wking;
                }
            }else if(i==7){
                if(j==0 || j==7){
                    game->gameBoard[i][j]= brook;
                }else if(j==1 || j==6){
                    game->gameBoard[i][j]= bknight;
                }else if(j==2 || j==5){
                    game->gameBoard[i][j]= bbishop;
                }else if(j==3){
                    game->gameBoard[i][j]= bqueen;
                }else if(j==4){
                    game->gameBoard[i][j]=  bking;
                }

            }
        }
    }
}
int getcolor(SPChessGame* game, int i,int j){
    bool cond=i>-1 && j>-1 && i<SP_Chess_GAME_DIM && j<SP_Chess_GAME_DIM;
    if(!cond){
        printf("i j:%d %d",i,j);
    }
    assert(cond);
    return color(game->gameBoard[i][j]);
}

COLOR color(char piece){
    if(piece == wrook || piece == wknight || piece == wbishop ||
            piece == wqueen || piece == wking || piece == wpawn){
        return WHITE;
    }else if(piece == brook || piece == bknight || piece == bbishop ||
             piece == bqueen || piece == bking || piece == bpawn){
        return BLACK;
    }
    return GREY;
}

PIECE get_piece_type(SPChessGame *game, int i, int j){
    if (!(i>-1 && j>-1 && i<SP_Chess_GAME_DIM && j<SP_Chess_GAME_DIM)){
        printf("i is: %d j is: %d\n",i,j);
    }
    return getpiece(game->gameBoard[i][j]);
}

PIECE getpiece(char piece){
    if(piece == wrook || piece==brook){
        return ROOK;
    }else if(piece == wbishop || piece==bbishop){
        return BISHOP;
    }else if(piece == wknight || piece==bknight){
        return KNIGHT;
    }else if(piece == wqueen || piece==bqueen){
        return QUEEN;
    }else if(piece == wking || piece==bking){
        return KING;
    }else if(piece==wpawn || piece==bpawn){
        return PAWN;
    }
    return NOT;
}

char move_piece_c(SPChessGame *game, int a, int b, int c, int d){
    char orig = game->gameBoard[c][d];
    game->gameBoard[c][d]=game->gameBoard[a][b];
    game->gameBoard[a][b]=SP_Chess_GAME_EMPTY_ENTRY;
    return orig;
}

char move_piece(SPChessGame *game, SPCommand *com){
    char c =move_piece_c(game, com->arg, com->arg2, com->arg3, com->arg4);
    if(ispawnpromotioncommand(game,com))
        com->cmd=PROMOTE;
    spArrayListAddFirst(game->hist,*com);
    return c;
}

int isOnBoard(SPCommand *com){
	if ((com->arg >= 0) && (8 > com->arg2) && (com->arg3 >= 0) && (8 > com->arg4)){
		return 1;
	}
	return 0;
}

void SPChessRestart(SPChessGame* game){
	while (!spArrayListIsEmpty(game->hist)){
		spArrayListRemoveLast(game->hist);
	}
    game->currentPlayer = WHITE;
    initboard(game);
	printf("Restarting...\n");
}


int SPChessUndoMove(SPChessGame* game){
	if (game->mode == TWO_PLAYERS){
		printf("Undo command not available in 2 players mode\n");
		return 0;
	}
	else if (spArrayListSize(game->hist) < 2){
		printf("Empty history, move cannot be undone\n");
		return 0;
	}
	SPCommand lastCom = spArrayListGetFirst(game->hist);
	spArrayListRemoveFirst(game->hist);
	SPCommand seconLastCom = spArrayListGetFirst(game->hist);
	spArrayListRemoveFirst(game->hist);
	if (BLACK == getcolor(game, lastCom.arg3,lastCom.arg4)){
		if (lastCom.cmd == CASTLE){
			printf("Undo move for player black casteling move at: <%d,%c>\n"
					,lastCom.arg+1,'A'+lastCom.arg2);
		}
		else {
			printf("Undo move for player black : <%d,%c> -> <%d,%c>\n",1+lastCom.arg3,'A'+lastCom.arg4,
					lastCom.arg+1,'A'+lastCom.arg2);
		}
		if (seconLastCom.cmd == CASTLE){
			printf("Undo move for player white casteling move at: <%d,%c>\n"
					,seconLastCom.arg+1,'A'+seconLastCom.arg2);
		}
		else {
			printf("Undo move for player white : <%d,%c> -> <%d,%c>\n",1+seconLastCom.arg3,
					'A'+seconLastCom.arg4,seconLastCom.arg+1,'A'+seconLastCom.arg2);
		}
	}
	else {
		if (lastCom.cmd == CASTLE){
			printf("Undo move for player white casteling move at: <%d,%c>\n"
					,lastCom.arg+1,'A'+lastCom.arg2);
		}
		else {
			printf("Undo move for player white : <%d,%c> -> <%d,%c>\n",
					lastCom.arg3+1,'A'+lastCom.arg4,lastCom.arg+1,'A'+lastCom.arg2);
		}
		if (seconLastCom.cmd == CASTLE){
			printf("Undo move for player black casteling move at: <%d,%c>\n"
					,seconLastCom.arg+1,'A'+seconLastCom.arg2);
		}
		else {
			printf("Undo move for player black : <%d,%c> -> <%d,%c>\n",
					seconLastCom.arg3+1,'A'+seconLastCom.arg4,seconLastCom.arg+1,'A'+seconLastCom.arg2);
		}
	}
    undomoveboard(game,&lastCom);
    undomoveboard(game,&seconLastCom);
	return 1;
}

void undomoveboard(SPChessGame* game, SPCommand* com){
    if(com->cmd!=CASTLE && (com->cmd== MOVE||com->cmd==PROMOTE)) {
        game->gameBoard[com->arg][com->arg2] = game->gameBoard[com->arg3][com->arg4];
        game->gameBoard[com->arg3][com->arg4] = com->overriddenpiece;
        if(com->cmd==PROMOTE){
            game->gameBoard[com->arg][com->arg2] = (char)((game->currentPlayer==WHITE)
                                                                ?wpawn:bpawn);
        }
    }else if(com->cmd==CASTLE){
        game->gameBoard[com->arg][4]=game->gameBoard[com->arg][(com->arg2==0)?2:6];
        game->gameBoard[com->arg][(com->arg2==0)?2:6]=SP_Chess_GAME_EMPTY_ENTRY;
        game->gameBoard[com->arg][com->arg2]=game->gameBoard[com->arg][(com->arg2==0)?3:5];
        game->gameBoard[com->arg][(com->arg2==0)?3:5]=SP_Chess_GAME_EMPTY_ENTRY;
    }
}

MOVE_RET SPChessMovePiece(SPChessGame* game, SPCommand* com, bool guiMode){
	if (!isOnBoard(com)){
		printf("Invalid position on the board\n");
		return INVALID_POSITION;
	}
	else if (game->currentPlayer != getcolor(game, com->arg,com->arg2)){
		printf("The specified position does not contain your piece\n");
        return PIECE_NOT_HERE;
	}
	else if (!SPChessIsLeagelMove(game,com)){
		printf("Illegal move\n");
		return ILLEAGEL_MOVE;
	}
	else {
		com->overriddenpiece = game->gameBoard[com->arg3][com->arg4];
		move_piece(game,com);
        if(com->cmd==PROMOTE){
            if(guiMode)
                pawnPromotion(game,com->arg3,com->arg4);
            else
                pawnpromotion(game, com);
        }
		MOVE_RET mate = isThereCheckMate(game);
		if (mate != VALID_MOVE){
			if (mate == TIE){
				printf("The game is tied\n");
			}
			return mate;
		}
		if (KingThreatened(game,WHITE)){
			printf("Check: white King is threatened!\n");
			if (guiMode){
				showCheckMateDialog(WHITE);
			}
		}

		if (KingThreatened(game,BLACK)){
			printf("Check: black King is threatened!\n");
			if (guiMode){
				showCheckMateDialog(BLACK);
			}
		}
		if (game->currentPlayer == BLACK){
			game->currentPlayer = WHITE;
		}
		else {
			game->currentPlayer = BLACK;
		}
		if (game->mode == ONE_PLAYER){
			return SPComputerMove(game,guiMode);
		}
		return VALID_MOVE;
	}
}
void showCheckMateDialog(COLOR color){
	if (color == WHITE){
		if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
				"Check","Check: white King is threatened!\n", NULL )<0){
			printf("Error in SDL_ShowSimpleMessageBox\n");
		}
	}
	else {
		if (SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
				"Check","Check: black King is threatened!\n", NULL )<0){
			printf("Error in SDL_ShowSimpleMessageBox\n");
		}
	}

}

int ispawnpromotioncommand(SPChessGame* game, SPCommand* command){
    int color = getcolor(game, command->arg3, command->arg4);
    if (command->arg3 == 3.5 * color + 3.5 &&
        get_piece_type(game, command->arg3, command->arg4) == PAWN) {
        return 1;
    }
    return 0;
}

MOVE_RET SPComputerMove(SPChessGame* game, bool guiMode){
	SPCommand com;
	char promotion = 'z';
	com =  get_computer_move(game,game->diff,game->currentPlayer,&promotion);
	com.overriddenpiece = game->gameBoard[com.arg3][com.arg4];
	char compPiece[8];
	PIECE pieceType = get_piece_type(game,com.arg,com.arg2);
	switch(pieceType){
	case(PAWN):
			strcpy(compPiece,"pawn");
			break;
	case(BISHOP):
			strcpy(compPiece,"bishop");
			break;
	case(KNIGHT):
			strcpy(compPiece,"knight");
			break;
	case(ROOK):
			strcpy(compPiece,"rook");
			break;
	case(QUEEN):
			strcpy(compPiece,"queen");
			break;
	case(KING):
			strcpy(compPiece,"king");
			break;
	default:
			break;

	}
	setCasteling(game,&com);
	move_piece(game,&com);
	if (promotion == 'z'){
		printf("Computer: move %s at <%d,%c> to <%d,%c>\n"
				,compPiece,com.arg+1,'A'+com.arg2,com.arg3+1,'A'+com.arg4);
	}
	else {
		game->gameBoard[com.arg3][com.arg4] = promotion;
		printPromotion(promotion,com.arg,com.arg2,com.arg3,com.arg4,compPiece);
	}
	MOVE_RET checkmate = isThereCheckMate(game);
	if (checkmate == TIE){
		printf("The game ends in a tie\n");
	}
	if (game->currentPlayer == BLACK){
		game->currentPlayer = WHITE;
	}
	else {
		game->currentPlayer = BLACK;
	
	}
	if (KingThreatened(game,game->UserCol) && checkmate != WHITE_MATE && checkmate != BLACK_MATE){
		printf("Check!\n");
		if (guiMode){
			showCheckMateDialog(game->UserCol);
		}
	}
	return checkmate;
}

void printPromotion(char promotion,int x, int y, int z, int w, char* compPiece){
	char piece[8];
	if (promotion == 'z'){
		return;
	}
	else if (promotion == wpawn || promotion == bpawn){
		strcpy(piece,"pawn");
	}
	else if (promotion == wbishop || promotion == bbishop){
		strcpy(piece,"bishop");
	}
	else if (promotion == wknight || promotion == bknight){
		strcpy(piece,"knight");
	}
	else if (promotion == wrook || promotion == brook){
		strcpy(piece,"rook");
	}
	else if (promotion == wqueen || promotion == bqueen){
		strcpy(piece,"queen");
	}
	printf("Computer: move %s <%d,%c> to <%d,%c> and promote to %s\n",
			compPiece,x+1,'A'+y,z+1,'A'+w,piece);

}

MOVE_RET isThereCheckMate(SPChessGame *game){
	int whiteMate = 1;
	int blackMate = 1;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (getcolor(game,i,j) == WHITE){
				if (try_moves(game,i,j,WHITE)){
					whiteMate = 0;
				}
			}
		}
	}
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (getcolor(game,i,j) == BLACK){
				if (try_moves(game,i,j,BLACK)){
					blackMate = 0;
				}
			}
		}
	}
	if (blackMate && KingThreatened(game,BLACK)){
		return BLACK_MATE;
	}
	else if (whiteMate && KingThreatened(game,WHITE)){
		return WHITE_MATE;
	}
	else if ((blackMate && game->currentPlayer == WHITE) ||
			(whiteMate && game->currentPlayer == BLACK)){
		return TIE;
	}
	return VALID_MOVE;
}

void SPChessGetMoves(SPChessGame *game,SPCommand* com){
	int moves[64];
	get_moves(game,com->arg,com->arg2,moves);
	int index = 0;
	while (moves[index] != -1){
		int threaten = 0;
		int eat = 0;
		int x = 1+(moves[index]/8);
		char y = (char) 'A' + (moves[index]%8);
		if (game->mode == ONE_PLAYER && (game->diff == 1 || game->diff == 2)
				&& try_move_threatened(game,com->arg,com->arg2,moves[index]/8,moves[index]%8)){
			threaten = 1;
		}
		if (game->mode == ONE_PLAYER && (game->diff == 1 || game->diff == 2) &&
				game->currentPlayer != getcolor(game,moves[index]/8,moves[index]%8)
				&& GREY != getcolor(game,moves[index]/8,moves[index]%8)){
			eat = 1;
		}
		if (eat && threaten){
			printf("<%d,%c>*^\n",x,y);
		}
		else if (eat){
			printf("<%d,%c>^\n",x,y);
		}
		else if (threaten){
			printf("<%d,%c>*\n",x,y);
		}
		else {
			printf("<%d,%c>\n",x,y);
		}
        index++;
	}
	if (get_piece_type(game,com->arg,com->arg2) == ROOK &&
			isLeagleCastelingMove(game,com->arg,com->arg2,game->currentPlayer)){
		printf("castle <%d,%c>\n",com->arg,'A'+com->arg2);
	}
	if (get_piece_type(game,com->arg,com->arg2) == KING){
		if (game->currentPlayer == BLACK){
			if (isLeagleCastelingMove(game,7,0,game->currentPlayer)){
				printf("castle <8,A>\n");
			}
			else if (isLeagleCastelingMove(game,7,7,game->currentPlayer)){
				printf("castle <8,H>\n");
			}
		}
		else {
			if (isLeagleCastelingMove(game,0,0,game->currentPlayer)){
				printf("castle <1,A>\n");
			}
			else if (isLeagleCastelingMove(game,0,7,game->currentPlayer)){
				printf("castle <1,H>\n");
			}
		}
	}
}

int get_moves(SPChessGame *game, int x, int y, int* moves){
	moves[0] = -1;
	int index = 0;
	SPCommand com;
	com.arg = x;
	com.arg2 = y;
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			com.arg3 = i;
			com.arg4 = j;
            com.overriddenpiece = game->gameBoard[com.arg3][com.arg4];
			if (SPChessIsLeagelMove(game,&com)){
				moves[index] = i*8 + j;
				index++;
				moves[index] = -1;
			}
		}
	}
    return index;
}

int try_moves(SPChessGame *game, int x, int y, COLOR color){
	int moves[64];
	get_moves(game,x,y,moves);
	int index = 0;
	while (moves[index] != -1){
		int i = moves[index]/8;
		int j = moves[index]%8;
		if(try_move(game,x,y,i,j,color)){
			return 1;
		}
		index++;
	}
	return 0;
}

int try_move_threatened(SPChessGame *game, int x, int y, int i, int j){
	int ret = 0;
	char captured = game->gameBoard[i][j];
	game->gameBoard[i][j] = game->gameBoard[x][y];
	game->gameBoard[x][y] = SP_Chess_GAME_EMPTY_ENTRY;
	ret = PieceThreatened(game,i,j,game->currentPlayer);
	game->gameBoard[x][y] = game->gameBoard[i][j];
	game->gameBoard[i][j] = captured;
	return ret;
}

int try_move(SPChessGame *game, int x, int y, int i, int j, COLOR color){
	int ret = 0;
	char captured = game->gameBoard[i][j];
	game->gameBoard[i][j] = game->gameBoard[x][y];
	game->gameBoard[x][y] = SP_Chess_GAME_EMPTY_ENTRY;
	if (!KingThreatened(game, color)){
		ret = 1;
	}
	game->gameBoard[x][y] = game->gameBoard[i][j];
	game->gameBoard[i][j] = captured;
	return ret;
}

int getPiecePos(SPChessGame *game, COLOR color, PIECE type){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			if (get_piece_type(game,i,j) == type && getcolor(game,i,j) == color){
				return 8*i + j;
			}
		}
	}
	return 0;
}

int SPChessIsLeagelMove(SPChessGame *game, SPCommand *com){
	PIECE pieceSource = getpiece(game->gameBoard[com->arg][com->arg2]);
	COLOR colorSource = getcolor(game, com->arg, com->arg2);
	COLOR colorDest = getcolor(game, com->arg3, com->arg4);
	int ret = 0;
	switch(pieceSource){
	case(PAWN):
			ret = isLeagelMovePawn(game, com, colorSource, colorDest);
			break;
	case(BISHOP):
			ret = isLeagelMoveBishop(game, com, colorSource, colorDest);
			break;
	case(ROOK):
			ret = isLeagelMoveRook(game, com, colorSource, colorDest);
			break;
	case(KNIGHT):
			ret = isLeagelMoveKinght(com, colorSource, colorDest);
			break;
	case(QUEEN):
			ret = isLeagelMoveQueen(game, com, colorSource, colorDest);
			break;
	case(KING):
            ret = isLeagelMoveKing(com, colorSource, colorDest);
			break;
	case(NOT):
			break;

	}
	if (ret == 1 && colorSource == BLACK
        && !try_move(game,com->arg,com->arg2,com->arg3,com->arg4,BLACK)){
		return 0;
	}
	else if (ret == 1 && colorSource == WHITE
             && !try_move(game,com->arg,com->arg2,com->arg3,com->arg4,WHITE)){
		return 0;
	}
	return ret;
}



int isDiagClear(SPChessGame *game,
                int x1, int y1, int x2, int y2, COLOR colorSource,COLOR colorDest){
	int dirX = 0;
	int dirY = 0;
	int Steps = abs(x1-x2);
	if (x1 < x2 ){
		dirX = 1;
	}
	else {
		dirX = -1;
	}
	if (y1 < y2){
		dirY = 1;
	}
	else {
		dirY = -1;
	}
	for (int i = 1; i < Steps; i++){
        PIECE piece = get_piece_type(game,x1+i*dirX,y1+i*dirY);
        if (piece != NOT) {
            return 0;
        }
	}
	PIECE pieceDest = get_piece_type(game,x2,y2);
	if (pieceDest != NOT && colorDest == colorSource){
		return 0;
	}
	return 1;
}

bool isLeagleCastelingMove(SPChessGame* game, int i, int j, COLOR color){
	if (KingThreatened(game,color) || color != getcolor(game,i,j)){
		return false;
	}
	if (i == 7 && color == BLACK && (j == 0 || j == 7) && game->casteling[4] ){
		if (j == 0 && game->gameBoard[7][1] == SP_Chess_GAME_EMPTY_ENTRY &&
				game->gameBoard[7][2] == SP_Chess_GAME_EMPTY_ENTRY && game->gameBoard[7][3] == SP_Chess_GAME_EMPTY_ENTRY
						&& game->casteling[3] ){
			if (!try_move_threatened(game,7,4,7,3) && !try_move_threatened(game,7,4,7,2)){
				return true;
			}
		}
		else if (j == 7 && game->gameBoard[7][6] == SP_Chess_GAME_EMPTY_ENTRY && game->gameBoard[7][5] == SP_Chess_GAME_EMPTY_ENTRY
				&& game->casteling[5]){
			if (!try_move_threatened(game,7,4,7,5) && !try_move_threatened(game,7,4,7,6)){
				return true;
			}
		}
	}
	else if (i == 0 && color == WHITE && (j == 0 || j == 7) && game->casteling[1]){
		if (j == 0 && game->gameBoard[0][1] == SP_Chess_GAME_EMPTY_ENTRY &&
				game->gameBoard[0][2] == SP_Chess_GAME_EMPTY_ENTRY && game->gameBoard[0][3] == SP_Chess_GAME_EMPTY_ENTRY
						&& game->casteling[0]){
			if (!try_move_threatened(game,0,4,0,3) && !try_move_threatened(game,0,4,0,2)){
				return true;
			}
		}
		else if (j == 7 && game->gameBoard[0][6] == SP_Chess_GAME_EMPTY_ENTRY && game->gameBoard[0][5] == SP_Chess_GAME_EMPTY_ENTRY
				&& game->casteling[2]){
			if (!try_move_threatened(game,0,4,0,5) && !try_move_threatened(game,0,4,0,6)){
				return true;
			}
		}
	}
	return false;
}

MOVE_RET SPCastelingMove(SPChessGame* game, int i, int j){
	if (i > 7 || i < 0 || j > 7 || j < 0){
		printf("Invalid position on the board\n");
		return INVALID_POSITION;
	}
	if (getcolor(game,i,j) != game->currentPlayer){
		printf("The specified position does not contain your piece\n");
		return PIECE_NOT_HERE;
	}
	if (!isLeagleCastelingMove(game,i,j,game->currentPlayer)){
		printf("Illegal castling move\n");
		return ILLEAGEL_MOVE;
	}
	SPCommand com;
	com.arg = i;
	com.arg2 = j;
    com.cmd=CASTLE;
	spArrayListAddFirst(game->hist,com);
	castelingMove(game,i,j);
	MOVE_RET mate = isThereCheckMate(game);
	if (mate != VALID_MOVE){
		if (mate == TIE){
			printf("The game is tied\n");
		}
		return mate;
	}
	if (KingThreatened(game,WHITE)){
		printf("Check: white King is threatened!\n");
	}

	if (KingThreatened(game,BLACK)){
		printf("Check: black King is threatened!\n");
	}
	if (game->currentPlayer == BLACK){
		game->currentPlayer = WHITE;
	}
	else {
		game->currentPlayer = BLACK;
	}
	if (game->mode == ONE_PLAYER){
		return  SPComputerMove(game,false);
	}
	return VALID_MOVE;
}

void castelingMove(SPChessGame* game, int i, int j){
	if (i == 0){
		if (j == 0){
			game->gameBoard[0][3] = game->gameBoard[0][0];
			game->gameBoard[0][2] = game->gameBoard[0][4];
			game->gameBoard[0][0] = SP_Chess_GAME_EMPTY_ENTRY;
			game->gameBoard[0][4] = SP_Chess_GAME_EMPTY_ENTRY;
		}
		else {
			game->gameBoard[0][5] = game->gameBoard[0][7];
			game->gameBoard[0][6] = game->gameBoard[0][4];
			game->gameBoard[0][7] = SP_Chess_GAME_EMPTY_ENTRY;
			game->gameBoard[0][4] = SP_Chess_GAME_EMPTY_ENTRY;
		}
	}
	else {
		if (j == 0){
			game->gameBoard[7][3] = game->gameBoard[7][0];
			game->gameBoard[7][2] = game->gameBoard[7][4];
			game->gameBoard[7][0] = SP_Chess_GAME_EMPTY_ENTRY;
			game->gameBoard[7][4] = SP_Chess_GAME_EMPTY_ENTRY;
		}
		else {
			game->gameBoard[7][5] = game->gameBoard[7][7];
			game->gameBoard[7][6] = game->gameBoard[7][4];
			game->gameBoard[7][7] = SP_Chess_GAME_EMPTY_ENTRY;
			game->gameBoard[7][4] = SP_Chess_GAME_EMPTY_ENTRY;
		}
	}
}

void setCasteling(SPChessGame* game, SPCommand* com){
	if (game->currentPlayer == WHITE && ((com->arg == 0 && com->arg2 == 0))){
		game->casteling[0] = false;
	}
	else if (game->currentPlayer == WHITE && ((com->arg == 0 && com->arg2 == 4))){
		game->casteling[1] = false;
	}
	else if (game->currentPlayer == WHITE && ((com->arg == 0 && com->arg2 == 7))){
		game->casteling[2] = false;
	}
	else if (game->currentPlayer == BLACK && ((com->arg == 0 && com->arg2 == 7))){
		game->casteling[3] = false;
	}
	else if (game->currentPlayer == BLACK && ((com->arg == 4 && com->arg2 == 7))){
		game->casteling[4] = false;
	}
	else if (game->currentPlayer == BLACK && ((com->arg == 7 && com->arg2 == 7))){
		game->casteling[5] = false;
	}
}

int isDestInBoard(int x, int y){
	if (x >= 0 && 8 > x && y >= 0 && 8 > y){
		return 1;
	}
	return 0;
}

int nonRecursiveLeagleMoveKing(SPCommand *com){
	if ( ((abs(com->arg-com->arg3)==1 && abs(com->arg2-com->arg4)==0)
			||(abs(com->arg-com->arg3)==0 && abs(com->arg2-com->arg4)==1) ||
			(abs(com->arg-com->arg3)==1 && abs(com->arg2-com->arg4)==1))){
		return 1;
	}
	return 0;
}


int isThreatenByPawn(SPCommand *com, COLOR colorSource, COLOR colorDest){
	if ((com->arg2 == com->arg4-1 ||com->arg2 == com->arg4+1) && colorSource != colorDest){
		if (colorSource == BLACK){
			if (com->arg == com->arg3+1){
				return 1;
			}
			return 0;
		}
		else if (colorSource == WHITE){
			if (com->arg == com->arg3-1){
				return 1;
			}
			return 0;
		}
	}
	return 0;
}
int isLineClear(SPChessGame *game, int x1, int y1, int x2, int y2,  COLOR colorSource,
				COLOR colorDest){
	int dirY = 0;
	int dirX = 0;
	int Steps = max(abs(x1-x2),abs(y1-y2))-1;
	if (x1 == x2){
		if (y1 < y2){
			dirY = 1;
		}
		else {
			dirY = -1;
		}
		for (int i = 1; i < Steps; i++){
			PIECE piece = get_piece_type(game,x1,y1+i*dirY);
			if (piece != NOT){
				return 0;
			}
		}
	}
	else {
		if (x1 < x2){
			dirX = 1;
		}
		else {
			dirX = -1;
		}
        for (int i = 1; Steps >= i; i++){
			PIECE piece = get_piece_type(game,x1+i*dirX,y1);
			if (piece != NOT){
				return 0;
			}
		}
	}
	PIECE pieceDest = get_piece_type(game,x2,y2);
	if (pieceDest != NOT && colorSource == colorDest){
		return 0;
	}
	return 1;
}

int KingThreatened(SPChessGame *game, COLOR colorDest){
	int kingPos = getPiecePos(game, colorDest, KING);
	int x = kingPos/8;
	int y = kingPos%8;
	return PieceThreatened(game,x,y,colorDest);
}

int PieceThreatened(SPChessGame *game, int x, int y,COLOR colorDest){
	for (int i = 0; i < 8; i++){
		for (int j = 0; j < 8; j++){
			COLOR colorSource = getcolor(game,i,j);
			if (GREY != colorSource && colorSource != colorDest){
				PIECE piece = get_piece_type(game,i,j);
				SPCommand com;
				com.arg = i;com.arg2 = j;com.arg3 = x; com.arg4 = y;
                com.overriddenpiece=game->gameBoard[com.arg3][com.arg4];
				switch(piece){
				case(PAWN):
						if(isThreatenByPawn(&com, colorSource, colorDest)){
							return 1;
						}
						break;
				case(BISHOP):
						if (isLeagelMoveBishop(game, &com, colorSource, colorDest)){
							return 1;
						}
						break;
				case(ROOK):
						if(isLeagelMoveRook(game, &com, colorSource, colorDest)){
							return 1;
						}
						break;
				case(KNIGHT):
						if(isLeagelMoveKinght(&com, colorSource, colorDest)){
							return 1;
						}
						break;
				case(QUEEN):
						if(isLeagelMoveQueen(game, &com, colorSource, colorDest)){
							return 1;
						}
						break;
				case(KING):
						if(nonRecursiveLeagleMoveKing(&com)){
							return 1;
						}
						break;
				case(NOT):
						break;

				}
			}
		}
	}
	return 0;
}


int isLeagelMoveKing(SPCommand *com, COLOR colorSource, COLOR colorDest){
	if (colorSource != colorDest && ((abs(com->arg-com->arg3)==1 && abs(com->arg2-com->arg4)==0)||
			(abs(com->arg-com->arg3)==0 && abs(com->arg2-com->arg4)==1) ||
			(abs(com->arg-com->arg3)==1 && abs(com->arg2-com->arg4)==1))){
		return 1;
	}
	return 0;
}

int isLeagelMoveQueen(SPChessGame *game, SPCommand *com, COLOR colorSource, COLOR colorDest){
	if (!isDestInBoard(com->arg3, com->arg4) || colorDest == colorSource){
		return 0;
	}
	else if (com->arg == com->arg3 || com->arg2 == com->arg4){
		return isLineClear(game, com->arg, com->arg2, com->arg3, com->arg4, colorSource, colorDest);
	}
	else if (abs(com->arg-com->arg3)==abs(com->arg2-com->arg4)){
		return isDiagClear(game, com->arg, com->arg2, com->arg3, com->arg4, colorSource, colorDest);
	}
	return 0;
}


int isLeagelMoveKinght(SPCommand *com, COLOR colorSource, COLOR colorDest){
	if (!isDestInBoard(com->arg3, com->arg4) || colorDest == colorSource){
		return 0;
	}
	if (!(abs(com->arg-com->arg3)==2 && abs(com->arg2-com->arg4)==1) && !(abs(com->arg-com->arg3)==1
			&& abs(com->arg2-com->arg4)==2)){
		return 0;
	}
	return 1;
}

int isLeagelMoveRook(SPChessGame *game, SPCommand *com, COLOR colorSource, COLOR colorDest){
	if (!isDestInBoard(com->arg3, com->arg4) || ((com->arg != com->arg3) && (com->arg2 != com->arg4))
			|| !isLineClear(game, com->arg, com->arg2, com->arg3, com->arg4, colorSource,colorDest)){
		return 0;
	}
	return 1;
}

int isLeagelMoveBishop(SPChessGame *game, SPCommand *com, COLOR colorSource, COLOR colorDest){
	if (!isDestInBoard(com->arg3, com->arg4) || (abs(com->arg - com->arg3) != abs(com->arg2 - com->arg4))
			|| !isDiagClear(game, com->arg, com->arg2, com->arg3, com->arg4, colorSource,colorDest)){
		return 0;
	}
	return 1;
}



int isLeagelMovePawn(SPChessGame *game, SPCommand *com, COLOR colorSource, COLOR colorDest){
	if (!isDestInBoard(com->arg3, com->arg4)){
		return 0;
	}
	PIECE pieceDest = get_piece_type(game,com->arg3,com->arg4);
	if (com->arg2 == com->arg4 && pieceDest == NOT){
		if (colorSource == BLACK){
			if (com->arg == com->arg3+1){
				return 1;
			}
			else if (com->arg == 6 && com->arg == com->arg3+2 && GREY == getcolor(game,com->arg-1,com->arg2)){
				return 1;
			}
			return 0;
		}
		else if (colorSource == WHITE){
			if (com->arg == com->arg3-1){
				return 1;
			}
			else if (com->arg == 1 && com->arg == com->arg3-2 && GREY == getcolor(game,com->arg+1,com->arg2)){
				return 1;
			}
			return 0;
		}
	}
	else if ((com->arg2 == com->arg4-1 ||com->arg2 == com->arg4+1) && colorSource != colorDest){
		if (colorSource == BLACK && colorDest == WHITE){
			if (com->arg == com->arg3+1){
				return 1;
			}
			return 0;
		}
		else if (colorSource == WHITE && colorDest == BLACK){
			if (com->arg == com->arg3-1){
				return 1;
			}
			return 0;
		}
	}
	return 0;
}


SP_Chess_GAME_MESSAGE print_board(SPChessGame *game){
    if(game == NULL) {
        return SP_Chess_GAME_INVALID_ARGUMENT;
    }

    for(int i=SP_Chess_GAME_DIM-1;i>-1;i--){
        printf("%d|",i+1);
        for(int j=0;j<SP_Chess_GAME_DIM;j++){
            printf(" %c", game->gameBoard[i][j]);
        }
        printf(" |\n");
    }

    printf("  -----------------\n");
    printf("   A B C D E F G H\n");
    return SP_Chess_GAME_SUCCESS;
}

