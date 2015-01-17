#include "joyArrow.h"
#include "STexture.h"
#include <string>

joyArrow::joyArrow(){
	tex = new STexture();
	tex->loadFromFile("arrow.png");
}

joyArrow::~joyArrow(){
	tex->free();
}

void joyArrow::setAngle(double angl){
	angle = angl;
}

void joyArrow::render(int x, int y){
	tex->render(x, y, NULL, angle, 1.0, NULL, SDL_FLIP_NONE);

	/*for(int i=0; i<16; i++){
		for(int j=0; j<9; j++){
			tex->render(x + 50 * i, y + 50 * j, NULL, angle, NULL, SDL_FLIP_NONE);
		}
	}*/


}