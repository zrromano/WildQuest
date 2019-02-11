
#include "SDL.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

class Game{
	SDL_Window *window;
	SDL_Renderer *renderer;
	public:
	Game(){
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow(
			"WildWildQuest",                  	// window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			1366,                             // width, in pixels
			768,                            // height, in pixels
			SDL_WINDOW_OPENGL                  // flags
		);
		if (window == NULL) {
			// In the case that the window could not be made...
			printf("Could not create window: %s\n", SDL_GetError());
			return; 
		}
		
		SDL_Renderer *renderer;
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer == NULL) {
			// In the case that the renderer could not be made...
			printf("Could not create renderer: %s\n", SDL_GetError());
			return; 
		}
	}
	~Game(){
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	//Abstract mehtods defined by inheriting classes
	virtual void init()=0;
	virtual void loop()=0;
	
	SDL_Renderer *getRenderer(){ return this->renderer; }
	SDL_Window *getWindow() { return this->window; }
	void run(){
		if (renderer == NULL || window == NULL) return;
		init();
		loop();
	}
};

class MyGame:public Game {
	SDL_Texture *texture;
    SDL_Rect src,dest;

    void init(){
		SDL_Surface *background;
		background=SDL_LoadBMP("res/back.bmp");
		if (background==NULL) {
			cout << "Background image could not be loaded." << endl;
			return;
		}

		texture = SDL_CreateTextureFromSurface(this->getRenderer(), background);
		SDL_QueryTexture(texture,NULL,NULL,&(src.w),&(src.h));
		src.x=0;
		src.y=0;
		dest.w=src.w;
		dest.h=src.h;
		dest.x=0;
		dest.y=0;
		if (texture == NULL) {
			fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
			//exit(1)
		}
		SDL_FreeSurface(background);
	}
	//Game loop, basic gameplay functionality goes here
	void loop(){
		for (int x=0;x<480;x++) {
			dest.x=x;
			dest.y=x;
			SDL_RenderCopy(this->getRenderer(), texture, &src, &dest);
			SDL_RenderPresent(this->getRenderer());
			SDL_Delay(16);  // Pause execution for 3000 milliseconds, for example
		}
	}
};

int main(int argc, char* argv[]) {

	Game *g;
	g = new MyGame();
	g->run();
	delete g;
	
    return 0;
}
