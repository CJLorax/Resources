#if defined(_WIN32) || (_WIN64)

#include "SDL.h"
#include "SDL_image.h"

#endif

#if defined(__APPLE__)

#include "SDL2/SDL.h"
#include "SDL2_image/SDL_image.h"

#endif

#if defined(__linux__)

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

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
}

// variables for all Menus button over
bool players1Over = false, players2Over = false, instructionsOver = false,
		quitOver = false, menuOver = false, playOver = false;

// ************************************ NEW **********************************

int main(int argc, char* argv[]) {

#if defined(_WIN32) || (_WIN64)

	std::cout << "Running on Windows..." << endl;
	std::cout << "Added on Windows..." << endl;

	string currentWorkingDirectory(getcwd(NULL, 0));

	string images_dir = currentWorkingDirectory + "\\Resources\\Images\\";

#endif

#if defined(__APPLE__)

	cout << "Running on Apple..." << endl;

	// string var to hold the current working directory on __APPLE__
	string currentWorkingDirectory(getcwd(NULL, 0));

	// create a string to link to the images folder on __APPLE__
	string images_dir = currentWorkingDirectory + "/Resources/Images/";

#endif

#if defined(__linux__)

	cout << "Running on Linux..." << endl;
	cout << "Added on the Linux platform..." << endl;

	// string var to hold the current working directory on __APPLE__
	string currentWorkingDirectory(getcwd(NULL,0));

	// create a string to link to the images folder on __APPLE__
	string images_dir = currentWorkingDirectory +"/Resources/Images/";

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

	// create the SDL surface to hold the texture file
	SDL_Surface *surface = IMG_Load((images_dir + "bkgd.png").c_str());

	if (surface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n",
				images_dir.c_str(), IMG_GetError());
	}

	// create in game texture - background 1
	SDL_Texture *bkgd1;

	// place surface with image to display in the texture
	bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

	// create in game texture - background 2
	SDL_Texture *bkgd2;

	// place surface with image to display in the texture
	bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

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

	// ***************** Create Backgrounds - END *************

	// ***************** Create Cursor - START *************
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "cursor.png").c_str());

	// create in game texture - 2 player button
	SDL_Texture *cursor;

	// place surface with image to display in the texture
	cursor = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

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

	// ***************** Create Cursor - END *************

	// ***************** Create INSTRUCTIONS Menu - START *************

	// ******** INSTRUCTIONS GRAPHICS  - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "instructions.png").c_str());

	// create in game texture - background 1
	SDL_Texture *instructionsText;

	// place surface with image to display in the texture
	instructionsText = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect instructionsTextPos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	instructionsTextPos.x = 120;
	instructionsTextPos.y = 271;
	instructionsTextPos.w = 781;
	instructionsTextPos.h = 258;
	// ******** INSTRUCTIONS GRAPHICS - end

	// ******** Small Menu Button   - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "menu_n.png").c_str());

	// create in game texture - background 1
	SDL_Texture *menuN;

	// place surface with image to display in the texture
	menuN = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "menu_o.png").c_str());

	// create in game texture - background 1
	SDL_Texture *menuO;

	// place surface with image to display in the texture
	menuO = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect menuNPos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	menuNPos.x = 22;
	menuNPos.y = 702;
	menuNPos.w = 145;
	menuNPos.h = 41;
	// ******** Small Menu Button - end

	// ***************** Create Instructions Menu - END *************

	// ***************** Create MAIN MENU - START *************

	// ******** TITLE - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "title.png").c_str());

	// create in game texture - background 1
	SDL_Texture *title;

	// place surface with image to display in the texture
	title = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect titlePos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	titlePos.x = 264;
	titlePos.y = 73;
	titlePos.w = 532;
	titlePos.h = 71;
	// ******** TITLE - end

	// ******** 1 PLAYER - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "one_n.png").c_str());

	// create in game texture - background 1
	SDL_Texture *players1N;

	// place surface with image to display in the texture
	players1N = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "one_o.png").c_str());

	// create in game texture - background 1
	SDL_Texture *players1O;

	// place surface with image to display in the texture
	players1O = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect players1NPos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	players1NPos.x = 380;
	players1NPos.y = 261;
	players1NPos.w = 311;
	players1NPos.h = 42;
	// ******** 1 PLAYER - end

	// ******** 2 PLAYER - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "two_n.png").c_str());

	// create in game texture - 2 player button
	SDL_Texture *players2N;

	// place surface with image to display in the texture
	players2N = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "two_o.png").c_str());

	// create in game texture - background 1
	SDL_Texture *players2O;

	// place surface with image to display in the texture
	players2O = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the 2 player  graphic
	SDL_Rect players2NPos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - 2 players
	players2NPos.x = 360;
	players2NPos.y = 343;
	players2NPos.w = 352;
	players2NPos.h = 42;
	// ******** 2 PLAYER - end

	// ******** INSTRUCTIONS - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "instruct_n.png").c_str());

	// create in game texture - 2 player button
	SDL_Texture *instructN;

	// place surface with image to display in the texture
	instructN = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "instruct_o.png").c_str());

	// create in game texture - background 1
	SDL_Texture *instructO;

	// place surface with image to display in the texture
	instructO = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the 2 player  graphic
	SDL_Rect instructNPos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - 2 players
	instructNPos.x = 351;
	instructNPos.y = 424;
	instructNPos.w = 374;
	instructNPos.h = 42;
	// ******** INSTRUCTIONS - end

	// ******** QUIT GAME - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "quit_n.png").c_str());

	// create in game texture - 2 player button
	SDL_Texture *quitN;

	// place surface with image to display in the texture
	quitN = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "quit_o.png").c_str());

	// create in game texture - background 1
	SDL_Texture *quitO;

	// place surface with image to display in the texture
	quitO = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the 2 player  graphic
	SDL_Rect quitNPos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - 2 players
	quitNPos.x = 391;
	quitNPos.y = 503;
	quitNPos.w = 288;
	quitNPos.h = 42;
	// ******** INSTRUCTIONS - end

	// ***************** Create MIN MENU - END *************

	// ***************** Create WIN MENU - START *************

	// ******** WIN GRAPHICS  - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "win.png").c_str());

	// create in game texture - background 1
	SDL_Texture *winText;

	// place surface with image to display in the texture
	winText = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect winTextPos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	winTextPos.x = 240;
	winTextPos.y = 288;
	winTextPos.w = 596;
	winTextPos.h = 114;
	// ******** WIN GRAPHICS - end

	// ******** Small PLAY AGAIN Button   - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "play_n.png").c_str());

	// create in game texture - background 1
	SDL_Texture *playN;

	// place surface with image to display in the texture
	playN = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "play_o.png").c_str());

	// create in game texture - background 1
	SDL_Texture *playO;

	// place surface with image to display in the texture
	playO = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect playNPos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	playNPos.x = 875;
	playNPos.y = 702;
	playNPos.w = 125;
	playNPos.h = 40;
	// ******** Small Menu Button - end

	// ***************** Create WIN Menu - END *************

	// ***************** Create LOSE Menu - START *************

	// ******** Lose GRAPHICS  - START
	// create the SDL surface to hold the texture file
	surface = IMG_Load((images_dir + "lose.png").c_str());

	// create in game texture - background 1
	SDL_Texture *loseText;

	// place surface with image to display in the texture
	loseText = SDL_CreateTextureFromSurface(renderer, surface);

	//Release the SDL surface for later use
	SDL_FreeSurface(surface);

	// create SDL Rectangle for the title graphic
	SDL_Rect loseTextPos;

	// the rectangle which has the X pos, Ypos, texture Width and texture Height - title
	loseTextPos.x = 148;
	loseTextPos.y = 288;
	loseTextPos.w = 778;
	loseTextPos.h = 114;
	// ******** Lose GRAPHICS - end
	// ***************** Create LOSE Menu - END *************

	// ***** set up a Game Controller variable *****
	SDL_GameController* gGameController = NULL;

	// ***** Open Game Controller *****
	gGameController = SDL_GameControllerOpen(0);

	// ***** Turn on Game Controller Events *****
	SDL_GameControllerEventState(SDL_ENABLE);

	// ***** SDL event to handle input *****
	SDL_Event event;

	// set up various game states the game can be placed in
	enum GameState {
		MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE
	};
	GameState gameState = MENU;
	bool menu, instructions, players1, players2, win, lose, quit;

	quit = false;

	// The window is open: could enter program loop here (see SDL_PollEvent())

	// ***** basic game loop *****
	while (!quit) {

		switch (gameState) {
		case MENU:

			menu = true;
/*
			cout << "The Game State is Menu" << endl;
			cout << "Press A Button for Instructions" << endl;
			cout << "Press B Button for 1 Player Game" << endl;
			cout << "Press X Button for 2 Player Game" << endl;
			cout << "Press Y Button for Quit Game" << endl;
			cout << endl;
*/
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
									menu = false;
									gameState = PLAYERS1;
									players1Over = false;
								}

								// If player chooses 2 player game
								if (players2Over) {
									menu = false;
									gameState = PLAYERS2;
									players2Over = false;
								}

								// If player chooses Instructions
								if (instructionsOver) {
									menu = false;
									gameState = INSTRUCTIONS;
									players2Over = false;
								}

								// If player chooses to Quit Game
								if (quitOver) {
									menu = false;
									quit = true;
									players2Over = false;
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

				// check for collision between cursor active state and player 1 button
				if (SDL_HasIntersection(&activePos, &players1NPos)) {
					if (!players1Over) {
						//cout << "Within Button" << endl;
						players1Over = true;
						// play audio sound from bullet
						//Mix_PlayChannel(-1, over, 0);
					}
				} else {
					if (players1Over) {
						//cout << "Outside Button" << endl;
						players1Over = false;
					}
				}

				// check for collision between cursor active state and player 2 button
				if (SDL_HasIntersection(&activePos, &players2NPos)) {
					if (!players2Over) {
						//cout << "Within Button" << endl;
						players2Over = true;
						// play audio sound from bullet
						//Mix_PlayChannel(-1, over, 0);
					}
				} else {
					if (players2Over) {
						//cout << "Outside Button" << endl;
						players2Over = false;
					}
				}

				// check for collision between cursor active state and player 2 button
				if (SDL_HasIntersection(&activePos, &instructNPos)) {
					if (!instructionsOver) {
						//cout << "Within Button" << endl;
						instructionsOver = true;
						// play audio sound from bullet
						//Mix_PlayChannel(-1, over, 0);
					}
				} else {
					if (instructionsOver) {
						//cout << "Outside Button" << endl;
						instructionsOver = false;
					}
				}

				// check for collision between cursor active state and player 2 button
				if (SDL_HasIntersection(&activePos, &quitNPos)) {
					if (!quitOver) {
						//cout << "Within Button" << endl;
						quitOver = true;
						// play audio sound from bullet
						//Mix_PlayChannel(-1, over, 0);
					}
				} else {
					if (quitOver) {
						//cout << "Outside Button" << endl;
						quitOver = false;
					}
				}

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
/*
			cout << "The Game State is Instructions" << endl;
			cout << "Press A Button for Main Menu" << endl;
			cout << endl;
*/

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

				// check for collision between cursor active state and Main Menu button
				if (SDL_HasIntersection(&activePos, &menuNPos)) {
					if (!menuOver) {
						//cout << "Within Button" << endl;
						menuOver = true;
						// play audio sound from bullet
						//Mix_PlayChannel(-1, over, 0);
					}
				} else {
					if (menuOver) {
						//cout << "Outside Button" << endl;
						menuOver = false;
					}
				}

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

			players1 = true;
/*
			cout << "The Game State is Players1" << endl;
			cout << "Press A Button for Win Screen" << endl;
			cout << "Press B Button for Lose Screen" << endl;
			cout << endl;
*/
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
									== SDL_CONTROLLER_BUTTON_A) {
								players1 = false;
								gameState = WIN;
							}
							if (event.cbutton.button
									== SDL_CONTROLLER_BUTTON_B) {
								players1 = false;
								gameState = LOSE;
							}
						}
						break;

					}
				}

				// Update Section
				UpdateBackground();

				// Draw Section
				// clear the old screen buffer
				SDL_RenderClear(renderer);

				// Draw Bkgd1
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				// Draw Bkgd2
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw Title
				SDL_RenderCopy(renderer, players1N, NULL, &players1NPos);

				// draw new, updated screen
				SDL_RenderPresent(renderer);

			}

			break;
			// end players1 case

		case PLAYERS2:

			players2 = true;

/*
			cout << "The Game State is Players2" << endl;
			cout << "Press A Button for Win Screen" << endl;
			cout << "Press B Button for Lose Screen" << endl;
			cout << endl;
*/

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
						players1 = false;
						break;
					}

					switch (event.type) {
					case SDL_CONTROLLERBUTTONDOWN:
						if (event.cdevice.which == 0) {
							if (event.cbutton.button
									== SDL_CONTROLLER_BUTTON_A) {
								players2 = false;
								gameState = WIN;
							}
							if (event.cbutton.button
									== SDL_CONTROLLER_BUTTON_B) {
								players2 = false;
								gameState = LOSE;
							}
						}
						break;

					}
				}

				// Update Section
				UpdateBackground();

				// Draw Section
				// clear the old screen buffer
				SDL_RenderClear(renderer);

				// Draw Bkgd1
				SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

				// Draw Bkgd2
				SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

				// Draw Title
				SDL_RenderCopy(renderer, players2N, NULL, &players2NPos);

				// draw new, updated screen
				SDL_RenderPresent(renderer);

			}

			break;
			// end players2 case

		case WIN:

			win = true;
/*
			cout << "The Game State is Win" << endl;
			cout << "Press A Button for Main Menu" << endl;
			cout << "Press B Button to Replay Game" << endl;
			cout << endl;
*/
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

				// check for collision between cursor active state and Main Menu button
				if (SDL_HasIntersection(&activePos, &menuNPos)) {
					if (!menuOver) {
						//cout << "Within Button" << endl;
						menuOver = true;
						// play audio sound from bullet
						//Mix_PlayChannel(-1, over, 0);
					}
				} else {
					if (menuOver) {
						//cout << "Outside Button" << endl;
						menuOver = false;
					}
				}
				// check for collision between cursor active state and play again button
				if (SDL_HasIntersection(&activePos, &playNPos)) {
					if (!playOver) {
						//cout << "Within Button" << endl;
						playOver = true;
						// play audio sound from bullet
						//Mix_PlayChannel(-1, over, 0);
					}
				} else {
					if (playOver) {
						//cout << "Outside Button" << endl;
						playOver = false;
					}
				}

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
/*
			cout << "The Game State is Lose" << endl;
			cout << "Press A Button for Main Menu" << endl;
			cout << "Press B Button to Replay Game" << endl;
			cout << endl;
*/
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

				// check for collision between cursor active state and Main Menu button
				if (SDL_HasIntersection(&activePos, &menuNPos)) {
					if (!menuOver) {
						//cout << "Within Button" << endl;
						menuOver = true;
						// play audio sound from bullet
						//Mix_PlayChannel(-1, over, 0);
					}
				} else {
					if (menuOver) {
						//cout << "Outside Button" << endl;
						menuOver = false;
					}
				}
				// check for collision between cursor active state and play again button
				if (SDL_HasIntersection(&activePos, &playNPos)) {
					if (!playOver) {
						//cout << "Within Button" << endl;
						playOver = true;
						// play audio sound from bullet
						//Mix_PlayChannel(-1, over, 0);
					}
				} else {
					if (playOver) {
						//cout << "Outside Button" << endl;
						playOver = false;
					}
				}

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
