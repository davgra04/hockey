#ifndef _JOYOVERLAY_H_
#define _JOYOVERLAY_H_

#include <cmath>
#include <sstream>
#include <SDL.h>

class LTexture;

class joyOverlay {
private:
	const static int NUM_BUTTONS = 14;

	// Thumbstick
	LTexture* activePosition;
	LTexture* deadzone;
	LTexture* fullRange;

	// Buttons
	LTexture* buttonOff;
	LTexture* buttonOn;

	// Text Output
	LTexture* dispXAxis;
	LTexture* dispYAxis;
	LTexture* dispAngle;
	LTexture* dispMagnitude;
	std::stringstream xAxisText;
	std::stringstream yAxisText;
	std::stringstream angleText;
	std::stringstream magnitudeText;

	Sint16 xAxis;
	Sint16 yAxis;

	bool buttonStates[NUM_BUTTONS];
	std::string buttonNames[NUM_BUTTONS];
	LTexture* dispButtons[NUM_BUTTONS];


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
	void render();

};

#endif /* _JOYOVERLAY_H_ */

