
#if defined(_WIN32) || (_WIN64)

	#include "SDL.h"

#endif


#if defined(__APPLE__)

	#include "SDL2/SDL.h"

#endif

#if defined(__linux__)

	#include "SDL2/SDL.h"

#endif


#include <stdio.h>
#include <iostream>
using namespace std;



int main(int argc, char* argv[]) {

#if defined(_WIN32) || (_WIN64)

	cout << "Running on Windows..." << endl;
	cout << "Added on Windows..." << endl;

#endif


#if defined(__APPLE__)

	cout << "Running on Apple..." << endl;

#endif

#if defined(__linux__)

	cout << "Running on Linux..." << endl;
	cout << "Added on the Linux platform..." << endl;

#endif



    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        640,                               // width, in pixels
        480,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }


	//The surface contained by the window
	SDL_Surface* screenSurface = NULL;

	//Get window surface
	screenSurface = SDL_GetWindowSurface( window );

	//Fill the surface white
	SDL_FillRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0, 42, 254 ) );

	//Update the surface
	SDL_UpdateWindowSurface( window );


	// ***** set up a Game Controller variable *****
	SDL_GameController* gGameController = NULL;

	// ***** Open Game Controller *****
	gGameController = SDL_GameControllerOpen(0);

	// ***** Turn on Game Controller Events *****
	SDL_GameControllerEventState(SDL_ENABLE);

	// ***** SDL event to handle input *****
	SDL_Event event;

    // set up various game states the game can be placed in
    enum GameState { MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE};
    GameState gameState = MENU;
    bool menu, instructions, players1, players2, win, lose, quit;



	// The window is open: could enter program loop here (see SDL_PollEvent())

	// ***** basic game loop *****
	while(!quit)
	{

		switch(gameState)
		{
			case MENU:

				menu = true;
				cout << "The Game State is Menu" << endl;
				cout << "Press A Button for Instructions" << endl;
				cout << "Press B Button for 1 Player Game" << endl;
				cout << "Press X Button for 2 Player Game" << endl;
				cout << "Press Y Button for Quit Game" << endl;
				cout << endl;
				while(menu)
				{
					// *****check for input events *****
					if (SDL_PollEvent(&event)) {
						// If window is closed be X in corner
						if (event.type == SDL_QUIT) {
							quit = true;
							menu = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0){
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A){
										menu = false;
										gameState = INSTRUCTIONS;
									}
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B){
										menu = false;
										gameState = PLAYERS1;
									}
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X){
										menu = false;
										gameState = PLAYERS2;
									}
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y){
										quit = true;
										menu = false;
									}
								}
								break;

						}
					}
				}

				break;
				// end main menu case

			case INSTRUCTIONS:

				instructions = true;
				cout << "The Game State is Instructions" << endl;
				cout << "Press A Button for Main Menu" << endl;
				cout << endl;
				while(instructions)
				{
					// *****check for input events *****
					if (SDL_PollEvent(&event)) {
						// If window is closed be X in corner
						if (event.type == SDL_QUIT) {
							quit = true;
							menu = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0){
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A){
										instructions = false;
										gameState = MENU;
									}
								}
								break;

						}
					}
				}

				break;
				// end instructions case



			case PLAYERS1:

				players1 = true;
				cout << "The Game State is Players1" << endl;
				cout << "Press A Button for Win Screen" << endl;
				cout << "Press B Button for Lose Screen" << endl;
				cout << endl;
				while(players1)
				{
					// *****check for input events *****
					if (SDL_PollEvent(&event)) {
						// If window is closed be X in corner
						if (event.type == SDL_QUIT) {
							quit = true;
							players1 = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0){
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A){
										players1 = false;
										gameState = WIN;
									}
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B){
										players1 = false;
										gameState = LOSE;
									}
								}
								break;

						}
					}
				}

				break;
				// end players1 case


			case PLAYERS2:

				players2 = true;
				cout << "The Game State is Players2" << endl;
				cout << "Press A Button for Win Screen" << endl;
				cout << "Press B Button for Lose Screen" << endl;
				cout << endl;
				while(players2)
				{
					// *****check for input events *****
					if (SDL_PollEvent(&event)) {
						// If window is closed be X in corner
						if (event.type == SDL_QUIT) {
							quit = true;
							players1 = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0){
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A){
										players2 = false;
										gameState = WIN;
									}
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B){
										players2 = false;
										gameState = LOSE;
									}
								}
								break;

						}
					}
				}

				break;
				// end players2 case


			case WIN:

				win = true;
				cout << "The Game State is Win" << endl;
				cout << "Press A Button for Main Menu" << endl;
				cout << "Press B Button to Quit Game" << endl;
				cout << endl;
				while(win)
				{
					// *****check for input events *****
					if (SDL_PollEvent(&event)) {
						// If window is closed be X in corner
						if (event.type == SDL_QUIT) {
							quit = true;
							win = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0){
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A){
										win = false;
										gameState = MENU;
									}
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B){
										quit = true;
										win = false;
									}
								}
								break;

						}
					}
				}

				break;
				// end win case



			case LOSE:

				lose = true;
				cout << "The Game State is Lose" << endl;
				cout << "Press A Button for Main Menu" << endl;
				cout << "Press B Button to Quit Game" << endl;
				cout << endl;
				while(lose)
				{
					// *****check for input events *****
					if (SDL_PollEvent(&event)) {
						// If window is closed be X in corner
						if (event.type == SDL_QUIT) {
							quit = true;
							lose = false;
							break;
						}

						switch(event.type)
						{
							case SDL_CONTROLLERBUTTONDOWN:
								if(event.cdevice.which == 0){
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A){
										lose = false;
										gameState = MENU;
									}
									if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B){
										quit = true;
										lose = false;
									}
								}
								break;

						}
					}
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
