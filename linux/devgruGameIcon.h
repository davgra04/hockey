#ifndef _DEVGRUGAMEICON_H_
#define _DEVGRUGAMEICON_H_

#include <cmath>
#include <SDL2/SDL.h>

class STexture;

class devgruGameIcon {
private:

	int PAUSE_LIMIT;
	int FRAME_LIMIT;

	STexture* spriteSheet;
	SDL_Rect* clip;
	int animFrame;
	int pauseFrame;
	int frameStarted;
	bool pause;

	// Display Info
	int posX;
	int posY;
	double scale;

public:

	devgruGameIcon(int x=0, int y=0, double scl=0.0);
	~devgruGameIcon();

	void render(int frame, double h, double s, double v);

};

#endif /* _DEVGRUGAMEICON_H_ */

