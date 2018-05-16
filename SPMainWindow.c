/*
 * window.c
 *
 *  Created on: Aug 8, 2017
 *      Author: nitzan
 */
#include "SPMainWindow.h"
#include "SimpleButton.h"
#include <stdlib.h>
#include <stdio.h>

void handleNewGameButtonEvenet(Widget* src, SDL_Event* event, void* main) {
	if (src == NULL || event == NULL || main != NULL) {
		return; //Better to return an error value
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			activeWin = SP_MODE_WINDOW_ACTIVE;
			prevWin = SP_MAIN_WINDOW_ACTIVE;
		}
	} else {
		return;
	}
}

void handleLoadButtonEvenet(Widget* src, SDL_Event* event, void* main) {
	if (src == NULL || event == NULL || main != NULL) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			activeWin = SP_LOAD_WINDOW_ACTIVE;
			prevWin = SP_MAIN_WINDOW_ACTIVE;
		}
	} else {
		return;
	}
}

void handleExitButtonEvenet(Widget* src, SDL_Event* event, void* main) {
	if (src == NULL || event == NULL || main != NULL) {
		return; //Better to return an error value
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			activeWin = SP_EXIT;
		}
	} else {
		return;
	}
}

Widget** createMainWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*) * 3);
	if (widgets == NULL ) {
		printf("Error in malloc\n");
		activeWin = SP_EXIT;
		return NULL ;
	}
	SDL_Rect newR = { .x = 75, .y = 20, .h = 80, .w = 250 };
	SDL_Rect loadR = { .x = 75, .y = 140, .h = 80, .w = 250 };
	SDL_Rect exitR = { .x = 75, .y = 260, .h = 80, .w = 250 };
	widgets[0] = createSimpleButton(renderer, &newR, "./images/button_new-game.bmp");
	widgets[1] = createSimpleButton(renderer, &loadR, "./images/button_load.bmp");
	widgets[2] = createSimpleButton(renderer, &exitR, "./images/button_exit.bmp");
	widgets[0]->handleEvent = handleNewGameButtonEvenet;
	widgets[1]->handleEvent = handleLoadButtonEvenet;
	widgets[2]->handleEvent = handleExitButtonEvenet;
	if (widgets[0] == NULL || widgets[1] == NULL || widgets[2] == NULL ) {
		destroyWidget(widgets[0]);
		destroyWidget(widgets[1]);
		destroyWidget(widgets[2]);
		free(widgets);
		return NULL ;
	}
	return widgets;
}

SPWindow* spMainWindowCreate() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPMainWin* data = malloc(sizeof(SPMainWin));
	if (res == NULL || data == NULL){
		printf("Error in malloc\n");
	}
	SDL_Window* window = SDL_CreateWindow("Main", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_OPENGL);
	if (window == NULL){
		printf("Error in SDL_CreateWindow\n");
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	if (renderer == NULL){
		printf("Error in SDL_CreateRenderer\n");
	}
	Widget** widgets = createMainWidgets(renderer);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL
			|| widgets == NULL ) {
		activeWin = SP_EXIT;
		free(res);
		free(data);
		free(widgets);
		//We first destroy the renderer
		SDL_DestroyRenderer(renderer); //NULL safe
		SDL_DestroyWindow(window); //NULL safe
		return NULL ;
	}
	data->numOfWidgets = 3;
	data->mainWindow = window;
	data->mainRenderer = renderer;
	data->widgets = widgets;
	res->data = (void*) data;
	res->destroyWindow = destroyMainWindow;
	res->drawWindow = drawMainWindow;
	res->handleEventWindow = handleEvenetMainWindow;
	return res;
}


void destroyMainWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPMainWin* data = (SPMainWin*) src->data;
	int i = 0;
	for (; i < 3; i++) {
		destroyWidget(data->widgets[i]);//
	}
	free(data->widgets);
	SDL_DestroyRenderer(data->mainRenderer);
	SDL_DestroyWindow(data->mainWindow);
	free(data);
	free(src);
}


void handleEvenetMainWindow(SPWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL){
		return;
	}
	if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
		activeWin = SP_EXIT;
		return;
	}
	SPMainWin* data = (SPMainWin*)src->data;
	int i =0;
	for(;i<data->numOfWidgets;i++){
		data->widgets[i]->handleEvent(data->widgets[i],event,NULL);
	}
	return;
}

void drawMainWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPMainWin* data = (SPMainWin*) src->data;
	if (SDL_SetRenderDrawColor(data->mainRenderer, 255, 255, 255, 255)<0){
		printf("error in SDL_SetRenderDrawColor: %s\n", SDL_GetError());
		activeWin = SP_EXIT;
	}
	if (SDL_RenderClear(data->mainRenderer)<0){
		printf("error in SDL_RenderClear: %s\n", SDL_GetError());
		activeWin = SP_EXIT;
	}
	int i = 0;
	for (; i < data->numOfWidgets; i++) {
		(data->widgets[i])->drawWidget(data->widgets[i],NULL);
	}
	SDL_RenderPresent(data->mainRenderer);
}
