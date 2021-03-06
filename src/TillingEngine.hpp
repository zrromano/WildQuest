#pragma once

#include <string>
#include <fstream>

#include "Image.hpp"
#include "Tile.hpp"
#include "Utilities.hpp"

using namespace std;

typedef pair<Image *, Tile *> Tile_Image_Pair;

class TillingEngine{
	//Tile Contstants 
	const int TILE_WIDTH = 64;
	const int TILE_HEIGHT = 64;
	static const int TOTAL_TILES = 1024;
	
	const static int TOTAL_TILE_SPRITES = 5;
	
	const int DEV_TILE_BLACK = 0;
	const int DEV_TILE_RED = 1;
	const int DEV_TILE_GREEN = 2;
	const int DEV_TILE_BLUE = 3;
	const int DEV_TILE_BROWN = 4;
	
	
	SDL_Rect Level_Dimentions;
	SDL_Rect Tile_Clip[ TOTAL_TILE_SPRITES ];
	SDL_Rect CollidedTile;
	
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
	
		Tile_Clip[DEV_TILE_GREEN].x = 128;
		Tile_Clip[DEV_TILE_GREEN].y = 0;
		Tile_Clip[DEV_TILE_GREEN].w = TILE_WIDTH;
		Tile_Clip[DEV_TILE_GREEN].h = TILE_HEIGHT;
		
		Tile_Clip[DEV_TILE_BLUE].x = 0;
		Tile_Clip[DEV_TILE_BLUE].y = 64;
		Tile_Clip[DEV_TILE_BLUE].w = TILE_WIDTH;
		Tile_Clip[DEV_TILE_BLUE].h = TILE_HEIGHT;
		
		Tile_Clip[DEV_TILE_BROWN].x = 64;
		Tile_Clip[DEV_TILE_BROWN].y = 64;
		Tile_Clip[DEV_TILE_BROWN].w = TILE_WIDTH;
		Tile_Clip[DEV_TILE_BROWN].h = TILE_HEIGHT;
		
		Level_Dimentions = g->getLevelDimentions();
		if(!(readMap(g, spritesheet, levelfile))){
			cout << "Could not load map file!" << endl;
		}
	}
	
	bool readMap(Game *g, string spritesheet, string mapfile){
		/*fstream outputfile("outfile.txt"); 
		if(!outputfile.is_open()){
			cout << "Error opening output file." << endl;
			
		}*/
		bool success = true;
		int x = 0, y = 0; //initial tile offest values
		ifstream map(mapfile);
		if(!map.is_open()) {cout << "Error Reading Map File" << endl;}
		else{
			for(int i = 0; i < TOTAL_TILES; i++){
				Tile *tempTile = new Tile();
				//cout << "Tile Number: " << i << endl;
				int tileType = -1; //initial tile type value that is not valid
				map >> tileType;
				//outputfile << "Tile Type: " << tileType << endl;
				if (map.fail()){
					cout << "Error Reading Map File: Unexpected End of File!" << endl;
					success = false;
					break;
				}
				if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
				{
                tempTile->setTileProperties(tileType, x, y);
                //outputfile << "Created New Tile w/ Properties: " << tempTile->getTileXPos() << ", " << tempTile->getTileYPos() << endl;
                //Next I need to icrement x value, then pair tile with image + sprite offset and push them into the vector
                x = x + TILE_WIDTH;
                if (x >= Level_Dimentions.w)
					{
						//outputfile << "X is out of tile bounds! Updating to 0" << endl;
						x = 0;
						y = y + TILE_HEIGHT;
					}
				}				
				else {
					cout << "Error: Tile Type not Reconginzed." << endl;
					success = false;
					break;
				}
				Image *tileImage = new Image(g, spritesheet, Tile_Clip[tileType].x, Tile_Clip[tileType].y, Tile_Clip[tileType]);
				//outputfile << "Tile Clip " << Tile_Clip[tileType].x << " ," << Tile_Clip[tileType].y << endl;
				tiles.push_back(Tile_Image_Pair(tileImage, tempTile));
				//outputfile << "Tile and Image Pair placed at vector index " << i << endl;
				//outputfile << "Vector at index: " << i << ", properties: " << tiles[i].second->getTileXPos() << ", " << tiles[i].second->getTileYPos() << endl; 
			}
		}
		return success;
	}

	void RenderTiles(Game *g){
		//cout << "Vector Size: " << tiles.size() << endl;
		for(int i = 0; i < TOTAL_TILES; ++i){
			//cout << "Rendering Tile: " << i << endl;
			//tiles[i].second->tileDebug();
			tiles[i].first->Render(g, tiles[i].second->getTileXPos(), tiles[i].second->getTileYPos());
		}
	}
	
	bool checkCollision(SDL_Rect mBox){
		for(int i = 0; i < TOTAL_TILES; i++)
		{
			if( (tiles[i].second->getTileType() >= DEV_TILE_BLUE) && (tiles[i].second->getTileType() <= DEV_TILE_BROWN))
			{
				if(CollisionCheck(mBox, tiles[i].second->getMBox())){ 
					CollidedTile = tiles[i].second->getMBox();
					cout << "Collision with collidable Tile (x,y): " << CollidedTile.x << ", " << CollidedTile.y << endl;
					return true;
				}
			}
		}
	}
	SDL_Rect getCollidedTile(){return CollidedTile; }
};
