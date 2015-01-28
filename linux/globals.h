#ifndef _GLOBALS_H_
#define _GLOBALS_H_

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>

// pre-declare classes
class STexture;
class joyOverlay;
class devgruGameIcon;

//Screen dimension constants
// const int SCREEN_WIDTH = 854;
// const int SCREEN_HEIGHT = 480;
// const int SCREEN_WIDTH = 1708;
// const int SCREEN_HEIGHT = 960;
const int SCREEN_WIDTH = 1366;
const int SCREEN_HEIGHT = 768;

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 6000;

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//Globally used font
extern TTF_Font* gFont;
extern int gFontSize;
extern std::string gFontName;

//Game Controller handlers
extern SDL_Joystick* gGameController0;
extern SDL_Joystick* gGameController1;
extern SDL_Joystick* gGameController2;
extern SDL_Joystick* gGameController3;

// global joyOverlay
extern joyOverlay* jOverlay0;
extern joyOverlay* jOverlay1;
extern joyOverlay* jOverlay2;
extern joyOverlay* jOverlay3;

// global gameicon
extern devgruGameIcon* gameIcon;

// global frame count
extern int frame;

// vector of all collision boxes in scene
extern std::vector<SDL_Rect*> allCollisionBoxes;

// Function for determining collision
extern bool isCollision( SDL_Rect* a, SDL_Rect* b);

#endif /* _GLOBALS_H_ */
