#pragma once

#include <string>
#include <map>
#include <iostream>

#include "SDL.h"
#include "Game.hpp"

using namespace std;

class MediaManager {
	map<string, SDL_Texture *> images;
	public:
	SDL_Texture *get(Game *game, string filename) {
		if (images.find(filename)==images.end()){
			SDL_Surface *surface;
			SDL_Texture *texture;
			surface=SDL_LoadBMP(filename.c_str());
			if (surface == NULL) {
				printf("LoadBMP failed: %s\n", SDL_GetError());
			}
			cout << "surface loaded" << endl;
		
			Uint32 colorKey = SDL_MapRGB(surface->format,0,255,0);
			SDL_SetColorKey(surface, SDL_TRUE,colorKey);
			texture = SDL_CreateTextureFromSurface(game->getRenderer(), surface);
			if (texture == NULL) {
				fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
			}
			images[filename] = texture;
			cout << "texture created" << endl;
		
			SDL_FreeSurface(surface);
		}
		return images[filename];
	}
};

extern MediaManager mm;
