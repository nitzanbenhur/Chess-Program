/*
 * SPDifficultyWindow.h
 *
 *  Created on: Aug 10, 2017
 *      Author: nitzan
 */
#ifndef SPDIFFICULTYWINDOW_H_
#define SPDIFFICULTYWINDOW_H_
#include <SDL.h>
#include <SDL_video.h>
#include "active_window.h"
#include "SPWindow.h"
#include "Widget.h"

typedef enum {
	SP_DIFFICULTY_BACK, SP_DIFFICULTY_NEXT, SP_DIFFICULTY_EXIT, SP_DIFFICULTY_NONE,
	SP_DIFFICULTY_NOOB, SP_DIFFICULTY_EASY, SP_DIFFICULTY_MODERATE, SP_DIFFICULTY_HARD, SP_DIFFICULTY_EXPERT, SP_DIFF_INVALID_ARGUMENT
} SP_DIFF_EVENT;

typedef enum {
	SP_DIFF_NOOB = 1
	,SP_DIFF_EASY = 2
	,SP_DIFF_MODERATE = 3
	,SP_DIFF_HARD = 4
	,SP_DIFF_EXPERT = 5
	,SP_DIFF_NONE = 6
} SP_DIFF_BUTTON_PRESSED;


typedef struct {
	SDL_Window* diffWindow;
	Widget** widgets;
	int numOfWidgets;
	ACTIVE_WINDOW prev;
	SDL_Renderer* diffRenderer;
	int diff;
} SPDiffWin;
/**
 * handle noob button events
 * @param src the widget
 * @param event the event
 * @param diff diff window
 */
void handleNoobButtonEvenet(Widget* src, SDL_Event* event, void* diff);
/**
 * handle easy button events
 * @param src the widget
 * @param event the event
 * @param diff diff window
 */
void handleEasyButtonEvenet(Widget* src, SDL_Event* event, void* diff);
/**
 * handle moderate button events
 * @param src the widget
 * @param event the event
 * @param diff diff window
 */
void handleModerateButtonEvenet(Widget* src, SDL_Event* event, void* diff);
/**
 * handle hard button events
 * @param src the widget
 * @param event the event
 * @param diff diff window
 */
void handleHardButtonEvenet(Widget* src, SDL_Event* event, void* diff);
/**
 * handle expert button events
 * @param src the widget
 * @param event the event
 * @param diff diff window
 */
void handleExpertButtonEvenet(Widget* src, SDL_Event* event, void* diff);
/**
 * handle back button events
 * @param src the widget
 * @param event the event
 * @param diff diff window
 */
void handleBackButtonEvenetDiff(Widget* src, SDL_Event* event, void* diff);
/**
 * handle next button events
 * @param src the widget
 * @param event the event
 * @param diff diff window
 */
void handleNextButtonEvenetDiff(Widget* src, SDL_Event* event, void* diff);
/**
 * creates a game window
 * @return the game window
 */
SPWindow* spDiffWindowCreate();
/**
 * draws the game window
 * @param gameWin game
 */
void drawDiffWindow(SPWindow* src);
/**
 * frees the game window
 * @param src game window
 */
void destroyDiffWindow(SPWindow* src);
/**
 * handles a game event(click, drag, etc)
 * @param src the game window
 * @param event event
 */
void handleEvenetDiffWindow(SPWindow* src, SDL_Event* event);

#endif /* SPDIFFICULTYWINDOW_H_ */
