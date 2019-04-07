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
	
	const int TOTAL_TILE_SPRITES = 3;
	
	const int DEV_TILE_BLACK = 0;
	const int DEV_TILE_RED = 1;
	const int DEV_TILE_BLUE = 2;
	
	vector < Tile_Image_Pair > tiles;
	public:
	
	TillingEngine(Game *g, string spritesheet, string levelfile){
		
	}
	
	bool readMap(string filename){
		bool success = false;
		Tile *tempTile = new Tile();
		int x = 0, y = 0; //initial tile offest values
		ifstream map(filename);
		if(!map.is_open()) {cout << "Error Reading Map File" << endl;}
		else{
			for(int i = 0; i < TOTAL_TILES; ++i){
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
				}
				
			}
		}
		return success;
	}
};
