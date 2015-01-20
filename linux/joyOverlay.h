#ifndef _JOYOVERLAY_H_
#define _JOYOVERLAY_H_

#include <cmath>
#include <sstream>
#include <SDL2/SDL.h>

class STexture;

class joyOverlay {
private:
	const static int NUM_BUTTONS = 15;

	const static int DISP_SIZE_RANGE = 40;
	const static int DISP_SIZE_CENTER = 2;
	const static int DISP_SIZE_BUFFER = 3;
	const static int DISP_SIZE_TRIG_HEIGHT = 8;
	const static int DISP_SIZE_BUTTON = 12;

	// Thumbstick
	STexture* activePosition;
	STexture* deadzone;
	STexture* fullRange;

	// Buttons
	STexture* buttonOff;
	STexture* buttonOn;

	// Text Output
	STexture* dispLXAxis;
	STexture* dispLYAxis;
	STexture* dispLAngle;
	STexture* dispLMagnitude;
	std::stringstream axisLXText;
	std::stringstream axisLYText;
	std::stringstream angleLText;
	std::stringstream magnitudeLText;
	STexture* dispRXAxis;
	STexture* dispRYAxis;
	STexture* dispRAngle;
	STexture* dispRMagnitude;
	std::stringstream axisRXText;
	std::stringstream axisRYText;
	std::stringstream angleRText;
	std::stringstream magnitudeRText;

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

	// Display Position
	int posX;
	int posY;
	double scale;

public:

	joyOverlay(int x=0, int y=0, double scl=0.0);
	~joyOverlay();

	void getState();

	void oldrender();
	void newrender();
	void render();

	Sint16 getLXAxis();
	Sint16 getLYAxis();

	int convertSDLColorToHex(SDL_Color col);

};

#endif /* _JOYOVERLAY_H_ */

