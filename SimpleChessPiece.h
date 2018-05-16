/*
 * SimpleChessPiece.h
 *
 *  Created on: Aug 24, 2017
 *      Author: nitzan
 */

#ifndef SIMPLECHESSPIECE_H_
#define SIMPLECHESSPIECE_H_

#include "Widget.h" //A ChessPiece is a widget also

typedef struct {
	char piece;
	SDL_Rect pos;
}Piece_Pos;

typedef struct simpleChessPiece_t SimpleChessPiece;
struct simpleChessPiece_t{
	SDL_Texture* ChessPieceTexture;
	SDL_Renderer* windowRenderer;
	char type;
	void* data;
};

/**
 * create a widget for a game piece
 * @param windowRender game renderer
 * @param image image file path
 * @param type type of piece, e.g queen or king or any other one
 * @return Widget of the game piece
 */
Widget* createSimpleChessPiece(SDL_Renderer* windowRender, const char* image, char type);
/**
 * destroys the game piece and frees resources
 * @param src the game piece
 */
void destroySimpleChessPiece(Widget* src);
/**
 * draws the game piece
 * @param src game piece
 * @param PiecePos position
 */
void drawSimpleChessPiece(Widget* src,void* PiecePos);
#endif /* SIMPLECHESSPIECE_H_ */
