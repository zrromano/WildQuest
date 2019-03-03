//Note: strip command in command prompt will shrink filesize
#include "SDL.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

typedef enum {
		TitleScreen, 	//Starting Title Sceen
		Running, 		//Gameplay
		Inventory,		//Inventory Screen
		Pause,			//Pause Menu
		DeathScreen,	//Death Screen
		NPCDialog 		//Dialog with interacting with NPC
} SceneType;

class Game{
	SDL_Window *window;
	SDL_Renderer *renderer;
	SDL_Rect resolution;
	bool inGameLoop;
	int mouseX; //For keeping track of where the mouse curson is on screen
	int mouseY;
	public:
	const Uint8 *keystate;
	Game(string title, int width=1280, int height=720){
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
	virtual void mouseInputHandler(SDL_Event *event, int MouseX, int MouseY)=0;
	virtual void mouseCusorHandler(int MouseX, int MouseY)=0;
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

class Image{
	SDL_Texture *texture;
	SDL_Rect src;
	SDL_Rect dest;
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
			dest.w = src.w;
			dest.h = src.h;
			dest.x = x;
			dest.y = y;
			SDL_Renderer *renderer = game->getRenderer();
			SDL_RenderCopy(renderer, texture, &src, &dest);
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
	bool dead, wallCollision;
	SDL_Rect size;
	public:
	bool within(float otherX, float otherY){
		return x<=otherX && otherX<=x+size.w && y<=otherY && otherY<=y+size.h;
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
		size = getSize();
	}
	void random() {
		x=(float)(rand() % gw);
		y=(float)(rand() % gh);
		dx=(float)((rand()% (40*100))-2000)/100.0;
		dy=(float)((rand()% (40*100))-2000)/100.0;
	}
	bool collision(){
		bool ret = false;
		if(wallCollision) ret = true;
		return ret;
	}
	void update(float dt /* in ms */, int setFrame=-1){
		if(wallCollision) wallCollision = false;
		if(!dead){
		//Animation::update(dt, setFrame);
		dt/=1000.0;
		x = x + dx * dt;
		y = y + dy * dt;
		if(y<0 && dy < 0){
			wallCollision = true;
			cout << "hit top wall" << endl;
			dy = 0;
			y = 0;
		}
		else if(y>gh-size.h && dy > 0) {
			wallCollision = true;
			cout << "hit bottom wall" << endl;
			dy = 0;
			y = gh-size.h;
		}
		if(x<0 && dx < 0){
			wallCollision = true;
			cout << "hit left wall" << endl;
			dx = 0;
			x = 0;
		}
		else if(x>gw-size.w && dx > 0) {
			wallCollision = true;
			cout << "hit right wall" << endl;
			dx = 0;
			x = gw-size.w;
		}
	}
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
	void kill(){ dead = true; }
	float getDx(){ return dx; }
	float getDy(){ return dy; }
	float getX() { return x; }
	float getY() { return y; }
	float getW() { return size.w; }
	float getH() { return size.h; }
};

class Player:public Sprite{
	int lastShot; //game ticks since last shot
	int shotDelay;
	public:
	Player(Game *g, string filename, int count=2, int fps=1, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, float spriteX=0.0, float spriteY=0.0):
	  Sprite(g, filename, count, fps, _x, _y, _dx, _dy, spriteX, spriteY){
		lastShot = 0;
		shotDelay = 30;
	}
	void shoot(){ lastShot = 0; }
	void noShoot(){ lastShot += 1; }
	bool canShoot(){ return lastShot >= shotDelay; }
};

class SceneState {
	SceneType currentScene;
	int levelID = 1; //Possible Future Implementation
	int cutSceneID = 1; //Possible Future Implementation
	unsigned lastUpdatedTime;
	public:
	SceneState(SceneType scene){
		currentScene = scene;
	};
	void setScene(SceneType newScene){ currentScene = newScene; }
	SceneType getCurrentScene(){ return currentScene; }
	void setTime(unsigned time){lastUpdatedTime = time; }
	unsigned getLastUpdatedTime() {return lastUpdatedTime; } 
	
};

class Projectile: public Sprite{
	int damage;
	public:
	Projectile(Game *g, string filename, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, int _damage=0, int count=1, int fps=1, float spriteX=0.0, float spriteY=0.0):
	  Sprite(g, filename, count, fps, _x, _y, _dx, _dy, spriteX, spriteY){
		damage = _damage; 
	}	
};

class MyGame:public Game {
	Image *background, *TitleScreenBackground, *playSign, *pauseScreenBackground, *pauseLogo, *resumeImage, *mainMenuSign, *quitSign;
	Player *player;
	SceneState *scene;
	vector<Projectile *> projectiles;
	public:
	MyGame():Game("Wild Quest"){};
    void init(){
		scene = new SceneState(TitleScreen); //The Scene the game will start on
		TitleScreenBackground = new Image(this, "../res/titleScreenBack.bmp");
		playSign = new Image(this, "../res/playSign.bmp");
		background = new Image(this, "../res/back.bmp");
		player = new Player(this, "../res/playerSprite");
		//pauseScreenBackground = new Image(this, "../res/pauseBackground.bmp");
		pauseLogo = new Image(this, "../res/pauseLogo.bmp"); 
		resumeImage = new Image(this, "../res/resumeSign.bmp");
		mainMenuSign = new Image(this, "../res/mainMenuSign.bmp");
		quitSign = new Image(this, "../res/quitSign.bmp");
	}
	
	//keystate[SDL_SCANCODE_???] is a Uint8 array keeping track of current keyboard states
	//1 = true = pressed
	//0 = false = released
	void keyboardHandler(SDL_Event *event){
		//Escape key - exit game loop
		//note: Should be changed to open in-game menu with an exit game option
		if(keystate[SDL_SCANCODE_ESCAPE]){
			unsigned currentTicks = SDL_GetTicks();
			//Check to see if the pause button has been pressed, but only pause if its been 250ms since last scene change
			if(scene->getCurrentScene() == Running && currentTicks - scene->getLastUpdatedTime() > 250){
				scene->setScene(Pause);
				scene->setTime(currentTicks);
			}
			else if(scene->getCurrentScene() == Pause && currentTicks - scene->getLastUpdatedTime() > 250) {
				scene->setScene(Running);
				scene->setTime(currentTicks);
			}
			//setInGameLoop(false);
		}
		//A key - move left
		if(keystate[SDL_SCANCODE_A]){
			if(player->getDx() > -250)
				player->accelerateX(-10);
		}
		else if(player->getDx() < 0)	
			player->accelerateX(5);
		//D key - move right	
		if(keystate[SDL_SCANCODE_D]){
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
			
		if(keystate[SDL_SCANCODE_RIGHT] && player->canShoot()){
			player->setFrame(0);
			Projectile *projectile = new Projectile(this,"../res/bullet", 
											(player->getX() + player->getW() - 28), 
											(player->getY() + (player->getH()/2)), 
											((player->getDx()/2) + 400), 
											(player->getDy() * 0.8),
											0);
			projectiles.push_back(projectile);
			player->shoot();
		}
		else if(keystate[SDL_SCANCODE_LEFT] && player->canShoot()){
			player->setFrame(1);
			Projectile *projectile = new Projectile(this,"../res/bullet",  
											(player->getX() + 20), 
											(player->getY() + (player->getH()/2)), 
											(player->getDx()/2 - 400), 
											(player->getDy() * 0.8),
											0);
			projectiles.push_back(projectile);
			player->shoot();
		}
		else if(keystate[SDL_SCANCODE_DOWN] && player->canShoot()){
			Projectile *projectile = new Projectile(this,"../res/bullet", 
											(player->getX() + (player->getW()/2)), 
											(player->getY() + player->getH()), 
											(player->getDx() * 0.8), 
											(player->getDy()/2 + 400),
											0);
			projectiles.push_back(projectile);
			player->shoot();
		}
		else if(keystate[SDL_SCANCODE_UP] && player->canShoot()){
			Projectile *projectile = new Projectile(this,"../res/bullet", 
											(player->getX() + (player->getW()/2)), 
											player->getY(), 
											(player->getDx() * 0.8), 
											(player->getDy()/2 - 400),
											0);
			projectiles.push_back(projectile);
			player->shoot();
		} else { player->noShoot(); }
	}
	
	void mouseInputHandler(SDL_Event *event, int mX, int mY){
		cout << "Mouse Down Event Triggered With Cords: " << mX << " " << mY << endl;
		switch (scene->getCurrentScene()){
			case TitleScreen:
				menuHandler(mX, mY, true); //Passes cords to the menu handler with the flag that the user clicked
			break;
			case Pause:
				menuHandler(mX, mY, true);
			break;
		}
	}
	void mouseCusorHandler(int mX, int mY){
		switch (scene->getCurrentScene()){
			case TitleScreen:
				menuHandler(mX, mY, false); //Passes cords to the menu handler with the flag that the user did NOT click 
			break;
			case Pause:
				menuHandler(mX, mY, false);
			break;
		}
	}
	
	void menuHandler(int mouseX, int mouseY, bool userPressed){
		unsigned currentTicks = SDL_GetTicks();
		switch (scene->getCurrentScene()){
			case TitleScreen:
				// Very Ugly. Checking if we are in the bounds of the Play Button
				if(mouseX >= playSign->getXPos() && mouseX <= playSign->getXPos() + playSign->getWidth() 
					&& mouseY >= playSign->getYPos() && mouseY <= playSign->getYPos() + playSign->getHeight()){
					playSign->setImageTint(150, 150, 150); // Tints the button gray if hovering over
					//cout << "In Play Button Bounds" << endl;
					if(userPressed)
						scene->setScene(Running);
				}
				else if(mouseX >= quitSign->getXPos() && mouseX <= quitSign->getXPos() + quitSign->getWidth() 
					&& mouseY >= quitSign->getYPos() && mouseY <= quitSign->getYPos() + quitSign->getHeight()){
					quitSign->setImageTint(150, 150, 150);
					if(userPressed && currentTicks - scene->getLastUpdatedTime() > 250) // To Prevent Going to main Menu and quitting right away
						setInGameLoop(false);
				}
				else{
					// Returns Image tint to normal
					playSign->setImageTint(250, 250, 250);
					quitSign->setImageTint(250, 250, 250);
				}
			break;
			
			case Pause:
				if(mouseX >= resumeImage->getXPos() && mouseX <= resumeImage->getXPos() + resumeImage->getWidth() 
					&& mouseY >= resumeImage->getYPos() && mouseY <= resumeImage->getYPos() + resumeImage->getHeight()){
					resumeImage->setImageTint(150, 150, 150);
					//cout << "In Resume Bounds." << endl;
					if(userPressed)
						scene->setScene(Running);
				}
				else if(mouseX >= mainMenuSign->getXPos() && mouseX <= mainMenuSign->getXPos() + mainMenuSign->getWidth() 
					&& mouseY >= mainMenuSign->getYPos() && mouseY <= mainMenuSign->getYPos() + mainMenuSign->getHeight()){
					mainMenuSign->setImageTint(150, 150, 150);
					if(userPressed){
						scene->setTime(currentTicks); // To Prevent Going to main Menu and quitting right away
						scene->setScene(TitleScreen);
					}
				}
				else{
					// Returns Image tint to normal
					resumeImage->setImageTint(250, 250, 250);
					mainMenuSign->setImageTint(250, 250, 250);
					
				}
			break;
		}
	}
	
	void update(float dt){
		switch( scene->getCurrentScene() ) {
			case TitleScreen:
				
			break;
			case Running: 
				player->update(dt, player->getFrame());
				for(int i = 0; i < projectiles.size(); i++)
				projectiles[i]->update(dt);
			break;
			case Pause:
				
			break;
		}
	}
	
	void Render(){
		SDL_Renderer *renderer = getRenderer();
		switch( scene->getCurrentScene() ) {
			case TitleScreen:
				TitleScreenBackground->Render(this);
				playSign->Render(this, Game::getResolution().w /2 - 150, Game::getResolution().h / 2 - 150);
				quitSign->Render(this, Game::getResolution().w /2 - 150, Game::getResolution().h / 2 + 200);
				SDL_RenderPresent(renderer);
			break;
			case Running:
				background->Render(this);
				player->Render(this);
				for(int i = 0; i < projectiles.size(); i++)
					projectiles[i]->Render(this);
				SDL_RenderPresent(renderer);
			break;
			case Pause:
				// cout << "Paused" << endl;
				pauseLogo->Render(this, Game::getResolution().w / 2 - 100, Game::getResolution().h / 2 - 300);
				resumeImage->Render(this, Game::getResolution().w / 2 - 150, Game::getResolution().h / 2 - 100);
				mainMenuSign->Render(this, Game::getResolution().w / 2 - 150, Game::getResolution().h / 2 + 200);
				SDL_RenderPresent(renderer);
			break;
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
