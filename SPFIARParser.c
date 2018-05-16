#include "SPFIARParser.h"
#include "SPChessGame.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
bool isInt(const char *str){
	int i = 0;
	while (str[i] != '\0'){
		if((i==0 && (str[i] == '+' || str[i] == '-')) ||
             (str[i] >= '0' && str[i] <= '9')){
			i++;
		}
		else {
			return false;
		}
	}
	return true;
}
void strip_spaces(char str[BUFF_SIZE]){
    char ptr[BUFF_SIZE+1];
    int i,j=0;
    for(i=0;str[i]!='\0';i++){
        if (*(str+i) != ' ' && *(str+i) != '\t' && *(str+i)!='\r') {
            ptr[j++] = *(str+i);
        }
    }
    for(int k=0;k<j;k++){
        *(str+k) = ptr[k];
    }
    str[j]='\0';
}
int checkdelim(char c, int option){
    if(isachar(c)){
        if(option==1){
            if(c!=',' && c!='<' && c!='>'){
                return 1;
            }
            return 0;
        }else {
            return 1;
        }
    }
    return 0;
}
int readtoken(int i, char* dest,const char* src, int option){
    while(!(checkdelim(src[i], option))){
        i++;
    }
    int j =0;
    while(checkdelim(src[i], option) && src[i]!='\0'){
        dest[j] = src[i];
        i++;
        j++;
    }
    dest[j] = '\0';
    return i;
}
//command makes sure arg is int, but doesn't care about value
void invalidate(SPCommand *com) {
    com->cmd = SP_INVALID_LINE;
}
bool startwith(char *s1, char *s2){
    int i=0;
    int j=0;
    while(s1[j]==' ' || s1[j]=='\t' || s1[j]=='\r')
        j++;
    while(s1[j]!='\0' && s2[i]!='\0'){
        if(s1[j] != s2[i]){
            return false;
        }
        j++;
        i++;
    }
    if(s1[i]=='\0' && s2[i]!='\0'){
        return false;
    }
    return true;

}


int setIntArg(SPCommand *com, int option, char *str1, const char *str, int j, int commandtype){
    if(com->cmd != SP_INVALID_LINE){
        j = readtoken(j, str1, str, commandtype);
        if(str1 != NULL && ((isInt(str1) && option%2==0) || (option%2==1 &&
                str1[1]=='\0'))){
            if(option == 0) {
                com->arg = atoi(str1);
                if(com->cmd!=DIFFICULTY && com->cmd!=USERCOLOR && com->cmd!=GAMEMODE)
                    com->arg-=1;
            }else if(option == 1){
                if(str1[0]-'A' > -1 && str1[0]-'A' < 8)
                    com->arg2 = str1[0]-'A';
                else{
                    com->arg2 = -1;
                }
            }else if(option == 2){
                com->arg3 = atoi(str1)-1;
            }else if(option == 3){
                if(str1[0]-'A' > -1 && str1[0]-'A' < 8)
                    com->arg4 = str1[0]-'A';
                else{
                    com->arg4=-1;
                }
            }
        }else{
            com->cmd = SP_INVALID_LINE;
            return -1;
        }
        return j;
    }
    return -1;
}
void errors(SPCommand *com, int gamemode){
    //print error messages about commmands
    if((com->cmd == GAMEMODE &&(com->arg != 1 && com->arg != 2))){
        printf("Wrong game mode\n");
        invalidate(com);
    }else if(com->cmd == DIFFICULTY){
        if(com->arg<1 || com->arg>5){
            invalidate(com);
            printf("Wrong difficulty level. The value should be between 1 to 5\n");
        }
    }else if(com->cmd == USERCOLOR && gamemode == TWO_PLAYERS){
        invalidate(com);
        //printf("ILLEGAL COMMAND, make sure arg is int if necessary");
    }else if(com->cmd == USERCOLOR && (com->arg !=0 && com->arg != 1)){
        invalidate(com);
        //printf("ILLEGAL COMMAND, make sure arg is int if necessary");
    }else if(com->cmd == SP_INVALID_LINE){
        //printf("ILLEGAL COMMAND, make sure arg is int if necessary");
    }else if(com->cmd == MOVE || com->cmd == GETMOVES || com->cmd==CASTLE){
        if(com->arg < 0 || com->arg > 7 ||
                 com->arg2 < 0 || com->arg2 > 7){
            invalidate(com);
        }
        if(com->cmd==MOVE && (com->arg3 < 0 || com->arg3 > 7 ||
                              com->arg4 < 0 || com->arg4 > 7)){
            invalidate(com);
        }
        if(com->cmd==SP_INVALID_LINE){
            printf("Invalid position on the board\n");
        }
    }


}
int command_type(char *str1, SPCommand *com){
    char *cats[][size_of_com] = {{"game_mode","difficulty","user_color","user_color","user_color",
                               "user_color"},
                      {"default", "print_setting","quit","start","undo","reset"},
                      {"load", "save", "save", "save", "save", "save"}
    };
    SP_COMMAND catscommands[][size_of_com] = {{GAMEMODE, DIFFICULTY, USERCOLOR,USERCOLOR,USERCOLOR,
                                      USERCOLOR},
                       {DEFAULT, PRINT, QUIT, START, UNDO, RESET},
                       {LOAD,SAVE,SAVE,SAVE,SAVE,SAVE}
    };

    for(int j=0;j<3;j++) {
        for (int i = 0; i < size_of_com; i++) {
            if(strcmp(cats[j][i], str1) == 0){
                com->cmd = catscommands[j][i];
                return j;
            }
        }
    }
    if(strcmp("get_moves",str1)==0){
        com->cmd = GETMOVES;
        return 3;
    }if(strcmp("castle",str1)==0){
        com->cmd = CASTLE;
        return 3;
    }else if(strcmp("move",str1)==0){
        com->cmd=MOVE;
        return 4;
    }
    com->cmd=SP_INVALID_LINE;
    return -1;
}
void copy(SPCommand *com, char* string){
    int i=0;
    while(string[i]!='\0' && i<MAX_FILE_LENGTH){
        com->file[i]=string[i];
        i++;
    }
    com->file[i]='\0';
}
SPCommand praseLine(const char *str, int gamemode, char gameBoard[GAMESIZE][GAMESIZE]){
	SPCommand out;
    char str1[120];
    int commandtype=0;
    int j = readtoken(0, str1, str, commandtype);
    int category= command_type(str1, &out);
    if(out.cmd!=SP_INVALID_LINE){
        if(category==0){
            bool flag = false;
            int k=j;
            for(;str[k]!='\0';k++){
                if(isachar(str[k])){
                    flag = true;
                    break;
                }
            }
            if(flag) {
                j = setIntArg(&out, 0, str1, str, j, commandtype);
                j = readtoken(j, str1, str, commandtype);
            }else{
                j=k;
                if(out.cmd==GAMEMODE||out.cmd==USERCOLOR){
                    out.arg=1;
                }else if(out.cmd==DIFFICULTY){
                    out.arg=2;
                }
            }
        }else if(category==1){
            j = readtoken(j, str1, str, commandtype);
        }else if(category==2){
            j = readtoken(j, str1, str, commandtype);
            copy(&out,str1);
            j = readtoken(j, str1, str, commandtype);
        }else if(out.cmd == MOVE){
            commandtype=1;
            j = setIntArg(&out, 0, str1, str, j, commandtype);
            j = setIntArg(&out, 1, str1, str, j, commandtype);
            j = readtoken(j, str1, str, commandtype); // matches to
            if(strcmp("to", str1) ==0) {
                j = setIntArg(&out, 2, str1, str, j, commandtype);
                j = setIntArg(&out, 3, str1, str, j, commandtype);
                if(out.cmd!=SP_INVALID_LINE && out.arg3 !=-1 && out.arg4!=-1)
                    out.overriddenpiece=gameBoard[out.arg3][out.arg4];
            }else{
                invalidate(&out);
            }
            j=readtoken(j, str1, str, commandtype);
        }else if(out.cmd == GETMOVES || out.cmd == CASTLE){
            commandtype=1;
            j = setIntArg(&out, 0, str1, str, j, commandtype);
            j = setIntArg(&out, 1, str1, str, j, commandtype);
            //j = readtoken(j, str1, str, commandtype); // matches to
            j=readtoken(j, str1, str, commandtype);
        }
        if(str[j]!='\0'){
            invalidate(&out);
        }
    }

    if(out.cmd==SP_INVALID_LINE){
        //printf("not a valid command name!\n");
    }else{
        errors(&out, gamemode);
    }
    return out;
}
