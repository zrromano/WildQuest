//Note: strip command in command prompt will shrink filesize
#include "SDL.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

class Game{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Rect resolution;
	bool inGameLoop;
	public:
	const Uint8 *keystate;
	Game(string title, int width=1366, int height=768){
		resolution.w = width;
		resolution.h = height;
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
	virtual void update(float dt)=0;
	virtual void Render()=0;
	
	SDL_Renderer *getRenderer(){ return renderer; }
	SDL_Rect getResolution(){ return resolution; }
	SDL_Window *getWindow() { return window; }
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
			update(dt);
			Render();
			SDL_PollEvent(&event);
			keyboardHandler(&event);
			SDL_Delay(16);
		}
	}
};

class Image{
	SDL_Texture *texture;
	SDL_Rect src;
	public:
	Image(Game *game, string filename, int _x=0, int _y=0){
		cout << filename << endl;
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
			//cout << "image rendered at " << x << " " << y << endl;
	}
	
	SDL_Rect getSize(){ return src; }
};

typedef pair<Image *,int /*time*/> frameWithTime;
class Animation{
	vector < frameWithTime > frames;
	int time, totalTime, frame; //in ms
	public:
	Animation(Game *game, string path, int count, int fps, int spriteX=0, int spriteY=0) {
		stringstream filename;
		//path images/frame i .bmp
		totalTime = 0;
		for(int i=0; i < count; i++) {
			filename.str("");
			filename << path << setw(4) << setfill('0') << i << ".bmp";
			Image *img = new Image(game, filename.str(), spriteX, spriteY);
			frames.push_back(frameWithTime(img,1000/fps));
			totalTime += 1000;
		}
		frame = 0;
	}
	void update(float dt=0, int setFrame=-1){
		if(setFrame == -1){
			time += (int)dt;
			time = time % totalTime;
			int currentTime = time;
			frame = 0;
			for(int i=0; i < frames.size(); i++){
				if(currentTime<frames[i].second){
					frame = i;
					break;
				}
				currentTime -= frames[i].second;
			} 
		}
		else{
			frame = setFrame;
		}
	}
	SDL_Rect getSize() {
		SDL_Rect neverHappen;
		if (frames.size() > 0) 
			return frames[0].first->getSize();
		return neverHappen;
	}
	int getFrame(){ return frame; }
	void setFrame(int _frame){ frame = _frame; }
	void Render(Game *game, int x=0, int y=0){
		frames[frame].first->Render(game,x,y);
		//cout << "animation rendered at " << x << " " << y << endl;
	}
};

class Sprite:public Animation {
	float x,y,dx,dy,ax,ay;
	int gw,gh;
	bool dead;
	public:
	bool within(float otherX, float otherY){
		SDL_Rect us = getSize();
		return x<=otherX && otherX<=x+us.w && y<=otherY && otherY<=y+us.h;
	}
	bool collidesWith(Sprite &other){
		SDL_Rect them = other.getSize();
		return within(other.x, other.y) || 
				within(other.x + them.w, other.y) ||
				within(other.x + them.w, other.y + them.h) ||
				within(other.x, other.y + them.h);
	}
	Sprite(Game *g, string filename, int count=1, int fps=30, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, float spriteX=0.0, float spriteY=0.0):
	  Animation(g, filename, count, fps, spriteX, spriteY){
		x = _x;
		y = _y;
		dx = _dx;
		dy = _dy;
		SDL_Rect resolution = g->getResolution();
		gw = resolution.w;
		gh = resolution.h;
		dead = false;
	}
	void random() {
		x=(float)(rand() % gw);
		y=(float)(rand() % gh);
		dx=(float)((rand()% (40*100))-2000)/100.0;
		dy=(float)((rand()% (40*100))-2000)/100.0;
	}
	void update(float dt /* in ms */, int setFrame=-1){
		if(!dead){
		//Animation::update(dt, setFrame);
		dt/=1000.0;
		x = x + dx * dt;
		y = y + dy * dt;
		SDL_Rect src = getSize();
		if(y<0 && dy < 0){
			cout << "hit top wall" << endl;
			dy = 0;
			y = 0;
		}
		else if(y>gh-src.h && dy > 0) {
			cout << "hit bottom wall" << endl;
			dy = 0;
			y = gh-src.h;
		}
		if(x<0 && dx < 0){
			cout << "hit left wall" << endl;
			dx = 0;
			x = 0;
		}
		else if(x>gw-src.w && dx > 0) {
			cout << "hit right wall" << endl;
			dx = 0;
			x = gw-src.w;
		}
		}
		else {delete this;}
	}
	void Render(Game *g, int setX=-1, int setY=-1){
		if(setX != -1 && setY != -1) {
			//cout << "pre-render" << endl;
			Animation::Render(g, setX, setY);
			//cout << "Sprite rendered at " << setX << " " << setY << endl;
		}
		else {
			//cout << "pre-render" << endl;
			Animation::Render(g,(int)x,(int)y);
			//cout << "Sprite rendered at " << x << " " << y << endl;
		}
	}
	
	void accelerateX(float ddx) { dx += ddx; }
	void accelerateY(float ddy) { dy += ddy; }
	void setDx(float _dx){ dx = _dx; }
	void setDy(float _dy){ dy = _dy; }
	float getDx(){ return dx; }
	float getDy(){ return dy; }
	float getX() { return x; }
	float getY() { return y; }
};

class MyGame:public Game {
	Image *background;
	Sprite *player;
	vector<Sprite *> projectiles;
	public:
	MyGame():Game("Wild Quest"){};
    void init(){
		background = new Image(this, "../res/back.bmp");
		player = new Sprite(this, "../res/playerSprite", 2, 1);
		Sprite *projectile = new Sprite(this,"../res/bullet", 1, 30, player->getX(), 500.0,500.0);
		projectiles.push_back(projectile);
	}
	//Game loop, basic gameplay functionality goes here
	/*void loop(){

		int facing = 0;
		//0 = right
		//1 = left
		//2 = up
		//3 = down
		bool again = true;
		SDL_Event event;
		cout << "enter game loop" << endl;

		unsigned start = SDL_GetTicks();
		while(again){
			unsigned end = SDL_GetTicks();
			float dt = (end - start);
			start = end;
	
			
			SDL_PollEvent(&event);
			switch(event.type){
				case SDL_KEYDOWN:
					switch (event.key.keysym.sym){
						//Accelerate in a direction up to a max speed
						case SDLK_LEFT:  
							facing = 1;
							if(player->getDx() > -250)
								player->accelerateX(-10);
							//cout << "right" << endl;
							break;
						case SDLK_RIGHT: 
							facing = 0;
							if(player->getDx() < 250)
								player->accelerateX(10);
							//cout << "right" << endl;
							break;
						case SDLK_UP:
							if(player->getDy() > -250)
								player->accelerateY(-10);
							//cout << "up" << endl;
							break;
						case SDLK_DOWN:
							if(player->getDy() < 250)
								player->accelerateY(10);
							//cout << "down" << endl;
							break;
					}
					
				case SDL_KEYUP:
					switch (event.key.keysym.sym){
						//Break game loop
						case SDLK_ESCAPE:
							again = false;
							break;
					}
				}
			SDL_Delay(16);
		}
		cout << "exit game loop" << endl;
	}*/
	
	//keystate[SDL_SCANCODE_???] is a Uint8 array keeping track of current keyboard states
	//1 = true = pressed
	//0 = false = released
	void keyboardHandler(SDL_Event *event){
		//Escape key - exit game loop
		//note: Should be changed to open in-game menu with an exit game option
		if(keystate[SDL_SCANCODE_ESCAPE])
			setInGameLoop(false);
			
		//A key - move left
		if(keystate[SDL_SCANCODE_A]){
			player->setFrame(1);
			if(player->getDx() > -250)
				player->accelerateX(-10);
		}
		else if(player->getDx() < 0)	
			player->accelerateX(5);
		//D key - move right	
		if(keystate[SDL_SCANCODE_D]){
			player->setFrame(0);
			if(player->getDx() < 250){
				player->accelerateX(10);
			}
		}
		else if(player->getDx() > 0)	
			player->accelerateX(-5);
		//W key - move up
		if(keystate[SDL_SCANCODE_W]){
			if(player->getDy() > -250)
				player->accelerateY(-10);
		}
		else if(player->getDy() < 0)	
			player->accelerateY(5);
		//S key - move up
		if(keystate[SDL_SCANCODE_S]){
			if(player->getDy() < 250)
				player->accelerateY(10);
		}
		else if(player->getDy() > 0)	
			player->accelerateY(-5);
			
		if(keystate[SDL_SCANCODE_RIGHT]){
			player->setFrame(0);
			Sprite *projectile = new Sprite(this,"../res/bullet", 1, 30, player->getX(), player->getY(), (player->getDx()/2 + 400), player->getDy());
			projectiles.push_back(projectile);
		}
		if(keystate[SDL_SCANCODE_LEFT]){
			player->setFrame(1);
			Sprite *projectile = new Sprite(this,"../res/bullet", 1, 30, player->getX(), player->getY(), (player->getDx()/2 - 400), player->getDy());
			projectiles.push_back(projectile);
		}
		if(keystate[SDL_SCANCODE_DOWN]){
			Sprite *projectile = new Sprite(this,"../res/bullet", 1, 30, player->getX(), player->getY(), player->getDx(), (player->getDy()/2 + 400));
			projectiles.push_back(projectile);
		}
		if(keystate[SDL_SCANCODE_UP]){
			Sprite *projectile = new Sprite(this,"../res/bullet", 1, 30, player->getX(), player->getY(), player->getDx(), (player->getDy()/2 - 400));
			projectiles.push_back(projectile);
		}
	}
	
	void update(float dt){
		//cout << dt << endl;
		player->update(dt, player->getFrame());
		for(int i = 0; i < projectiles.size(); i++)
			projectiles[i]->update(dt,projectiles[i]->getFrame());
	}
	
	void Render(){
		SDL_Renderer *renderer = getRenderer();
		background->Render(this);
		player->Render(this);
		for(int i = 0; i < projectiles.size(); i++)
			projectiles[i]->Render(this);
		SDL_RenderPresent(renderer);
	}	
};

int main(int argc, char* argv[]) {
	Game *g;
	g = new MyGame();
	g->run();
	delete g;
    return 0;
}
