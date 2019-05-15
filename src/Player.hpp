#pragma once

#include "Sprite.hpp"
#include "Projectile.hpp"

using namespace std;

class Player:public Sprite{
	int lastShot; //game ticks since last shot
	int shotDelay;
	int maxHealth;
	int currentHealth;
	
	public:
	Player(Game *g, string filename, int _maxHealth, int count=4, int fps=1, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, float spriteX=0.0, float spriteY=0.0):
	  Sprite(g, filename, count, fps, _x, _y, _dx, _dy, spriteX, spriteY){
		lastShot = 0;
		shotDelay = 30;
		maxHealth = _maxHealth;
		currentHealth = maxHealth;
		this->setFriendly(true);
	}
	
	void update(float dt /* in ms */, int setFrame=-1, bool Collision=false, SDL_Rect CollidedTile={-1,-1,-1,-1}){
		Sprite::update(dt, setFrame, Collision, CollidedTile);
		if (currentHealth <= 0){
			this->kill();
		}
	}

	bool hitByProjectile(Projectile *other){
		bool ret;
		SDL_Rect them = other->getSize();
		if(!other->isFriendly())
			if (Sprite::within(other->getX(), other->getY()) || Sprite::within(other->getX() + them.w, other->getY()) ||
			Sprite::within(other->getX() + them.w, other->getY() + them.h) || Sprite::within(other->getX(), other->getY() + them.h)){
					ret = true;
					currentHealth -= other->getDamage();
					other->kill();
				}
		return ret;
	}
	
	void shoot(){ lastShot = 0; }
	void noShoot(){ lastShot += 1; }
	bool canShoot(){ return lastShot >= shotDelay; }
	int getHeath() {return currentHealth; }
	//TODO Dev function to decrement player health
	void decrementHealth() {currentHealth--;}
};
