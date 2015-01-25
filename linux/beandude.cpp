#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <iostream>
#include <cmath>
#include "beandude.h"
#include "STexture.h"
#include "globals.h"

beandude::beandude(int x, int y, double scl, bool actv, SDL_Joystick* ctrlr){

	posX = x;
	posY = y;
	velX = 0;
	velY = 0;
	velMultiplier = 1;
	scale = scl;
	facingLeft = false;
	isActive = actv;

	controller = ctrlr;

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

	collision = SDL_Rect();
	collision.x = 0;
	collision.y = spriteSheet->getHeight() - 40;
	collision.w = spriteSheet->getWidth();
	collision.h = 40;

	allCollisionBoxes.push_back(&collision);

	colorBounds = { 0, 80, 255, 255 };

}

beandude::~beandude(){

	spriteSheet->free();

}

void beandude::handleEventMovement(){

	Sint16 x_move = SDL_JoystickGetAxis(controller, 0);
	Sint16 y_move = SDL_JoystickGetAxis(controller, 1);

	//Inside of dead zone
	if( (x_move > -JOYSTICK_DEAD_ZONE) && (y_move > -JOYSTICK_DEAD_ZONE) &&
		(x_move < JOYSTICK_DEAD_ZONE) && (y_move < JOYSTICK_DEAD_ZONE) )
	{
		velX = 0;
		velY = 0;
	}
	else
	{
		Sint16 rButtonAxis = SDL_JoystickGetAxis(controller, 5);
		double multiplier = (rButtonAxis + 32768) / (65536.0);
		int newVelocity = BEANDUDE_VELOCITY * (1 + multiplier*12);
		double rotation = atan2( (double)y_move, (double)x_move );

		velX = newVelocity * cos(rotation);					
		velY = newVelocity * sin(rotation);

		curAnimState = ANIM_WALK;
		if(velX > 0){
			facingLeft = false;
		}
		else{
			facingLeft = true;
		}
	}


}
	
//Takes key presses and adjusts beandude's velocity
void beandude::handleEvent( SDL_Event& e ){

	if(isActive){
		if( e.type == SDL_JOYAXISMOTION )
		{
			//Motion on controller 0
			// if( e.jaxis.which == controller )
			// {
				//X + Y axis motion
				if( e.jaxis.axis == 0 || e.jaxis.axis == 1 || e.jaxis.axis == 5)
				{
					handleEventMovement();
				}
				// std::cout << "new velocity: (" << velX << ", " << velY << ")" << std::endl;

				// Old, 8 direction style			
				//X axis motion
				// if( e.jaxis.axis == 0 )
				// {
				// 	//Left of dead zone
				// 	if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
				// 	{
				// 		// velX = -BEANDUDE_VELOCITY;
				// 		velX = -newVelocity;
				// 		curAnimState = ANIM_WALK;
				// 		facingLeft = true;
				// 	}
				// 	//Right of dead zone
				// 	else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
				// 	{
				// 		// velX = BEANDUDE_VELOCITY;
				// 		velX = newVelocity;
				// 		curAnimState = ANIM_WALK;
				// 		facingLeft = false;
				// 	}
				// 	else
				// 	{
				// 		velX = 0;
				// 	}
				// }
				// //Y axis motion
				// else if( e.jaxis.axis == 1 )
				// {
				// 	//Below of dead zone
				// 	if( e.jaxis.value < -JOYSTICK_DEAD_ZONE )
				// 	{
				// 		// velY = -BEANDUDE_VELOCITY;
				// 		velY = -newVelocity;
				// 		curAnimState = ANIM_WALK;
				// 	}
				// 	//Above of dead zone
				// 	else if( e.jaxis.value > JOYSTICK_DEAD_ZONE )
				// 	{
				// 		// velY = BEANDUDE_VELOCITY;
				// 		velY = newVelocity;
				// 		curAnimState = ANIM_WALK;
				// 	}
				// 	else
				// 	{
				// 		velY = 0;
				// 	}
				// }


			// }
		}
	}

}

//Moves beandude and might eventually check collision
void beandude::move(){

	// Sint16 xVal = jOverlay->getLXAxis();
	// Sint16 yVal = jOverlay->getLXAxis();

	handleEventMovement();

	if(isActive){

		collision.x += velX;
		collision.y += velY;
		colorBounds = { 0, 80, 255, 255 };

		// Check collision against each collisionBox
		for(int i=0; i<allCollisionBoxes.size(); ++i){
			if(allCollisionBoxes[i] == &collision) continue;

			// Check x movement
			if(isCollision(&collision, allCollisionBoxes[i])){
				// Move back and render collision box red
				while(isCollision(&collision, allCollisionBoxes[i]) && velX > 0){
					collision.x -= velX;
					velX--;
					collision.x += velX;
				}
				posX += velX;
				velX = 0;
				colorBounds = { 255, 0, 0, 255 };
			}
			else{
				// render collision box normally
				posX += velX;
			}

			// Check y movement
			if(isCollision(&collision, allCollisionBoxes[i])){
				// Move back and render collision box red
				collision.y -= velY;
				colorBounds = { 255, 0, 0, 255 };
			}
			else{
				// render collision box normally
				posY += velY;
			}
		}

	}
	else{
		velX = 0;
		velY = 0;
		colorBounds = { 0, 80, 255, 255 };
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
		clip->y = anim_walkCycle[frame/4 % ANIM_WALKFRAMES] * BEANDUDE_HEIGHT;
		clip->w = BEANDUDE_WIDTH;
		clip->h = BEANDUDE_HEIGHT;
	}
	else{
		clip->x = 0;
		clip->y = anim_idleCycle[frame/16 % ANIM_IDLEFRAMES] * BEANDUDE_HEIGHT;
		clip->w = BEANDUDE_WIDTH;
		clip->h = BEANDUDE_HEIGHT;
	}
	
	// Render
	if(facingLeft){
		spriteSheet->render(posX, posY, clip, 0.0, scale, NULL, SDL_FLIP_NONE);
	}
	else{
		spriteSheet->render(posX, posY, clip, 0.0, scale, NULL, SDL_FLIP_HORIZONTAL);
	}
	
	// Render debug info
	SDL_Color textColor = { 0, 80, 255, 255 };
	debugText.str("");
	debugText << "(" << posX << "," << posY << ") animState:" << curAnimState;
	debugInfo->loadFromRenderedText(debugText.str().c_str(), textColor);
	debugInfo->render(posX + BEANDUDE_WIDTH*scale, posY - 10, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);

	// Render collision box
	collision.x = posX + 2*scale;
	collision.y = posY + 7*scale;
	collision.w = 6 * scale;
	collision.h = 3 * scale;

	rectangleRGBA(gRenderer, 
				  collision.x, collision.y,
				  collision.x + collision.w, collision.y + collision.h, 
				  colorBounds.r, colorBounds.g, colorBounds.b, colorBounds.a);

}

//Sets whether beandude listens to controller input
void beandude::setActive(bool actv){
	isActive = actv;
}

bool beandude::getActive(){
	return isActive;
}

