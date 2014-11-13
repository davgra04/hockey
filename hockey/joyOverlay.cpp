#include <SDL_ttf.h>
#include <cmath>
#include "joyOverlay.h"
#include "LTexture.h"
#include "globals.h"

joyOverlay::joyOverlay(int x, int y, double scl){

	SDL_Color textColor = { 0, 80, 255, 255 };

	posX = x;
	posY = y;
	scale = scl;

	//Setup Button Members
	buttonNames[0] = "D_up";
	buttonNames[1] = "D_down";
	buttonNames[2] = "D_left";
	buttonNames[3] = "D_Right";
	buttonNames[4] = "Start";
	buttonNames[5] = "Select";
	buttonNames[6] = "L3";
	buttonNames[7] = "R3";
	buttonNames[8] = "LB";
	buttonNames[9] = "RB";
	buttonNames[10] = "A";
	buttonNames[11] = "B";
	buttonNames[12] = "X";
	buttonNames[13] = "Y";
	for(int i=0; i<NUM_BUTTONS; i++){
		buttonStates[i] = false;
		dispButtons[i] = new LTexture();
		dispButtons[i]->loadFromRenderedText(buttonNames[i], textColor);
	}
	
	
	activePosition = new LTexture();
	activePosition->loadFromFile("assets/ControllerOverlay2/activePosition.png");
	deadzone = new LTexture();
	deadzone->loadFromFile("assets/ControllerOverlay2/deadzone.png");
	fullRange = new LTexture();
	fullRange->loadFromFile("assets/ControllerOverlay2/fullRange.png");
	buttonOff = new LTexture();
	buttonOff->loadFromFile("assets/ControllerOverlay2/buttonOff.png");
	buttonOn = new LTexture();
	buttonOn->loadFromFile("assets/ControllerOverlay2/buttonOn.png");

	dispXAxis = new LTexture();
	dispYAxis = new LTexture();
	dispAngle = new LTexture();
	dispMagnitude = new LTexture();
	
}

joyOverlay::~joyOverlay(){

	activePosition->free();
	deadzone->free();
	fullRange->free();
	buttonOff->free();
	buttonOn->free();
	dispXAxis->free();
	dispYAxis->free();
	dispAngle->free();
	dispMagnitude->free();

}

void joyOverlay::getState(){

	xAxis = SDL_JoystickGetAxis(gGameController, 0);
	yAxis = SDL_JoystickGetAxis(gGameController, 1);
	//buttonA = (bool) SDL_JoystickGetButton(gGameController, 0);
	//buttonB = (bool) SDL_JoystickGetButton(gGameController, 1);
	//buttonX = (bool) SDL_JoystickGetButton(gGameController, 2);
	//buttonY = (bool) SDL_JoystickGetButton(gGameController, 3);
	for(int i=0; i<NUM_BUTTONS; i++){
		buttonStates[i] = (bool) SDL_JoystickGetButton(gGameController, i);
	}

}

void joyOverlay::render(){

	getState();

	SDL_Color textColor = { 0, 80, 255, 255 };

	// Render display
	xAxisText.str("");
	xAxisText << "X: " << xAxis;
	yAxisText.str("");
	yAxisText << "Y: " << yAxis;
	
	dispXAxis->loadFromRenderedText(xAxisText.str().c_str(), textColor);
	dispYAxis->loadFromRenderedText(yAxisText.str().c_str(), textColor);
	dispMagnitude->loadFromRenderedText("assets\ControllerOverlay2\activePosition.png", textColor);

	dispXAxis->render(posX + scale * (256.0 + 16.0), posY, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispYAxis->render(posX + scale * (256.0 + 16.0), posY + scale * (60.0), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispAngle->render(posX + scale * (256.0 + 16.0), posY + scale * (60.0 * 2), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);


	// Render Thumbstick
	fullRange->render(posX, posY, NULL, 0.0, scale, NULL, SDL_FLIP_NONE);

	int activeX = (int)(( 128.0 - 16.0 + ( xAxis * 120.0 / 32768.0 ) ) * scale) + posX;
	int activeY = (int)(( 128.0 - 16.0 + ( yAxis * 120.0 / 32768.0 ) ) * scale) + posY;

	double magnitude = sqrt( (double)xAxis*(double)xAxis + (double)yAxis*(double)yAxis ) / 32768.0;
	double angle = atan2( (double)yAxis , (double)xAxis ) * ( 180.0 / M_PI );

	//int activeX = (int)( 120.0 * magnitude * cos(angle));
	//int activeY = (int)( 120.0 * magnitude * sin(angle));

	activePosition->render(activeX, activeY, NULL, 0.0, scale, NULL, SDL_FLIP_NONE);

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