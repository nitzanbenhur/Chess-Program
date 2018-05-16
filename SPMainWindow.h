/*
 * window.h
 *
 *  Created on: Aug 8, 2017
 *      Author: nitzan
 */

#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL.h>
#include <SDL_video.h>
#include "active_window.h"
#include "SPWindow.h"
#include "Widget.h"
typedef enum {
	SP_MAIN_EXIT, SP_MAIN_LOAD, SP_MAIN_NEW_GAME, SP_MAIN_INVALID_ARGUMENT, SP_MAIN_NONE
} SP_MAIN_EVENT;

typedef struct {
	//SDL_Window* window;
	SDL_Window* mainWindow;
	Widget** widgets;
	ACTIVE_WINDOW prev;
	SDL_Renderer* mainRenderer;
	int numOfWidgets;
} SPMainWin;
/**
 * handle new game button events
 * @param src the widget
 * @param event the event
 * @param main main window
 */
void handleNewGameButtonEvenet(Widget* src, SDL_Event* event, void* main);
/**
 * handle load button events
 * @param src the widget
 * @param event the event
 * @param main main window
 */
void handleLoadButtonEvenet(Widget* src, SDL_Event* event, void* main);
/**
 * handle exit button events
 * @param src the widget
 * @param event the event
 * @param main main window
 */
void handleExitButtonEvenet(Widget* src, SDL_Event* event, void* main);
/**
 * handles a game event(click, drag, etc)
 * @param src the game window
 * @param event event
 */
void handleEvenetMainWindow(SPWindow* src, SDL_Event* event);
/**
 * draws the game window
 * @param gameWin game
 */
void drawMainWindow(SPWindow* src);
/**
 * frees the game window
 * @param src game window
 */
void destroyMainWindow(SPWindow* src);
/**
 * creates a game window
 * @return the game window
 */
SPWindow* spMainWindowCreate();


#endif /* WINDOW_H_ */
