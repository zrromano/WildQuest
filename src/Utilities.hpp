#pragma once

using namespace std;

bool CollisionCheck(SDL_Rect a, SDL_Rect b)
{
    //rectange sides
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    leftA = a.x;
    rightA = a.x + a.w;
    topA = a.y;
    bottomA = a.y + a.h;

    leftB = b.x;
    rightB = b.x + b.w;
    topB = b.y;
    bottomB = b.y + b.h;

    //If any of the sides from A are outside of B
    if( bottomA <= topB ){ 
		//cout << "BottomA: " << bottomA << " is less than TopB: " << topB << endl;
		return false;
		}
    if( topA >= bottomB ){ 
		//cout << "TopA: " << topA << " is greater than BottomB: " << bottomB << endl;
		return false;
		}
    if( rightA <= leftB ){ 
		//cout << "rightA: " << rightA << " is less than leftB: " << leftB << endl;
		return false;
		}
    if( leftA >= rightB ){ 
		//cout << "leftA: " << leftA << " is greater than rightB: " << rightB << endl;
		return false;
		}

    //If none of the sides from A are outside B
    return true;
}
