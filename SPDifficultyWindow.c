/*
 * SPDifficultyWindow.c
 *
 *  Created on: Aug 10, 2017
 *      Author: nitzan
 */
#include <stdlib.h>
#include <stdio.h>
#include "SPDifficultyWindow.h"
#include "SimpleButton.h"
#include "TwoStateButton.h"

void handleNoobButtonEvenet(Widget* src, SDL_Event* event, void* diff) {
	if (src == NULL || event == NULL) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPDiffWin* diffWin = (SPDiffWin*) diff;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			diffWin->diff = 1;
			castData->buttonOn = 0;
			for (int i = 1; i < 6; i++){
				TwoStateButton* but = (TwoStateButton*)diffWin->widgets[i]->data;
				but->buttonOn = 1;
			}
		}
	} else {
		return;
	}
}

void handleEasyButtonEvenet(Widget* src, SDL_Event* event, void* diff) {
	if (src == NULL || event == NULL || diff == NULL) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPDiffWin* diffWin = (SPDiffWin*) diff;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			diffWin->diff = 2;
			castData->buttonOn = 0;
			for (int i = 0; i < 6; i++){
				if (i != 1){
					TwoStateButton* but = (TwoStateButton*)diffWin->widgets[i]->data;
					but->buttonOn = 1;
				}
			}
		}
	} else {
		return;
	}
}

void handleModerateButtonEvenet(Widget* src, SDL_Event* event, void* diff) {
	if (src == NULL || event == NULL || diff == NULL) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPDiffWin* diffWin = (SPDiffWin*) diff;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			diffWin->diff = 3;
			castData->buttonOn = 0;
			for (int i = 0; i < 6; i++){
				if (i != 2){
					TwoStateButton* but = (TwoStateButton*)diffWin->widgets[i]->data;
					but->buttonOn = 1;
				}
			}
		}
	} else {
		return;
	}
}

void handleHardButtonEvenet(Widget* src, SDL_Event* event, void* diff) {
	if (src == NULL || event == NULL || diff == NULL) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPDiffWin* diffWin = (SPDiffWin*) diff;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			diffWin->diff = 4;
			castData->buttonOn = 0;
			for (int i = 0; i < 6; i++){
				if (i != 3){
					TwoStateButton* but = (TwoStateButton*)diffWin->widgets[i]->data;
					but->buttonOn = 1;
				}
			}
		}
	} else {
		return;
	}
}

void handleExpertButtonEvenet(Widget* src, SDL_Event* event, void* diff) {
	if (src == NULL || event == NULL || diff == NULL) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPDiffWin* diffWin = (SPDiffWin*) diff;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			diffWin->diff = 5;
			castData->buttonOn = 0;
			for (int i = 0; i < 6; i++){
				if (i != 4){
					TwoStateButton* but = (TwoStateButton*)diffWin->widgets[i]->data;
					but->buttonOn = 1;
				}
			}
		}
	} else {
		return;
	}
}

void handleBackButtonEvenetDiff(Widget* src, SDL_Event* event, void* diff) {
	if (src == NULL || event == NULL || diff == NULL) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SPDiffWin* diffWin = (SPDiffWin*) diff;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			diffWin->diff = 0;
			activeWin = SP_MODE_WINDOW_ACTIVE;
		}
	} else {
		return;
	}
}

void handleNextButtonEvenetDiff(Widget* src, SDL_Event* event, void* diff) {
	if (src == NULL || event == NULL || diff == NULL) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPDiffWin* diffWin = (SPDiffWin*) diff;
	if (diffWin->diff == 0){
		return;
	}
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			activeWin = SP_COLOR_WINDOW_ACTIVE;
		}
	} else {
		return;
	}
}

Widget** createDiffWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*)*7);
	if (widgets == NULL ) {
		printf("Error in malloc\n");
		activeWin = SP_EXIT;
		return NULL ;
	}
	SDL_Rect noobR = { .x = 150, .y = 10, .h = 50, .w = 100 };
	SDL_Rect easyR = { .x = 150, .y = 70, .h = 50, .w = 100 };
	SDL_Rect moderateR = { .x = 150, .y = 130, .h = 50, .w = 100 };
	SDL_Rect hardR = { .x = 150, .y = 190, .h = 50, .w = 100 };
	SDL_Rect expertR = { .x = 150, .y = 250, .h = 50, .w = 100 };
	SDL_Rect nextR = { .x = 10, .y = 320, .h = 50, .w = 100 };
	SDL_Rect backR = { .x = 290, .y = 320, .h = 50, .w = 100 };
	widgets[0] =  createTwoStateButton(renderer, &noobR,
			"./images/button_noob.bmp","./images/button_noob_black.bmp",1);
	widgets[1] = createTwoStateButton(renderer, &easyR,
			"./images/button_easy.bmp","./images/button_easy_black.bmp",1);
	widgets[2] = createTwoStateButton(renderer, &moderateR,
			"./images/button_moderate.bmp","./images/button_moderate_black.bmp",1);
	widgets[3] = createTwoStateButton(renderer, &hardR,
			"./images/button_hard.bmp","./images/button_hard_black.bmp",1);
	widgets[4] = createTwoStateButton(renderer, &expertR,
			"./images/button_expert.bmp","./images/button_expert_black.bmp",1);
	widgets[5] = createTwoStateButton(renderer, &nextR,
			"./images/button_next.bmp","./images/button_next_black.bmp",0);
	widgets[6] = createSimpleButton(renderer, &backR,"./images/button_back.bmp");
	widgets[0]->handleEvent = handleNoobButtonEvenet;
	widgets[1]->handleEvent = handleEasyButtonEvenet;
	widgets[2]->handleEvent = handleModerateButtonEvenet;
	widgets[3]->handleEvent = handleHardButtonEvenet;
	widgets[4]->handleEvent = handleExpertButtonEvenet;
	widgets[5]->handleEvent = handleNextButtonEvenetDiff;
	widgets[6]->handleEvent = handleBackButtonEvenetDiff;
	for (int i = 0; i < 7; i++){
		if (widgets[0] == NULL){
			for (int j = 0; j < 7; j++){
				destroyWidget(widgets[j]);
			}
			free(widgets);
			return NULL ;
		}
	}
	return widgets;
}

SPWindow* spDiffWindowCreate() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPDiffWin* data = malloc(sizeof(SPDiffWin));
	if (res == NULL || data == NULL){
		printf("Error in malloc\n");
		activeWin = SP_EXIT;
	}
	SDL_Window* window = SDL_CreateWindow("Difficulty Window", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_OPENGL);
	if (window == NULL){
		printf("Error in SDL_CreateWindow\n");
	}
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
	if (renderer == NULL){
		printf("Error in SDL_CreateRenderer\n");
	}
	Widget** widgets = createDiffWidgets(renderer);
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
	data->diff = 0;
	data->widgets = widgets;
	data->numOfWidgets = 7;
	data->diffWindow = window;
	data->diffRenderer = renderer;
	data->widgets = widgets;
	res->data = (void*) data;
	res->destroyWindow = destroyDiffWindow;
	res->drawWindow = drawDiffWindow;
	res->handleEventWindow = handleEvenetDiffWindow;
	return res;
}


void destroyDiffWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPDiffWin* data = (SPDiffWin*) src->data;
	int i = 0;
	for (; i < 7; i++) {
		destroyWidget(data->widgets[i]);//
	}
	free(data->widgets);
	SDL_DestroyRenderer(data->diffRenderer);
	SDL_DestroyWindow(data->diffWindow);
	free(data);
	free(src);
}


void handleEvenetDiffWindow(SPWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL){
		return;
	}
	if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
		activeWin = SP_EXIT;
		return;
	}
	SPDiffWin* data = (SPDiffWin*)src->data;
	int i =0;
	for(;i<7;i++){
		data->widgets[i]->handleEvent(data->widgets[i],event,data);
	}
	return;
}

void drawDiffWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPDiffWin* data = (SPDiffWin*) src->data;
	if (SDL_SetRenderDrawColor(data->diffRenderer, 255, 255, 255, 255)<0){
		printf("Error in SDL_SetRenderDrawColor\n");
		activeWin = SP_EXIT;
	}
	if (SDL_RenderClear(data->diffRenderer)<0){
		printf("Error in SDL_RenderClear\n");
		activeWin = SP_EXIT;
	}
	for (int i = 0; i < data->numOfWidgets; i++) {
		(data->widgets[i])->drawWidget(data->widgets[i],NULL);

	}
	SDL_RenderPresent(data->diffRenderer);
}
