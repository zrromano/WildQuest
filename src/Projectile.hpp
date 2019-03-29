#pragma once

#include "Sprite.hpp"

using namespace std;

class Projectile: public Sprite{
	int damage;
	public:
	Projectile(Game *g, string filename, float _x=0.0, float _y=0.0, float _dx=0.0, float _dy=0.0, int _damage=0, int count=1, int fps=1, float spriteX=0.0, float spriteY=0.0):
	  Sprite(g, filename, count, fps, _x, _y, _dx, _dy, spriteX, spriteY){
		damage = _damage; 
	}	
};
