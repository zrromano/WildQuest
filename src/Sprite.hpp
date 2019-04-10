#pragma once

#include "Animation.hpp"

using namespace std;

class Sprite:public Animation {
	float x,y,dx,dy,ax,ay;
	int gw,gh;
	bool dead, friendly, wallCollision, spriteCollision;
	SDL_Rect size, spriteHitBox;
	public:
	Sprite(Game *g, string filename, int count=1, int fps=30, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, float spriteX=0.0, float spriteY=0.0):
	  Animation(g, filename, count, fps, spriteX, spriteY){
		x = _x;
		y = _y;
		dx = _dx;
		dy = _dy;
		SDL_Rect resolution = g->getResolution();
		SDL_Rect level_dimentions = g->getLevelDimentions();
		gw = level_dimentions.w;
		gh = level_dimentions.h;
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
	void update(float dt /* in ms */, int setFrame=-1, bool tileCollision=false){
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
		else if(tileCollision){
			cout << "Collision with tile!" << endl;
		}
	}
	}
	void Render(Game *g, int setX=-1, int setY=-1){
		if(setX != -1 && setY != -1 && !dead) {
			//cout << "pre-render" << endl;
			Animation::Render(g, setX, setY);
			//cout << "Sprite rendered at " << setX << " " << setY << endl;
		}
		else if (!dead){
			//cout << "pre-render" << endl;
			Animation::Render(g,(int)x, (int)y);
			//cout << "Sprite rendered at " << x << " " << y << endl;
		}
	}
	
	bool collides(){
		bool ret = false;
		if(wallCollision) ret = true;
		if(spriteCollision) ret = true;
		return ret;
	}
	
	bool within(float otherX, float otherY){
		return x<=otherX && otherX<=x+size.w && y<=otherY && otherY<=y+size.h;
		
	}
	bool collidesWith(Sprite &other){
		spriteCollision = false;
		SDL_Rect them = other.getSize();
		if (within(other.x, other.y) || within(other.x + them.w, other.y) ||
			within(other.x + them.w, other.y + them.h) || within(other.x, other.y + them.h)){
			spriteCollision = true;
		}
		return spriteCollision;
	}
	
	void accelerateX(float ddx) { dx += ddx; }
	void accelerateY(float ddy) { dy += ddy; }
	void setDx(float _dx){ dx = _dx; }
	void setDy(float _dy){ dy = _dy; }
	void setFriendly(bool _friendly){ friendly = _friendly; }
	void setSpriteCollision(bool _collision){ spriteCollision = _collision; }
	void kill(){ dead = true; }
	
	bool getSpriteCollision(){ return spriteCollision; }
	bool isDead(){ return dead; }
	bool isFriendly(){ return friendly; }
	
	float getDx(){ return dx; }
	float getDy(){ return dy; }
	float getX() { return x; }
	float getY() { return y; }
	float getW() { return size.w; }
	float getH() { return size.h; }
	SDL_Rect getHitBox(){
		spriteHitBox = {(int)x, (int)y, size.w, size.h};
		return spriteHitBox;
	}
};
