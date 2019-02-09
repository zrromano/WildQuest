
#include "SDL.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[]) {

    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_VIDEO);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "WildWildQuest",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1366,                               // width, in pixels
        768,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1; 
   }
	 SDL_Renderer *renderer;
     renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL) {
        // In the case that the window could not be made...
        printf("Could not create renderer: %s\n", SDL_GetError());
        return 1; 
   }
     
    SDL_Surface *background;
    SDL_Texture *texture;
    SDL_Rect src,dest;
    
    
	background=SDL_LoadBMP("back.bmp");
    // The window is open: could enter program loop here (see SDL_PollEvent())
    if (background==NULL) {
		cout << "Something happened" << endl;
		return 1;
	}
	
	texture = SDL_CreateTextureFromSurface(renderer, background);
	SDL_QueryTexture(texture,NULL,NULL,&(src.w),&(src.h));
	src.x=0;
	src.y=0;
	dest.w=src.w;
	dest.h=src.h;
	dest.x=0;
	dest.y=0;

	if (texture == NULL) {
		fprintf(stderr, "CreateTextureFromSurface failed: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_FreeSurface(background);
	

	for (int x=0;x<480;x++) {
	  dest.x=x;
	  dest.y=x;
      SDL_RenderCopy(renderer, texture, &src, &dest);
      SDL_RenderPresent(renderer);
      SDL_Delay(16);  // Pause execution for 3000 milliseconds, for example
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}
