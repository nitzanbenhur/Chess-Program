#include <stdlib.h>
#include "SimpleButton.h"
#include <SDL.h>
#include <SDL_video.h>



//You need a create function:
Widget* createSimpleButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image) {
	if (windowRender == NULL || location == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	SimpleButton* data = (SimpleButton*) malloc(sizeof(SimpleButton));
	if (res == NULL || data == NULL){
		printf("Error in malloc\n");
	}
	SDL_Surface* loadingSurface = SDL_LoadBMP(image);
	if (loadingSurface == NULL){
		printf("Error in SDL_LoadBMP\n");
	}
	SDL_Texture* buttonTexture = SDL_CreateTextureFromSurface(windowRender,
			loadingSurface);
	if (buttonTexture == NULL){
		printf("Error in  SDL_CreateTextureFromSurface\n");
	}
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| buttonTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(buttonTexture);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	data->buttonTexture = buttonTexture;
	data->location = spCopyRect(location);
	data->windowRenderer = windowRender;
	res->destroyWidget = destroySimpleButton;
	res->drawWidget = drawSimpleButton;
	res->handleEvent = NULL;
	res->data = data;
	return res;
}

//You need this function in order to destroy all data Associate with a button:
void destroySimpleButton(Widget* src) {
	if (src == NULL ) {
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	free(castData->location);
	SDL_DestroyTexture(castData->buttonTexture);
	free(castData);
	free(src);
}


void drawSimpleButton(Widget* src, void* data) {
	if (src == NULL ) {
		return;
	}
	if (data != NULL){
		return;
	}
	SimpleButton* castData = (SimpleButton*) src->data;
	if (SDL_RenderCopy(castData->windowRenderer, castData->buttonTexture, NULL,
			castData->location)<0){
		printf("Error in SDL_RenderCopy\n");
	}
}
