#pragma once

#include "Game.hpp"

using namespace std;

class Image{
	SDL_Texture *texture;
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Rect camera;
	SDL_Rect spriteClip;
	
	public:
	Image(Game *game, string filename, int _x=0, int _y=0, SDL_Rect clip = {-1, -1, -1, -1}){
		cout << filename << endl;
		texture = mm.get(game, filename);
		SDL_QueryTexture(texture,NULL,NULL,&src.w,&src.h);
		src.x=_x;
		src.y=_y;
		spriteClip = clip;
		//cout << "Created Image from " << filename << " with clip: " << _x << ", " << _y << endl;
	};
	
	void Render(Game *game, int x=0, int y=0){
		camera = game->getCameCameraPos();
		SDL_Renderer *renderer = game->getRenderer();
			if(spriteClip.x > -1 && spriteClip.y > -1){
				dest.w = spriteClip.w;
				dest.h = spriteClip.h;
				dest.x = x - camera.x;
				dest.y = y - camera.y;
				SDL_RenderCopy(renderer, texture, &spriteClip, &dest);
			}
			
			else {
			//cout << "Rendering w/o Clip" << endl;
			dest.w = src.w;
			dest.h = src.h;
			dest.x = x - camera.x;
			dest.y = y - camera.y;
			//dest.x = x;
			//dest.y = y;
			SDL_RenderCopy(renderer, texture, &src, &dest);
		}
			//cout << "image rendered at " << x << " " << y << endl;
	}
	
	void setImageTint(int r, int g, int b){
		SDL_SetTextureColorMod(texture, r, g, b);
	}
	SDL_Rect getSize(){ return src; }
	int getWidth(){return src.w; }
	int getHeight(){return src.h; }
	int getXPos(){return dest.x; }
	int getYPos(){return dest.y; }
	void imageDebug(){
		cout << "Image Debug Called! Image Properties: x: " << src.x << ", y: " << src.y << endl;
	}
};
