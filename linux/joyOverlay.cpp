#include "joyOverlay.h"

joyOverlay::joyOverlay(int x, int y, double scl, SDL_Color mainColor, SDL_Color bkgColor){

	// SDL_Color mainColor = { 0, 80, 255, 255 };
	uiMainColor = mainColor;
	uiBkgColor = bkgColor;

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
		dispButtons[i]->loadFromRenderedText(buttonNames[i], mainColor);
	}

	// Text textures for L Thumbstick info
	dispLXAxis = new STexture();
	dispLYAxis = new STexture();
	dispLAngle = new STexture();

	// Text textures for R Thumbstick info
	dispRXAxis = new STexture();
	dispRYAxis = new STexture();
	dispRAngle = new STexture();

}

joyOverlay::~joyOverlay(){

	delete dispLXAxis;
	delete dispLYAxis;
	delete dispLAngle;
	delete dispRXAxis;
	delete dispRYAxis;
	delete dispRAngle;

	// delete [] dispButtons;

	for(int i=0; i<NUM_BUTTONS; i++){
		delete dispButtons[i];
	}

	// dispLXAxis->free();
	// dispLYAxis->free();
	// dispLAngle->free();
	// dispRXAxis->free();
	// dispRYAxis->free();
	// dispRAngle->free();

}

void joyOverlay::getState(){

	xAxisL = SDL_JoystickGetAxis(gGameController, 0);
	yAxisL = SDL_JoystickGetAxis(gGameController, 1);

	xAxisR = SDL_JoystickGetAxis(gGameController, 3);
	yAxisR = SDL_JoystickGetAxis(gGameController, 4);

	trigL = SDL_JoystickGetAxis(gGameController, 2);
	trigR = SDL_JoystickGetAxis(gGameController, 5);

	for(int i=0; i<NUM_BUTTONS; i++){
		buttonStates[i] = (bool) SDL_JoystickGetButton(gGameController, i);
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

	SDL_Color mainColor = { 0, 160, 255, 255 };
	// SDL_Color mainColor = { 0, 20, 64, 255 };

	// Render L Thumbstick
	// L Range
	tempx = posX;
	tempy = posY;

	boxRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE*scale, tempy + DISP_SIZE_RANGE*scale, 
				  uiBkgColor.r, uiBkgColor.g, uiBkgColor.b, uiBkgColor.a);

	rectangleRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE*scale, tempy + DISP_SIZE_RANGE*scale, 
				  mainColor.r, mainColor.g, mainColor.b, mainColor.a);

    // L Active Center
    tempx = posX + activeLX*scale;
	tempy = posY + activeLY*scale;
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
	    			 mainColor.r, mainColor.g, mainColor.b, mainColor.a);
	}

	// L Thumb text
	axisLXText.str("");
	axisLXText << "X: " << xAxisL;
	axisLYText.str("");
	axisLYText << "Y: " << yAxisL;
	
	dispLXAxis->loadFromRenderedText(axisLXText.str().c_str(), mainColor);
	dispLYAxis->loadFromRenderedText(axisLYText.str().c_str(), mainColor);

	dispLXAxis->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispLYAxis->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (16.0), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispLAngle->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (16.0 * 2), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);


	// Render R Thumbstick
	// R Range
	tempx = posX;
	tempy = posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER);

	boxRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE*scale, tempy + DISP_SIZE_RANGE*scale, 
				  uiBkgColor.r, uiBkgColor.g, uiBkgColor.b, uiBkgColor.a);

	rectangleRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE*scale, tempy + DISP_SIZE_RANGE*scale, 
				  mainColor.r, mainColor.g, mainColor.b, mainColor.a);

    // R Active Center
	tempx = posX + activeRX*scale;
	tempy = posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER + activeRY);
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
	    			 mainColor.r, mainColor.g, mainColor.b, mainColor.a);
	}


	// R Thumb text
	axisRXText.str("");
	axisRXText << "X: " << xAxisR;
	axisRYText.str("");
	axisRYText << "Y: " << yAxisR;
	
	dispRXAxis->loadFromRenderedText(axisRXText.str().c_str(), mainColor);
	dispRYAxis->loadFromRenderedText(axisRYText.str().c_str(), mainColor);

	dispRXAxis->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispRYAxis->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER) + scale * (16.0), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
	dispRAngle->render(posX + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER), posY + scale * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER) + scale * (16.0 * 2), NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);

	// Render L Trigger
	tempx = posX;
	tempy = posY + scale * 2 * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER);

	boxRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE*scale, tempy + DISP_SIZE_TRIG_HEIGHT*scale, 
				  uiBkgColor.r, uiBkgColor.g, uiBkgColor.b, uiBkgColor.a);

	rectangleRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE*scale, tempy + DISP_SIZE_TRIG_HEIGHT*scale, 
				  mainColor.r, mainColor.g, mainColor.b, mainColor.a);


	if(activeLTrig >= 1){
		boxRGBA(gRenderer, 
				tempx, tempy,
				tempx + activeLTrig*scale, tempy + DISP_SIZE_TRIG_HEIGHT*scale, 
				mainColor.r, mainColor.g, mainColor.b, mainColor.a);
	}

	// Render R Trigger
	tempx = posX;
	tempy = posY + scale * 2 * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER) + scale * (DISP_SIZE_TRIG_HEIGHT + DISP_SIZE_BUFFER);
	
	boxRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE*scale, tempy + DISP_SIZE_TRIG_HEIGHT*scale, 
				  uiBkgColor.r, uiBkgColor.g, uiBkgColor.b, uiBkgColor.a);

	rectangleRGBA(gRenderer, 
				  tempx, tempy,
				  tempx + DISP_SIZE_RANGE*scale, tempy + DISP_SIZE_TRIG_HEIGHT*scale, 
				  mainColor.r, mainColor.g, mainColor.b, mainColor.a);

	if(activeRTrig >= 1){
		boxRGBA(gRenderer, 
				tempx, tempy,
				tempx + activeRTrig*scale, tempy + DISP_SIZE_TRIG_HEIGHT*scale, 
				mainColor.r, mainColor.g, mainColor.b, mainColor.a);
	}

	// Render Buttons
	int tempButtonX, tempButtonY;
	tempx = posX;
	tempy = posY + scale * 2 * (DISP_SIZE_RANGE + DISP_SIZE_BUFFER) + scale * 2 * (DISP_SIZE_TRIG_HEIGHT + DISP_SIZE_BUFFER);

	for(int i=0; i<NUM_BUTTONS; i++){
		tempButtonX = tempx;
		tempButtonY = tempy + scale * i * (DISP_SIZE_BUTTON + DISP_SIZE_BUFFER);
		if(buttonStates[i]){
			boxRGBA(gRenderer, 
					tempButtonX, tempButtonY,
					tempButtonX + DISP_SIZE_BUTTON*scale, tempButtonY + DISP_SIZE_BUTTON*scale, 
					mainColor.r, mainColor.g, mainColor.b, mainColor.a);
			dispButtons[i]->render(tempButtonX + (DISP_SIZE_BUTTON + DISP_SIZE_BUFFER)*scale, tempButtonY, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);
		}
		else{
			boxRGBA(gRenderer, 
						  tempButtonX, tempButtonY,
						  tempButtonX + DISP_SIZE_BUTTON*scale, tempButtonY + DISP_SIZE_BUTTON*scale, 
						  uiBkgColor.r, uiBkgColor.g, uiBkgColor.b, uiBkgColor.a);
			rectangleRGBA(gRenderer, 
						  tempButtonX, tempButtonY,
						  tempButtonX + DISP_SIZE_BUTTON*scale, tempButtonY + DISP_SIZE_BUTTON*scale, 
						  mainColor.r, mainColor.g, mainColor.b, mainColor.a);
		}
	}

	posX = oldPosX;
	posY = oldPosY;

}

void joyOverlay::render(){

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
