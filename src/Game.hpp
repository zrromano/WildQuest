#ifndef GAME_HPP
#define GAME_HPP

#include <iostream>
#include <time.h>
#include <string>

#include "SDL.h"

using namespace std;

class Game{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Rect resolution;
	SDL_Rect Level_Dimentions;
	SDL_Rect GameCamera;
	bool inGameLoop;
	int mouseX; //For keeping track of where the mouse curson is on screen
	int mouseY;
	public:
	const Uint8 *keystate;
	Game(string title, int width=1280, int height=720, int Level_Width = 3200, int Level_Height = 3200){
		resolution.w = width;
		resolution.h = height;
		Level_Dimentions.w = Level_Width;
		Level_Dimentions.h = Level_Height;
		GameCamera = {0, 0, width, height};
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
		keystate = SDL_GetKeyboardState(NULL);
	}
	~Game(){
		SDL_DestroyWindow(window);
		SDL_Quit();
	}
	
	//Abstract methods defined by inheriting classes
	virtual void init()=0;
	virtual void keyboardHandler(SDL_Event *event)=0;
	virtual void mouseInputHandler(SDL_Event *event, int MouseX, int MouseY)=0;
	virtual void mouseCusorHandler(int MouseX, int MouseY)=0;
	virtual void update(float dt)=0;
	virtual void Render()=0;
	
	SDL_Renderer *getRenderer(){ return renderer; }
	SDL_Rect getResolution(){ return resolution; }
	SDL_Window *getWindow() { return window; }
	SDL_Rect getLevelDimentions(){return Level_Dimentions;}
	void setGameCameraPos(SDL_Rect camera){GameCamera = camera;	}
	SDL_Rect getCameCameraPos() {return GameCamera; }
	
	void setInGameLoop(bool _inGameLoop) { inGameLoop = _inGameLoop; }
	void run(){
		SDL_Event event;
		if (renderer == NULL || window == NULL) {return;}
		cout << "init" << endl;
		init();
		inGameLoop = true;
		unsigned start = SDL_GetTicks();
		while(inGameLoop){
			unsigned end = SDL_GetTicks();
			float dt = (end - start);
			start = end;
			SDL_GetMouseState(&mouseX, &mouseY); // Gets the location of the cursor
			mouseCusorHandler(mouseX, mouseY);
			update(dt);
			Render();
			SDL_PollEvent(&event);
			keyboardHandler(&event);
			if (event.type == SDL_MOUSEBUTTONDOWN) //Checks if mouse has been pressed down
					mouseInputHandler(&event, mouseX, mouseY);
			SDL_Delay(16);
		}
	}
};

#endif
