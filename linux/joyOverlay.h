#ifndef _JOYOVERLAY_H_
#define _JOYOVERLAY_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <SDL2/SDL_ttf.h>
#include <cmath>
#include <sstream>
#include <iostream>
#include "globals.h"
#include "STexture.h"

class joyOverlay {
private:
	const static int NUM_BUTTONS = 15;

	const static int DISP_SIZE_RANGE = 40;
	const static int DISP_SIZE_CENTER = 2;
	const static int DISP_SIZE_BUFFER = 3;
	const static int DISP_SIZE_TRIG_HEIGHT = 8;
	const static int DISP_SIZE_BUTTON = 12;

	// Texture text and string streams for L Thumbstick Info
	STexture* dispLXAxis;
	STexture* dispLYAxis;
	STexture* dispLAngle;
	std::stringstream axisLXText;
	std::stringstream axisLYText;
	std::stringstream angleLText;
	// Texture text and string streams for R Thumbstick Info
	STexture* dispRXAxis;
	STexture* dispRYAxis;
	STexture* dispRAngle;
	std::stringstream axisRXText;
	std::stringstream axisRYText;
	std::stringstream angleRText;

	// Raw values for joy axes
	Sint16 xAxisL;
	Sint16 yAxisL;
	Sint16 xAxisR;
	Sint16 yAxisR;
	Sint16 trigL;
	Sint16 trigR;

	bool buttonStates[NUM_BUTTONS];
	std::string buttonNames[NUM_BUTTONS];
	STexture* dispButtons[NUM_BUTTONS];

	bool buttonA;
	bool buttonB;
	bool buttonX;
	bool buttonY;
	bool buttonStart;
	bool buttonSelect;
	bool buttonL;
	bool buttonR;

	SDL_Color uiMainColor;
	SDL_Color uiBkgColor;

	// Display Position
	int posX;
	int posY;
	double scale;

	// SDL_Joystick overlay is listening to
	SDL_Joystick* controller;

public:

	joyOverlay(int x=0, int y=0, SDL_Joystick* ctrlr=NULL, double scl=0.0, SDL_Color mainColor={0, 160, 255, 255}, SDL_Color bkgColor={0, 0, 0, 32});
	~joyOverlay();

	void updateControllerState();

	void newrender();
	void render();

	Sint16 getLXAxis();
	Sint16 getLYAxis();

	int convertSDLColorToHex(SDL_Color col);

};

#endif /* _JOYOVERLAY_H_ */

