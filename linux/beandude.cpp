#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "beandude.h"
#include "STexture.h"
#include "globals.h"

beandude::beandude(int x, int y, double scl, bool actv){

	posX = x;
	posY = y;
	velX = 0;
	velY = 0;
	scale = scl;
	facingLeft = false;
	isActive = actv;

	curAnimState = ANIM_IDLE;

	anim_walkCycle[0] = 1;
	anim_walkCycle[1] = 2;
	anim_walkCycle[2] = 1;
	anim_walkCycle[3] = 3;

	anim_idleCycle[0] = 0;
	anim_idleCycle[1] = 1;

	animFrame = 0;
	spriteSheet = new STexture();
	spriteSheet->loadFromFile("assets/beandude/beandude_sprite.png");
	debugInfo = new STexture();

	clip = new SDL_Rect();

}

beandude::~beandude(){

	spriteSheet->free();

}
	
//Takes key presses and adjusts beandude's velocity
void beandude::handleEvent( SDL_Event& e ){

	if(isActive){
		if( e.type == SDL_JOYAXISMOTION )
		{
			//Motion on controller 0
			if( e.jaxis.which == 0 )
			{	
				Sint16 rButtonAxis = SDL_JoystickGetAxis(gGameController, 5);
				double multiplier = (rButtonAxis + 32768) / (65536.0);
				std::cout << "value: " << rButtonAxis << std::endl;
				std::cout << "multiplier: " << multiplier << std::endl;
				int newVelocity = BEANDUDE_VELOCITY * (1 + multiplier*9);
								
				//X axis motion
				if( e.jaxis.axis == 0 )
				{
					//Left of dead zone
					if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
					{
						// velX = -BEANDUDE_VELOCITY;
						velX = -newVelocity;
						curAnimState = ANIM_WALK;
						facingLeft = true;
					}
					//Right of dead zone
					else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
					{
						// velX = BEANDUDE_VELOCITY;
						velX = newVelocity;
						curAnimState = ANIM_WALK;
						facingLeft = false;
					}
					else
					{
						velX = 0;
					}
				}
				//Y axis motion
				else if( e.jaxis.axis == 1 )
				{
					//Below of dead zone
					if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
					{
						// velY = -BEANDUDE_VELOCITY;
						velY = -newVelocity;
						curAnimState = ANIM_WALK;
					}
					//Above of dead zone
					else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
					{
						// velY = BEANDUDE_VELOCITY;
						velY = newVelocity;
						curAnimState = ANIM_WALK;
					}
					else
					{
						velY = 0;
					}
				}
			}
		}
	}

}

//Moves beandude and might eventually check collision
void beandude::move(){

	// Sint16 xVal = jOverlay->getLXAxis();
	// Sint16 yVal = jOverlay->getLXAxis();

	if(isActive){	
		posX += velX;
		posY += velY;
	}
	else{
		velX = 0;
		velY = 0;
	}
}

//Renders beandude
void beandude::render(){

	if( (velX == 0 && velY == 0) || !isActive ){
		curAnimState = ANIM_IDLE;
	}

	// Set sprite clip
	if( curAnimState == ANIM_WALK ){
		clip->x = 0;
		clip->y = anim_walkCycle[frame/4 % ANIM_WALKFRAMES] * 30;
		clip->w = 30;
		clip->h = 30;
	}
	else{
		clip->x = 0;
		clip->y = anim_idleCycle[frame/16 % ANIM_IDLEFRAMES] * 30;
		clip->w = 30;
		clip->h = 30;
	}
	
	// Render
	if(facingLeft){
		spriteSheet->render(posX, posY, clip, 0.0, scale, NULL, SDL_FLIP_NONE);
	}
	else{
		spriteSheet->render(posX, posY, clip, 0.0, scale, NULL, SDL_FLIP_HORIZONTAL);
	}
	

	SDL_Color textColor = { 0, 80, 255, 255 };
	debugText.str("");
	debugText << "(" << posX << "," << posY << ") animState:" << curAnimState;
	debugInfo->loadFromRenderedText(debugText.str().c_str(), textColor);
	debugInfo->render(posX + 30, posY - 10, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);

}

//Sets whether beandude listens to controller input
void beandude::setActive(bool actv){
	isActive = actv;
}

bool beandude::getActive(){
	return isActive;
}

