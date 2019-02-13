
#include "SDL.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

class Game{
	SDL_Window *window;
	SDL_Renderer *renderer;
	public:
	Game(string title, int width=1366, int height=768){
		SDL_Init(SDL_INIT_VIDEO);

		window = SDL_CreateWindow(
			title.c_str(),                  	// window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			width,                             // width, in pixels
			height,                            // height, in pixels
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
	
	//Abstract methods defined by inheriting classes
	virtual void init()=0;
	virtual void loop()=0;
	
	SDL_Renderer *getRenderer(){ return renderer; }
	SDL_Window *getWindow() { return window; }
	void run(){
		if (renderer == NULL || window == NULL) return;
		init();
		loop();
	}
};

class Image{
	SDL_Texture *texture;
	SDL_Rect src;
	public:
	Image(Game *game, string filename){
		SDL_Surface *surface;
		surface=SDL_LoadBMP(filename.c_str());
		if (surface == NULL) {
			printf("LoadBMP failed: %s\n", SDL_GetError());
			return;
		}
		
		texture = SDL_CreateTextureFromSurface(game->getRenderer(), surface);
		SDL_QueryTexture(texture,NULL,NULL,&(src.w),&(src.h));
		src.x=0;
		src.y=0;
		if (texture == NULL) {
			fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
			return;
		}
		
		SDL_FreeSurface(surface);
	}
	
	void Render(Game *game, int x=0, int y=0){
			SDL_Rect dest;
			dest.w = src.w;
			dest.h = src.h;
			dest.x = x;
			dest.y = y;
			SDL_Renderer *renderer = game->getRenderer();
			SDL_RenderCopy(renderer, texture, &src, &dest);
	}
	
	SDL_Rect getSize(){ return src; }
};

class MyGame:public Game {
	Image *background;
    SDL_Rect src,dest;
	public:
	MyGame():Game("Wild Quest"){};
	
    void init(){
		background = new Image(this, "res/back.bmp");
	}
	//Game loop, basic gameplay functionality goes here
	void loop(){
		for (int x=0;x<480;x++) {
			SDL_Renderer *renderer = getRenderer();
			background->Render(this,x,x);
			SDL_RenderPresent(renderer);
			SDL_Delay(16);
		}
	}
};

int main(int argc, char* argv[]) {

	Game *g;
	g = new MyGame();
	g->run();
	delete g;
	
	cin.get();
    return 0;
}
