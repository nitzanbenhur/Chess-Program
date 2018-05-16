/*
 * LoadStore.c
 *
 *  Created on: Aug 27, 2017
 *      Author: nitzan
 */

#include "LoadStore.h"

void setvaluesfromloadedfile(int *values, SPChessGame *game,
                             char tempboard[SP_Chess_GAME_DIM][SP_Chess_GAME_DIM]){
    int k;
    if(values[0]==1){
        game->diff=values[2];
        game->UserCol=(values[3]==0)?BLACK:WHITE;
    }
    game->mode=(MODE)values[0];//(values[0]==1)?ONE_PLAYER:TWO_PLAYERS;
    if(values[1]==0)
        game->currentPlayer=BLACK;
    else
        game->currentPlayer=WHITE;
    for(k=0;k<SP_Chess_GAME_DIM;k++){
        for(int i=0;i<SP_Chess_GAME_DIM;i++){
            game->gameBoard[k][i]=tempboard[k][i];
        }
    }
}



int load(SPCommand *com, SPChessGame* game){
    FILE *fp=NULL;
    fp=fopen(com->file,"r");
    if(fp==NULL){
        printf("Error: File doesn't exist or cannot be opened\n");
        return 0;
    }
    char buff[BUFF_SIZE];
    int values[numofs];
    int k;
    for(k=0;k<numofs;k++){
        values[k]=-997;
    }
    int rownum=0;
    char tempboard[SP_Chess_GAME_DIM][SP_Chess_GAME_DIM]={{0}};
    for(int index=0;index<SP_Chess_GAME_DIM;index++){
        tempboard[index][0]='Y';
    }
    while(fgets(buff, BUFF_SIZE, fp)){//read from file
        strip_spaces(&buff[0]);
        char *vals[][numofs-1]={{"<game_mode>","<current_turn>","<difficulty>"},
                              {"<game_mode>%d</game_mode>", "<current_turn>%d</current_turn>",
                                                              "<difficulty>%d</difficulty>",}};
        for(k=0;k<numofs-1;k++) {
            int temp, n;
            if (startwith(buff, vals[0][k])) {
                n=sscanf(buff, vals[1][k], &temp); //int n=
                if(n==1 && temp != -997) {
                    values[k] = temp;
                }
            }
        }
        char bm[40]="<user_color>0</user_color>";
        char wm[40]="<user_color>1</user_color>";
        if(startwith(buff, bm)){
            values[3]=0;
        }else if(startwith(buff, wm)){
            values[3]=1;
        }
        if(startwith(buff,"<row_")){
            int i,j,n;
            char temps[SP_Chess_GAME_DIM];
            n=sscanf(buff, "<row_%d>%c%c%c%c%c%c%c%c</row_%d>\n", &i,&temps[0],&temps[1],
                     &temps[2],&temps[3],&temps[4],&temps[5],&temps[6],&temps[7],&j);
            if(i<9 && i>0 && tempboard[i-1][0]=='Y' && (n==10) && buff[5] == buff[21]){//row_d in
                // begin tag
                // equals that in end tag
                for (k = 0; k < SP_Chess_GAME_DIM; k++) {
                    tempboard[i-1][k] = temps[k];
                }
            }
        }
        rownum++;
    }
    for(int index=0;index<SP_Chess_GAME_DIM;index++){
        if(tempboard[index][0]=='Y')
            return -1;
    }
    if(values[0] ==-997 || values[1] == -997)
        return -1;
    if(values[0]==1 && (values[2] == -997 || values[3] == -997))
        return -1;

    setvaluesfromloadedfile(values, game, tempboard);
    fclose(fp);
    return 1;
}

void save(SPCommand *com, SPChessGame *game){
    FILE *fp=NULL;
    fp=fopen(com->file,"w");
    if(fp==NULL){
        printf("File cannot be created or modified\n");
        return;
    }
    const char *string1="<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<game>\n";
    fputs(string1, fp);

    int val=(game->currentPlayer==WHITE)?1:0;
    char str[40];
    sprintf(str, "<current_turn>%d</current_turn>\n", val);
    fputs(str, fp);
    sprintf(str, "<game_mode>%d</game_mode>\n", game->mode);
    fputs(str, fp);
    sprintf(str, "<difficulty>%d</difficulty>\n", game->diff);
    fputs(str, fp);
    sprintf(str, "<user_color>%d</user_color>\n<board>\n", (game->UserCol==BLACK)?0:1);
    fputs(str,fp);
    for(int i=SP_Chess_GAME_DIM-1;i>-1;i--){//write game
        sprintf(str, "<row_%d>",i+1);
        fputs(str,fp);
        char *s=malloc(sizeof(char)*(SP_Chess_GAME_DIM+1));
        for(int j=0;j<SP_Chess_GAME_DIM;j++){
            s[j]=game->gameBoard[i][j];
        }
        s[SP_Chess_GAME_DIM]='\0';
        fputs(s, fp);
        sprintf(str, "</row_%d>\n",i+1);
        fputs(str, fp);
        free(s);
    }
    fputs("</board>\n</game>", fp);
    fclose(fp);
}
