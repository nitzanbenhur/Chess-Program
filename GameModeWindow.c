/*
 * GameModeWindow.c
 *
 *  Created on: Aug 9, 2017
 *      Author: nitzan
 */

#include "GameModeWindow.h"
#include "SimpleButton.h"
#include "TwoStateButton.h"
#include <stdlib.h>
#include <stdio.h>
#define numofwidgets 5
void handleOnePlayerButtonEvenet(Widget* src, SDL_Event* event, void* mode) {
	if (src == NULL || event == NULL) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	SPModeWin* modeWin = (SPModeWin*) mode;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			modeWin->onePlayer = 1;
			castData->buttonOn = 0;
			TwoStateButton* but = (TwoStateButton*)modeWin->widgets[1]->data;
			but->buttonOn = 1;
		}
	} else {
		return;
	}
}

void handleTwoPlayersButtonEvenet(Widget* src, SDL_Event* event, void* mode) {
	if (src == NULL || event == NULL || mode == NULL) {
		return;
	}
    TwoStateButton* castData = (TwoStateButton*) src->data;
	SPModeWin* modeWin = (SPModeWin*) mode;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			modeWin->onePlayer = 2;
            castData->buttonOn = 0;
            TwoStateButton* but = (TwoStateButton*)modeWin->widgets[0]->data;
            but->buttonOn = 1;

        }
	} else {
		return;
	}
}

void handleBackButtonEvenet(Widget* src, SDL_Event* event, void* mode) {
	if (src == NULL || event == NULL || mode == NULL) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			prevWin = SP_MODE_WINDOW_ACTIVE;
			activeWin = SP_MAIN_WINDOW_ACTIVE;
		}
	} else {
		return;
	}
}

void handleNextButtonEvenet(Widget* src, SDL_Event* event, void* mode) {
	if (src == NULL || event == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SPModeWin* modeWin = (SPModeWin*) mode;
	if (modeWin->onePlayer == 2){
		return;
	}
	if (event->type == SDL_MOUSEBUTTONUP) {
		SDL_Point point;
		point.x = event->button.x;
		point.y = event->button.y;
		if (SDL_PointInRect(&point, castData->location)) {
			activeWin = SP_DIFFICULTY_WINDOW_ACTIVE;
		}
	} else {
		return;
	}
}

void handleStartButtonEvenet(Widget* src, SDL_Event* event, void* mode) {
	if (src == NULL || event == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	SPModeWin* modeWin = (SPModeWin*) mode;
	if (modeWin->onePlayer == 1){
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

Widget** createModeWidgets(SDL_Renderer* renderer) {
	if (renderer == NULL ) {
		return NULL ;
	}
	Widget** widgets = malloc(sizeof(Widget*)*numofwidgets);
	if (widgets == NULL ) {
        printf("Error in malloc\n");
        activeWin = SP_EXIT;
        return NULL ;
	}
	SDL_Rect startR = { .x = 10, .y = 290, .h = 80, .w = 100 };
	SDL_Rect nextR = { .x = 10, .y = 290, .h = 80, .w = 100 };
	SDL_Rect onePlayerR = { .x = 10, .y = 10, .h = 80, .w = 100 };
	SDL_Rect twoPlayerR = { .x = 290, .y = 10, .h = 80, .w = 100 };
	SDL_Rect backR = { .x = 290, .y = 290, .h = 80, .w = 100 };
    widgets[0] = createTwoStateButton(renderer, &onePlayerR, "./images/button_one-player.bmp"
                      ,"./images/button_one-player_black.bmp",0);
    widgets[1] = createTwoStateButton(renderer, &twoPlayerR, "./images/button_two-players.bmp"
                                                 ,"./images/button_two_player's_black.bmp",1);
    widgets[2] = createSimpleButton(renderer, &backR, "./images/button_back.bmp");
	widgets[3] = createSimpleButton(renderer, &nextR, "./images/button_next.bmp");
	widgets[4] = createSimpleButton(renderer, &startR, "./images/button_start.bmp");
	widgets[0]->handleEvent = handleOnePlayerButtonEvenet;
	widgets[1]->handleEvent = handleTwoPlayersButtonEvenet;
	widgets[2]->handleEvent = handleBackButtonEvenet;
	widgets[3]->handleEvent = handleNextButtonEvenet;
	widgets[4]->handleEvent = handleStartButtonEvenet;
	for (int i = 0; i < numofwidgets; i++){
		if (widgets[0] == NULL){
			for (int j = 0; j < numofwidgets; j++){
				destroyWidget(widgets[j]);
			}
			free(widgets);
			return NULL ;
		}
	}
	return widgets;
}

SPWindow* spModeWindowCreate() {
	SPWindow* res = malloc(sizeof(SPWindow));
	SPModeWin* data = malloc(sizeof(SPModeWin));
    if (res == NULL || data == NULL){
        printf("error in malloc\n");
    }
    SDL_Window* window = SDL_CreateWindow("Mode", SDL_WINDOWPOS_CENTERED,
			SDL_WINDOWPOS_CENTERED, 400, 400, SDL_WINDOW_OPENGL);
    if(window==NULL){
        printf("error in SDL_CreateWindow\n");
    }
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED);
    if(renderer==NULL){
        printf("error in SDL_CreateRenderer\n");
    }
	Widget** widgets = createModeWidgets(renderer);
	if (res == NULL || data == NULL || window == NULL || renderer == NULL
			|| widgets == NULL ) {
		free(res);
		free(data);
		free(widgets);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		return NULL ;
	}
	data->onePlayer = 1;
	data->widgets = widgets;
	data->numOfWidgets = numofwidgets;
	data->modeWindow = window;
	data->modeRenderer = renderer;
	data->widgets = widgets;
	res->data = (void*) data;
	res->destroyWindow = destroyModeWindow;
	res->drawWindow = drawModeWindow;
	res->handleEventWindow = handleEvenetModeWindow;
	return res;
}


void destroyModeWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
	SPModeWin* data = (SPModeWin*) src->data;
	int i = 0;
	for (; i < numofwidgets; i++) {
		destroyWidget(data->widgets[i]);//
	}
	free(data->widgets);
	SDL_DestroyRenderer(data->modeRenderer);
	SDL_DestroyWindow(data->modeWindow);
	free(data);
	free(src);
}


void handleEvenetModeWindow(SPWindow* src, SDL_Event* event){
	if(src == NULL || event==NULL){
		return;
	}
	if (event->window.event == SDL_WINDOWEVENT_CLOSE) {
		activeWin = SP_EXIT;
		return;
	}
	SPModeWin* data = (SPModeWin*)src->data;
	int i =0;
	for(;i<5;i++){
		data->widgets[i]->handleEvent(data->widgets[i],event,data);
	}
	return;
}

void drawModeWindow(SPWindow* src) {
	if (src == NULL ) {
		return;
	}
    SPModeWin* data = (SPModeWin*) src->data;
    if (SDL_SetRenderDrawColor(data->modeRenderer, 255, 255, 255, 255)<0){
        printf("error in SDL_SetRenderDrawColor\n");
        activeWin = SP_EXIT;
        return;
    }
    if (SDL_RenderClear(data->modeRenderer)<0){
        printf("error in SDL_RenderClear\n");
        activeWin = SP_EXIT;
        return;
    }
    int i = 0;
	for (; i < data->numOfWidgets; i++) {
		if ((i == 4 && data->onePlayer == 1) || (i == 3 && data->onePlayer == 2)){
			continue;
		}
		(data->widgets[i])->drawWidget(data->widgets[i],NULL);
	}
	SDL_RenderPresent(data->modeRenderer);
}

