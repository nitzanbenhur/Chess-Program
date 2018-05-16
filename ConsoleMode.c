/*
 * ConsoleMode.c

 *
 *  Created on: Aug 14, 2017
 *      Author: nitzan
 */

#include <stdlib.h>
#include "ConsoleMode.h"


void consuleMainLoop(GAME_STATE *mode, int *cond, SPChessGame* game, SPCommand
*command){
    while(*cond){
        char str[MAXLEN+1];
        readline(str);
        *command = praseLine(str,game->mode, game->gameBoard);
        gameplay(mode, cond, game, command);
	}
}

void printgame(SPChessGame *game){
    print_board(game);
	printUserInput(game);
}

void printUserInput(SPChessGame *game){
    if (game->currentPlayer == BLACK){
        printf("black player - enter your move:\n");
    }
    else {
        printf("white player - enter your move:\n");
    }
}

void gameplay(GAME_STATE *mode, int *cond, SPChessGame* game,
              SPCommand *command){
    if (*mode == SETTING){
        inoutCommandFromUserMode(cond,game,mode,command);
    }
    else {
        inoutCommandFromUserGame(cond,game,command,mode);
    }
}

void inoutCommandFromUserMode(int* cond, SPChessGame* game, GAME_STATE* mode, SPCommand *command){
	switch(command->cmd){
	case(GAMEMODE):
			if (command->arg == 1){
				printf("Game mode is set to 1 player\n");
				game->mode = ONE_PLAYER;
			}
			else if (command->arg == 2){
				printf("Game mode is set to 2 players\n");
				game->mode = TWO_PLAYERS;
			}
			break;
	case(DIFFICULTY):
            game->diff = command->arg;
			break;
	case(USERCOLOR):
			if (command->arg == 0)
				game->UserCol = BLACK;
			else
				game->UserCol = WHITE;
            break;
	case(LOAD):
            load(command, game);
			break;
	case(DEFAULT):
			game->UserCol = WHITE;
			game->diff = 2;
			game->mode = ONE_PLAYER;
			break;
	case(PRINT):
            onprint(game);
			break;
	case(QUIT):
			*cond = 0;
			printf("Exiting...\n");
			break;
	case(START):
        onstart(cond, game, mode);
        break;
	default:	
		 	break;

	}
}
void onprint(SPChessGame* game){
    if (game->mode == TWO_PLAYERS){
        printf("SETTINGS:\n");
        printf("GAME_MODE: 2\n");
    }
    else {
        printf("SETTINGS:\n");
        printf("GAME_MODE: 1\n");
        printf("DIFFICULTY_LVL: %d\n",game->diff);
        if (game->UserCol == BLACK)
            printf("USER_CLR: BLACK\n");
        else
            printf("USER_CLR: WHITE\n");
    }

}
void onstart(int* cond, SPChessGame* game, GAME_STATE* mode){
    *mode = GAME;
    if (game->mode == ONE_PLAYER && game->UserCol != game->currentPlayer){
        MOVE_RET ret = SPComputerMove(game,false);
        switch(ret){
            case BLACK_MATE:
                printf("Checkmate! white player wins the game\n");
                *cond = 0;
                return;
            case WHITE_MATE:
                printf("Checkmate! black player wins the game\n");
                *cond = 0;
                return;
            case TIE:
                *cond = 0;
                return;
            default:
                break;
        }
    }
    printgame(game);
}
void pawnpromotion(SPChessGame* game, SPCommand* command){
    char str[MAXLEN+1];
    int color=getcolor(game, command->arg3, command->arg4);
    if(ispawnpromotioncommand(game,command)){
        str[0]='\0';
        char* options[5]={"queen","rook","knight","bishop","pawn"};
        char optionchar[2][5]={{bqueen,brook,bknight,bbishop,bpawn},
                               {wqueen,wrook,wknight,wbishop,wpawn}};
        bool promoteflag=false;
        bool firsttime=true;
        int i;
        while(!promoteflag) {
            for (i = 0; i < 5; i++) {
                if (startwith(options[i], str) &&
                    startwith(str, options[i])) {//if str are equal
                    promoteflag = true;
                    break;
                }
            }
            if(!promoteflag){
                if(!firsttime)
                    printf("Invalid Type\n");
                printf("Pawn promotion- please replace the pawn by queen, rook, knight, bishop, "
                               "or pawn:\n");
                readline(str);
            }
            firsttime=false;
        }
        game->gameBoard[command->arg3][command->arg4]=optionchar[(color==WHITE)?1:0][i];
    }
}
void onmove(int* cond, SPChessGame* game, SPCommand *command){
    MOVE_RET ret;
    ret = SPChessMovePiece(game, command,false);
    switch(ret){
        case BLACK_MATE:
            printf("Checkmate! white player wins the game\n");
            *cond = 0;
            return;
        case WHITE_MATE:
            printf("Checkmate! black player wins the game\n");
            *cond = 0;
            return;
        case TIE:
            *cond = 0;
            return;
        case(VALID_MOVE):
            printgame(game);
            break;
        default:
            printUserInput(game);
            break;
    }
}
void oncastle(int* cond, SPChessGame* game, SPCommand *command){
    MOVE_RET ret;
    ret = SPCastelingMove(game,command->arg,command->arg2);
    switch(ret){
        case BLACK_MATE:
            printf("Checkmate! white player wins the game\n");
            *cond = 0;
            return;
        case WHITE_MATE:
            printf("Checkmate! black player wins the game\n");
            *cond = 0;
            return;
        case TIE:
            *cond = 0;
            return;
        case(VALID_MOVE):
            printgame(game);
            break;
        default:
            break;
    }

}
void inoutCommandFromUserGame(int* cond, SPChessGame* game, SPCommand *command, GAME_STATE* mode){

        switch(command->cmd){
        case(QUIT):
            *cond = 0;
            printf("Exiting...\n");
            break;
        case(MOVE):
            onmove(cond, game,command);
            break;
    case(UNDO):
        if(SPChessUndoMove(game))
            printgame(game);
        else
			printUserInput(game);
        break;
    case(RESET):
        SPChessRestart(game);
        game->UserCol = WHITE;
        game->diff = 2;
        game->mode = ONE_PLAYER;
        *mode = SETTING;
        printf("Specify game setting or type 'start' to begin a game with the current setting:\n");
        break;
    case(GETMOVES):
        if((game->diff!=2 && game->diff!=1) || game->mode!=1)
            printf("Level must be 1 or 2 to use get_moves, game mode must be 1\n");
        else if(getcolor(game,command->arg,command->arg2)!=game->currentPlayer)
            printf("The specified position does not contain %s player piece\n", colors(game));
        else
            SPChessGetMoves(game, command);
        printUserInput(game);
        break;
    case(SAVE):
        save(command,game);
        printUserInput(game);
        break;
    case(CASTLE):
        oncastle(cond,game,command);
    default:
        if(*cond != 0)
		    printUserInput(game);
        break;
    }
}

void readline(char str[MAXLEN+1]){
    char c;
    int i=0;
    char a =getchar();  // make sure no \n remain from scanf.
    while ((c = getchar()) != EOF && i < MAXLEN){
        if(a!='\n' && i==0){
            str[i] = a;
            i++;
        }
        if (c != '\n'){
            str[i] = c;
        }
        else{
            break;
        }
        i++;
    }
    str[i] = '\000';
}
