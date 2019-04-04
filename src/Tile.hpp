#pragma once

#include "Sprite.hpp"

using namespace std;

Class Tile:public Sprite{
	Const int tilesize = 64;
	int TileType;
	SDL_Rect mBox;
	
	public:
	Tile(Game *g, string filename, int type, int count=4, int fps=1, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, float spriteX=0.0, float spriteY=0.0):
	Sprite(g, filename, count, fps, _x, _y, _dx, _dy, spriteX, spriteY){
		TileType = type;
		mBox.x = _x;
		mBox.y = _y;
		mBox.w = tilesize;
		mBox.h = tilesize;
	}
	int getTileType(){return TileType;}
	SDL_Rect getMBox(){return mBox;}
};
