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
	/* bool collisioncheck(){
		bool ret = false;
		if(wallCollision) ret = true;
		return ret;
	} */
	void update(float dt /* in ms */, int setFrame=-1, bool collision=false, SDL_Rect CollidedTile={-1,-1,-1,-1}){
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
		if(collision){
				int leftA, leftB, rightA, rightB, topA, topB, bottomA, bottomB;
				bool leftSideCollision = false;
				bool rightSideCollision = false;
				bool topSideCollision = false;
				bool bottomSideCollision = false;
				
				leftA = x;
				rightA = x + size.w;
				topA = y;
				bottomA = y + size.h;
				
				leftB = CollidedTile.x;
				rightB = CollidedTile.x + CollidedTile.w;
				topB = CollidedTile.y;
				bottomB = CollidedTile.y + CollidedTile.h;
				
				if(bottomA >= topB && !(topA >= topB)) {topSideCollision = true;}
				if(topA <= bottomB && !(bottomA <= bottomB)) {bottomSideCollision = true;}
				if(rightA >= leftB && !(leftA >= leftB)) {leftSideCollision = true;}
				if(leftA <= rightB && !(rightA <= rightB)) {rightSideCollision = true;}
				
					//If Only Top of Tile Collision
					if(topSideCollision && dx == 0){
						dy = -dy;
						if(topB != 0) {y = y - (bottomA - topB) - 10;}
						cout << "Top Side Collision Detected with tile (x,y) : " << CollidedTile.x << ", " << CollidedTile.y << endl;
						cout << "Moving Player to: " << x << ", " << y << endl;
						topSideCollision = false;
					}
					//Only Bottom Side Tile Collision
					else if(bottomSideCollision && dx == 0){
						dy = -dy;
						if(bottomB != 0) {y = y + (bottomB - topA) + 10;}
						cout << "Bottom Side Collision Detected with tile (x,y) : " << CollidedTile.x << ", " << CollidedTile.y << endl;
						cout << "Moving Player to: " << x << ", " << y << endl;
						bottomSideCollision = false;
					}
					//Only Left Side Tile Collision
					else if(leftSideCollision && dy == 0){
						dx = -dx;
						if(leftB != 0){ x = x - (rightA - leftB) - 10;}
						cout << "Left Side Collision Detected with tile (x,y) : " << CollidedTile.x << ", " << CollidedTile.y << endl;
						cout << "Moving Player to: " << x << ", " << y << endl;
						leftSideCollision = false;
					}
					//Only Right Side Tile Collision
					else if(rightSideCollision && dy == 0){
						dx = -dx;
						if(rightB != 0 ){x = x + (rightB - leftA) + 10;}
						cout << "Right Side Collision Detected with tile (x,y) : " << CollidedTile.x << ", " << CollidedTile.y << endl;
						cout << "Moving Player to: " << x << ", " << y << endl;
						rightSideCollision = false;
					}
					else if(dx != 0 && dy != 0){
						//If dx and dy are both positive, moving down and right
						if(dx > 0 && dy > 0 && leftSideCollision && !topSideCollision){
							dx = -dx;
							x = x - (rightA - leftB) - 10;
							leftSideCollision = false;
						}
						//If dx is positive by dy is negative, moving up and right
						if(dx > 0 && dy < 0 && leftSideCollision){
							dx = -dx;
							x = x - (rightA - leftB) - 10;
							leftSideCollision = false;
						}
						//If dx is negative and dy is positive, moving down and left
						if(dx < 0 && dy > 0 && rightSideCollision && !topSideCollision){
							dx = -dx;
							x = x + (rightB - leftA) + 10;
							rightSideCollision = false;
						}
						//If dx and dy are negative, moving up and left
						if(dx < 0 && dy < 0 && rightSideCollision){
							dx = -dx;
							x = x + (rightB - leftA) + 10;
							rightSideCollision = false;
						}
						//If dx and dy are both positive, moving down and right
						if(dx > 0 && dy > 0 && topSideCollision){
							dy = -dy;
							y = y - (bottomA - topB) - 10;
							topSideCollision = false;
						}
					}
					
					
				
				//if(bottomSideCollision && !topSideCollision){
					//dy = -dy;
					//if(topB != 0){y = y + (bottomB - topA);}
				//}
			////Moving Right
			//if(dx > 0 && x < CollidedTile.x && dy == 0){
				//cout << "Collision with tile! positive x velocity: " << dx << endl;
				//dx = -dx;
				//x -= 1;
			//}
			////Moving Left
			//else if(dx < 0 && x > (CollidedTile.x ) && dy == 0){
				//cout << "Collision with tile! negative x velocity: " << dx << endl;
				//dx = -dx;
				//x += 1;
			//}
			////Moving Down
			//else if(dy > 0 && (y + size.h) <= (CollidedTile.y + 20) && dx == 0){
				//cout << "Collision with tile! negative y velocity: " << dy << endl;
				//dy = -dy;
				//y -= 1;
			//}
			////Moving Up
			//else if(dy < 0 && y > (CollidedTile.y + (CollidedTile.h - 20))&& dx == 0){
				//cout << "Collision with tile! positive y velocity: " << dy << endl;
				//dy = -dy;
				//y += 1;
			//}
			//Collision if there is velocities in multiple directions
			//If moving Right and either up or down				
				
				//cout << "Collision with multiple velocities!" << endl;
				//if(x + size.w < (CollidedTile.x + 20)){
					//dx = -dx;
					//x -= 1;
					//cout << "Collision with tile! x velocity: " << dx << ", y velocity: " << dy;
				//}
				////Moving Right and Down
				//else if(dy > 0 && (y + size.h) <= (CollidedTile.y + 20)){
					//cout << "Collision with tile! x velocity: " << dx << ", y velocity: " << dy;
					//dy = -dy;
					//y -= 1;
				//}
				////Moving Right and Up
				//else if(dy < 0 && y > (CollidedTile.y + (CollidedTile.h - 20))&& dx == 0){
					//cout << "Collision with tile! x velocity: " << dx << ", y velocity: " << dy;
					//dy = -dy;
					//y += 1;
				//}
			
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
	void setX(float _x){x = _x; }
	void setY(float _y){ y = _y; }
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
