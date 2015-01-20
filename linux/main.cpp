#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <iostream>
#include "globals.h"
#include "STexture.h"
#include "STimer.h"
#include "joyArrow.h"
#include "joyOverlay.h"
#include "devgruGameIcon.h"
#include "beandude.h"

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font* gFont = NULL;
int gFontSize = 12;

std::string gFontName = "Squarea Regular.ttf";

//Game Controller 1 handler
SDL_Joystick* gGameController = NULL;

// global joyArrow
joyArrow* jArrow = NULL;

// global joyOverlay
joyOverlay* jOverlay = NULL;

// global gameicon
devgruGameIcon* gameIcon = NULL;

// global frame count
int frame = 0;

// FRAME CONSTANTS
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;

//Starts up SDL and creates window
bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_JOYSTICK ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to none
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "0" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Check for joysticks
		if( SDL_NumJoysticks() < 1 )
		{
			printf( "Warning: No joysticks connected!\n" );
		}
		else
		{
			//Load joystick
			gGameController = SDL_JoystickOpen( 0 );
			if( gGameController == NULL )
			{
				printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
			}
		}

		//Create window
		gWindow = SDL_CreateWindow( "Hockey", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

//Loads media
bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont( gFontName.c_str(), gFontSize );
	if( gFont == NULL )
	{
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	
	return success;
}

//Frees media and shuts down SDL
void close()
{
	//Close game controller
	SDL_JoystickClose( gGameController );
	gGameController = NULL;

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//Normalized direction
			int xDir = 0;
			int yDir = 0;

			//Create joyArrow
			jArrow = new joyArrow();

			//Create joyOverlay
			jOverlay = new joyOverlay(3, 12, 1.0);

			//Create gameIcon
			gameIcon = new devgruGameIcon(SCREEN_WIDTH/2 - 160, 40, 1.0);
			// gameIcon = new devgruGameIcon(240, 219, 1.0);

			//frame display
			SDL_Color textColor = { 0, 80, 255, 255 };
			STexture* frameTex = new STexture();
			std::stringstream frameString;
			
			//The frames per second timer
            STimer* fpsTimer = new STimer();

            //The frames per second cap timer
            STimer* capTimer = new STimer();

			//beandude
			beandude* bDude = new beandude( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, 1.0, true);
			beandude* otherDude = new beandude( SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 100, 1.0, false);

			// start frames timer
            fpsTimer->start();

            // hockey rink background texture
            STexture hockeyRink = STexture();
            hockeyRink.loadFromFile("assets/hockeyrink/hockeyrink3test4.png");

			//While application is running
			while( !quit )
			{
				
				//Start cap timer
                capTimer->start();
				
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//Update jOverlay
					jOverlay->getState();

					//Update beandude
					bDude->handleEvent(e);
					otherDude->handleEvent(e);

					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_JOYAXISMOTION )
					{
						std::cout << "A joy axis event occurred!\n"
								  << "  joystick: " << e.jaxis.which
								  << "  axis index: " << (int)e.jaxis.axis
								  << "  value: " << e.jaxis.value << std::endl;
					}
					else if( e.type == SDL_JOYHATMOTION )
					{
						std::cout << "A joy hat event has occurred!\n"
								  << "  joystick: " << e.jhat.which
								  << "  hat index: " << e.jhat.hat
								  << "  value: " << e.jhat.value << std::endl;
					}
					else if( e.type == SDL_JOYBALLMOTION )
					{
						std::cout << "A trackball event has occurred!\n"
								  << "  joystick: " << e.jball.which
								  << "  ball index: " << e.jball.ball
								  << "  value: " << e.jball.xrel << "," << e.jball.yrel << std::endl;
					}
					else if( e.type == SDL_JOYBUTTONDOWN )
					{
						std::cout << "A controller button event has occurred!\n"
								  << "  joystick: " << e.jbutton.which
								  << "  button: " << (int)e.jbutton.button
								  << "  state: " << (int)e.jbutton.state << std::endl;
						if( (int)e.jbutton.button == 4 || (int)e.jbutton.button == 5){
							bDude->setActive(!bDude->getActive());
							otherDude->setActive(!bDude->getActive());
						}
					}
					else if( e.type == SDL_JOYAXISMOTION )
					{
						//Motion on controller 0
						if( e.jaxis.which == 0 )
						{						
							Sint16 x_move = SDL_JoystickGetAxis(gGameController, 0);
							Sint16 y_move = SDL_JoystickGetAxis(gGameController, 1);
							double rotation = atan2( (double)y_move, (double)x_move ) * ( 180.0 / M_PI );

							jArrow->setAngle(rotation);
							
						}
					}
				}
				
				//Calculate and correct fps
                float avgFPS = frame / ( fpsTimer->getTicks() / 1000.f );
                if( avgFPS > 2000000 )
                {
                    avgFPS = 0;
                }

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				//Draw hockey rink
				hockeyRink.render(0, 80, NULL, 0.0, 3.0, NULL, SDL_FLIP_NONE);

				//Draw arrow

				//Draw gameIcon
				gameIcon->render(frame, (int)(frame)%360, 0.0, 1.0);

				//Draw frames
				frameString.str("");
				frameString << frame;
				frameTex->loadFromRenderedText(frameString.str().c_str(), textColor);
				frameTex->render(0, 0, NULL, 0.0, 1.0, NULL, SDL_FLIP_NONE);

				//Draw beandude
				bDude->move();
				bDude->render();
				otherDude->move();
				otherDude->render();
				jArrow->render(50, 420);

				//Draw overlay
				jOverlay->render();

				//Update screen
				SDL_RenderPresent( gRenderer );

				++frame;
				
				//If frame finished early
                int frameTicks = capTimer->getTicks();
                if( frameTicks < SCREEN_TICK_PER_FRAME )
                {
                    //Wait remaining time
                    SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
                }
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
