#ifndef SPWINDOW_H_
#define SPWINDOW_H_
#include <SDL.h>
#include <SDL_video.h>

typedef struct spwindow_t SPWindow;
struct spwindow_t {
	void* data;
	void (*drawWindow)(SPWindow* );
	void (*handleEventWindow)(SPWindow* , SDL_Event* );
	void (*destroyWindow)(SPWindow* );
	//You can add more common functions Like exit/hide etc...
};

//NULL safe FUNCTION/
/**
 * destorys the given window and its resources
 * @param src the window
 */
void destroyWindow(SPWindow* src);//TODO delete this

#endif
