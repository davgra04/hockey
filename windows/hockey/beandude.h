#ifndef _BEANDUDE_H_
#define _BEANDUDE_H_

#include <cmath>
#include <sstream>
#include <SDL.h>

class STexture;

class beandude {
private:
	static const enum animStates {
		ANIM_IDLE,
		ANIM_WALK,
		ANIM_JUMP
	};

	static const int NUM_BUTTONS = 14;

	STexture* spriteSheet;
	STexture* debugInfo;
	std::stringstream debugText;
	SDL_Rect* clip;
	int animFrame;

	// Animations
	static const int ANIM_WALKFRAMES = 4;
	static const int ANIM_IDLEFRAMES = 2;
	int anim_walkCycle[ANIM_WALKFRAMES];
	int anim_idleCycle[ANIM_IDLEFRAMES];

	// Display Position
	int posX, posY;
	double scale;
	int velX, velY;

	bool facingLeft;
	int curAnimState;

	// beandude's collision box
	SDL_Rect mCollider;

public:

	//dimensions of beandude
	static const int BEANDUDE_WIDTH = 10;
	static const int BEANDUDE_HEIGHT = 10;

	//max velocity of beandude
	static const int BEANDUDE_VELOCITY = 1;

	beandude(int x=0, int y=0, double scl=0.0);
	~beandude();
	
	//Takes key presses and adjusts beandude's velocity
	void handleEvent( SDL_Event& e );

	//Moves beandude and might eventually check collision
	void move();

	//Renders beandude
	void render();

};

#endif /* _BEANDUDE_H_ */

