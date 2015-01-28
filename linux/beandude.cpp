#include "beandude.h"

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
	// spriteSheet->loadFromFile("assets/beandude/beandude_sprite.png");
	// debugInfo = new STexture();
	for(int i=0; i<DEBUG_LINE_COUNT; ++i){
		debugInfo[i] = new STexture();
	}

	clip = new SDL_Rect();

	collision = SDL_Rect();
	collision.x = 0;
	collision.y = spriteSheet->getHeight() - 40;
	collision.w = spriteSheet->getWidth();
	collision.h = 40;

	allCollisionBoxes.push_back(&collision);

	// colorBounds = { 0, 80, 255, 255 };

	// textColor = { 0, 80, 255, 255 };

	setBeandudeColor(BLUE);

	colorBounds.r = textColor.r / 2;
	colorBounds.g = textColor.g / 2;
	colorBounds.b = textColor.b / 2;
	colorBounds.a = 255;

}

beandude::~beandude(){

	spriteSheet->free();

	for(int i=0; i<DEBUG_LINE_COUNT; ++i){
		debugInfo[i]->free();
	}

}

void beandude::handleEventMovement(){

	Sint16 x_move = SDL_JoystickGetAxis(controller, 0);
	Sint16 y_move = SDL_JoystickGetAxis(controller, 1);
	Sint16 rButtonAxis = SDL_JoystickGetAxis(controller, 5);

	double rotation = atan2( (double)y_move, (double)x_move );
	double rawMagnitude = sqrt( pow(x_move, 2) + pow(y_move, 2)) - JOYSTICK_DEAD_ZONE;

	if(rawMagnitude > JOYSTICK_DEAD_ZONE){



		double multiplierX = (x_move + 32768) / (65536.0);
		double multiplierY = (x_move + 32768) / (65536.0);
		double multiplierMag = (rawMagnitude - JOYSTICK_DEAD_ZONE) / (32768.0 - JOYSTICK_DEAD_ZONE);
		double multiplierTrig = (rButtonAxis + 32768) / (65536.0);
		double newVelocity = BEANDUDE_VELOCITY * multiplierMag;
		if(multiplierTrig > 0.25){
			newVelocity *= 1 + (multiplierTrig - 0.25)*2*4/3.0;
		}
		double currentMagnitude = sqrt(pow(velX,2)+pow(velY,2));	

		// Decay speed but update direction
		if(newVelocity < currentMagnitude){
			velX = currentMagnitude * cos(rotation);
			velY = currentMagnitude * sin(rotation);
			velX *= 0.99;
			velY *= 0.99;
		}
		// Update speed and direction
		else{	
			velX = newVelocity * cos(rotation);					
			velY = newVelocity * sin(rotation);
		}


		// double newVelocityX = newVelocity * cos(rotation);					
		// double newVelocityY = newVelocity * sin(rotation);

		
		if(velX > 0){
			facingLeft = false;
		}
		else{
			facingLeft = true;
		}

		currentMagnitude = sqrt(pow(velX,2)+pow(velY,2));
		if(currentMagnitude < 0.02){
			curAnimState = ANIM_IDLE;
		}
		else{
			curAnimState = ANIM_WALK;
		}

	}
	else{
		//decay velocity and update direction
		velX *= 0.99;
		velY *= 0.99;



		curAnimState = ANIM_IDLE;

	}

	// if(newVelocity < 0.0001){
	// 	velX = 0;
	// }
	// if(velY < 0.0001){
	// 	velY = 0;
	// }
	// Sint16 x_move = SDL_JoystickGetAxis(controller, 0);
	// Sint16 y_move = SDL_JoystickGetAxis(controller, 1);

	// //Inside of dead zone
	// if( (x_move > -JOYSTICK_DEAD_ZONE) && (y_move > -JOYSTICK_DEAD_ZONE) &&
	// 	(x_move < JOYSTICK_DEAD_ZONE) && (y_move < JOYSTICK_DEAD_ZONE) )
	// {
	// 	velX = 0;
	// 	velY = 0;
	// }
	// else
	// {
	// 	Sint16 rButtonAxis = SDL_JoystickGetAxis(controller, 5);
	// 	// double multiplier = (rButtonAxis + 32768) / (65536.0);
	// 	double multiplier = (rButtonAxis + 32768) / (65536.0);
	// 	double newVelocity = BEANDUDE_VELOCITY * (1 + multiplier*6);
	// 	double rotation = atan2( (double)y_move, (double)x_move );

	// 	velX = newVelocity * cos(rotation);					
	// 	velY = newVelocity * sin(rotation);

	// 	curAnimState = ANIM_WALK;
	// 	if(velX > 0){
	// 		facingLeft = false;
	// 	}
	// 	else{
	// 		facingLeft = true;
	// 	}
	// }


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

//Checks current collision box for collision against all others
bool beandude::checkCollision(){

	for(int i=0; i<allCollisionBoxes.size(); ++i){
		if(allCollisionBoxes[i] == &collision) continue;

		if(isCollision(&collision, allCollisionBoxes[i])){
			return true;
		}
	}

	return false;

}

//Moves beandude and might eventually check collision
void beandude::move(){

	// Sint16 xVal = jOverlay->getLXAxis();
	// Sint16 yVal = jOverlay->getLXAxis();

	handleEventMovement();

	if(isActive){

		// colorBounds = { 0, 80, 255, 255 };
		colorBounds.r = textColor.r / 2;
		colorBounds.g = textColor.g / 2;
		colorBounds.b = textColor.b / 2;
		colorBounds.a = 255;

		// Check collision against each collisionBox for X movement
		collision.x += velX;
		if(checkCollision()){
			// Move back and render collision box red
			while(checkCollision() && velX > 0){
				collision.x -= velX;
				velX--;
				collision.x += velX;
			}
			if(velX < 0) velX = 0;
			posX += velX;
			velX = 0;
			colorBounds = { 255, 0, 0, 255 };
		}
		else{
			// render collision box normally
			posX += velX;
		}

		// Check collision against each collisionBox for Y movement
		collision.y += velY;
		if(checkCollision()){
			// Move back and render collision box red
			while(checkCollision() && velY > 0){
				collision.y -= velY;
				velY--;
				collision.y += velY;
			}
			if(velY < 0) velY = 0;
			posY += velY;
			velY = 0;
			colorBounds = { 255, 0, 0, 255 };
		}
		else{
			// render collision box normally
			posY += velY;
		}




		// // Check x movement
		// if(isCollision(&collision, allCollisionBoxes[i])){
		// 	// Move back and render collision box red
		// 	while(isCollision(&collision, allCollisionBoxes[i]) && velX > 0){
		// 		collision.x -= velX;
		// 		velX--;
		// 		collision.x += velX;
		// 	}
		// 	posX += velX;
		// 	velX = 0;
		// 	colorBounds = { 255, 0, 0, 255 };
		// }
		// else{
		// 	// render collision box normally
		// 	posX += velX;
		// }

		// Check y movement
		// if(isCollision(&collision, allCollisionBoxes[i])){
		// 	// Move back and render collision box red
		// 	collision.y -= velY;
		// 	colorBounds = { 255, 0, 0, 255 };
		// }
		// if(isCollision(&collision, allCollisionBoxes[i])){
		// 	// Move back and render collision box red
		// 	while(isCollision(&collision, allCollisionBoxes[i]) && velY > 0){
		// 		collision.y -= velY;
		// 		velY--;
		// 		collision.y += velY;
		// 	}
		// 	posY += velY;
		// 	velY = 0;
		// 	colorBounds = { 255, 0, 0, 255 };
		// }
		// else{
		// 	// render collision box normally
		// 	posY += velY;
		// }

	}
	else{
		velX = 0;
		velY = 0;
		// colorBounds = { 0, 80, 255, 255 };
		colorBounds.r = textColor.r / 2;
		colorBounds.g = textColor.g / 2;
		colorBounds.b = textColor.b / 2;
		colorBounds.a = 255;
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
		// spriteSheet->render(posX - (int)posX%3, posY - (int)posY%3, clip, 0.0, scale, NULL, SDL_FLIP_NONE);
		spriteSheet->render(posX, posY, clip, 0.0, scale, NULL, SDL_FLIP_NONE);
	}
	else{
		// spriteSheet->render(posX - (int)posX%3, posY - (int)posY%3, clip, 0.0, scale, NULL, SDL_FLIP_HORIZONTAL);
		spriteSheet->render(posX, posY, clip, 0.0, scale, NULL, SDL_FLIP_HORIZONTAL);
	}
	
	// Set collision box bounds
	collision.x = posX + 2*scale;
	collision.y = posY + 6*scale;
	collision.w = 6 * scale;
	collision.h = 4 * scale;

	// Render debug info
	if(showDebugInfo){
		renderDebugInfo();

		// Render collision box

		for(int i=0; i<1; ++i){
			rectangleRGBA(gRenderer, 
						  collision.x + i, collision.y + i,
						  collision.x + collision.w - i, collision.y + collision.h - i, 
						  colorBounds.r, colorBounds.g, colorBounds.b, colorBounds.a);
		}
	}


}

//Sets whether beandude listens to controller input
void beandude::setActive(bool actv){
	isActive = actv;
}

bool beandude::getActive(){
	return isActive;
}


void beandude::setBeandudeColor(beandude::beandudeColor col){
	if(col == beandude::BLUE){
		spriteSheet->loadFromFile("assets/beandude/beandude_sprite.png");
		textColor = {0, 160, 255, 255};
	}
	else if(col == beandude::GREEN){
		spriteSheet->loadFromFile("assets/beandude/beandude_sprite_green.png");
		textColor = {109, 255, 1, 255};
	}
	else if(col == beandude::RED){
		spriteSheet->loadFromFile("assets/beandude/beandude_sprite_red.png");
		textColor = {255, 31, 1, 255};
	}
	else if(col == beandude::PURPLE){
		spriteSheet->loadFromFile("assets/beandude/beandude_sprite_purple.png");
		textColor = {103, 1, 255, 255};
	}



}

//Renders debug text about beandude
void beandude::renderDebugInfo(){
	debugText.precision(2);
	debugText.str("");
	debugText << std::fixed << "velX: " << velX;

	debugInfo[4]->loadFromRenderedText(debugText.str().c_str(), textColor);
	debugInfo[4]->render(posX + BEANDUDE_WIDTH*scale, posY - 10 - 4*(debugInfo[0]->getHeight()+1), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	debugText.str("");
	debugText << "velY: " << velY;
	debugInfo[3]->loadFromRenderedText(debugText.str().c_str(), textColor);
	debugInfo[3]->render(posX + BEANDUDE_WIDTH*scale, posY - 10 - 3*(debugInfo[0]->getHeight()+1), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	debugText.str("");
	debugText << "posX: " << (int)posX;
	debugInfo[2]->loadFromRenderedText(debugText.str().c_str(), textColor);
	debugInfo[2]->render(posX + BEANDUDE_WIDTH*scale, posY - 10 - 2*(debugInfo[0]->getHeight()+1), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	debugText.str("");
	debugText << "posY: " << (int)posY;
	debugInfo[1]->loadFromRenderedText(debugText.str().c_str(), textColor);
	debugInfo[1]->render(posX + BEANDUDE_WIDTH*scale, posY - 10 - 1*(debugInfo[0]->getHeight()+1), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	debugText.str("");
	debugText << "animState:" << curAnimState;
	debugInfo[0]->loadFromRenderedText(debugText.str().c_str(), textColor);
	debugInfo[0]->render(posX + BEANDUDE_WIDTH*scale, posY - 10, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);

}

//Sets whether debug info should be displayed or not
void beandude::setDebugInfoVisible(bool visible){
	showDebugInfo = visible;
}

