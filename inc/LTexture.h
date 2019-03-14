#pragma once
#include<iostream>
#include<SDL.h>

extern SDL_Renderer* gRenderer;

class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path);

	//Deallocates texture
	void free();

	//Set color modulation
	void setColor(Uint8 red, Uint8 green, Uint8 blue);

	//Set blending
	void setBlendMode(SDL_BlendMode blending);

	//Set alpha modulation
	void setAlpha(Uint8 alpha);

	//Box collision detector

	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	//Renders texture at given point
	void render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, double scale);
	void renderWithScale(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip, double scaleX, double scaleY, double percentage);

	//Gets image dimensions
	int getWidth();
	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};