#if defined(_WIN32) || (_WIN64)
	#include "SDL.h"
	#include "SDL_image.h"
	// Week 5 ********************************************************************************
	#include "SDL_mixer.h"

#endif

#if defined(__APPLE__)
	#include "SDL2/SDL.h"
	#include "SDL2_image/SDL_image.h"
	// Week 5 ********************************************************************************
	#include "SDL2_mixer/SDL_mixer.h"

#endif

#if defined(__linux__)
	#include "SDL2/SDL.h"
	#include "SDL2/SDL_image.h"
	// Week 5 ********************************************************************************
	#include "SDL2/SDL_mixer.h"

#endif

#if defined (_WIN32) || (_WIN64)
	#include <direct.h>
	#define getcwd _getcwd
#endif

#if defined (__linux__)
	#include <unistd.h>
#endif

#include <stdio.h>
#include <iostream>
using namespace std;

// variables for the background movement and position
//int for background
int bkgdSpeed = 100;

// create the SDL Rectangle for the background texture's position and size - background1 and 2
SDL_Rect bkgd1Pos, bkgd2Pos;

// Background float vars for movement
float b1pos_X = 0, b1pos_Y = 0;
float b2pos_X = 0, b2pos_Y = -768;

// deltaTime var intialization - for frame rate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

// move the background
void UpdateBackground() {

	// Update bkgd1's float position values
	b1pos_Y += (bkgdSpeed * 1) * deltaTime;
	bkgd1Pos.y = (int) (b1pos_Y + 0.5f);

	// reset the float position values of bkgd1 when off the bottom of screen
	if (bkgd1Pos.y >= 768) {
		bkgd1Pos.y = -768;
		b1pos_Y = bkgd1Pos.y;
	}

	// Update bkgd2's float position values
	b2pos_Y += (bkgdSpeed * 1) * deltaTime;
	bkgd2Pos.y = (int) (b2pos_Y + 0.5f);

	// reset the float position values of bkgd2 when off the bottom of screen
	if (bkgd2Pos.y >= 768) {
		bkgd2Pos.y = -768;
		b2pos_Y = bkgd2Pos.y;
	}

}
// ************************************ NEW **********************************
//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

// joystick direction vars
float xDir, yDir;

// cursor float vars for movement
float pos_X, pos_Y;

// create SDL Rectangle for the 2 player  graphic
SDL_Rect cursorPos, activePos;

// var from cursor speed
int cursorSpeed = 400;

void moveCursor(const SDL_ControllerAxisEvent event) {

	if (event.which == 0) {
		if (event.axis == 0) {

			if (event.value < -JOYSTICK_DEAD_ZONE) {
				xDir = -1.0f;
			} else if (event.value > JOYSTICK_DEAD_ZONE) {
				xDir = 1.0f;
			} else {
				xDir = 0.0f;
			}
		}

		if (event.axis == 1) {

			if (event.value < -JOYSTICK_DEAD_ZONE) {
				yDir = -1.0f;
			} else if (event.value > JOYSTICK_DEAD_ZONE) {
				yDir = 1.0f;
			} else {
				yDir = 0.0f;
			}
		}

	}

}

void UpdateCursor() {
	// Update Cursor
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	cursorPos.x = (int) (pos_X + 0.5f);
	cursorPos.y = (int) (pos_Y + 0.5f);

	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	if (cursorPos.x < 0) {
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if (cursorPos.x > 1024 - cursorPos.w) {
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	if (cursorPos.y < 0) {
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if (cursorPos.y > 768 - cursorPos.h) {
		cursorPos.y = 768 - cursorPos.h;
		pos_Y = cursorPos.y;
	}
}


SDL_Texture* loadTexture(const string file, SDL_Renderer *renderer){

	//Initialize to nullptr to avoid dangling pointer issues
	SDL_Texture *texture = NULL;

	//Load the image
	SDL_Surface *test = IMG_Load(file.c_str());

	texture = SDL_CreateTextureFromSurface(renderer,test);

	//Release the SDL surface for later use
	SDL_FreeSurface(test);

	// return the texture
	return texture;
}

SDL_Rect rectangle (SDL_Texture *texture, int x, int y)
{

	SDL_Rect posRect;

	// set the SDL_Rect X and Y for the player
	posRect.x = x;

	posRect.y = y;

	// Use SDL_QueryTexture to get the W and H of the player's texture
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;

	posRect.h = h;

	// return the texture
	return posRect;

}


// variables for all Menus button over
bool players1Over = false, players2Over = false, instructionsOver = false,
		quitOver = false, menuOver = false, playOver = false;

// ************************************ NEW **********************************

// ************************************ NEW **********************************

// class header includes
#include "player.h"
#include "enemy.h"
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include "explode.h"

// ************************************ NEW **********************************

// variable to hold the list of enemies: for 1 player game - 6 total, for 2 player game - 12 total
vector<Enemy> enemyList;

vector<Explode> explodeList;

void MakeExplosion(int x, int y){
	// see if there is a explosion not active to use
	for (int i = 0; i < explodeList.size(); i++)
	{
		// see if the bullet is not active
		if(explodeList[i].active == false){

			// set explosion to active
			explodeList[i].active = true;

			// use some math in the x position to get the bullet close to
			// the center of the player using player width
			explodeList[i].posRect.x = x;
			explodeList[i].posRect.y = y;

			// once explosion is found, break out of loop
			break;
		}

	}
}


/*
// code for max number of controllers
#define MAX_CONTROLLERS 2
SDL_GameController *ControllerHandles[MAX_CONTROLLERS];
*/

int main(int argc, char* argv[]) {

	/* initialize random seed: */
	srand (time(NULL));

#if defined(_WIN32) || (_WIN64)
	string currentWorkingDirectory(getcwd(NULL, 0));
	string images_dir = currentWorkingDirectory + "\\Resources\\Images\\";

	// Week 5 ********************************************************************************
	// create a string to link to the audio folder on __APPLE__
	string audio_dir = currentWorkingDirectory + "\\Resource\\Audio\\";
#endif

#if defined(__APPLE__)
	// string var to hold the current working directory on __APPLE__
	string currentWorkingDirectory(getcwd(NULL, 0));

	// create a string to link to the images folder on __APPLE__
	string images_dir = currentWorkingDirectory + "/Resources/Images/";

	// Week 5 ********************************************************************************
	// create a string to link to the audio folder on __APPLE__
	string audio_dir = currentWorkingDirectory + "/Resources/Audio/";
#endif

#if defined(__linux__)
	// string var to hold the current working directory on __linux__
	string currentWorkingDirectory(getcwd(NULL,0));

	// create a string to link to the images folder on __linux__
	string images_dir = currentWorkingDirectory +"/Resources/Images/";

	// create a string to link to the images folder on __linux__
	string audio_dir = currentWorkingDirectory +"/Resources/Audio/";
#endif

	SDL_Window *window;                    // Declare a pointer

	SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

	// Create an application window with the following settings:
	window = SDL_CreateWindow("An SDL2 window",                  // window title
			SDL_WINDOWPOS_UNDEFINED,           // initial x position
			SDL_WINDOWPOS_UNDEFINED,           // initial y position
			1024,                               // width, in pixels
			768,                               // height, in pixels
			SDL_WINDOW_OPENGL                  // flags - see below
	);

	// Check that the window was successfully created
	if (window == NULL) {
		// In the case that the window could not be made...
		printf("Could not create window: %s\n", SDL_GetError());
		return 1;
	}

	// Create a Renderer variable - pointer
	SDL_Renderer *renderer = NULL;

	//Create Renderer
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);



	// ***************** Create Backgrounds - START *************

		SDL_Texture * bkgd1 =  loadTexture((images_dir + "bkgd.png").c_str(), renderer);
		SDL_Texture * bkgd2 =  loadTexture((images_dir + "bkgd.png").c_str(), renderer);

		// the rectangle which has the X pos, Ypos, texture Width and texture Height - bkgd1
		bkgd1Pos.x = 0;
		bkgd1Pos.y = 0;
		bkgd1Pos.w = 1024;
		bkgd1Pos.h = 768;

		// the rectangle which has the X pos, Ypos, texture Width and texture Height - bkgd2
		bkgd2Pos.x = 0;
		bkgd2Pos.y = -768;
		bkgd2Pos.w = 1024;
		bkgd2Pos.h = 768;
	// ***************** Create BACKGROUNDS - END *************



	// ***************** Create CURSOR - START *************

		SDL_Texture * cursor =  loadTexture((images_dir + "cursor.png").c_str(), renderer);

		// the rectangle which has the X pos, Ypos, texture Width and texture Height - cursor graphic
		cursorPos.x = 0;
		cursorPos.y = 0;
		cursorPos.w = 58;
		cursorPos.h = 52;

		// the rectangle which has the X pos, Ypos, texture Width and texture Height - cursor graphic
		activePos.x = 10;
		activePos.y = 10;
		activePos.w = 10;
		activePos.h = 10;

	// ***************** Create CURSOR - END *************



	// ***************** Create INSTRUCTIONS MENU - START *************

		// ******** TITLE  - START
			// create the SDL surface to hold the texture file
			SDL_Texture * instructionsText =  loadTexture((images_dir + "instructions.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect instructionsTextPos = rectangle (instructionsText, 120, 271);
		// ******** TITLE - END

		// ******** SMALL MENU NORMAL - START
			// create the SDL surface to hold the texture file
			SDL_Texture * menuN =  loadTexture((images_dir + "menu_n.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect menuNPos = rectangle (menuN, 22, 702);
		// ******** SMALL MENU NORMAL - END

		// ******** SMALL MENU OVER - START
			// create the SDL surface to hold the texture file
			SDL_Texture * menuO =  loadTexture((images_dir + "menu_o.png").c_str(), renderer);
		// ******** SMALL MENU OVER - END

	// ***************** Create INSTRUCTION MENU - END *************



	// ***************** Create MAIN MENU - START *************

		// ******** TITLE  - START
			// create the SDL surface to hold the texture file
			SDL_Texture * title =  loadTexture((images_dir + "title.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect titlePos = rectangle (title, 264, 73);
		// ******** TITLE - END

		// ******** PLAYERS 1  - START
			// create the SDL surface to hold the texture file
			SDL_Texture * players1N =  loadTexture((images_dir + "one_n.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect players1NPos = rectangle (players1N, 380, 261);

			// create the SDL surface to hold the texture file
			SDL_Texture * players1O =  loadTexture((images_dir + "one_o.png").c_str(), renderer);
		// ******** PLAYERS 1  - END

		// ******** PLAYERS 2  - START
			// create the SDL surface to hold the texture file
			SDL_Texture * players2N =  loadTexture((images_dir + "two_n.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect players2NPos = rectangle (players2N, 360,343);

			// create the SDL surface to hold the texture file
			SDL_Texture * players2O =  loadTexture((images_dir + "two_o.png").c_str(), renderer);
		// ******** PLAYERS 2  - END

		// ******** INSTRUCTIONS  - START
			// create the SDL surface to hold the texture file
			SDL_Texture * instructN =  loadTexture((images_dir + "instruct_n.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect instructNPos = rectangle (instructN, 351, 424);

			// create the SDL surface to hold the texture file
			SDL_Texture * instructO =  loadTexture((images_dir + "instruct_o.png").c_str(), renderer);
		// ******** INSTRUCTIONS  - END

		// ******** QUIT  - START
			// create the SDL surface to hold the texture file
			SDL_Texture * quitN =  loadTexture((images_dir + "quit_n.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect quitNPos = rectangle (quitN, 391, 503);

			// create the SDL surface to hold the texture file
			SDL_Texture * quitO =  loadTexture((images_dir + "quit_o.png").c_str(), renderer);
		// ******** QUIT  - END

	// ***************** Create MAIN MENU - END *************



	// ***************** Create WIN MENU - START *************

		// ******** TEXT  - START
			// create the SDL surface to hold the texture file
			SDL_Texture * winText =  loadTexture((images_dir + "win.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect winTextPos = rectangle (winText, 240, 288);
		// ******** TEXT - END

		// ******** SMALL PLAY AGAIN NORMAL - START
			// create the SDL surface to hold the texture file
			SDL_Texture * playN =  loadTexture((images_dir + "play_n.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect playNPos = rectangle (playN, 875, 702);
			// ******** SMALL MENU NORMAL - END

			// ******** SMALL PLAY AGAIN OVER - START
			// create the SDL surface to hold the texture file
			SDL_Texture * playO =  loadTexture((images_dir + "play_o.png").c_str(), renderer);
			// ******** SMALL PLAY AGAIN OVER - END

	// ***************** Create WIN Menu - END *************



	// ***************** Create LOSE Menu - START *************

		// ******** TEXT  - START
			// create the SDL surface to hold the texture file
			SDL_Texture * loseText =  loadTexture((images_dir + "lose.png").c_str(), renderer);

			// the rectangle which has the X pos, Ypos, texture Width and texture Height
			SDL_Rect loseTextPos = rectangle (loseText, 148, 288);
		// ******** TEXT - END

	// ***************** Create LOSE Menu - END *************


// ***** Turn on Game Controller Events *****
	SDL_GameControllerEventState(SDL_ENABLE);

	// ***** set up a Game Controller variable for player 1 *****
	SDL_GameController* gGameController0 = NULL;

	// ***** set up a Game Controller variable player 2 *****
	SDL_GameController* gGameController1 = NULL;

	// ***** Open Game Controller for player 1 *****
	gGameController0 = SDL_GameControllerOpen(0);

	// ***** Open Game Controller for player 2  *****
	gGameController1 = SDL_GameControllerOpen(1);


/*
int MaxJoysticks = SDL_NumJoysticks();
int ControllerIndex = 0;
for(int JoystickIndex=0; JoystickIndex < MaxJoysticks; ++JoystickIndex)
{
    if (!SDL_IsGameController(JoystickIndex))
    {
        continue;
    }
    if (ControllerIndex >= MAX_CONTROLLERS)
    {
        break;
    }
    ControllerHandles[ControllerIndex] = SDL_GameControllerOpen(JoystickIndex);
    ControllerIndex++;
}
*/
	// ***** SDL event to handle input *****
	SDL_Event event;

	// set up various game states the game can be placed in
	enum GameState {
		MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE
	};

	// Set default GameState
	GameState gameState = MENU;


	// Variables to keep the player in the various states
	//declare AND initialize for windows
	bool menu= false, instructions= false, players1= false, players2= false, win= false, lose= false, quit= false;


	// Week 5 ********************************************************************************
    // Open Audio Channel
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

    //Load a MUSIC file
	Mix_Music *bgm = Mix_LoadMUS((audio_dir + "background.mp3").c_str());

   // IF the MUSIC file is not playing - Play it.
	if(!Mix_PlayingMusic())
        Mix_PlayMusic(bgm, -1);

	// Set up a Sound Effect CHUNK for the button over state
	Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "over.wav").c_str());

	// bool value to control the over sound effect and the buttons
	bool alreadyOver = false;

	// Set up a Sound Effect CHUNK for the button pressed state
	Mix_Chunk *pressedSound = Mix_LoadWAV((audio_dir + "pressed.wav").c_str());

	// Set up a Sound Effect CHUNK for the button pressed state
	Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "enemy.wav").c_str());

	// Week 5 ********************************************************************************
	// MOVED PLAYERS AFTER AUDIO SOURCE CREATION
	// *********************************** Create Players - START **************************
	Player player1 = Player(renderer, 0, images_dir.c_str(), audio_dir.c_str(), 250.0, 500.0);
	Player player2 = Player(renderer, 1, images_dir.c_str(), audio_dir.c_str(), 750.0, 500.0);
	// *********************************** Create Players - END **************************

	//Create a pool of explosions - 20
	for (int i = 0; i < 20; i++)
	{
		// create the enemy
		Explode tmpExplode(renderer, images_dir, -1000, -1000);

		// add to enemylist
		explodeList.push_back(tmpExplode);

	}



	// The window is open: could enter program loop here (see SDL_PollEvent())

	// ***** basic game loop *****
	while (!quit) {

		switch (gameState) {

		case MENU:

			menu = true;

			while (menu) {
				// Create deltaTime - for frame rate independence
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// *****check for input events *****
				if (SDL_PollEvent(&event)) {
					// If window is closed be X in corner
					if (event.type == SDL_QUIT) {
						quit = true;
						menu = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0) {
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {

								// ************************************ NEW **********************************
								// If player chooses 1 player game
								if (players1Over) {
									// Week 5 ************************************************************************************************
									// Play the Over Sound - plays fine through levels, must pause for QUIT
									Mix_PlayChannel(-1, pressedSound, 0);
									menu = false;
									gameState = PLAYERS1;
									players1Over = false;
								}

								// If player chooses 2 player game
								if (players2Over) {
									// Week 5 ************************************************************************************************
									// Play the Over Sound - plays fine through levels, must pause for QUIT
									Mix_PlayChannel(-1, pressedSound, 0);
									menu = false;
									gameState = PLAYERS2;
									players2Over = false;
								}

								// If player chooses Instructions
								if (instructionsOver) {
									// Week 5 ************************************************************************************************
									// Play the Over Sound - plays fine through levels, must pause for QUIT
									Mix_PlayChannel(-1, pressedSound, 0);
									menu = false;
									gameState = INSTRUCTIONS;
									instructionsOver = false;
								}

								// If player chooses to Quit Game
								if (quitOver) {
									// Week 5 ************************************************************************************************
									// Play the Over Sound - plays fine through levels, must pause for QUIT
									Mix_PlayChannel(-1, pressedSound, 0);
									// Add a slight delay
									SDL_Delay(200);
									menu = false;
									quit = true;
									quitOver = false;
								}

								// ************************************ NEW **********************************
							}

						}
						break;
						// ************************************ NEW **********************************
					case SDL_CONTROLLERAXISMOTION:

						moveCursor(event.caxis);
						break;
						// ************************************ NEW **********************************
					}
				}

				// Update Section
				UpdateBackground();

				// ************************************ NEW **********************************
				UpdateCursor();

				// check for collision between cursor active state and buttons
				players1Over = SDL_HasIntersection(&activePos, &players1NPos);
				players2Over = SDL_HasIntersection(&activePos, &players2NPos);
				instructionsOver = SDL_HasIntersection(&activePos, &instructNPos);
				quitOver = SDL_HasIntersection(&activePos, &quitNPos);

				// ************************************ NEW **********************************


				// Week 5 ************************************************************************************************
				// If the cursor is over a button, play the over sound
				if(players1Over || players2Over || instructionsOver || quitOver){
					if(alreadyOver == false){
					 Mix_PlayChannel(-1, overSound, 0);
					 alreadyOver = true;
					}
				}

				// if the cursor is not over ANY button, reset the alreadyOver var
				if(!players1Over && !players2Over && !instructionsOver && !quitOver){
					alreadyOver = false;
				}





				// Draw Section
				// clear the old screen buffer
				SDL_RenderClear(renderer);

				// Draw Bkgd1
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				// Draw Bkgd2
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw Title
				SDL_RenderCopy(renderer, title, NULL, &titlePos);

				// ************************************ NEW **********************************
				// Draw 1 player button
				if (players1Over) {
					SDL_RenderCopy(renderer, players1O, NULL, &players1NPos);
				} else {
					SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);
				}

				// Draw 2 player button
				if (players2Over) {
					SDL_RenderCopy(renderer, players2O, NULL, &players2NPos);
				} else {
					SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);
				}

				// Draw Instructions button
				if (instructionsOver) {
					SDL_RenderCopy(renderer, instructO, NULL, &instructNPos);
				} else {
					SDL_RenderCopy(renderer, instructN, NULL, &instructNPos);
				}

				// Draw Quit Game button
				if (quitOver) {
					SDL_RenderCopy(renderer, quitO, NULL, &quitNPos);
				} else {
					SDL_RenderCopy(renderer, quitN, NULL, &quitNPos);
				}

				// ************************************ NEW **********************************

				// Draw Cursor button
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// draw new, updated screen
				SDL_RenderPresent(renderer);

			}

			break;
			// end main menu case

		case INSTRUCTIONS:

			instructions = true;

			while (instructions) {

				// Create deltaTime - for frame rate independence
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// *****check for input events *****
				if (SDL_PollEvent(&event)) {
					// If window is closed be X in corner
					if (event.type == SDL_QUIT) {
						quit = true;
						menu = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0) {
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
								// ************************************ NEW **********************************
								// If player chooses main menu game
								if (menuOver) {
									instructions = false;
									gameState = MENU;
									menuOver = false;
								}

								// ************************************ NEW **********************************
							}

						}
						break;
						// ************************************ NEW **********************************
					case SDL_CONTROLLERAXISMOTION:

						moveCursor(event.caxis);
						break;
						// ************************************ NEW **********************************
					}
				}

				// Update Section
				UpdateBackground();

				// ************************************ NEW **********************************
				UpdateCursor();

				menuOver = SDL_HasIntersection(&activePos, &menuNPos);

				// ************************************ NEW **********************************

				// Draw Section
				// clear the old screen buffer
				SDL_RenderClear(renderer);

				// Draw Bkgd1
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				// Draw Bkgd2
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw Title
				SDL_RenderCopy(renderer, title, NULL, &titlePos);

				// Draw instructions Graphic
				SDL_RenderCopy(renderer, instructionsText, NULL,
						&instructionsTextPos);

				// ************************************ NEW **********************************
				// Draw Main Menu button
				if (menuOver) {
					SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
				} else {
					SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
				}

				// ************************************ NEW **********************************

				// Draw cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// draw new, updated screen
				SDL_RenderPresent(renderer);

			}

			break;
			// end instructions case

		case PLAYERS1:

			enemyList.clear();

			// reset the player
			player1.Reset();

			players1 = true;

			//Create the enemy pool - 6
			for (int i = 0; i < 6; i++)
			{
				// create the enemy
				Enemy tmpEnemy(renderer, images_dir);

				// add to enemylist
				enemyList.push_back(tmpEnemy);

			}

			while (players1) {

				// Create deltaTime - for frame rate independence
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// *****check for input events *****
				if (SDL_PollEvent(&event)) {
					// If window is closed be X in corner
					if (event.type == SDL_QUIT) {
						quit = true;
						players1 = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0) {
							if (event.cbutton.button
									== SDL_CONTROLLER_BUTTON_X) {
								players1 = false;
								gameState = WIN;
							}
							if (event.cbutton.button
									== SDL_CONTROLLER_BUTTON_Y) {
								players1 = false;
								gameState = LOSE;
							}
							// ************************************ NEW **********************************
							// send button press info to player1
							if(player1.active)
							player1.OnControllerButton(event.cbutton);
							// ************************************ NEW **********************************

						}
						break;
						// ************************************ NEW **********************************
					case SDL_CONTROLLERAXISMOTION:

						if(player1.active)
						player1.OnControllerAxis(event.caxis);
						break;
						// ************************************ NEW **********************************

					}
				}

				// Update Section
				UpdateBackground();

				// ************************************ NEW **********************************
				// Update Player 1
				// Week 5 ********************************************************************************
				if(player1.active)
				player1.Update(deltaTime, renderer);
				// ************************************ NEW **********************************

				// Update the enemies
				for (int i = 0; i < enemyList.size(); i++)
				{
					// update enemy
					enemyList[i].Update(deltaTime);
				}



				if (player1.active) {

					// use the player 1 bullet list to see if the active bullets
					// hit any of the enemies
					for (int i = 0; i < player1.bulletList.size(); i++) {
						// check to see if this bullet is active (onscreen)
						if (player1.bulletList[i].active == true) {

							// check all enemies against the active bullet
							for (int j = 0; j < enemyList.size(); j++) {
								// if there is a collision between the two objects
								if (SDL_HasIntersection(&player1.bulletList[i].posRect,&enemyList[j].posRect)) {

									// play explosion sound
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									// reset the enemy
									enemyList[j].Reset();

									// reset the enemy
									player1.bulletList[i].Reset();

									//give the player some points
									player1.playerScore += 50;

									// check to see if there is a winning condition
									if(player1.playerScore >= 1000)
									{
										// go to win
										players1 = false;
										gameState = WIN;
									}
								}
							}

						}
					}

					// check to see if the enemies hit the player
					for (int i = 0; i < enemyList.size(); i++) {
						// if there is a collision between the two objects
						if (SDL_HasIntersection(&player1.posRect,&enemyList[i].posRect)) {

							// play explosion sound
							Mix_PlayChannel(-1, explosionSound, 0);

							MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);

							// reset the enemy
							enemyList[i].Reset();

							//give the player some points
							player1.playerLives -= 1;

							//if game over - player lives <= 0
							if (player1.playerLives <= 0) {
								players1 = false;
								gameState = LOSE;
								break;
							}
						}

					}

				}

				//Update the pool of explosions - 20
				for (int i = 0; i < explodeList.size(); i++)
				{
					// check to see if active
					if(explodeList[i].active == true){
						// draw explode
						explodeList[i].Update(deltaTime);
					}

				}

				// Draw Section
				// clear the old screen buffer
				SDL_RenderClear(renderer);

				// Draw Bkgd1
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				// Draw Bkgd2
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw Title
				//SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);



				// ************************************ NEW **********************************
				// Draw Player 1
				if(player1.active)
				player1.Draw(renderer);
				// ************************************ NEW **********************************

				// Draw the enemies
				for (int i = 0; i < enemyList.size(); i++)
				{
					// update enemy
					enemyList[i].Draw(renderer);
				}

				//Draw the pool of explosions - 20
				for (int i = 0; i < explodeList.size(); i++)
				{
					// check to see if active
					if(explodeList[i].active == true){
						// draw explode
						explodeList[i].Draw(renderer);
					}

				}

				// draw new, updated screen
				SDL_RenderPresent(renderer);

			}

			break;
			// end players1 case

		case PLAYERS2:

			enemyList.clear();

			// reset the player
			player1.Reset();
			player2.Reset();

			players2 = true;

			//Create the enemy pool - 12
			for (int i = 0; i < 12; i++)
			{
				// create the enemy
				Enemy tmpEnemy(renderer, images_dir);

				// add to enemylist
				enemyList.push_back(tmpEnemy);

			}



			while (players2) {
				// Create deltaTime - for frame rate independence
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// *****check for input events *****
				if (SDL_PollEvent(&event)) {
					// If window is closed be X in corner
					if (event.type == SDL_QUIT) {
						quit = true;
						players2 = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0 || event.cdevice.which == 1) {
							if (event.cbutton.button
									== SDL_CONTROLLER_BUTTON_X) {
								players2 = false;
								gameState = WIN;
							}
							if (event.cbutton.button
									== SDL_CONTROLLER_BUTTON_Y) {
								players2 = false;
								gameState = LOSE;
							}
						}
							// ************************************ NEW **********************************
							// send button press info to player 1
						if(player1.active)
						player1.OnControllerButton(event.cbutton);

							// send button press info to player 2
						if(player2.active)
						player2.OnControllerButton(event.cbutton);
							// ************************************ NEW **********************************


						break;
						// ************************************ NEW **********************************
					case SDL_CONTROLLERAXISMOTION:
						// send axis info to player 1
						if(player1.active)
						player1.OnControllerAxis(event.caxis);

						// send axis info to player 2
						if(player2.active)
						player2.OnControllerAxis(event.caxis);
						break;
						// ************************************ NEW **********************************

					}
				}

				// Update Section
				UpdateBackground();

				// ************************************ NEW **********************************
				// Update Player 1
				// Week 5 ********************************************************************************
				if(player1.active)
				player1.Update(deltaTime, renderer);

				// Update Player 2
				if(player2.active)
				player2.Update(deltaTime, renderer);
				// ************************************ NEW **********************************

				// Update the enemies
				for (int i = 0; i < enemyList.size(); i++)
				{
					// update enemy
					enemyList[i].Update(deltaTime);
				}

				//Update the pool of explosions - 20
				for (int i = 0; i < explodeList.size(); i++)
				{
					// check to see if active
					if(explodeList[i].active == true){
						// draw explode
						explodeList[i].Update(deltaTime);
					}

				}

				if (player1.active) {

					// use the player 1 bullet list to see if the active bullets
					// hit any of the enemies
					for (int i = 0; i < player1.bulletList.size(); i++) {
						// check to see if this bullet is active (onscreen)
						if (player1.bulletList[i].active == true) {

							// check all enemies against the active bullet
							for (int j = 0; j < enemyList.size(); j++) {
								// if there is a collision between the two objects
								if (SDL_HasIntersection(
										&player1.bulletList[i].posRect,
										&enemyList[j].posRect)) {

									// play explosion sound
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									// reset the enemy
									enemyList[j].Reset();

									// reset the enemy
									player1.bulletList[i].Reset();

									//give the player some points
									player1.playerScore += 50;

									// check to see if there is a winning condition
									if(player1.playerScore >= 1000)
									{
										// go to win
										players2 = false;
										gameState = WIN;
									}
								}
							}

						}
					}

					// check to see if the enemies hit the player
					for (int i = 0; i < enemyList.size(); i++) {
						// if there is a collision between the two objects
						if (SDL_HasIntersection(&player1.posRect,
								&enemyList[i].posRect)) {

							// play explosion sound
							Mix_PlayChannel(-1, explosionSound, 0);

							MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);

							// reset the enemy
							enemyList[i].Reset();

							//give the player some points
							player1.playerLives -= 1;

							//if game over - player lives <= 0
							if (player1.playerLives <= 0 && player2.playerLives <= 0) {
								players2 = false;
								gameState = LOSE;
								break;
							}
						}

					}

				}


				if (player2.active) {

					// use the player 2 bullet list to see if the active bullets
					// hit any of the enemies
					for (int i = 0; i < player2.bulletList.size(); i++) {
						// check to see if this bullet is active (onscreen)
						if (player2.bulletList[i].active == true) {

							// check all enemies against the active bullet
							for (int j = 0; j < enemyList.size(); j++) {
								// if there is a collision between the two objects
								if (SDL_HasIntersection(
										&player2.bulletList[i].posRect,
										&enemyList[j].posRect)) {

									// play explosion sound
									Mix_PlayChannel(-1, explosionSound, 0);

									MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);

									// reset the enemy
									enemyList[j].Reset();

									// reset the enemy
									player2.bulletList[i].Reset();

									//give the player some points
									player2.playerScore += 50;

									// check to see if there is a winning condition
									if(player2.playerScore >= 1000)
									{
										// go to win
										players2 = false;
										gameState = WIN;
									}
								}
							}

						}
					}

					// check to see if the enemies hit the player
					for (int i = 0; i < enemyList.size(); i++) {
						// if there is a collision between the two objects
						if (SDL_HasIntersection(&player2.posRect,
								&enemyList[i].posRect)) {

							// play explosion sound
							Mix_PlayChannel(-1, explosionSound, 0);

							MakeExplosion(player2.posRect.x-32, player2.posRect.y-32);

							// reset the enemy
							enemyList[i].Reset();

							//give the player some points
							player2.playerLives -= 1;

							//if game over - player lives <= 0
							if (player1.playerLives <= 0 && player2.playerLives <= 0) {
								players2 = false;
								gameState = LOSE;
								break;
							}
						}

					}

				}



				// Draw Section
				// clear the old screen buffer
				SDL_RenderClear(renderer);

				// Draw Bkgd1
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				// Draw Bkgd2
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw Title
				//SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);

				// ************************************ NEW **********************************
				// Draw Player 1
				player1.Draw(renderer);

				// Draw Player 2
				player2.Draw(renderer);
				// ************************************ NEW **********************************

				// Draw the enemies
				for (int i = 0; i < enemyList.size(); i++)
				{
					// update enemy
					enemyList[i].Draw(renderer);
				}

				//Draw the pool of explosions - 20
				for (int i = 0; i < explodeList.size(); i++)
				{
					// check to see if active
					if(explodeList[i].active == true){
						// draw explode
						explodeList[i].Draw(renderer);
					}

				}

				// draw new, updated screen
				SDL_RenderPresent(renderer);

			}

			break;
			// end players2 case

		case WIN:

			win = true;

			while (win) {
				// Create deltaTime - for frame rate independence
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// *****check for input events *****
				if (SDL_PollEvent(&event)) {
					// If window is closed be X in corner
					if (event.type == SDL_QUIT) {
						quit = true;
						win = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0) {
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
								// ************************************ NEW **********************************
								// If player chooses main menu game
								if (menuOver) {
									win = false;
									gameState = MENU;
									menuOver = false;
								}

								// If player chooses play again game
								if (playOver) {
									win = false;
									gameState = PLAYERS1;
									playOver = false;
								}

								// ************************************ NEW **********************************
							}

						}
						break;
						// ************************************ NEW **********************************
					case SDL_CONTROLLERAXISMOTION:

						moveCursor(event.caxis);
						break;
						// ************************************ NEW **********************************
					}
				}

				// Update Section
				UpdateBackground();

				// ************************************ NEW **********************************
				UpdateCursor();

				menuOver = SDL_HasIntersection(&activePos, &menuNPos);

				playOver = SDL_HasIntersection(&activePos, &playNPos);

				// ************************************ NEW **********************************

				// Draw Section
				// clear the old screen buffer
				SDL_RenderClear(renderer);

				// Draw Bkgd1
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				// Draw Bkgd2
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw Win Text
				SDL_RenderCopy(renderer, winText, NULL, &winTextPos);

				// ************************************ NEW **********************************
				// Draw Main Menu button
				if (menuOver) {
					SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
				} else {
					SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
				}

				// Draw Play Again button
				if (playOver) {
					SDL_RenderCopy(renderer, playO, NULL, &playNPos);
				} else {
					SDL_RenderCopy(renderer, playN, NULL, &playNPos);
				}

				// ************************************ NEW **********************************

				// Draw cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// draw new, updated screen
				SDL_RenderPresent(renderer);

			}

			break;
			// end win case

		case LOSE:

			lose = true;

			while (lose) {
				// Create deltaTime - for frame rate independence
				thisTime = SDL_GetTicks();
				deltaTime = (float) (thisTime - lastTime) / 1000;
				lastTime = thisTime;

				// *****check for input events *****
				if (SDL_PollEvent(&event)) {
					// If window is closed be X in corner
					if (event.type == SDL_QUIT) {
						quit = true;
						lose = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0) {
							if (event.cbutton.button == SDL_CONTROLLER_BUTTON_A) {
								// ************************************ NEW **********************************
								// If player chooses main menu game
								if (menuOver) {
									lose = false;
									gameState = MENU;
									menuOver = false;
								}

								// If player chooses play again game
								if (playOver) {
									lose = false;
									gameState = PLAYERS1;
									playOver = false;
								}

								// ************************************ NEW **********************************
							}

						}
						break;
						// ************************************ NEW **********************************
					case SDL_CONTROLLERAXISMOTION:

						moveCursor(event.caxis);
						break;
						// ************************************ NEW **********************************
					}
				}

				// Update Section
				UpdateBackground();

				// ************************************ NEW **********************************
				UpdateCursor();

				menuOver = SDL_HasIntersection(&activePos, &menuNPos);

				playOver = SDL_HasIntersection(&activePos, &playNPos);

				// ************************************ NEW **********************************

				// Draw Section
				// clear the old screen buffer
				SDL_RenderClear(renderer);

				// Draw Bkgd1
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				// Draw Bkgd2
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw Win Text
				SDL_RenderCopy(renderer, loseText, NULL, &loseTextPos);

				// ************************************ NEW **********************************
				// Draw Main Menu button
				if (menuOver) {
					SDL_RenderCopy(renderer, menuO, NULL, &menuNPos);
				} else {
					SDL_RenderCopy(renderer, menuN, NULL, &menuNPos);
				}

				// Draw Play Again button
				if (playOver) {
					SDL_RenderCopy(renderer, playO, NULL, &playNPos);
				} else {
					SDL_RenderCopy(renderer, playN, NULL, &playNPos);
				}

				// ************************************ NEW **********************************

				// Draw cursor
				SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

				// draw new, updated screen
				SDL_RenderPresent(renderer);
			}

			break;
			// end lose case

		} // end switch(gameState)

	}

	// message
	cout << "Player quit the game" << endl;

	//SDL_Delay(3000);

	// Close and destroy the window
	SDL_DestroyWindow(window);

	// Clean up
	SDL_Quit();
	return 0;
}
