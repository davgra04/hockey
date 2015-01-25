#include "globals.h"
#include "STexture.h"
#include "STimer.h"
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
SDL_Joystick* gGameController0 = NULL;
SDL_Joystick* gGameController1 = NULL;
SDL_Joystick* gGameController2 = NULL;
SDL_Joystick* gGameController3 = NULL;

// global joyOverlay
joyOverlay* jOverlay0 = NULL;
joyOverlay* jOverlay1 = NULL;
joyOverlay* jOverlay2 = NULL;
joyOverlay* jOverlay3 = NULL;

// global gameicon
devgruGameIcon* gameIcon = NULL;

// global frame count
int frame = 0;

// vector of all collision boxes in scene
std::vector<SDL_Rect*> allCollisionBoxes;

// Function for determining collision
bool isCollision( SDL_Rect* a, SDL_Rect* b){

	int atop, abottom, aleft, aright;
	int btop, bbottom, bleft, bright;

	atop = a->y;
	abottom = a->y + a->h;
	aleft = a->x;
	aright = a->x + a->w;

	btop = b->y;
	bbottom = b->y + b->h;
	bleft = b->x;
	bright = b->x + b->w;

	if( abottom <= btop ) return false;
	if( atop >= bbottom ) return false;
	if( aright <= bleft ) return false;
	if( aleft >= bright ) return false;

	return true;

}

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
			//Load joysticks
			gGameController0 = SDL_JoystickOpen( 0 );
			gGameController1 = SDL_JoystickOpen( 1 );
			gGameController2 = SDL_JoystickOpen( 2 );
			gGameController3 = SDL_JoystickOpen( 3 );
			if( gGameController0 == NULL )
			{
				printf( "Warning: Unable to open game controller 0! SDL Error: %s\n", SDL_GetError() );
			}
			if( gGameController1 == NULL )
			{
				printf( "Warning: Unable to open game controller 1! SDL Error: %s\n", SDL_GetError() );
			}
			if( gGameController2 == NULL )
			{
				printf( "Warning: Unable to open game controller 2! SDL Error: %s\n", SDL_GetError() );
			}
			if( gGameController3 == NULL )
			{
				printf( "Warning: Unable to open game controller 3! SDL Error: %s\n", SDL_GetError() );
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
	SDL_JoystickClose( gGameController0 );
	gGameController0 = NULL;

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

			//Create joyOverlay
			int joyXIncrement = 210;

			SDL_Color initColorA = {0, 160, 255, 255};
			SDL_Color initColorB;
			double tempdivisor = 4;
			initColorB.a = 128;
			initColorB.r = 255 - (255 - initColorA.r)/tempdivisor;
			initColorB.g = 255 - (255 - initColorA.g)/tempdivisor;
			initColorB.b = 255 - (255 - initColorA.b)/tempdivisor;
			jOverlay0 = new joyOverlay(3+0*joyXIncrement, 12, gGameController0, 1.0, initColorA, initColorB);
			initColorA = {160, 0, 255, 255};
			initColorB.r = 255 - (255 - initColorA.r)/tempdivisor;
			initColorB.g = 255 - (255 - initColorA.g)/tempdivisor;
			initColorB.b = 255 - (255 - initColorA.b)/tempdivisor;
			jOverlay1 = new joyOverlay(3+1*joyXIncrement, 12, gGameController1, 1.0, initColorA, initColorB);
			initColorA = {0, 255, 0, 255};
			initColorB.r = 255 - (255 - initColorA.r)/tempdivisor;
			initColorB.g = 255 - (255 - initColorA.g)/tempdivisor;
			initColorB.b = 255 - (255 - initColorA.b)/tempdivisor;
			jOverlay2 = new joyOverlay(3+2*joyXIncrement, 12, gGameController2, 1.0, initColorA, initColorB);
			initColorA = {255, 160, 0, 255};
			initColorB.r = 255 - (255 - initColorA.r)/tempdivisor;
			initColorB.g = 255 - (255 - initColorA.g)/tempdivisor;
			initColorB.b = 255 - (255 - initColorA.b)/tempdivisor;
			jOverlay3 = new joyOverlay(3+3*joyXIncrement, 12, gGameController3, 1.0, initColorA, initColorB);

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
			beandude* bDude = new beandude( SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 + 100, 7.0, true, gGameController0);
			beandude* otherDude = new beandude( SCREEN_WIDTH / 2 + 100, SCREEN_HEIGHT / 2 + 100, 44.0, true, gGameController1);

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
					jOverlay0->updateControllerState();
					jOverlay1->updateControllerState();
					jOverlay2->updateControllerState();
					jOverlay3->updateControllerState();

					//Update beandude
					// bDude->handleEvent(e);
					// otherDude->handleEvent(e);

					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					else if( e.type == SDL_KEYDOWN ){
						if(e.key.keysym.sym == SDLK_q
						   || e.key.keysym.sym == SDLK_ESCAPE){
							quit = true;
						}
					}
					else if( e.type == SDL_JOYAXISMOTION )
					{
						// std::cout << "A joy axis event occurred!\n"
						// 		  << "  joystick: " << e.jaxis.which
						// 		  << "  axis index: " << (int)e.jaxis.axis
						// 		  << "  value: " << e.jaxis.value << std::endl;
					}
					else if( e.type == SDL_JOYHATMOTION )
					{
						// std::cout << "A joy hat event has occurred!\n"
						// 		  << "  joystick: " << e.jhat.which
						// 		  << "  hat index: " << e.jhat.hat
						// 		  << "  value: " << e.jhat.value << std::endl;
					}
					else if( e.type == SDL_JOYBALLMOTION )
					{
						// std::cout << "A trackball event has occurred!\n"
						// 		  << "  joystick: " << e.jball.which
						// 		  << "  ball index: " << e.jball.ball
						// 		  << "  value: " << e.jball.xrel << "," << e.jball.yrel << std::endl;
					}
					else if( e.type == SDL_JOYBUTTONDOWN )
					{
					// 	std::cout << "A controller button event has occurred!\n"
					// 			  << "  joystick: " << e.jbutton.which
					// 			  << "  button: " << (int)e.jbutton.button
					// 			  << "  state: " << (int)e.jbutton.state << std::endl;
						// if( (int)e.jbutton.button == 4 || (int)e.jbutton.button == 5){
						// 	bDude->setActive(!bDude->getActive());
						// 	otherDude->setActive(!otherDude->getActive());
						// }
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

				//Draw overlay
				jOverlay0->render();
				jOverlay1->render();
				jOverlay2->render();
				jOverlay3->render();

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
