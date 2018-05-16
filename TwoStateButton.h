/*
 * TwoStateBottun.h
 *
 *  Created on: Sep 9, 2017
 *      Author: nitzn
 */

#ifndef TEMP_TWOSTATEBUTTON_H_
#define TEMP_TWOSTATEBUTTON_H_

#include "Widget.h" //A button is a widget also

typedef struct TwoStateButton_t TwoStateButton;
struct TwoStateButton_t{
	SDL_Texture* buttonTextureOn;
	SDL_Texture* buttonTextureOff;
	SDL_Renderer* windowRenderer;
	SDL_Rect* location;
	int buttonOn;
	void* data;
};

/**
 * creates a button
 * @param windowRender renderer
 * @param location location of button
 * @param imageOn on image
 * @param imageOff off image
 * @param initialState weather turned on in beginning
 * @return the button
 */
Widget* createTwoStateButton(SDL_Renderer* windowRender, SDL_Rect* location,
		const char* imageOn, const char* imageOff,int initialState);
/**
 * destroy button
 * @param src button to destroy
 */
//You need this function in order to destroy all data Associate with a button:
void destroyTwoStateButton(Widget* src);

/**
 * draw the button
 * @param src button
 * @param data data
 */
void drawTwoStateButton(Widget* src, void* data);


#endif /* TEMP_TWOSTATEBUTTON_H_ */
