/*
 * GameModeWindow.h
 *
 *  Created on: Aug 9, 2017
 *      Author: nitzan
 */
#ifndef GAMEMODEWINDOW_H_
#define GAMEMODEWINDOW_H_


#include <SDL_video.h>
#include <SDL.h>
#include "active_window.h"
#include "SPWindow.h"
#include "Widget.h"


typedef struct {
	SDL_Window* modeWindow;
	Widget** widgets;
	ACTIVE_WINDOW prev;
	SDL_Renderer* modeRenderer;
	int numOfWidgets;
	int onePlayer;
	ACTIVE_WINDOW prevWin;
} SPModeWin;
/**
 * frees memory and release window
 * @param src window to release
 */
void destroyModeWindow(SPWindow* src);
/**
 * set window to draw mode
 * @param src window
 */
void drawModeWindow(SPWindow* src);
/**
 * handle back button events
 * @param src the widget
 * @param event the event
 * @param mode window mode
 */
void handleBackButtonEvenet(Widget* src, SDL_Event* event, void* mode);
/**
 * handles a mode window event
 * @param src the window
 * @param event the event
 */
void handleEvenetModeWindow(SPWindow* src, SDL_Event* event);
/**
 * handle next button click event
 * @param src the widget aka button
 * @param event the event
 * @param mode the window mode
 */
void handleNextButtonEvenet(Widget* src, SDL_Event* event, void* mode);
/**
 * handle a one player button click
 * @param src the widget aka button
 * @param event the event
 * @param mode the window mode
 */
void handleOnePlayerButtonEvenet(Widget* src, SDL_Event* event, void* mode);
/**
 * handle a start button click
 * @param src the widget aka button
 * @param event the event
 * @param mode the window mode
 */
void handleStartButtonEvenet(Widget* src, SDL_Event* event, void* mode);
/**
 * handle a two player button click
 * @param src the widget aka button
 * @param event the event
 * @param mode the window mode
 */
void handleTwoPlayersButtonEvenet(Widget* src, SDL_Event* event, void* mode);
/**
 * creates and initilizes a new window
 * @return game window
 */
SPWindow* spModeWindowCreate();
/**
 * creates a widget button from rendere
 * @param renderer renderer
 * @return Widget button
 */
Widget** createModeWidgets(SDL_Renderer* renderer);


#endif /* GAMEMODEWINDOW_H_ */
