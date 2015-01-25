#ifndef _BEANDUDE_H_
#define _BEANDUDE_H_

#include "globals.h"
#include "STexture.h"

// class STexture;

class beandude {
private:
	enum animStates {
		ANIM_IDLE,
		ANIM_WALK,
		ANIM_JUMP
	};


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
	double posX, posY;
	double scale;
	double velX, velY;
	double velMultiplier;

	bool facingLeft;
	int curAnimState;

	// Is beandude selected?
	bool isActive;

	// SDL_Joystick beandude is listening to
	SDL_Joystick* controller;

	// beandude's collision box
	SDL_Rect collision;

	// color of collision box
	SDL_Color colorBounds;

	// color of debug info text
	SDL_Color textColor;

public:
	enum beandudeColor {
		BLUE,
		GREEN,
		RED,
		PURPLE
	};

	//dimensions of beandude
	static const int BEANDUDE_WIDTH = 10;
	static const int BEANDUDE_HEIGHT = 10;

	//max velocity of beandude
	static const double BEANDUDE_VELOCITY = 3.0;

	beandude(int x=0, int y=0, double scl=0.0, bool actv=false, SDL_Joystick* ctrlr=NULL);
	~beandude();

	// Calculates and updates movement. Helper for handleEvent()
	void handleEventMovement();
	
	//Takes key presses and adjusts beandude's velocity
	void handleEvent( SDL_Event& e );

	//Checks current collision box for collision against all others
	bool checkCollision();

	//Moves beandude and might eventually check collision
	void move();

	//Renders beandude
	void render();

	//Sets or gets whether beandude listens to controller input
	void setActive(bool actv=true);
	bool getActive();

	//Loads different colored sprite into beandude object
	void setBeandudeColor(beandudeColor col);

};

#endif /* _BEANDUDE_H_ */

