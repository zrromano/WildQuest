//Note: strip command in command prompt will shrink filesize
#include "SDL.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "MediaManager.hpp"
#include "Game.hpp"
#include "Image.hpp"
#include "Animation.hpp"
#include "Sprite.hpp"
#include "Player.hpp"
#include "SceneState.hpp"
#include "Projectile.hpp"
#include "Tile.hpp"
#include "TillingEngine.hpp"

using namespace std;

class MyGame:public Game {
	Image *background, *TitleScreenBackground, *playSign, *pauseScreenBackground, *pauseLogo, *resumeImage, *mainMenuSign, *quitSign;
	TillingEngine *tEngine;
	Player *player;
	SDL_Rect camera, level_dimentions;
	SceneState *scene;
	vector<Projectile *> projectiles;
	public:
	MyGame():Game("Wild Quest"){};
    void init(){
		camera = {0, 0, 1280, 720};
		level_dimentions = this->getLevelDimentions();
		tEngine = new TillingEngine(this, "../res/dev_tilesprites.bmp", "../level/dev_level.map");
		scene = new SceneState(TitleScreen); //The Scene the game will start o
		TitleScreenBackground = new Image(this, "../res/titleScreenBack.bmp");
		playSign = new Image(this, "../res/playSign.bmp");
		background = new Image(this, "../res/back.bmp");
		pauseLogo = new Image(this, "../res/pauseLogo.bmp"); 
		resumeImage = new Image(this, "../res/resumeSign.bmp");
		mainMenuSign = new Image(this, "../res/mainMenuSign.bmp");
		quitSign = new Image(this, "../res/quitSign.bmp");
		
		//Player spawn at 1000, 1000 to avoid camera issues
		player = new Player(this, "../res/playerSprite", 4, 1, 1000, 1000);
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
			player->setFrame(2);
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
			player->setFrame(3);
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
				camera = {0, 0, 1280, 720};
				this-> setGameCameraPos(camera);
			break;
			
			case Running: 
				if(!(tEngine->checkCollision(player->getHitBox()))){
					player->update(dt, player->getFrame());
				}
				//Camera Centered on the Player
				camera.x = player->getX() + (player->getW() / 2) - (camera.w / 2);
				camera.y = player->getY() + (player->getH() / 2) - (camera.h / 2);
				if( camera.x < 0 )
				{ 
					camera.x = 0;
				}
				if( camera.y < 0 )
				{
					camera.y = 0;
				}
				if( camera.x > level_dimentions.w - camera.w )
				{
					camera.x = level_dimentions.w - camera.w;
				}
				if( camera.y > level_dimentions.h - camera.h )
				{
					camera.y = level_dimentions.h - camera.h;
				}
				this->setGameCameraPos(camera);
				//cout << "Camera x: " << camera.x << " Camera y:" << camera.y;
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
				//background->Render(this);
				tEngine->RenderTiles(this);
				player->Render(this);
				for(int i = 0; i < projectiles.size(); i++)
					projectiles[i]->Render(this);
				SDL_RenderPresent(renderer);
			break;
			case Pause:
				// cout << "Paused" << endl;
				pauseLogo->Render(this, camera.x + (camera.w / 2) - 100 , camera.y + (camera.h / 2) - 300);
				resumeImage->Render(this, camera.x + (camera.w / 2) - 150, camera.y + (camera.h / 2) - 100);
				mainMenuSign->Render(this, camera.x + (camera.w / 2) - 150, camera.y + (camera.h / 2) + 200);
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
