
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
		
		window = NULL;
		renderer = NULL;

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
		cout << "window created" << endl;
		
		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer == NULL) {
			// In the case that the renderer could not be made...
			printf("Could not create renderer: %s\n", SDL_GetError());
			return; 
		}
		cout << "renderer created" << endl;
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
		if (renderer == NULL || window == NULL) {return;}
		cout << "init" << endl;
		init();
		loop();
	}
};

class Image{
	SDL_Texture *texture;
	SDL_Rect src;
	public:
	Image(Game *game, string filename, int _x=0, int _y=0){
		SDL_Surface *surface;
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
		}
		cout << "texture created" << endl;
		
		SDL_FreeSurface(surface);
	};
	
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

class Sprite:public Image {
	float x,y,dx,dy,ax,ay;
	public:
	Sprite(Game *g, string filename, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, float _ax=0.0, float _ay=0.0):Image(g,filename) {
		x = _x;
		y = _y;
		dx = _dx;
		dy = _dy;
		ax = _ax;
		ay = _ay;
	}
	void update(float dt /* in ms */){
		dt/=1000.0;
		dx = dx + ax * dt;
		dy = dy + ay * dt;
		x = x + dx * dt;
		y = y + dy * dt;
	}
	void render(Game *g, int setX=-1, int setY=-1){
		if(setX != -1 && setY != -1) Image::Render(g, setX, setY);
		else Image::Render(g,(int)x,(int)y);
	}
	
	void setDX(float _dx) { dx = _dx; }
};

class MyGame:public Game {
	Image *background;
	Sprite *player;
	public:
	MyGame():Game("Wild Quest"){};
	
    void init(){
		background = new Image(this, "../res/back.bmp");
		player = new Sprite(this, "../res/characterSprite.bmp",688.0,384.0);
	}
	//Game loop, basic gameplay functionality goes here
	void loop(){
		unsigned start = SDL_GetTicks();
		bool again = true;
		SDL_Event event;
		int xVelocity=0;
		int yVelocity=0;
		cout << "enter game loop" << endl;

		while(again){
			unsigned end = SDL_GetTicks();
			float dt = start - end;
			start = end;
			SDL_Renderer *renderer = this->getRenderer();
			background->Render(this);
			player->Render(this);
			SDL_RenderPresent(renderer);	
			
			SDL_WaitEvent(&event);
			if (event.type == SDL_KEYDOWN){
				switch (event.key.keysym.sym){
					case SDLK_LEFT:  
						xVelocity -= 1;
						cout << "left" << endl;
						break;
					case SDLK_RIGHT: 
						xVelocity += 1;
						cout << "right" << endl;
						break;
					case SDLK_UP:    
						yVelocity -= 1; 
						cout << "up" << endl;
						break;
					case SDLK_DOWN:  
						yVelocity += 1;
						cout << "down" << endl;
						break;
					case SDLK_ESCAPE:
						again = false;
						break;
				}
			SDL_Delay(16);
			}
		}
		cout << "exit game loop" << endl;
	}
};

int main(int argc, char* argv[]) {

	Game *g;
	g = new MyGame();
	g->run();
	delete g;
	cout << "press enter to end program..." << endl;
	cin.get();
    return 0;
}
