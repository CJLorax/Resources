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

#include <stdlib.h>     /* srand, rand */


class Enemy {

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

	// enemy angle
	float angle;

	// point for the center of you object. Used for rotation
	SDL_Point center;

	// Enemy float positions to prevent precision loss
	float pos_X, pos_Y;

	// Enemy creation method. Requires the renderer, a path to the needed image
	Enemy(SDL_Renderer *renderer, string filePath);

	// Enemy Update - requires deltaTime be passed
	void Update(float deltaTime);

	// Enemy Draw - requires renderer be passed
	void Draw(SDL_Renderer *renderer);

	// Enemy Reset
	void Reset();

	// Enemy destruction method
	~Enemy();


};

