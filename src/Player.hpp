#pragma once

#include "Sprite.hpp"

using namespace std;

class Player:public Sprite{
	int lastShot; //game ticks since last shot
	int shotDelay;
	public:
	Player(Game *g, string filename, int count=4, int fps=1, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, float spriteX=0.0, float spriteY=0.0):
	  Sprite(g, filename, count, fps, _x, _y, _dx, _dy, spriteX, spriteY){
		lastShot = 0;
		shotDelay = 30;
	}
	void shoot(){ lastShot = 0; }
	void noShoot(){ lastShot += 1; }
	bool canShoot(){ return lastShot >= shotDelay; }
};
