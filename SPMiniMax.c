/*
 * SPMiniMaxNode.c
 *
 *  Created on: May 20, 2017
 *      Author: nitzan
 */
#include "SPMiniMax.h"
#include <time.h>
#include <assert.h>


SPCommand get_computer_move(SPChessGame* game, int recursionDepth, COLOR player, char* promote){
    int numofnodes=0;
    SPCommand com;
    if(recursionDepth==5){
    	computePlayAlvl5(game,&com,INT_MIN,INT_MAX,4,player,4,promote,true,&numofnodes);
        if ((com.arg3 != 7 && com.arg3 != 0) ||(get_piece_type(game,com.arg,com.arg2) != PAWN)){
            *promote = 'z';
        }
    	return com;
    }
    computePlayA(game,&com,INT_MIN,INT_MAX,recursionDepth,player,recursionDepth,promote,false,
                 &numofnodes);
    if ((com.arg3 != 7 && com.arg3 != 0) ||(get_piece_type(game,com.arg,com.arg2) != PAWN)){
        *promote = 'z';
    }
	return com;
}

int computePlayAlvl5(SPChessGame* game, SPCommand* BestMove,int alpha, int beta,
                 int recursionDepth, COLOR player,int maxDepth, char* promote, bool expert, int*
numofnodes){
    if(*numofnodes>nodestimeout)
        return INT_MIN;
    (*numofnodes)++;
    if (recursionDepth == 0 && expert)
    	return ScoringFunction_lvl5(game,player);
    int maxVal = INT_MIN;
    int i,j,a,b;
    int allmoves[allmovesmax][maxminarrlen];
    int allmovesindex=get_all_moves(game, player,allmoves, allmovesmax,0);
    if(allmovesindex==0){
        if(KingThreatened(game,game->currentPlayer))
            return maxVal;
        else
            return 0; // tie
    }
    quicksort(allmoves,0,allmovesindex-1);
    for(int index=allmovesindex-1;index>-1;index--) {
        i=allmoves[index][0];
        j=allmoves[index][1];
        a=allmoves[index][2]/8;
        b=allmoves[index][2]%8;
        if (player == getcolor(game, i, j)) {
            char captured = game->gameBoard[a][b];
            move_piece_c(game, i, j, a, b);
            int tmp;
            if (get_piece_type(game, a, b) ==
                PAWN && (a==7 || a==0)){
                tmp = promotePlayBlvl5(game, BestMove, alpha, beta, recursionDepth, player,
                                       maxDepth, a, b,
                                       promote, expert, numofnodes);
            } else {
                tmp = computePlayBlvl5(game, BestMove, alpha, beta, recursionDepth - 1, player,
                                       maxDepth, promote, expert, numofnodes);
            }
            undo_move_min_max(game, i, j, a, b, captured);
            if (tmp > maxVal || (tmp == INT_MIN && maxVal == INT_MIN)) {
                maxVal = tmp;
                if (recursionDepth == maxDepth) {
                    BestMove->arg = i;
                    BestMove->arg2 = j;
                    BestMove->arg3 = a;
                    BestMove->arg4 = b;
                    BestMove->cmd = MOVE;
                }
                if (tmp > alpha)
                    alpha = tmp;
                if (alpha >= beta)
                    return maxVal;
            }
        }
    }
    return maxVal;
}

int promotePlayBlvl5(SPChessGame* game,SPCommand* BestMove,
                 int alpha,int beta,int recursionDepth,COLOR player,int maxDepth,int i,int j,
                 char* promote, bool expert, int* numofnodes){
    int maxVal = INT_MIN;
    if (i == 7){
        char promotions[5] = {wpawn,wbishop,wknight,wrook,wqueen};
        for (int k = 0; k < 5; k++){
            game->gameBoard[i][j] = promotions[k];
            int tmp = computePlayBlvl5(game,BestMove,alpha,beta,recursionDepth-1,player,maxDepth,
                                   promote,expert,numofnodes);
            if (tmp>maxVal){
                maxVal = tmp;
                if (recursionDepth == maxDepth){
                    *promote =  promotions[k];
                }
            }
            game->gameBoard[i][j] = wpawn;
        }
    }
    else {
        char promotions[5] = {bpawn,bbishop,bknight,brook,bqueen};
        for (int k = 0; k < 5; k++){
            game->gameBoard[i][j] = promotions[k];
            int tmp = computePlayBlvl5(game,BestMove,alpha,beta,recursionDepth-1,player,maxDepth,
                                   promote,expert,numofnodes);
            if (tmp>maxVal){
                maxVal = tmp;
                if (recursionDepth == maxDepth){
                    *promote =  promotions[k];
                }
            }
            game->gameBoard[i][j] = bpawn;
        }
    }

    return maxVal;
}

int computePlayBlvl5(SPChessGame* game, SPCommand* BestMove, int alpha, int beta,
                 int recursionDepth, COLOR player, int maxDepth,char* promote,bool expert, int*
numofnodes){
    if(*numofnodes>nodestimeout)
        return INT_MAX;
    (*numofnodes)++;
    if (recursionDepth == 0 && expert)
    	return ScoringFunction_lvl5(game,player);
    int minVal = INT_MAX;
    int i,j,a,b;
    int allmoves[allmovesmax][maxminarrlen];
    int allmovesindex=get_all_moves(game, -1*player,allmoves, allmovesmax,0);
    if(allmovesindex==0) {
        if(KingThreatened(game,game->currentPlayer))
            return minVal;
        else
            return 0;
    }
    quicksort(allmoves,0,allmovesindex-1);
    for(int index=0;index<allmovesindex;index++) {
        i = allmoves[index][0];
        j = allmoves[index][1];
        a = allmoves[index][2] / 8;
        b = allmoves[index][2] % 8;
        if (player != getcolor(game, i, j) && GREY != getcolor(game, i, j)) {
            char captured = game->gameBoard[a][b];
            move_piece_c(game, i, j, a, b);
            int tmp;
            if (get_piece_type(game, a, b) ==
                PAWN && (a == 7 || a == 0)) {
                tmp = promotePlayAlvl5(game, BestMove, alpha, beta, recursionDepth, player, maxDepth, a,
                                   b, promote, expert, numofnodes);
            } else {
                tmp = computePlayAlvl5(game, BestMove, alpha, beta, recursionDepth - 1, player,
                                   maxDepth, promote, expert, numofnodes);
            }
            undo_move_min_max(game, i, j, a, b, captured);
            if (minVal > tmp || (tmp == INT_MAX && minVal == INT_MAX)) {
                minVal = tmp;
                if (beta > minVal)
                    beta = minVal;
                if (alpha >= beta)
                    return minVal;
            }
        }
    }
    return minVal;
}

int promotePlayAlvl5(SPChessGame* game,SPCommand* BestMove,
                 int alpha,int beta,int recursionDepth,COLOR player,int maxDepth,int i,
				 int j,char* promote, bool expert, int* numofnodes){
    int minVal = INT_MAX;
    if (i == 7){
        char promotions[5] = {wpawn,wbishop,wknight,wrook,wqueen};
        for (int k = 0; k < 5; k++){
            game->gameBoard[i][j] = promotions[k];
            int tmp = computePlayAlvl5(game,BestMove,alpha,beta,recursionDepth-1,player,maxDepth,
                                   promote,expert,numofnodes);
            if (tmp<minVal){
                minVal = tmp;
            }
            game->gameBoard[i][j] = wpawn;
        }
    }
    else {
        char promotions[5] = {bpawn,bbishop,bknight,brook,bqueen};
        for (int k = 0; k < 5; k++){
            game->gameBoard[i][j] = promotions[k];
            int tmp = computePlayAlvl5(game,BestMove,alpha,beta,recursionDepth-1,player,maxDepth,
                                   promote,expert,numofnodes);
            if (tmp<minVal){
                minVal = tmp;
            }
            game->gameBoard[i][j] = bpawn;
        }
    }

    return minVal;
}


int computePlayA(SPChessGame* game, SPCommand* BestMove,int alpha, int beta,
                     int recursionDepth, COLOR player,int maxDepth, char* promote, bool expert, int*
numofnodes){
    if(*numofnodes>nodestimeout){
        return INT_MIN;
    }
    (*numofnodes)++;
    if (recursionDepth == 0 && !expert){
        return ScoringFunction(game,player);
    }
    int maxVal = INT_MIN;
    int moves[64];
    moves[0] = -1;
    int i,j,a,b;
    bool hasmoves=false;
    for (i = 0; i < 8; i++){
        for (j = 0; j < 8; j++){
            if (player == getcolor(game, i, j)){
                int index = 0;
                get_moves(game, i, j, moves);
                while (moves[index] != -1) {
                    hasmoves=true;
                    a=moves[index]/8;
                    b=moves[index]%8;
                    char captured = game->gameBoard[a][b];
                    move_piece_c(game, i, j, a, b);
                    int tmp;
                    if (get_piece_type(game, a, b) == PAWN && (a==7 || a==0)){
                        tmp = promotePlayB(game, BestMove, alpha, beta, recursionDepth, player,
                                           maxDepth, a, b,
                                           promote, expert, numofnodes);
                    } else {
                        tmp = computePlayB(game, BestMove, alpha, beta, recursionDepth - 1, player,
                                           maxDepth, promote, expert, numofnodes);
                    }
                    undo_move_min_max(game, i, j, a, b, captured);
                    if (tmp > maxVal || (tmp == INT_MIN && maxVal == INT_MIN)) {
                        maxVal = tmp;
                        if (recursionDepth == maxDepth) {
                            BestMove->arg = i;
                            BestMove->arg2 = j;
                            BestMove->arg3 = a;
                            BestMove->arg4 = b;
                            BestMove->cmd = MOVE;
                        }
                        if (tmp > alpha) {
                            alpha = tmp;
                        }
                        if (alpha >= beta) {
                            return maxVal;
                        }
                    }
                    index++;
                }
            }
        }
    }
    if(!hasmoves && !KingThreatened(game,player))
        return 0; // tie
    return maxVal;
}

int promotePlayB(SPChessGame* game,SPCommand* BestMove,
                     int alpha,int beta,int recursionDepth,COLOR player,int maxDepth,int i,int j,
                     char* promote, bool expert, int* numofnodes){
    int maxVal = INT_MIN;
    if (i == 7){
        char promotions[5] = {wpawn,wbishop,wknight,wrook,wqueen};
        for (int k = 0; k < 5; k++){
            game->gameBoard[i][j] = promotions[k];
            int tmp = computePlayB(game,BestMove,alpha,beta,recursionDepth-1,player,maxDepth,
                                       promote,expert,numofnodes);
            if (tmp>maxVal){
                maxVal = tmp;
                if (recursionDepth == maxDepth){
                    *promote =  promotions[k];
                }
            }
            game->gameBoard[i][j] = wpawn;
        }
    }
    else {
        char promotions[5] = {bpawn,bbishop,bknight,brook,bqueen};
        for (int k = 0; k < 5; k++){
            game->gameBoard[i][j] = promotions[k];
            int tmp = computePlayB(game,BestMove,alpha,beta,recursionDepth-1,player,maxDepth,
                                       promote,expert,numofnodes);
            if (tmp>maxVal){
                maxVal = tmp;
                if (recursionDepth == maxDepth){
                    *promote =  promotions[k];
                }
            }
            game->gameBoard[i][j] = bpawn;
        }
    }

    return maxVal;
}

int computePlayB(SPChessGame* game, SPCommand* BestMove, int alpha, int beta,
                     int recursionDepth, COLOR player, int maxDepth,char* promote,bool expert, int*
numofnodes){
    if(*numofnodes>nodestimeout)
        return INT_MAX;
    (*numofnodes)++;
    if (recursionDepth == 0 && !expert)
        return ScoringFunction(game,player);
    if (recursionDepth == 0 && expert)
        return ScoringFunction_lvl5(game,player);
    int minVal = INT_MAX;
    int moves[64];
    moves[0] = -1;
    int i,j,a,b;
    bool hasmoves=false;
    for (i = 0; i < 8; i++){
        for (j = 0; j < 8; j++){
            if (player != getcolor(game, i, j) && GREY != getcolor(game, i, j)) {
                int index = 0;
                get_moves(game,i,j,moves);
                while (moves[index] != -1){
                    hasmoves=true;
                    a=moves[index]/8;
                    b=moves[index]%8;
                    char captured = game->gameBoard[a][b];
                    move_piece_c(game, i, j, a, b);
                    int tmp;
                    if (get_piece_type(game, a, b) ==
                        PAWN && (a == 7 || a == 0)) {
                        tmp = promotePlayA(game, BestMove, alpha, beta, recursionDepth, player, maxDepth, a,
                                           b,
                                           promote, expert, numofnodes);
                    } else {
                        tmp = computePlayA(game, BestMove, alpha, beta, recursionDepth - 1, player,
                                           maxDepth, promote, expert, numofnodes);
                    }
                    undo_move_min_max(game, i, j, a, b, captured);
                    if (minVal > tmp || (tmp == INT_MAX && minVal == INT_MAX)) {
                        minVal = tmp;
                        if (beta > minVal) {
                            beta = minVal;
                        }
                        if (alpha >= beta) {
                            return minVal;
                        }
                    }
                    index++;
                }
            }
        }
    }
    if(!hasmoves && !KingThreatened(game,player*-1))
        return 0; // tie
    return minVal;
}

int promotePlayA(SPChessGame* game,SPCommand* BestMove,
                     int alpha,int beta,int recursionDepth,COLOR player,int maxDepth,int i,
                     int j,char* promote, bool expert, int* numofnodes){
    int minVal = INT_MAX;
    if (i == 7){
        char promotions[5] = {wpawn,wbishop,wknight,wrook,wqueen};
        for (int k = 0; k < 5; k++){
            game->gameBoard[i][j] = promotions[k];
            int tmp = computePlayA(game,BestMove,alpha,beta,recursionDepth-1,player,maxDepth,
                                       promote,expert,numofnodes);
            if (tmp<minVal){
                minVal = tmp;
            }
            game->gameBoard[i][j] = wpawn;
        }
    }
    else {
        char promotions[5] = {bpawn,bbishop,bknight,brook,bqueen};
        for (int k = 0; k < 5; k++){
            game->gameBoard[i][j] = promotions[k];
            int tmp = computePlayA(game,BestMove,alpha,beta,recursionDepth-1,player,maxDepth,
                                       promote,expert,numofnodes);
            if (tmp<minVal){
                minVal = tmp;
            }
            game->gameBoard[i][j] = bpawn;
        }
    }

    return minVal;
}


void undo_move_min_max(SPChessGame* game,int i,int j,int x,int y,char captured){
	game->gameBoard[i][j] = game->gameBoard[x][y];
	game->gameBoard[x][y] = captured;
}

int ScoringFunction(SPChessGame* game, COLOR player){
	MOVE_RET checkmate = isThereCheckMate(game);
	switch(checkmate){
	case BLACK_MATE:
		if (player == BLACK){
			return INT_MIN;
		}
		else {
			return INT_MAX;
		}
	case (WHITE_MATE):
			if (player == WHITE){
				return INT_MIN;
			}
			else {
				return INT_MAX;
			}
	case (TIE):
			return 0;
	default:
		break;
	}
	int score = 0;
    int i;
    int j;
	for (i = 0; i < 8; i++){
		for (j = 0; j < 8; j++){
			PIECE piece = get_piece_type(game,i,j);
			int tmp;
			switch(piece){
			case(PAWN):
					tmp = 1;
					break;
			case(KNIGHT):
					tmp = 3;
					break;
			case(BISHOP):
					tmp = 3;
					break;
			case(ROOK):
					tmp = 5;
					break;
			case(QUEEN):
					tmp = 9;
					break;
			case(KING):
					tmp = 100;
					break;
			default:
					tmp = 0;
					break;
			}
			if (player == getcolor(game,i,j)){
				score += tmp;
			}
			else {
				score -= tmp;
			}
		}
	}
	return score;
}

int ScoringFunction_lvl5(SPChessGame* game, int player){
	int score = 0;
    int i;
    int j;
	for (i = 0; i < 8; i++){
		for (j = 0; j < 8; j++){
			PIECE piece = get_piece_type(game,i,j);
			COLOR color = getcolor(game,i,j);
			int tmp = pieceScorelvl5(color,piece,i,j);
			if (player == getcolor(game,i,j)){
				score += tmp;
			}
			else {
				score -= tmp;
			}
		}
	}
	return score;
}

int pieceScorelvl5(COLOR color, PIECE type, int i, int j){
    return pieceval(type)+piecelocval(color,type,i,j);
}

int pieceval(PIECE type){
    int fact=4;
    switch(type){
        case(PAWN):
            return 1000*fact;
        case(ROOK):
            return 5000*fact;
        case(KNIGHT):
            return 3200*fact;
        case(BISHOP):
            return 3300*fact;
        case(QUEEN):
            return 9000*fact;
        case(KING):
            return 20000*fact;
        default:
            return 0;
    }

}

int piecelocval(COLOR color, PIECE type, int i,int j){
    int pawnTable[8][8] = {{0,  0,  0,  0,  0,  0,  0,  0},
                           {50, 50, 50, 50, 50, 50, 50, 50},
                           {10, 10, 20, 30, 30, 20, 10, 10},
                           {5,  5, 10, 25, 25, 10,  5,  5},
                           { 0,  0,  0, 20, 20,  0,  0,  0},
                           {5, -5,-10,  0,  0,-10, -5,  5},
                           { 5, 10, 10,-20,-20, 10, 10,  5},
                           {0,  0,  0,  0,  0,  0,  0,  0}};
    int knightTable[8][8]= {{-50,-40,-30,-30,-30,-30,-40,-50},
                            {-40,-20,  0,  0,  0,  0,-20,-40},
                            {-30,  0, 10, 15, 15, 10,  0,-30},
                            {-30,  5, 15, 20, 20, 15,  5,-30},
                            {-30,  0, 15, 20, 20, 15,  0,-30},
                            {-30,  5, 10, 15, 15, 10,  5,-30},
                            {-40,-20,  0,  5,  5,  0,-20,-40},
                            {-50,-40,-30,-30,-30,-30,-40,-50}};
    int bishopTable[8][8] =  {{-20,-10,-10,-10,-10,-10,-10,-20},
                              {-10,  0,  0,  0,  0,  0,  0,-10},
                              {-10,  0,  5, 10, 10,  5,  0,-10},
                              {-10,  5,  5, 10, 10,  5,  5,-10},
                              {-10,  0, 10, 10, 10, 10,  0,-10},
                              {-10, 10, 10, 10, 10, 10, 10,-10},
                              {-10,  5,  0,  0,  0,  0,  5,-10},
                              {-20,-10,-10,-10,-10,-10,-10,-20}};
    int rookTable[8][8] =  {{ 0,  0,  0,  0,  0,  0,  0,  0},
                            {5, 10, 10, 10, 10, 10, 10,  5},
                            {-5,  0,  0,  0,  0,  0,  0, -5},
                            {-5,  0,  0,  0,  0,  0,  0, -5},
                            {-5,  0,  0,  0,  0,  0,  0, -5},
                            {-5,  0,  0,  0,  0,  0,  0, -5},
                            {-5,  0,  0,  0,  0,  0,  0, -5},
                            {0,  0,  0,  5,  5,  0,  0,  0}};
    int queenTable[8][8] = {{-20,-10,-10, -5, -5,-10,-10,-20},
                            {-10,  0,  0,  0,  0,  0,  0,-10},
                            {-10,  0,  5,  5,  5,  5,  0,-10},
                            { -5,  0,  5,  5,  5,  5,  0, -5},
                            { 0,  0,  5,  5,  5,  5,  0, -5},
                            {-10,  5,  5,  5,  5,  5,  0,-10},
                            {-10,  0,  5,  0,  0,  0,  0,-10},
                            {-20,-10,-10, -5, -5,-10,-10,-20}};
    int kingTable[8][8] = {{-30,-40,-40,-50,-50,-40,-40,-30},
                           {-30,-40,-40,-50,-50,-40,-40,-30},
                           {-30,-40,-40,-50,-50,-40,-40,-30},
                           {-30,-40,-40,-50,-50,-40,-40,-30},
                           {-20,-30,-30,-40,-40,-30,-30,-20},
                           {-10,-20,-20,-20,-20,-20,-20,-10},
                           {20, 20,  0,  0,  0,  0, 20, 20},
                           { 20, 30, 10,  0,  0, 10, 30, 20}};
    if (color == BLACK){
        i = abs(i-7);
    }
    switch(type){
        case(PAWN):
            return pawnTable[i][j];
        case(ROOK):
            return rookTable[i][j];
        case(KNIGHT):
            return knightTable[i][j];
        case(BISHOP):
            return bishopTable[i][j];
        case(QUEEN):
            return queenTable[i][j];
        case(KING):
            return kingTable[i][j];
        default:
            return 0;
    }
}


void swap(int L[][maxminarrlen],int left, int right){
    double temp[maxminarrlen];
    for(int i=0;i<maxminarrlen;i++){
        temp[i]=L[left][i];
    }for(int i=0;i<maxminarrlen;i++) {
        L[left][i] = L[right][i];
        L[right][i] = temp[i];
    }
}

int partition(int L[][maxminarrlen], int left, int right, bool greaterorder){
    int pivot = left;
    double p_val = L[pivot][3];
    while (left < right) {
        while (((L[left][3] <= p_val && greaterorder) || (!greaterorder && L[left][3] >= p_val))
               && left<right)
            left++;
        while (((p_val < L[right][3] && greaterorder) || (!greaterorder && p_val> L[right][3])) &&
               left<right)
            right--;
        if (left < right)
            swap(L, left, right);
    }
    swap(L , left, right);
    return right;
}

void quicksort(int L[][maxminarrlen], int start, int end){
    if (start >= end)
        return;
    int splitPoint = partition(L, start, end, true);
    quicksort(L, start, splitPoint - 1);
    quicksort(L, splitPoint + 1, end);
}

int get_all_moves(SPChessGame *game, int player, int allmoves[][maxminarrlen], int arrlen, int
positiveonly){
    int all_moves_index=0;
    int index=0,i,j,diffholder=0;
    int moves[64];
    SPCommand com;COLOR color;PIECE type;
    //TODO recursive loop to ScoringFunctionlev5l!! create simple function to just get num of
    //rewrite using rules rather than naive way, which is 20 times slower!!
    for (i = 0; i < 8; i++){
        for (j = 0; j < 8; j++){
            if (player == getcolor(game,i,j)) {
                index = 0;
                get_moves(game,i,j,moves);
                while (moves[index] != -1) {
                    //pieceval(type)+piecelocval(color,type,i,j)
                    //treat pawn promotion as +queen
                    color=(COLOR)getcolor(game,i,j);
                    type=get_piece_type(game,i,j);
                    diffholder=-1*pieceScorelvl5(color,type,i,j);
                    diffholder-=pieceScorelvl5((COLOR)getcolor(game,moves[index]/8,moves[index]%8),
                                               get_piece_type(game,moves[index]/8,moves[index]%8),
                                               i,j);
                    com.arg=i;
                    com.arg2=j;
                    com.arg3=moves[index] / 8;
                    com.arg4=moves[index] % 8;
                    if(ispawnpromotioncommand(game,&com)){
                        diffholder+=pieceScorelvl5(color,QUEEN,moves[index]/8,moves[index]%8);
                    }else{
                        diffholder+=pieceScorelvl5(color,type,moves[index]/8,moves[index]%8);
                    }

                    char captured = move_piece_c(game, i, j, moves[index] / 8, moves[index] % 8);
                    int score=ScoringFunction_lvl5(game, player);
                    if (all_moves_index < arrlen && (positiveonly==0 || score>=0.0)) {
                        allmoves[all_moves_index][0] = i;
                        allmoves[all_moves_index][1] = j;
                        allmoves[all_moves_index][2] = moves[index];
                        allmoves[all_moves_index][3]= diffholder;
                        all_moves_index++;
                    }
                    undo_move_min_max(game, i, j, moves[index] / 8, moves[index] % 8, captured);
                    index++;
                }
            }
        }
    }
    return all_moves_index;
}