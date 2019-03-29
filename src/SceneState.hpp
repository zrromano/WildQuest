#pragma once

using namespace std;

typedef enum {
		TitleScreen, 	//Starting Title Sceen
		Running, 		//Gameplay
		Inventory,		//Inventory Screen
		Pause,			//Pause Menu
		DeathScreen,	//Death Screen
		NPCDialog 		//Dialog with interacting with NPC
} SceneType;

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
