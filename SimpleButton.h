#ifndef SIMPLEBUTTON_H_
#define SIMPLEBUTTON_H_

#include "Widget.h" //A button is a widget also

typedef struct simplebutton_t SimpleButton;
struct simplebutton_t{
	SDL_Texture* buttonTexture;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	void* data;
};




/**
 * creates a button
 * @param windowRender which window
 * @param location which rectangle
 * @param image string for image location
 * @return
 */
Widget* createSimpleButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* image);
/**
 *You need this function in order to destroy all data Associate with a button
 * @param src button to destory
 *
*/
void destroySimpleButton(Widget* src);
/**
 * draw a button
 * @param src which widget to draw on
 * @param data button
 */
void drawSimpleButton(Widget* src, void* data);

#endif
