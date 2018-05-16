/*
 * SPLoadWindow.c
 *
 *  Created on: Aug 9, 2017
 *      Author: nitzan
 */

/*
 * please note use of macro's defined in header file like in the project for
 * load window specification. also number of slots is a constant defined in the header
 * given new button graphics you can easily change the number of slots.
 */

#include "SPLoadWindow.h"
#include "SimpleButton.h"
#include "TwoStateButton.h"
#include <stdlib.h>
#include <stdio.h>

void handleBackButtonEvenetLoad(Widget* src, SDL_Event* event, void* Load) {
	if (src == NULL || event == NULL ) {
		return; //Better to return an error value
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SPLoadWin* LoadWin = (SPLoadWin*) Load;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location) ) {
			activeWin = prevWin;
			if (prevWin == SP_GAME_WINDOW_ACTIVE){
				LoadWin->backToGame = 1;
			}
		}
	} else {
		return;
	}
}

void handleLoadButtonEvenetLoad(Widget* src, SDL_Event* event, void* Load) {
	if (src == NULL || event == NULL ) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPLoadWin* LoadWin = (SPLoadWin*) Load;
	if (LoadWin->slotPressed == 0){
		return;
	}
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			LoadWin->LoadActiveted = 1;
		}
	} else {
		return;
	}
}


void handleSlotButtonEvenet(Widget* src, SDL_Event* event, void* Load) {
	if (src == NULL || event == NULL ) {
		return; //Better to return an error value
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPLoadWin* LoadWin = (SPLoadWin*) Load;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			LoadWin->slotPressed = 1+(castData->location->y-10)/60;
			castData->buttonOn = 0;
			for (int i = 1; i < 2+SLOTS; i++){
				if (i != 1 + LoadWin->slotPressed){
					TwoStateButton* but = (TwoStateButton*)LoadWin->widgets[i]->data;
					but->buttonOn = 1;
				}
			}
		}
	} else {
		return;
	}
}



Widget** createLoadWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*)*(2+SLOTS));
	if (widgets == NULL ) {
		printf("Error in malloc\n");
		activeWin = SP_EXIT;
		return NULL ;
	}
	SDL_Rect backR = { .x = 10, .y = 320 , .h = 60, .w = 100 };
	SDL_Rect loadR = { .x = 290 , .y = 320 , .h = 60, .w = 100 };
	widgets[0] = createSimpleButton(renderer, &backR, "./images/button_back.bmp");
	widgets[1] = createTwoStateButton(renderer, &loadR,
			"./images/button_load.bmp","./images/button_loadB.bmp",0);
	for (int i = 1; i < SLOTS+1; i++){
		creatSlot(i,widgets[i+1]);
	}
	widgets[0]->handleEvent = handleBackButtonEvenetLoad;
	widgets[1]->handleEvent = handleLoadButtonEvenetLoad;
	return widgets;
}

SPWindow* spLoadWindowCreate() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPLoadWin* data = malloc(sizeof(SPLoadWin));
	if (res == NULL || data == NULL){
		printf("Error in malloc\n");
	}
	SDL_Window* window = SDL_CreateWindow("Load Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_OPENGL);
	if (window == NULL){
		printf("Error in SDL_CreateWindow\n");
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	if (renderer == NULL){
		printf("Error in SDL_CreateRenderer");
	}
	Widget** widgets = createLoadWidgets(renderer);
	for (int i = 0; i < 2 + SLOTS; i++){
		if (widgets[i] == NULL){
			destroyLoadWindow(res);
		}
	}
	if (res == NULL || data == NULL || window == NULL
			|| renderer == NULL || widgets == NULL ) {
		free(res);
		free(data);
		free(widgets);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return NULL ;
	}
	data->LoadActiveted = 0;
	data->slotPressed = 0;
	data->backToGame = 0;
	data->numOfWidgets = 2 + SLOTS;
	data->loadWindow = window;
	data->loadRenderer = renderer;
	data->widgets = widgets;
	res->data = (void*) data;
	res->destroyWindow = destroyLoadWindow;
	res->drawWindow = drawLoadWindow;
	res->handleEventWindow = handleEvenetLoadWindow;
	return res;
}


void destroyLoadWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPLoadWin* data = (SPLoadWin*) src->data;
	for (int i = 0; i < SLOTS + 2; i++) {
		destroyWidget(data->widgets[i]);
	}
	free(data->widgets);
	SDL_DestroyRenderer(data->loadRenderer);
	SDL_DestroyWindow(data->loadWindow);
	free(data);
	free(src);
}


void handleEvenetLoadWindow(SPWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL){
		return;
	}
	if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
		activeWin = SP_EXIT;
		return;
	}

	SPLoadWin* data = (SPLoadWin*)src->data;
	for(int i =0;i<data->numOfsavedGames+2;i++){
		data->widgets[i]->handleEvent(data->widgets[i],event,data);
	}
	return;
}

void drawLoadWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPLoadWin* data = (SPLoadWin*) src->data;
	if (SDL_SetRenderDrawColor(data->loadRenderer, 255, 255, 255, 255)<0){
		printf("Error in SDL_SetRenderDrawColor\n");
		activeWin = SP_EXIT;
		return;
	}
	if (SDL_RenderClear(data->loadRenderer)<0){
		printf("Error in SDL_RenderClear\n");
		activeWin = SP_EXIT;
		return;
	}
	for (int i = 0; i < data->numOfsavedGames + 2; i++) {
		(data->widgets[i])->drawWidget(data->widgets[i],NULL);
	}
	SDL_RenderPresent(data->loadRenderer);
}
