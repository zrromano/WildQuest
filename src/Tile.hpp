#pragma once

#include "Game.hpp"

using namespace std;

class Tile{
	const int tilesize = 64;
	int TileType;
	SDL_Rect mBox;
	
	public:
	Tile(int _x = 0, int _y = 0, int type = 0){
		mBox.x = _x;
		mBox.y = _y;
		mBox.w = tilesize;
		mBox.h = tilesize;
		TileType = type;
	};
	int getTileType(){return TileType;}
	SDL_Rect getMBox(){return mBox;}
	int getTileYPos(){return mBox.y;}
	int getTileXPos(){return mBox.x;}
	void setTileX(int _x){mBox.x = _x;}
	void setTileY(int _y){mBox.y = _y;}
	void setTileType(int type){TileType = type;}
	void setTileProperties(int type, int _x, int _y){
		mBox.x = _x;
		mBox.y = _y;
		TileType = type;
	}
	
	void tileDebug(){
		cout << "Tile Debug Called! Tile Properties: x: " << mBox.x << ", y: " << mBox.y << endl;
	}
};
