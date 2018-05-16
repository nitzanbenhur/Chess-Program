/*
 * SPLoadWindow.h
 *
 *  Created on: Aug 9, 2017
 *      Author: nitzan
 */

#ifndef SPLOADWINDOW_H_
#define SPLOADWINDOW_H_


#include <SDL.h>
#include <SDL_video.h>
#include "active_window.h"
#include "SPWindow.h"
#include "Widget.h"
#define SLOTS 5


#define creatSlot(z,WidgetPtr)\
	SDL_Rect slotR = { .x = 150 , .y = 10 + 60*(z-1), .h = 50, .w = 100 };	\
	char s1[40];								\
	char s2[40];								\
	sprintf(s1,"./images/button_game-slot%d.bmp",z);			\
	sprintf(s2,"./images/button_game-slot%dB.bmp",z);			\
	WidgetPtr =	createTwoStateButton(renderer,&slotR,s1,s2,1);		\
	WidgetPtr->handleEvent = handleSlotButtonEvenet;			\




typedef struct {
	//SDL_Window* window;
	SDL_Window* loadWindow;
	Widget** widgets;
	ACTIVE_WINDOW prev;
	SDL_Renderer* loadRenderer;
	int slotPressed;
	int numOfWidgets;
	int numOfsavedGames;
	int LoadActiveted;
	int backToGame;
} SPLoadWin;
/**
 * handle slot1 button events
 * @param src the widget
 * @param event the event
 * @param Load load window
 */
void handleSlot1ButtonEvenet(Widget* src, SDL_Event* event, void* Load);
/**
 * handle slot2 button events
 * @param src the widget
 * @param event the event
 * @param Load load window
 */
void handleSlot2ButtonEvenet(Widget* src, SDL_Event* event, void* Load);
/**
 * handle slot3 button events
 * @param src the widget
 * @param event the event
 * @param Load load window
 */
void handleSlot3ButtonEvenet(Widget* src, SDL_Event* event, void* Load);
/**
 * handle slot4 button events
 * @param src the widget
 * @param event the event
 * @param Load load window
 */
void handleSlot4ButtonEvenet(Widget* src, SDL_Event* event, void* Load);
/**
 * handle slot5 button events
 * @param src the widget
 * @param event the event
 * @param Load load window
 */
void handleSlot5ButtonEvenet(Widget* src, SDL_Event* event, void* Load);
/**
 * handle back button events
 * @param src the widget
 * @param event the event
 * @param Load load window
 */
void handleBackButtonEvenetLoad(Widget* src, SDL_Event* event, void* Load);
/**
 * handle load button events
 * @param src the widget
 * @param event the event
 * @param Load load window
 */
void handleLoadButtonEvenetLoad(Widget* src, SDL_Event* event, void* Load);
/**
 * handles a game event(click, drag, etc)
 * @param src the game window
 * @param event event
 */
void handleEvenetLoadWindow(SPWindow* src, SDL_Event* event);
/**
 * draws the game window
 * @param gameWin game
 */
void drawLoadWindow(SPWindow* src);
/**
 * frees the game window
 * @param src game window
 */
void destroyLoadWindow(SPWindow* src);
/**
 * creates a game window
 * @return the game window
 */
SPWindow* spLoadWindowCreate();
#endif /* SPLOADWINDOW_H_ */
