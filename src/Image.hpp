#pragma once

#include "Game.hpp"

using namespace std;

class Image{
	SDL_Texture *texture;
	SDL_Rect src;
	SDL_Rect dest;
	SDL_Rect camera;
	SDL_Rect clip;
	public:
	Image(Game *game, string filename, int _x=0, int _y=0, SDL_Rect SpriteClip = {-1, -1, -1, -1}){
		cout << filename << endl;
		texture = mm.get(game, filename);
		SDL_QueryTexture(texture,NULL,NULL,&src.w,&src.h);
		src.x=_x;
		src.y=_y;
		clip = SpriteClip;
		/*SDL_Surface *surface;
		surface=SDL_LoadBMP(filename.c_str());
		if (surface == NULL) {
			printf("LoadBMP failed: %s\n", SDL_GetError());
			return;
		}
		cout << "surface loaded" << endl;
		
		Uint32 colorKey = SDL_MapRGB(surface->format,0,255,0);
		SDL_SetColorKey(surface, SDL_TRUE,colorKey);
		texture = SDL_CreateTextureFromSurface(game->getRenderer(), surface);
		SDL_QueryTexture(texture,NULL,NULL,&src.w,&src.h);
		src.x=_x;
		src.y=_y;
		if (texture == NULL) {
			fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
			return;
		}*/
	};
	
	void Render(Game *game, int x=0, int y=0){
		if(clip.x != -1 && clip.y != -1){
			//cout << "Rendering W/ Clip" << endl;
			dest.w = clip.w;
			dest.h = clip.h;
			dest.x = x - camera.x;
			dest.y = y - camera.y;
			SDL_Renderer *renderer = game->getRenderer();
			SDL_RenderCopy(renderer, texture, &src, &dest);
		}
		
		else{
			//cout << "Rendering w/o Clip" << endl;
			camera = game->getCameCameraPos();
			dest.w = src.w;
			dest.h = src.h;
			dest.x = x - camera.x;
			dest.y = y - camera.y;
			//dest.x = x;
			//dest.y = y;
			SDL_Renderer *renderer = game->getRenderer();
			SDL_RenderCopy(renderer, texture, &src, &dest);
			//cout << "image rendered at " << x << " " << y << endl;
		}
	}
	
	void setImageTint(int r, int g, int b){
		SDL_SetTextureColorMod(texture, r, g, b);
	}
	SDL_Rect getSize(){ return src; }
	int getWidth(){return src.w; }
	int getHeight(){return src.h; }
	int getXPos(){return dest.x; }
	int getYPos(){return dest.y; }
};
