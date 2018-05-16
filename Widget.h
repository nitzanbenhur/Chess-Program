#ifndef WIDGET_H_
#define WIDGET_H_
#include <SDL.h>
#include <SDL_video.h>
typedef struct widget_t Widget;
struct widget_t {
	void (*drawWidget)(Widget*,void*);
	void (*handleEvent)(Widget*, SDL_Event*,void*);
	void (*destroyWidget)(Widget*);
	void* data;
	int active;
};
/**
 * returns a copy of the rectangle
 * @param src rect. to copy
 * @return new copy of rect.
 */
SDL_Rect* spCopyRect(SDL_Rect* src);
/**
 * This function would be usefull for NULL safe desetroy
 * @param src widget to destory
 */
void destroyWidget(Widget* src);
#endif
