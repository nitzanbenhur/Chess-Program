#include <stdlib.h>
#include "Widget.h"

void destroyWidget(Widget* src) {//Making NULL Safe Destroy
	if (src == NULL ) {
		return;
	}
	src->destroyWidget(src);
}

SDL_Rect* spCopyRect(SDL_Rect* src){
	if(src == NULL){
		return NULL;
	}
	SDL_Rect* res = malloc(sizeof(SDL_Rect));
	if(res==NULL){
		printf("Error in malloc\n");
		return NULL;
	}
	res->h = src->h;
	res->w = src->w;
	res->x = src->x;
	res->y = src->y;
	return res;
}
