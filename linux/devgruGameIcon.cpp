#include <SDL2/SDL_ttf.h>
#include <cmath>
#include "devgruGameIcon.h"
#include "STexture.h"
#include "globals.h"

devgruGameIcon::devgruGameIcon(int x, int y, double scl){

	posX = x;
	posY = y;
	scale = scl;

	pauseFrame = 0;
	animFrame = 0;
	frameStarted = 0;
	pause = true;

	PAUSE_LIMIT = 600;
	FRAME_LIMIT = 24;

	
	spriteSheet = new STexture();
	spriteSheet->loadFromFile("assets/GameSplash/devgruIconSheen.png");
	spriteSheet->setBlendMode( SDL_BLENDMODE_NONE );
	
	clip = new SDL_Rect();
}

devgruGameIcon::~devgruGameIcon(){

	spriteSheet->free();

}

void devgruGameIcon::render(int frame, double h, double s, double v){

	if(pause){
		++pauseFrame;

		if(pauseFrame == PAUSE_LIMIT){
			pause = false;
			pauseFrame = 0;
			frameStarted = frame;
		}

	}
	else{
		animFrame = (frame-frameStarted)/2 % 24;

		if(animFrame == FRAME_LIMIT-1){
			pause = true;
			animFrame = 0;
		}
	}


	clip->x = 0;
	clip->y = 42 * animFrame;
	clip->w = 375;
	clip->h = 42;


	//SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" );
	//spriteSheet->setColorHSV(h, s, v);
	spriteSheet->render(posX, posY, clip, 0.0, scale, NULL, SDL_FLIP_NONE);
	//SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );
}

