#pragma once

#include <Math.h>
#include "Sprite.hpp"
#include "Projectile.hpp"

class Enemy:public Sprite{
	int lastShot; //game ticks since last shot
	int shotDelay;
	int maxHealth;
	int currentHealth;
	int attackRange; //Range the enemy will shoot at the player
	int aggroRange; //Range the enemy will begin to chase the player
	int maxSpeed;
	
	public:
	Enemy(Game *g, string filename, int _maxHealth=1, int _attackRange=500, int _aggroRange=1000, int _maxSpeed=200, int count=4, int fps=1, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, float spriteX=0.0, float spriteY=0.0):
	  Sprite(g, filename, count, fps, _x, _y, _dx, _dy, spriteX, spriteY){
		lastShot = 0;
		shotDelay = 30;
		maxHealth = _maxHealth;
		currentHealth = maxHealth;
		attackRange = _attackRange;
		aggroRange = _aggroRange;
		maxSpeed = _maxSpeed;
		this->setFriendly(false);
	}
	
	void update(float dt /* in ms */, int playerX, int playerY, int setFrame=-1){
		if( abs(distance(this->getX(), this->getY(), playerX, playerY)) < attackRange ){
			cout << "player in attack range" << endl;
			slowDown();
		}
		 /*else if( bulletComingTowardMe )
		  	avoidBullet*/
		else if( abs(distance(this->getX(), this->getY(), playerX, playerY)) < aggroRange ){
			//cout << "player in aggro range" << endl;
			walkToward(playerX, playerY);
		}
		 /*else
			wanderAimlessly*/
		Sprite::update(dt, setFrame);
		if (currentHealth <= 0){
			this->kill();
		}
	}
	
	
	bool hitByProjectile(Projectile *other){
		bool ret;
		SDL_Rect them = other->getSize();
		if(other->isFriendly())
			if (Sprite::within(other->getX(), other->getY()) || Sprite::within(other->getX() + them.w, other->getY()) ||
			Sprite::within(other->getX() + them.w, other->getY() + them.h) || Sprite::within(other->getX(), other->getY() + them.h)){
					ret = true;
					currentHealth -= other->getDamage();
					other->kill();
				}
		return ret;
	}
	
	float distance(float x1, float y1, float x2, float y2){
		float retVal;
		float x = x2 - x1;
		float y = y2 - y1;
		retVal = pow(x, 2) + pow(y, 2);
		retVal = sqrt(retVal);
		return retVal;
	}
	
	void walkToward(int x, int y){
			this->slowDown(); //helps change directions properly
			if(abs(this->getX() - x) > abs(this->getY() - y)){
				if(this->getX() > x){
					this->setFrame(1);
					if(this->getY() > y){
						if(abs(this->getDx()) <= maxSpeed)
							this->accelerateX(-10);
						if(abs(this->getDy()) <= maxSpeed)
							this->accelerateY(-10);
					}
					else if(this->getY() < y){
						if(abs(this->getDx()) <= maxSpeed)
							this->accelerateX(-10);
						if(abs(this->getDy()) <= maxSpeed)
							this->accelerateY(10);
					}
					else{
						if(abs(this->getDx()) <= maxSpeed)
							this->accelerateX(-10);
					}
				}
				else if(this->getX() < x){
					this->setFrame(0);
					if(this->getY() > y){
						if(abs(this->getDx()) < maxSpeed)
							this->accelerateX(10);
						if(abs(this->getDy()) < maxSpeed)
							this->accelerateY(-10);
					}
					else if(this->getY() < y){
						if(abs(this->getDx()) < maxSpeed)
							this->accelerateX(10);
						if(abs(this->getDy()) < maxSpeed)
							this->accelerateY(10);
					}
					else{
						if(abs(this->getDx()) < maxSpeed)
							this->accelerateX(10);
					}
				}
			}
			else{
				if(this->getY() > y){
					this->setFrame(3);
					if(this->getX() > x){
						if(abs(this->getDy()) < maxSpeed)
							this->accelerateY(-10);
						if(abs(this->getDx()) < maxSpeed)
							this->accelerateX(-10);
					}
					else if(this->getX() < x){
						if(abs(this->getDy()) < maxSpeed)
							this->accelerateY(-10);
						if(abs(this->getDx()) < maxSpeed)
							this->accelerateX(10);
					}
					else{
						if(abs(this->getDy()) < maxSpeed)
							this->accelerateY(-10);
					}
				}
				else if(this->getY() < y){
					this->setFrame(2);
					if(this->getX() > x){
						if(abs(this->getDy()) < maxSpeed)
							this->accelerateY(10);
						if(abs(this->getDx()) < maxSpeed)
							this->accelerateX(-10);
					}
					else if(this->getX() < x){
						if(abs(this->getDy()) < maxSpeed)
							this->accelerateY(10);
						if(abs(this->getDx()) < maxSpeed)
							this->accelerateX(10);
					}
					else{
						if(abs(this->getDy()) < maxSpeed)
							this->accelerateY(10);
					}
				}
			}
	}
	void slowDown(){
		if(this->getDx() > 0)
			this->accelerateX(-5);
		if(this->getDx() < 0)
			this->accelerateX(5);
		if(this->getDy() > 0)
			this->accelerateY(-5);
		if(this->getDy() < 0)
			this->accelerateY(5);
	}
	void shoot(){ lastShot = 0; }
	void noShoot(){ lastShot += 1; }
	bool canShoot(){ return lastShot >= shotDelay; }
};
