 #include "player.h"

//Analog joystick dead zone
const int JOYSTICK_DEAD_ZONE = 8000;

// PLayer creation Method
Player::Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y)
{
	// set the player number 0 or 1
	playerNum = pNum;

	// set float for player speed
	speed = 500.0f;

	// see if this is player 1, or player 2, and create the correct file path
	if(playerNum == 0){
		//Create the player 1 texture
		playerPath = filePath + "player.png";
	}else{
		//Create the player 2 texture
		playerPath = filePath + "player2.png";
	}

	// load the surface
	surface = IMG_Load(playerPath.c_str());

	// load the surface into the texture
	texture = SDL_CreateTextureFromSurface(renderer, surface);

	// free the surface for later use
	SDL_FreeSurface(surface);

	// set the SDL_Rect X and Y for the player
	posRect.x = x;

	posRect.y = y;

	// Use SDL_QueryTexture to get the W and H of the player's texture
	int w, h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;

	posRect.h = h;

	// Set the movement floats to the players original X and Y
	pos_X = x;

	pos_Y = y;
}

// Player Update method
void Player::Update(float deltaTime)
{
	// Adjust position floats based on speed, direction of joystick axis and deltaTime
	pos_X += (speed * xDir) * deltaTime;
	pos_Y += (speed * yDir) * deltaTime;

	// Update player position with code to account for precision loss
	posRect.x = (int)(pos_X + 0.5f);
	posRect.y = (int)(pos_Y + 0.5f);

}

// Player Draw method
void Player::Draw(SDL_Renderer *renderer)
{
	// Draw the player texture using the vars texture and posRect
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

// Player Destruction method
Player::~Player()
{
	SDL_DestroyTexture(texture);

}

// Player Joystick Button Method
void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	// if the player's number is 0 and the joystick button is from joystick 0
	if (event.which == 0 && playerNum == 0)
	{
		// if A Button
		if (event.button == 0)
		{
			cout << "Player 1 - Button A" << endl;

		}

	}

	// if the player's number is 1 and the joystick button is from joystick 1
	if (event.which == 1 && playerNum == 1)
	{
		// if A Button
		if (event.button == 0)
		{
			cout << "Player 2 - Button A" << endl;

		}

	}

}

// Player Joystick Axis Method
void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	// Axis movements and button presses both sent here as SDL_ControllerAxisEvent structures

	// if the player's number is 0 and the joystick axis is from joystick 0
	if (event.which == 0 && playerNum == 0)
	{
		// X axis
		if (event.axis == 0)
		{

			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f; // LEFT
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f; // RIGHT
			}
			else
			{
				xDir = 0.0f; // NONE
			}
		}

		// Y axis
		if (event.axis == 1)
		{

			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f; // DOWN
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f; // UP
			}
			else
			{
				yDir = 0.0f; // NONE
			}
		}

	}

	// if the player's number is 1 and the joystick axis is from joystick 1
	if (event.which == 1 && playerNum == 1)
	{
		// X axis
		if (event.axis == 0)
		{

			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f; // LEFT
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f; // RIGHT
			}
			else
			{
				xDir = 0.0f; // NONE
			}
		}

		// Y axis
		if (event.axis == 1)
		{

			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f; // DOWN
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f; // UP
			}
			else
			{
				yDir = 0.0f; // NONE
			}
		}

	}

}



