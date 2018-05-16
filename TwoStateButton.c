/*
 * TwoStateButton.c
 *
 *  Created on: Sep 9, 2017
 *      Author: nitzn
 */


#include <stdlib.h>
#include "TwoStateButton.h"
#include <SDL.h>
#include <SDL_video.h>



//You need a create function:
Widget* createTwoStateButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* imageOn, const char* imageOff,int initialState) {
	if (windowRender == NULL || location == NULL ||
			imageOn == NULL || imageOff == NULL) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	TwoStateButton* data = (TwoStateButton*) malloc(sizeof(TwoStateButton));
	if (res == NULL || data == NULL){
		printf("Error in malloc\n");
	}
	SDL_Surface* loadingSurface = SDL_LoadBMP(imageOn);
	if (loadingSurface == NULL){
		printf("Error in SDL_LoadBMP\n");
	}
	SDL_Texture* buttonTextureOn = SDL_CreateTextureFromSurface(windowRender,
			loadingSurface);
	if (buttonTextureOn == NULL){
		printf("Error in SDL_CreateTextureFromSurface\n");
	}
	SDL_FreeSurface(loadingSurface);
	loadingSurface = SDL_LoadBMP(imageOff);
	SDL_Texture* buttonTextureOff = SDL_CreateTextureFromSurface(windowRender,
				loadingSurface);
	if (buttonTextureOff == NULL){
		printf("Error in SDL_CreateTextureFromSurface\n");
	}
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| buttonTextureOn == NULL || buttonTextureOff == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(buttonTextureOn);
		SDL_DestroyTexture(buttonTextureOff);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	data->buttonTextureOn = buttonTextureOn;
	data->buttonTextureOff = buttonTextureOff;
	data->location = spCopyRect(location);
	data->windowRenderer = windowRender;
	data->buttonOn = initialState;
	res->destroyWidget = destroyTwoStateButton;
	res->drawWidget = drawTwoStateButton;
	res->handleEvent = NULL;
	res->data = data;
	return res;
}

//You need this function in order to destroy all data Associate with a button:
void destroyTwoStateButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTextureOn);
	SDL_DestroyTexture(castData->buttonTextureOff);
	free(castData);
	free(src);
}


void drawTwoStateButton(Widget* src, void* data) {
	if (src == NULL ) {
		return;
	}
	if (data != NULL){
		return;
	}
	TwoStateButton* castData = (TwoStateButton*) src->data;
	if (castData->buttonOn){
		if (SDL_RenderCopy(castData->windowRenderer, castData->buttonTextureOn, NULL,
				castData->location)<0){
			printf("Error in SDL_RenderCopy\n");
			return;
		}
	}
	else {
		if (SDL_RenderCopy(castData->windowRenderer, castData->buttonTextureOff, NULL,
				castData->location)<0){
			printf(" Error in SDL_RenderCopy\n");
			return;
		}
	}
}
