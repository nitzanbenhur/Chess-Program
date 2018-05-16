/*
 * SPColWindow.c
 *
 *  Created on: Aug 10, 2017
 *      Author: nitzan
 */

#include "SPColorWindow.h"
#include "SimpleButton.h"
#include "TwoStateButton.h"
#include <stdlib.h>
#include <stdio.h>

void handleWhiteButtonEvenet(Widget* src, SDL_Event* event, void* Col) {
	if (src == NULL || event == NULL) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPColWin* ColWin = (SPColWin*) Col;
	if (ColWin->UserCol == 1){
		return;
	}
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			ColWin->UserCol = 1;
			castData->buttonOn = 0;
			TwoStateButton* but1 = (TwoStateButton*)ColWin->widgets[1]->data;
			but1->buttonOn = 1;
			TwoStateButton* but2 = (TwoStateButton*)ColWin->widgets[3]->data;
			but2->buttonOn = 1;
		}
	} else {
		return;
	}
}

void handleBlackButtonEvenet(Widget* src, SDL_Event* event, void* Col) {
	if (src == NULL || event == NULL || Col == NULL) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPColWin* ColWin = (SPColWin*) Col;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			ColWin->UserCol = -1;
			castData->buttonOn = 0;
			TwoStateButton* but1 = (TwoStateButton*)ColWin->widgets[0]->data;
			but1->buttonOn = 1;
			TwoStateButton* but2 = (TwoStateButton*)ColWin->widgets[3]->data;
			but2->buttonOn = 1;
		}
	} else {
		return;
	}
}

void handleBackButtonEvenetCol(Widget* src, SDL_Event* event, void* Col) {
	if (src == NULL || event == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SPColWin* ColWin = (SPColWin*) Col;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			ColWin->UserCol = 0;
			activeWin = SP_DIFFICULTY_WINDOW_ACTIVE;
		}
	} else {
		return;
	}
}


void handleStartButtonEvenetCol(Widget* src, SDL_Event* event, void* Col) {
	if (src == NULL || event == NULL ) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPColWin* ColWin = (SPColWin*) Col;
	if (ColWin->UserCol == 0){
		return;
	}
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			activeWin = SP_GAME_WINDOW_ACTIVE;

		}
	} else {
		return;
	}
}

Widget** createColWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*)*4);
	if (widgets == NULL ) {
		printf("Error: malloc has failed\n");
		activeWin = SP_EXIT;
		return NULL ;
	}
	SDL_Rect startR = { .x = 10, .y = 290, .h = 80, .w = 100 };
	SDL_Rect whiteR = { .x = 10, .y = 10, .h = 80, .w = 100 };
	SDL_Rect blackR = { .x = 290, .y = 10, .h = 80, .w = 100 };
	SDL_Rect backR = { .x = 290, .y = 290, .h = 80, .w = 100 };
	widgets[0] = createTwoStateButton(renderer, &whiteR,
			"./images/King-W.bmp","./images/black.bmp",1);
	widgets[1] = createTwoStateButton(renderer, &blackR,
			"./images/King-B.bmp","./images/black.bmp",1);
	widgets[2] = createSimpleButton(renderer, &backR, "./images/button_back.bmp");
	widgets[3] = createTwoStateButton(renderer, &startR,
			"./images/button_start.bmp","./images/button_start_black.bmp",0);
	widgets[0]->handleEvent = handleWhiteButtonEvenet;
	widgets[1]->handleEvent = handleBlackButtonEvenet;
	widgets[2]->handleEvent = handleBackButtonEvenetCol;
	widgets[3]->handleEvent = handleStartButtonEvenetCol;
	for (int i = 0; i < 4; i++){
		if (widgets[i] == NULL){
			for (int j = 0; j < 4; j++){
				destroyWidget(widgets[j]);
			}
			free(widgets);
			return NULL ;
		}
	}
	return widgets;
}

SPWindow* spColWindowCreate() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPColWin* data = malloc(sizeof(SPColWin));
	if (res == NULL || data == NULL){
		printf("ERROR: malloc has failed\n");
	}
	SDL_Window* window = SDL_CreateWindow("Color Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_OPENGL);
	if (window == NULL){
		printf("ERROR: in SDL_CreateWindow\n");
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	if (renderer == NULL){
		printf("ERROR: in SDL_CreateRenderer\n");
	}
	Widget** widgets = createColWidgets(renderer);
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
	data->UserCol = 0;
	data->widgets = widgets;
	data->numOfWidgets = 4;
	data->colWindow = window;
	data->colRenderer = renderer;
	data->widgets = widgets;
	res->data = (void*) data;
	res->destroyWindow = destroyColWindow;
	res->drawWindow = drawColWindow;
	res->handleEventWindow = handleEvenetColWindow;
	return res;
}


void destroyColWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPColWin* data = (SPColWin*) src->data;
	int i = 0;
	for (; i < 4; i++) {
		destroyWidget(data->widgets[i]);//
	}
	free(data->widgets);
	SDL_DestroyRenderer(data->colRenderer);
	SDL_DestroyWindow(data->colWindow);
	free(data);
	free(src);
}


void handleEvenetColWindow(SPWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL){
		return;
	}
	if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
		activeWin = SP_EXIT;
		return;
	}
	SPColWin* data = (SPColWin*)src->data;
	int i =0;
	for(;i<4;i++){
		data->widgets[i]->handleEvent(data->widgets[i],event,data);
	}
	return;
}

void drawColWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPColWin* data = (SPColWin*) src->data;
	if (SDL_SetRenderDrawColor(data->colRenderer, 255, 255, 255, 255)<0){
		printf("ERROR: in SDL_SetRenderDrawColor\n");
		activeWin = SP_EXIT;
	}
	if (SDL_RenderClear(data->colRenderer)<0){
		printf("ERROR: in SDL_RenderClear\n");
		activeWin = SP_EXIT;
	}
	int i = 0;
	for (; i < data->numOfWidgets; i++) {
		(data->widgets[i])->drawWidget(data->widgets[i],NULL);
	}
	SDL_RenderPresent(data->colRenderer);
}
