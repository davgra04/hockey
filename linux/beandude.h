#ifndef _BEANDUDE_H_
#define _BEANDUDE_H_

#include <cmath>
#include <sstream>
#include <SDL2/SDL.h>

class STexture;

class beandude {
private:
	enum animStates {
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

	// Is beandude selected?
	bool isActive;

	// beandude's collision box
	SDL_Rect mCollider;

public:

	//dimensions of beandude
	static const int BEANDUDE_WIDTH = 10;
	static const int BEANDUDE_HEIGHT = 10;

	//max velocity of beandude
	static const int BEANDUDE_VELOCITY = 1;

	beandude(int x=0, int y=0, double scl=0.0, bool actv=false);
	~beandude();
	
	//Takes key presses and adjusts beandude's velocity
	void handleEvent( SDL_Event& e );

	//Moves beandude and might eventually check collision
	void move();

	//Renders beandude
	void render();

	//Sets or gets whether beandude listens to controller input
	void setActive(bool actv=true);
	bool getActive();

};

#endif /* _BEANDUDE_H_ */

