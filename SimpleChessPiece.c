/*
 * SimpleChessPiece.c
 *
 *  Created on: Aug 24, 2017
 *      Author: nitzan
 */

#include <stdlib.h>
#include "SimpleChessPiece.h"
#include <SDL.h>
#include <SDL_video.h>

Widget* createSimpleChessPiece(SDL_Renderer* windowRender, const char* image, char type) {
	if (windowRender == NULL || image == NULL ) {
		return NULL ;
	}
	//Allocate data
	Widget* res = (Widget*) malloc(sizeof(Widget));
	SimpleChessPiece* data = (SimpleChessPiece*) malloc(sizeof(SimpleChessPiece));
	if (res == NULL || data == NULL){
		printf("Error in malloc\n");
	}
	SDL_Surface* loadingSurface = SDL_LoadBMP(image);
	if (loadingSurface == NULL){
		printf("Error in SDL_LoadBMP\n");
	}
    if (SDL_SetColorKey(loadingSurface, SDL_TRUE,
                    SDL_MapRGB(loadingSurface->format, 255, 0, 255))<0){
    	printf("Error in SDL_SetColorKey\n");
    }
    SDL_Texture* ChessPieceTexture = SDL_CreateTextureFromSurface(windowRender,loadingSurface);
	if (ChessPieceTexture == NULL){
		printf("Error in SDL_CreateTextureFromSurface\n");
	}
	if (res == NULL || data == NULL || loadingSurface == NULL
			|| ChessPieceTexture == NULL) {
		free(res);
		free(data);
		SDL_FreeSurface(loadingSurface);
		SDL_DestroyTexture(ChessPieceTexture);
		return NULL ;
	}
	SDL_FreeSurface(loadingSurface);
	data->ChessPieceTexture = ChessPieceTexture;
	data->windowRenderer = windowRender;
	data->type = type;
	res->destroyWidget = destroySimpleChessPiece;
	res->drawWidget = drawSimpleChessPiece;
	res->handleEvent = NULL;
	res->data = data;
	return res;
}

void destroySimpleChessPiece(Widget* src) {
	if (src == NULL ) {
		return;
	}
	SimpleChessPiece* castData = (SimpleChessPiece*) src->data;
	SDL_DestroyTexture(castData->ChessPieceTexture);
	free(castData);
	free(src);
}


void drawSimpleChessPiece(Widget* src, void* PiecePos) {
	if (src == NULL) {
		return;
	}
	Piece_Pos* inputData = (Piece_Pos*) PiecePos;
	SimpleChessPiece* castData = (SimpleChessPiece*) src->data;
	if (inputData->piece != castData->type){
		return;
	}
	if (SDL_RenderCopy(castData->windowRenderer, castData->ChessPieceTexture,
			NULL, &inputData->pos)<0){
		printf("Error in SDL_RenderCopy\n");
	}
}
