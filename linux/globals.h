#ifndef _GLOBALS_H_
#define _GLOBALS_H_

// pre-declare classes
class STexture;
class joyArrow;
class joyOverlay;
class devgruGameIcon;

//Screen dimension constants
// const int SCREEN_WIDTH = 854;
// const int SCREEN_HEIGHT = 480;
const int SCREEN_WIDTH = 1708;
const int SCREEN_HEIGHT = 960;

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

//The window we'll be rendering to
extern SDL_Window* gWindow;

//The window renderer
extern SDL_Renderer* gRenderer;

//Globally used font
extern TTF_Font* gFont;
extern int gFontSize;
extern std::string gFontName;

//Game Controller 1 handler
extern SDL_Joystick* gGameController;

// global joyArrow
extern joyArrow* jArrow;

// global joyOverlay
extern joyOverlay* jOverlay;

// global gameicon
extern devgruGameIcon* gameIcon;

// global frame count
extern int frame;

#endif /* _GLOBALS_H_ */

