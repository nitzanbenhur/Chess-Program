#ifndef SPFIARPARSER_H_
#define SPFIARPARSER_H_
#include <stdbool.h>
#define GAMESIZE 8
#define MAX_FILE_LENGTH 1000
#define BUFF_SIZE 2550
#define size_of_com 6
#define isachar(c) (c) != ' ' && (c) != '\r' && (c) != '\t' && (c) != '\n'
//a type used to represent a command
typedef enum {
    GAMEMODE=0,
	DIFFICULTY=1,
	USERCOLOR=2,
	LOAD=3,
    DEFAULT=4,
	PRINT=5,
	QUIT=6,
	START=7,

    MOVE=8,
    GETMOVES=9,
    SAVE=10,
    UNDO=11,
    RESET=12,
	SP_INVALID_LINE=13,
	CASTLE=14,
	PROMOTE=15
} SP_COMMAND;
//a new type that is used to encapsulate a parsed line
typedef struct command_t {
	SP_COMMAND cmd;
	//bool validArg; //is set to true if the line contains a valid argument
	int arg;
    int arg2;
    int arg3;
    int arg4;
    char file[MAX_FILE_LENGTH];
    char overriddenpiece;
} SPCommand;
typedef struct gamesetting {
    int gamemode;//1
    int gamedifficulty; //2
    int usercolor;//1
} Setting;

/**
 * Checks if a specified string represents a valid integer. It is recommended
 * to use this function prior to calling the standard library function atoi.
 *
 * @return
 * true if the string represents a valid integer, and false otherwise.
 */
bool isInt(const char *str);
/**
 * given a line, returns a command out of it
 * @param str line
 * @param settings setting
 * @return command corresponding to line, or INVALID command
 */
SPCommand praseLine(const char *str, int gamemode,
					char gameBoard[GAMESIZE][GAMESIZE]);
/**
 * strips all spaces and tabs from string
 * @param str the string
 */
void strip_spaces(char str[BUFF_SIZE]);
/**
 * read from i until next delimiter and stores it
 * @param i start index
 * @param dest destination string
 * @param src source string
 * @param option for check delim method
 * @return next index after delimiter
 */
int readtoken(int i, char* dest,const char* src, int option);
/**
 * sets command argument(arg,arg2,arg3, or arg4) from given string
 * @param com command
 * @param option which argument
 * @param str1 value of argument
 * @param str string of command
 * @param j index of next letter after delimiter
 * @param commandtype which type of command, based on which parameters recieves
 * @return
 */
int setIntArg(SPCommand *com, int option, char *str1, const char *str, int j, int commandtype);
/**
 * set command to
 * @param com command
 */
void invalidate(SPCommand *com);
/**
 * prints error in command(ie out of range args, etc)
 * @param com command
 * @param setting settings
 */
void errors(SPCommand *com, int gamemode);
/**
 * classifies command according to parameters input and returns number of category
 * @param str1 string of command
 * @param com command
 * @return which type of command
 */
 int command_type(char *str1, SPCommand *com);
/**
 * return true if s1 starts with s2, otherwise false
 * @param s1
 * @param s2
 * @return true/false
 */
bool startwith(char *s1, char *s2);
#endif
