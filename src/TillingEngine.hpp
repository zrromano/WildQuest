#pragma once

#include <string>
#include <fstream>

#include "Image.hpp"
#include "Tile.hpp"

using namespace std;

typedef pair<Image *, Tile *> Tile_Image_Pair;

class TillingEngine{
	//Tile Contstants 
	const int TILE_WIDTH = 64;
	const int TILE_HEIGHT = 64;
	static const int TOTAL_TILES = 1024;
	
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
	
		Tile_Clip[DEV_TILE_RED].x = 63;
		Tile_Clip[DEV_TILE_RED].y = 0;
		Tile_Clip[DEV_TILE_RED].w = TILE_WIDTH;
		Tile_Clip[DEV_TILE_RED].h = TILE_HEIGHT;
	
		Tile_Clip[DEV_TILE_BLUE].x = 127;
		Tile_Clip[DEV_TILE_BLUE].y = 0;
		Tile_Clip[DEV_TILE_BLUE].w = TILE_WIDTH;
		Tile_Clip[DEV_TILE_BLUE].h = TILE_HEIGHT;
		
		Level_Dimentions = g->getLevelDimentions();
		if(!(readMap(g, spritesheet, levelfile))){
			cout << "Could not load map file!" << endl;
		}
	}
	
	bool readMap(Game *g, string spritesheet, string mapfile){
		fstream outputfile("outfile.txt"); 
		if(!outputfile.is_open()){
			cout << "Error opening output file." << endl;
			
		}
		bool success = true;
		int x = 0, y = 0; //initial tile offest values
		ifstream map(mapfile);
		if(!map.is_open()) {cout << "Error Reading Map File" << endl;}
		else{
			for(int i = 0; i < TOTAL_TILES; ++i){
				Tile *tempTile = new Tile();
				//cout << "Tile Number: " << i << endl;
				int tileType = -1; //initial tile type value that is not valid
				map >> tileType;
				outputfile << "Tile Type: " << tileType << endl;
				if (map.fail()){
					cout << "Error Reading Map File: Unexpected End of File!" << endl;
					success = false;
					break;
				}
				if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
				{
                tempTile->setTileProperties(tileType, x, y);
                outputfile << "Created New Tile w/ Properties: " << tempTile->getTileXPos() << ", " << tempTile->getTileYPos() << endl;
                //Next I need to icrement x value, then pair tile with image + sprite offset and push them into the vector
                x = x + TILE_WIDTH;
                if (x >= Level_Dimentions.w)
					{
						outputfile << "X is out of tile bounds! Updating to 0" << endl;
						x = 0;
						y = y + TILE_HEIGHT;
					}
				}				
				else {
					cout << "Error: Tile Type not Reconginzed." << endl;
					success = false;
					break;
				}
				Image *tileImage = new Image(g, spritesheet, Tile_Clip[tileType].x, Tile_Clip[tileType].y);
				outputfile << "Tile Clip " << Tile_Clip[tileType].x << " ," << Tile_Clip[tileType].y << endl;
				tiles.push_back(Tile_Image_Pair(tileImage, tempTile));
				outputfile << "Tile and Image Pair placed at vector index " << i << endl;
				outputfile << "Vector at index: " << i << ", properties: " << tiles[i].second->getTileXPos() << ", " << tiles[i].second->getTileYPos() << endl; 
			}
		}
		outputfile << "Finished Reading Map File.  Total Vector Size: " << tiles.size();
		/*TODO: REMOVE
		outputfile << "=======================" << endl << endl;
		for(int i = 1; i < TOTAL_TILES; ++i){
			outputfile << "Rendering Tile: " << i << endl;
			outputfile << "Vector at index: " << i << ", properties: " << tiles[i].second->getTileXPos() << ", " << tiles[i].second->getTileYPos() << endl; 
			outputfile << "Rendering at Pos: " << tiles[i].second->getTileXPos() << ", " << tiles[i].second->getTileYPos() << endl;
			tiles[i].first->Render(g, tiles[i].second->getTileXPos(), tiles[i].second->getTileYPos());
		}
		TODO: End Remove*/
		//RenderTiles(g);
		return success;
	}

	void RenderTiles(Game *g){
		
		cout << "Render Tiles Called" << endl;
		//cout << "Vector Size: " << tiles.size() << endl;
		for(int i = 1; i < TOTAL_TILES; ++i){
			cout << "Rendering Tile: " << i << endl;
			//tiles[i].second->tileDebug();
			tiles[i].first->Render(g, tiles[i].second->getTileXPos(), tiles[i].second->getTileYPos());
		}
	}
};
