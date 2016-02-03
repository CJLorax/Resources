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

class Bullet {

public:

	// Boolean for the state of the bullet
	bool active;

	// Bullet texture
	SDL_Texture *texture;

	// Bullet rectangle for position (X,Y) and size (W,H)
	SDL_Rect posRect;

	// movement direction values
	float xDir, yDir;

	// Bullet speed
	float speed;

	// Bullet float positions to prevent precision loss
	float pos_X, pos_Y;

	// Bullet creation method. Requires the renderer, a path to the needed image, an X position, a Y position
	Bullet(SDL_Renderer *renderer, string filePath, float x, float y);

	// Bullet Update - requires deltaTime be passed
	void Update(float deltaTime);

	// Bullet Draw - requires renderer be passed
	void Draw(SDL_Renderer *renderer);

	// Bullet destruction method
	~Bullet();


};
