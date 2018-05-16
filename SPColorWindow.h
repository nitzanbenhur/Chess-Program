/*
 * SPColorWindow.h
 *
 *  Created on: Aug 10, 2017
 *      Author: nitzan
 */
#ifndef SPCOLORWINDOW_H_
#define SPCOLORWINDOW_H_
#include <SDL.h>
#include <SDL_video.h>
#include "active_window.h"
#include "SPWindow.h"
#include "Widget.h"



typedef struct {
	// zero is black one is white.
	int UserCol;
	SDL_Window* colWindow;
	Widget** widgets;
	ACTIVE_WINDOW prev;
	SDL_Renderer* colRenderer;
	int numOfWidgets;
	ACTIVE_WINDOW prevWin;
} SPColWin;

/**
 * handles start button event
 * @param src button
 * @param event event
 * @param Col colored window
 */
void handleStartButtonEvenetCol(Widget* src, SDL_Event* event, void* Col);
/**
 * handles back button event
 * @param src button
 * @param event event
 * @param Col colored window
 */
void handleBackButtonEvenetCol(Widget* src, SDL_Event* event, void* Col);
/**
 * handles black button event
 * @param src button
 * @param event event
 * @param Col colored window
 */
void handleBlackButtonEvenet(Widget* src, SDL_Event* event, void* Col);
/**
 * handles white button event
 * @param src button
 * @param event event
 * @param Col colored window
 */
void handleWhiteButtonEvenet(Widget* src, SDL_Event* event, void* Col);
/**
 * create colored button
 * @param renderer renderer
 * @return
 */
Widget** createColWidgets(SDL_Renderer* renderer);
/**
 * inits and creates a new color window
 * @return
 */
SPWindow* spColWindowCreate();
/**
 * destroys window and frees all resources
 * @param src
 */
void destroyColWindow(SPWindow* src);
/**
 * handles click or other events
 * @param src window
 * @param event event
 */
void handleEvenetColWindow(SPWindow* src, SDL_Event* event);
/**
 * renders the window
 * @param src window
 */
void drawColWindow(SPWindow* src);

#endif /* SPCOLORWINDOW_H_ */
