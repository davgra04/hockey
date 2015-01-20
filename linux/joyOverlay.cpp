#include <SDL2/SDL_ttf.h>
// #include <SDL2/SDL2_gfx.h>
#include <SDL2/SDL2_gfxPrimitives.h>
// #include <SDL2/SDL_gfxPrimitives.h>
#include <cmath>
#include <iostream>
#include "joyOverlay.h"
#include "STexture.h"
#include "globals.h"

joyOverlay::joyOverlay(int x, int y, double scl){

	SDL_Color textColor = { 0, 80, 255, 255 };

	posX = x;
	posY = y;
	scale = scl;

	//Setup Button Members
	buttonNames[0] = "A";	//A
	buttonNames[1] = "B";	//B
	buttonNames[2] = "X";	//X
	buttonNames[3] = "Y";	//Y
	buttonNames[4] = "LB";	//LB
	buttonNames[5] = "RB";	//RB
	buttonNames[6] = "Select";	// Select
	buttonNames[7] = "Start";	// Start
	buttonNames[8] = "Center Button";	// Center Button
	buttonNames[9] = "L3";	// L3
	buttonNames[10] = "R3";	// R3
	buttonNames[11] = "D_Left";	// DL
	buttonNames[12] = "D_Right";	// DR
	buttonNames[13] = "D_Up";	// DU
	buttonNames[14] = "D_Down";	// DD
	for(int i=0; i<NUM_BUTTONS; i++){
		buttonStates[i] = false;
		dispButtons[i] = new STexture();
		dispButtons[i]->loadFromRenderedText(buttonNames[i], textColor);
	}
	
	
	activePosition = new STexture();
	activePosition->loadFromFile("assets/ControllerOverlay2/activePosition.png");
	deadzone = new STexture();
	deadzone->loadFromFile("assets/ControllerOverlay2/deadzone.png");
	fullRange = new STexture();
	fullRange->loadFromFile("assets/ControllerOverlay2/fullRange.png");
	buttonOff = new STexture();
	buttonOff->loadFromFile("assets/ControllerOverlay2/buttonOff.png");
	buttonOn = new STexture();
	buttonOn->loadFromFile("assets/ControllerOverlay2/buttonOn.png");

	dispLXAxis = new STexture();
	dispLYAxis = new STexture();
	dispLAngle = new STexture();
	dispLMagnitude = new STexture();
	dispRXAxis = new STexture();
	dispRYAxis = new STexture();
	dispRAngle = new STexture();
	dispRMagnitude = new STexture();
	
}

joyOverlay::~joyOverlay(){

	activePosition->free();
	deadzone->free();
	fullRange->free();
	buttonOff->free();
	buttonOn->free();
	dispLXAxis->free();
	dispLYAxis->free();
	dispLAngle->free();
	dispLMagnitude->free();
	dispRXAxis->free();
	dispRYAxis->free();
	dispRAngle->free();
	dispRMagnitude->free();

}

void joyOverlay::getState(){

	xAxisL = SDL_JoystickGetAxis(gGameController, 0);
	yAxisL = SDL_JoystickGetAxis(gGameController, 1);

	xAxisR = SDL_JoystickGetAxis(gGameController, 3);
	yAxisR = SDL_JoystickGetAxis(gGameController, 4);

	trigL = SDL_JoystickGetAxis(gGameController, 2);
	trigR = SDL_JoystickGetAxis(gGameController, 5);

	//buttonA = (bool) SDL_JoystickGetButton(gGameController, 0);
	//buttonB = (bool) SDL_JoystickGetButton(gGameController, 1);
	//buttonX = (bool) SDL_JoystickGetButton(gGameController, 2);
	//buttonY = (bool) SDL_JoystickGetButton(gGameController, 3);
	for(int i=0; i<NUM_BUTTONS; i++){
		buttonStates[i] = (bool) SDL_JoystickGetButton(gGameController, i);
	}

}

void joyOverlay::oldrender(){

	getState();

	SDL_Color textColor = { 0, 80, 255, 255 };

	// Render display
	axisLXText.str("");
	axisLXText << "X: " << xAxisL;
	axisLYText.str("");
	axisLYText << "Y: " << yAxisL;
	
	dispLXAxis->loadFromRenderedText(axisLXText.str().c_str(), textColor);
	dispLYAxis->loadFromRenderedText(axisLYText.str().c_str(), textColor);
	dispLMagnitude->loadFromRenderedText("assets/ControllerOverlay2/activePosition.png", textColor);

	dispLXAxis->render(posX + scale * (256.0 + 16.0), posY, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispLYAxis->render(posX + scale * (256.0 + 16.0), posY + scale * (60.0), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispLAngle->render(posX + scale * (256.0 + 16.0), posY + scale * (60.0 * 2), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);


	// Render Thumbstick L
	fullRange->render(posX, posY, NULL, 0.0, scale, NULL, SDL_FLIP_NONE);

	int activeLX = (int)(( 128.0 - 16.0 + ( xAxisL * 120.0 / 32768.0 ) ) * scale) + posX;
	int activeLY = (int)(( 128.0 - 16.0 + ( yAxisL * 120.0 / 32768.0 ) ) * scale) + posY;

	double magnitudeL = sqrt( (double)xAxisL*(double)xAxisL + (double)yAxisL*(double)yAxisL ) / 32768.0;
	double angleL = atan2( (double)yAxisL , (double)xAxisL ) * ( 180.0 / M_PI );

	//int activeX = (int)( 120.0 * magnitude * cos(angle));
	//int activeY = (int)( 120.0 * magnitude * sin(angle));

	activePosition->render(activeLX, activeLY, NULL, 0.0, scale, NULL, SDL_FLIP_NONE);

	// Render Thumbstick R
	fullRange->render(posX, posY + 100, NULL, 0.0, scale, NULL, SDL_FLIP_NONE);

	int activeRX = (int)(( 128.0 - 16.0 + ( xAxisR * 120.0 / 32768.0 ) ) * scale) + posX;
	int activeRY = (int)(( 128.0 - 16.0 + ( yAxisR * 120.0 / 32768.0 ) ) * scale) + posY;

	double magnitudeR = sqrt( (double)xAxisR*(double)xAxisR + (double)yAxisR*(double)yAxisR ) / 32768.0;
	double angleR = atan2( (double)yAxisR , (double)xAxisR ) * ( 180.0 / M_PI );

	//int activeX = (int)( 120.0 * magnitude * cos(angle));
	//int activeY = (int)( 120.0 * magnitude * sin(angle));

	activePosition->render(activeRX, activeRY, NULL, 0.0, scale, NULL, SDL_FLIP_NONE);

	// Render Buttons
	int tempButtonX, tempButtonY;

	for(int i=0; i<NUM_BUTTONS; i++){
		tempButtonX = posX;
		tempButtonY = posY + scale * (256 + (96/2)*i);
		if(buttonStates[i]){
			buttonOn->render(tempButtonX, tempButtonY, NULL, 0.0, scale/2, NULL, SDL_FLIP_NONE);
			dispButtons[i]->render(tempButtonX + 96*scale/2, tempButtonY, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
		}
		else{
			buttonOff->render(tempButtonX, tempButtonY, NULL, 0.0, scale/2, NULL, SDL_FLIP_NONE);
		}
	}

}

void joyOverlay::newrender(){

	int tempx, tempy, tempradius;

	getState();

	int activeLX = (int)(DISP_SIZE_RANGE * (xAxisL + 32768.0) / 65536.0);	// Value from 0 to DISP_SIZE_RANGE
	int activeLY = (int)(DISP_SIZE_RANGE * (yAxisL + 32768.0) / 65536.0);	// Value from 0 to DISP_SIZE_RANGE

	int activeRX = (int)(DISP_SIZE_RANGE * (xAxisR + 32768.0) / 65536.0);	// Value from 0 to DISP_SIZE_RANGE
	int activeRY = (int)(DISP_SIZE_RANGE * (yAxisR + 32768.0) / 65536.0);	// Value from 0 to DISP_SIZE_RANGE

	int activeLTrig = (int)(DISP_SIZE_RANGE * (trigL + 32768.0) / 65536.0);	// Value from 0 to DISP_SIZE_RANGE
	int activeRTrig = (int)(DISP_SIZE_RANGE * (trigR + 32768.0) / 65536.0);	// Value from 0 to DISP_SIZE_RANGE

	int oldPosX = posX;
	int oldPosY = posY;

	// posX += activeLX/4;
	// posY += activeLY/4;

	SDL_Color colorActive = { 0, 80, 255, 255 };
	SDL_Color colorIdle = { 0, 160, 255, 255 };
	// SDL_Color colorIdle = { 0, 20, 64, 255 };

	// Render L Thumbstick
	// L Range
	tempx = posX;
	tempy = posY;

	rectangleRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE, tempy + DISP_SIZE_RANGE, 
				  colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);

    // L Active Center

    tempx = posX + activeLX;
	tempy = posY + activeLY;
	tempradius = DISP_SIZE_CENTER;

    
	if(buttonStates[9]){
		aacircleRGBA(gRenderer,
	    			 tempx, tempy, 
	    			 tempradius, 
	    			 0xFF, 0x00, 0x00, 0xFF);
	}
	else{
		aacircleRGBA(gRenderer,
	    			 tempx, tempy, 
	    			 tempradius, 
	    			 colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);
	}

	// L Thumb text
	axisLXText.str("");
	axisLXText << "X: " << xAxisL;
	axisLYText.str("");
	axisLYText << "Y: " << yAxisL;
	
	dispLXAxis->loadFromRenderedText(axisLXText.str().c_str(), colorActive);
	dispLYAxis->loadFromRenderedText(axisLYText.str().c_str(), colorActive);
	dispLMagnitude->loadFromRenderedText("assets/ControllerOverlay2/activePosition.png", colorActive);

	dispLXAxis->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispLYAxis->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (16.0), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispLAngle->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (16.0 * 2), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);


	// Render R Thumbstick
	// R Range
	tempx = posX;
	tempy = posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER);

	rectangleRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE, tempy + DISP_SIZE_RANGE, 
				  colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);

    // R Active Center


	tempx = posX + activeRX;
	tempy = posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER) + activeRY;
	tempradius = DISP_SIZE_CENTER;

	if(buttonStates[10]){
		aacircleRGBA(gRenderer,
	    			 tempx, tempy, 
	    			 tempradius, 
	    			 0xFF, 0x00, 0x00, 0xFF);
	}
	else{
		aacircleRGBA(gRenderer,
	    			 tempx, tempy, 
	    			 tempradius, 
	    			 colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);
	}


	// R Thumb text
	axisRXText.str("");
	axisRXText << "X: " << xAxisR;
	axisRYText.str("");
	axisRYText << "Y: " << yAxisR;
	
	dispRXAxis->loadFromRenderedText(axisRXText.str().c_str(), colorActive);
	dispRYAxis->loadFromRenderedText(axisRYText.str().c_str(), colorActive);
	dispRMagnitude->loadFromRenderedText("assets/ControllerOverlay2/activePosition.png", colorIdle);

	dispRXAxis->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispRYAxis->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER) + scale * (16.0), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispRAngle->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER) + scale * (16.0 * 2), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);

	// Render L Trigger
	tempx = posX;
	tempy = posY + scale * 2 * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER);

	rectangleRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE, tempy + DISP_SIZE_TRIG_HEIGHT, 
				  colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);


	if(activeLTrig >= 1){
		boxRGBA(gRenderer, 
				tempx, tempy,
				tempx + activeLTrig, tempy + DISP_SIZE_TRIG_HEIGHT, 
				colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);
	}

	// Render R Trigger
	tempx = posX;
	tempy = posY + scale * 2 * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER) + scale * (DISP_SIZE_TRIG_HEIGHT + DISP_SIZE_BUFFER);

	rectangleRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE, tempy + DISP_SIZE_TRIG_HEIGHT, 
				  colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);

	if(activeRTrig >= 1){
		boxRGBA(gRenderer, 
				tempx, tempy,
				tempx + activeRTrig, tempy + DISP_SIZE_TRIG_HEIGHT, 
				colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);
	}

	// Render Buttons
	int tempButtonX, tempButtonY;
	tempx = posX;
	tempy = posY + scale * 2 * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER) + scale * 2 * (DISP_SIZE_TRIG_HEIGHT + DISP_SIZE_BUFFER);

	for(int i=0; i<NUM_BUTTONS; i++){
		tempButtonX = tempx;
		tempButtonY = tempy + scale * i * (DISP_SIZE_BUTTON + DISP_SIZE_BUFFER);
		if(buttonStates[i]){
			// buttonOn->render(tempButtonX, tempButtonY, NULL, 0.0, scale/2, NULL, SDL_FLIP_NONE);
			boxRGBA(gRenderer, 
					tempButtonX, tempButtonY,
					tempButtonX + DISP_SIZE_BUTTON, tempButtonY + DISP_SIZE_BUTTON, 
					colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);
			dispButtons[i]->render(tempButtonX + DISP_SIZE_BUTTON + DISP_SIZE_BUFFER, tempButtonY, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
		}
		else{
			// buttonOff->render(tempButtonX, tempButtonY, NULL, 0.0, scale/2, NULL, SDL_FLIP_NONE);
			rectangleRGBA(gRenderer, 
						  tempButtonX, tempButtonY,
						  tempButtonX + DISP_SIZE_BUTTON, tempButtonY + DISP_SIZE_BUTTON, 
						  colorIdle.r, colorIdle.g, colorIdle.b, colorIdle.a);
		}
	}





	posX = oldPosX;
	posY = oldPosY;

}

void joyOverlay::render(){

	// oldrender();
	newrender();
	

}


Sint16 joyOverlay::getLXAxis(){
	return xAxisL;
}

Sint16 joyOverlay::getLYAxis(){
	return yAxisL;
}


int joyOverlay::convertSDLColorToHex(SDL_Color col){
	int retcol = ((int)col.a << 24) + ((int)col.b << 16) + ((int)col.g << 8) + (int)col.r;

	std::cout << "retcol: 0x" << std::hex << retcol << std::dec << std::endl;

	return retcol;
}
