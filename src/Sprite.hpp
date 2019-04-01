#pragma once

#include "Animation.hpp"

using namespace std;

class Sprite:public Animation {
	float x,y,dx,dy,ax,ay;
	int gw,gh,cameraW,cameraH;
	bool dead, wallCollision;
	SDL_Rect size;
	SDL_Rect level_dimentions;
	SDL_Rect camera;
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
		level_dimentions = g->getLevelDimentions();
		camera = g->getResolution();
		x = _x;
		y = _y;
		dx = _dx;
		dy = _dy;
		gw = level_dimentions.w;
		gh = level_dimentions.h;
		cameraW = camera.w;
		cameraH = camera.h;
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
			//Getting the correct location for the camera to center it on the player
			camera.x = (setX + 128 / 2 ) - cameraW / 2; 
			camera.y = (setY + 128 / 2 ) - cameraH / 2;
			//Keep the camera in bounds
			cout << "Camera X: " << camera.x << endl;
			cout << "Camera Y: " << camera.y << endl;
            if( camera.x < 0 )
                    camera.x = 0;
            if( camera.y < 0 )
                    camera.y = 0;
            if( camera.x > gw - camera.w )
                    camera.x = gw - camera.w;
            if( camera.y > gh - camera.h )
                    camera.y = gh - camera.h;
			Animation::Render(g, camera.x, camera.y);
			//cout << "Sprite rendered at " << setX << " " << setY << endl;
		}
		else {
			//Getting the correct location for the camera to center it on the player
			camera.x = (setX + 128 / 2 ) - cameraW / 2; 
			camera.y = (setY + 128 / 2 ) - cameraH / 2;
			//Keep the camera in bounds
			cout << "Camera X: " << camera.x << endl;
			cout << "Camera Y: " << camera.y << endl;
            if( camera.x < 0 )
                    camera.x = 0;
            if( camera.y < 0 )
                    camera.y = 0;
            if( camera.x > gw - camera.w )
                    camera.x = gw - camera.w;
            if( camera.y > gh - camera.h )
                    camera.y = gh - camera.h;
			Animation::Render(g, camera.x, camera.y);
			//cout << "pre-render" << endl;
			//Animation::Render(g,(int)x,(int)y);
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
