#pragma once

#include <string>
#include <fstream>

#include "Image.hpp"
#include "Tile.hpp"

using namespace std;

typedef pair<Image *,Tile *> Tile_Image_Pair;

class TillingEngine{
	
	//Tile Contstants 
	const int TILE_WIDTH = 64;
	const int TILE_HEIGHT = 64;
	const int TOTAL_TILES = 2500;
	
	const static int TOTAL_TILE_SPRITES = 3;
	
	const int DEV_TILE_BLACK = 0;
	const int DEV_TILE_RED = 1;
	const int DEV_TILE_BLUE = 2;
	
	SDL_Rect Level_Dimentions;
	SDL_Rect Tile_Clip[ TOTAL_TILE_SPRITES ];
	
	vector < Tile_Image_Pair > tiles;
	
	public:
	
	TillingEngine(Game *g, string spritesheet, string levelfile){
		
		Tile_Clip[DEV_TILE_BLACK].x = 0;
		Tile_Clip[DEV_TILE_BLACK].y = 0;
		Tile_Clip[DEV_TILE_BLACK].w = TILE_WIDTH;
		Tile_Clip[DEV_TILE_BLACK].h = TILE_HEIGHT;
	
		Tile_Clip[DEV_TILE_RED].x = 64;
		Tile_Clip[DEV_TILE_RED].y = 0;
		Tile_Clip[DEV_TILE_RED].w = TILE_WIDTH;
		Tile_Clip[DEV_TILE_RED].h = TILE_HEIGHT;
	
		Tile_Clip[DEV_TILE_BLUE].x = 128;
		Tile_Clip[DEV_TILE_BLUE].y = 0;
		Tile_Clip[DEV_TILE_BLUE].w = TILE_WIDTH;
		Tile_Clip[DEV_TILE_BLUE].h = TILE_HEIGHT;
		
		Level_Dimentions = g->getLevelDimentions();
		if(!(readMap(g, spritesheet, levelfile))){
			cout << "Could not load map file!" << endl;
		}
	}
	
	bool readMap(Game *g, string spritesheet, string mapfile){
		bool success = true;
		Tile *tempTile = new Tile();
		int x = 0, y = 0; //initial tile offest values
		ifstream map(mapfile);
		if(!map.is_open()) {cout << "Error Reading Map File" << endl;}
		else{
			for(int i = 0; i < TOTAL_TILES; ++i){
				//cout << "Tile Number: " << i << endl;
				int tileType = -1; //initial tile type value that is not valid
				map >> tileType;
				if (map.fail()){
					cout << "Error Reading Map File: Unexpected End of File!" << endl;
					success = false;
					break;
				}
				if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
				{
                tempTile->setTileProperties(x, y, tileType);
                //Next I need to icrement x value, then pair tile with image + sprite offset and push them into the vector
                x += TILE_WIDTH;
                if (x >= Level_Dimentions.w)
                {
					x = 0;
					y += TILE_HEIGHT;
				}
				}
				else {
					cout << "Error: Tile Type not Reconginzed." << endl;
					success = false;
					break;
				}
				Image *tileImage = new Image(g, spritesheet, 0, 0, Tile_Clip[tileType]);
				tiles.push_back(Tile_Image_Pair(tileImage, tempTile));
			}
		}
		return success;
	}


	void RenderTiles(Game *g){
		cout << "Render Tiles Called" << endl;
		for(int i = 0; i < TOTAL_TILES; ++i){
			cout << "Rendering Tile: " << endl;
			tiles[i].first->Render(g, tiles[i].second->getTileXPos(), tiles[i].second->getTileYPos());
		}
	}
};
