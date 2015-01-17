#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "globals.h"
#include "STexture.h"

STexture::STexture()
{
	//Initialize
	mTexture = NULL;
	mWidth = 0;
	mHeight = 0;
}

STexture::~STexture()
{
	//Deallocate
	free();
}

bool STexture::loadFromFile( std::string path )
{
	//Get rid of preexisting texture
	free();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Color key image
		SDL_SetColorKey( loadedSurface, SDL_TRUE, SDL_MapRGB( loadedSurface->format, 0, 0xFF, 0xFF ) );

		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = loadedSurface->w;
			mHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	mTexture = newTexture;
	return mTexture != NULL;
}

#ifdef _SDL_TTF_H
bool STexture::loadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	//Get rid of preexisting texture
	free();

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
		//Create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( mTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			//Get image dimensions
			mWidth = textSurface->w;
			mHeight = textSurface->h;
		}

		//Get rid of old surface
		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	
	//Return success
	return mTexture != NULL;
}
#endif

void STexture::free()
{
	//Free texture if it exists
	if( mTexture != NULL )
	{
		SDL_DestroyTexture( mTexture );
		mTexture = NULL;
		mWidth = 0;
		mHeight = 0;
	}
}

void STexture::setColor( Uint8 red, Uint8 green, Uint8 blue )
{
	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, red, green, blue );
}

void STexture::setColorHSV( double hue, double saturation, double value )
{
	if((0.0 > hue)        || (360.0 <= hue))	{ return; }
	if((0.0 > saturation) || (1.0 < saturation)){ return; }
	if((0.0 > value)	  || (1.0 < value))		{ return; }

	// HSV to RGB formula
	double C = value * saturation;
	double X = C * (1 - abs((int)(hue/60.0)%2 - 1));
	double m = value - C;
	double redDouble = 0.0;
	double greenDouble = 0.0;
	double blueDouble = 0.0;

	if(0.0 <= hue && hue < 60.0){
		redDouble = C + m;
		greenDouble = X + m;
		blueDouble = 0.0;
	}
	else if(60.0 <= hue && hue < 120.0){
		redDouble = X + m;
		greenDouble = C + m;
		blueDouble = 0.0;
	}
	else if(120.0 <= hue && hue < 180.0){
		redDouble = 0.0;
		greenDouble = C + m;
		blueDouble = X + m;
	}
	else if(180.0 <= hue && hue < 240.0){
		redDouble = 0.0;
		greenDouble = X + m;
		blueDouble = C + m;
	}
	else if(240.0 <= hue && hue < 300.0){
		redDouble = X + m;
		greenDouble = 0.0;
		blueDouble = C + m;
	}
	else if(300.0 <= hue && hue < 360.0){
		redDouble = C + m;
		greenDouble = 0.0;
		blueDouble = X + m;
	}


	//Modulate texture rgb
	SDL_SetTextureColorMod( mTexture, (Uint8)(redDouble*255), (Uint8)(greenDouble*255), (Uint8)(blueDouble*255));
	//SDL_SetTextureColorMod( mTexture, 255, 255, 255 );
}

void STexture::setBlendMode( SDL_BlendMode blending )
{
	//Set blending function
	SDL_SetTextureBlendMode( mTexture, blending );
}
		
void STexture::setAlpha( Uint8 alpha )
{
	//Modulate texture alpha
	SDL_SetTextureAlphaMod( mTexture, alpha );
}

void STexture::render( int x, int y, SDL_Rect* clip, double angle, double scale, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, mWidth * scale, mHeight * scale };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w * scale;
		renderQuad.h = clip->h * scale;
	}

	//Render to screen
	SDL_RenderCopyEx( gRenderer, mTexture, clip, &renderQuad, angle, center, flip );
}

int STexture::getWidth()
{
	return mWidth;
}

int STexture::getHeight()
{
	return mHeight;
}

