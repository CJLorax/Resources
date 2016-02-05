#if defined (_WIN32) ||  (_WIN64)
	#include <SDL.h>
	#include <SDL_image.h>
#endif

#if defined (__APPLE__)
	#include <SDL2/SDL.h>
	#include <SDL2_image/SDL_image.h>

#endif

#if defined (__linux__)
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_image.h"
#endif

#include <string>
#include <iostream>
using namespace std;

class Explode {

public:

	// Boolean for the state of the bullet
	bool active;

	// Explosion texture
	SDL_Texture *texture;

	// Explosion rectangle for position (X,Y) and size (W,H)
	SDL_Rect posRect;

	// Explosion rectangle for position (X,Y) and size (W,H)
	SDL_Rect drawRect;

	// width and height of each frame plus width and height of entire image
	int frameWidth, frameHeight, textureWidth, textureHeight;

	// float values to track time until next frame of animation
	float frameCounter;

	// Explode creation method. Requires the renderer, a path to the needed image, an X position, a Y position
	Explode(SDL_Renderer *renderer, string filePath, float x, float y);

	// Explode Update - requires deltaTime be passed
	void Update(float deltaTime);

	// Explode Draw - requires renderer be passed
	void Draw(SDL_Renderer *renderer);

	// Explode reset
	void Reset();

	// Explode destruction method
	~Explode();


};
