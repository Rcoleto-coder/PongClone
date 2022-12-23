// PongClone
// This is a clone of the classic Pong game based on Austin Morlan's Tutorial:
// https://austinmorlan.com/posts/pong_clone/
// SDL Tutorials: https://lazyfoo.net/tutorials/SDL/
// Sound effects from: https://freesound.org/people/NoiseCollector/sounds/4359/
//			 https://freesound.org/people/NoiseCollector/sounds/4360/


#include <chrono>
#include <SDL.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <iostream>
#include "Vec2.h"
#include "Ball.h"
#include "Paddle.h"
#include "PlayerScore.h"
#include "Contact.h"


//Screen dimension constants
const int WINDOW_WIDTH = 1280;
const int WINDOW_HEIGHT = 720;

//Ball dimensions
const int BALL_WIDTH = 15;
const int BALL_HEIGHT = 15;
const float BALL_SPEED = 1.0f;

//Paddle dimensions
const int PADDLE_HEIGHT = 100;
const int PADDLE_WIDTH = 10;
const float PADDLE_SPEED = 1.0f;

// Key's states
enum Buttons 
{
    PaddleOneUp = 0,
    PaddleOneDown,
    PaddleTwoUp,
    PaddleTwoDown
};

//Function prototypes
void drawNet(SDL_Renderer* renderer, const int window_height ,const int window_width);
Contact CheckPaddleCollision(Ball const& ball, Paddle const& paddle);
Contact CheckWallCollision(Ball const& ball, const int window_width, const int window_height);
bool isTopCollision(const float ballBottom, const float paddleTop, const float paddleRangeUpper);
bool isMiddleCollision(const float ballBottom, const float paddleRangeMiddle, const float paddleRangeUpper);
void gameRun(SDL_Window* window, SDL_Renderer* renderer);

int main(int argc, char* args[])
{
    //The window to render
    SDL_Window* window = nullptr;
    
    SDL_Renderer* renderer = nullptr;

    Vec2 vec;
    std::cout << vec << std::endl;
    
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
	   printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    }
    //Initialize SDL FONT
    else if (TTF_Init() < 0) {
	   printf("TTF could not initialize! SDL_Error: %s\n", TTF_GetError());
    }
    //Initialize SDL AUDIO
    else if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
	   printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
    else
    {   // Initialize Mixer with default values and run the game
	   Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	   gameRun(window, renderer);
    }
    return 0;
}

//Function implmentations
void gameRun(SDL_Window* window, SDL_Renderer* renderer) {
    //Create window
    window = SDL_CreateWindow("Pong", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, 0);

    // Initialize the font
    TTF_Font* scoreFont = TTF_OpenFont("./fonts/DejaVuSansMono.ttf", 40);

    // Initialize sound effects
    Mix_Chunk* wallHitSound = Mix_LoadWAV("./sound/WallHit.wav");
    Mix_Chunk* paddleHitSound = Mix_LoadWAV("./sound/PaddleHit.wav");

    if (!scoreFont) {
	   std::cout << "Font not found";
    }

    if (window == NULL || renderer == NULL)
    {
	   printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    }
    else
    {

	   bool running = true;
	   float dt = 0.0f;
	   bool buttons[4] = {};
	   int playerOneScore = 0;
	   int playerTwoScore = 0;

	   // Create the player score text fields
	   PlayerScore playerOneScoreText(Vec2(WINDOW_WIDTH / 4, 20), renderer, scoreFont);
	   PlayerScore playerTwoScoreText(Vec2(3 * WINDOW_WIDTH / 4, 20), renderer, scoreFont);

	   //Create a Ball
	   Vec2 ballPosition((WINDOW_WIDTH / 2.0f) - (BALL_WIDTH / 2.0f),
		  (WINDOW_HEIGHT / 2.0f) - (BALL_WIDTH / 2.0f));

	   Vec2 ballVelocity(BALL_SPEED, 0.0f);

	   Ball ball(BALL_HEIGHT, BALL_WIDTH, ballPosition, ballVelocity);

	   // Create the paddles
	   Vec2 paddleOnePosition (50.0f, (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f));
	   Vec2 paddleOneVelocity (0.0f, 0.0f);

	   Paddle paddleOne(PADDLE_HEIGHT, 
				    PADDLE_WIDTH,
				    paddleOnePosition, 
				    paddleOneVelocity);

	   Vec2 paddleTwoPosition (WINDOW_WIDTH - 50.0f - PADDLE_WIDTH,
						 (WINDOW_HEIGHT / 2.0f) - (PADDLE_HEIGHT / 2.0f));

	   Vec2 paddleTwoVelocity(0.0f, 0.0f);

	   Paddle paddleTwo(PADDLE_HEIGHT, 
				    PADDLE_WIDTH,
				    paddleTwoPosition,
				    paddleTwoVelocity);

	   // Continue looping and processing events until user exits
	   while (running)
	   {
		  auto startTime = std::chrono::high_resolution_clock::now();

		  SDL_Event event;
		  while (SDL_PollEvent(&event))
		  {
			 // Handling input
			 if (event.type == SDL_QUIT)
			 {
				running = false;
			 }
			 else if (event.type == SDL_KEYDOWN)
			 {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				    running = false;
				    break;
				case SDLK_w:
				    buttons[Buttons::PaddleOneUp] = true;

				    break;
				case SDLK_s:
				    buttons[Buttons::PaddleOneDown] = true;

				    break;
				case SDLK_UP:
				    buttons[Buttons::PaddleTwoUp] = true;

				    break;
				case SDLK_DOWN:
				    buttons[Buttons::PaddleTwoDown] = true;

				    break;
				}
			 }
			 else if (event.type == SDL_KEYUP) {
				switch (event.key.keysym.sym) {
				case SDLK_ESCAPE:
				    running = false;
				    break;
				case SDLK_w:
				    buttons[Buttons::PaddleOneUp] = false;
				    break;
				case SDLK_s:
				    buttons[Buttons::PaddleOneDown] = false;
				    break;
				case SDLK_UP:
				    buttons[Buttons::PaddleTwoUp] = false;
				    break;
				case SDLK_DOWN:
				    buttons[Buttons::PaddleTwoDown] = false;
				    break;
				}
			 }
		  }
		  if (buttons[Buttons::PaddleOneUp])
		  {
			 paddleOne.setVelocity(-PADDLE_SPEED);
			 std::cout << paddleOne.getVelocity() << std::endl;
		  }
		  else if (buttons[Buttons::PaddleOneDown])
		  {
			 paddleOne.setVelocity(PADDLE_SPEED);
			 std::cout << paddleOne.getVelocity() << std::endl;
		  }
		  else
		  {
			 paddleOne.setVelocity(0.0f);
		  }
		  if (buttons[Buttons::PaddleTwoUp])
		  {
			 paddleTwo.setVelocity(-PADDLE_SPEED);
		  }
		  else if (buttons[Buttons::PaddleTwoDown])
		  {
			 paddleTwo.setVelocity(PADDLE_SPEED);
		  }
		  else
		  {
			 paddleTwo.setVelocity(0.0f);
		  }

		  // Update the paddles positions
		  paddleOne.Update(dt, WINDOW_HEIGHT, PADDLE_HEIGHT);
		  paddleTwo.Update(dt, WINDOW_HEIGHT, PADDLE_HEIGHT);

		  // Update the ball position
		  ball.Update(dt);

		  // Check for collisions
		  Contact collidedOne = CheckPaddleCollision(ball, paddleOne);
		  Contact collidedTwo = CheckPaddleCollision(ball, paddleTwo);
		  Contact collidedWall = CheckWallCollision(ball, WINDOW_WIDTH, WINDOW_HEIGHT);

		  if (collidedOne.type != CollisionType::None)
		  {
			 ball.CollideWithPaddle(collidedOne, BALL_SPEED);
			 Mix_PlayChannel(-1, paddleHitSound, 0);
		  }
		  else if (collidedTwo.type != CollisionType::None)

		  {
			 ball.CollideWithPaddle(collidedTwo, BALL_SPEED);
			 Mix_PlayChannel(-1, paddleHitSound, 0);
		  }
		  else if (collidedWall.type != CollisionType::None)
		  {
			 ball.CollideWithWall(collidedWall, WINDOW_WIDTH, WINDOW_HEIGHT, BALL_SPEED);

			 if (collidedWall.type == CollisionType::Left)
			 {
				++playerTwoScore;

				playerTwoScoreText.SetScore(playerTwoScore);
			 }
			 else if (collidedWall.type == CollisionType::Right)
			 {
				++playerOneScore;

				playerOneScoreText.SetScore(playerOneScore);
			 }
			 else
			 {
				Mix_PlayChannel(-1, wallHitSound, 0);
			 }
		  }

		  // Clear the window to black
		  SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
		  SDL_RenderClear(renderer);

		  // Rendering will happen here

		  // Draw the net
		  drawNet(renderer, WINDOW_HEIGHT, WINDOW_WIDTH);

		  // Draw the ball
		  ball.Draw(renderer);

		  // Draw the paddles
		  paddleOne.Draw(renderer);
		  paddleTwo.Draw(renderer);

		  // Display the scores
		  playerOneScoreText.Draw();
		  playerTwoScoreText.Draw();

		  // Present the backbuffer
		  SDL_RenderPresent(renderer);

		  // Calculate frame time
		  auto stopTime = std::chrono::high_resolution_clock::now();
		  dt = std::chrono::duration<float, std::chrono::milliseconds::period>(stopTime - startTime).count();

	   }
    }
    // Clean up
    SDL_DestroyRenderer(renderer);
    TTF_CloseFont(scoreFont);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(wallHitSound);
    Mix_FreeChunk(paddleHitSound);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

void drawNet(SDL_Renderer* renderer, const int window_height, const int window_width) {
    // Set the draw color to be green
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    for (int y = 0; y < window_height; ++y)
    {
	   if (y % 5)
	   {
		  SDL_RenderDrawPoint(renderer, window_width / 2, y);
	   }
    }
}

bool isTopCollision(const float ballBottom, const float paddleTop, const float paddleRangeUpper) {
    if ((ballBottom > paddleTop) && (ballBottom < paddleRangeUpper))
	   return true;
    else
	   return false;
}
bool isMiddleCollision(const float ballBottom, const float paddleRangeMiddle, const float paddleRangeUpper) {
    if ((ballBottom > paddleRangeUpper) && (ballBottom < paddleRangeMiddle))
	   return true;
    else
	   return false;
}

Contact CheckPaddleCollision(Ball const& ball, Paddle const& paddle)
{

    float ballLeft = ball.position.x;
    float ballRight = ball.position.x + ball.rect.w;
    float ballTop = ball.position.y;
    float ballBottom = ball.position.y + ball.rect.h;

    float paddleLeft = paddle.position.x;
    float paddleRight = paddle.position.x + paddle.rect.w;
    float paddleTop = paddle.position.y;
    float paddleBottom = paddle.position.y + paddle.rect.h;

    float paddleRangeUpper = paddleBottom - (2.0f * paddle.rect.h / 3.0f);
    float paddleRangeMiddle = paddleBottom - (paddle.rect.h / 3.0f);

    Contact contact{};

    if (ballLeft >= paddleRight)
    {
	   return contact;
    }

    if (ballRight <= paddleLeft)
    {
	   return contact;
    }

    if (ballTop >= paddleBottom)
    {
	   return contact;
    }

    if (ballBottom <= paddleTop)
    {
	   return contact;
    }

    //if the x coordinate of velocity is negative, the ball is moving to the left
    if (ball.velocity.x < 0)
    {
	   // Left paddle
	   contact.penetration = paddleRight - ballLeft;

    }//if the x coordinate of velocity is positive, the ball is moving to the right
    else if (ball.velocity.x > 0)
    {
	   // Right paddle
	   contact.penetration = paddleLeft - ballRight;
    }

    if (isTopCollision(ballBottom, paddleTop, paddleRangeUpper))
    {
	   contact.type = CollisionType::Top;
    }

    else if (isMiddleCollision(ballBottom, paddleRangeMiddle, paddleRangeUpper))
    {
	   contact.type = CollisionType::Middle;
    }
    else
    {
	   contact.type = CollisionType::Bottom;
    }

    return contact;
}

Contact CheckWallCollision(Ball const& ball, const int window_width, const int window_height)
{
    float ballLeft = ball.position.x;
    float ballRight = ball.position.x + ball.rect.w;
    float ballTop = ball.position.y;
    float ballBottom = ball.position.y + ball.rect.h;

    Contact contact{};

    if (ballLeft < 0.0f)
    {
	   contact.type = CollisionType::Left;
    }
    else if (ballRight > window_width)
    {
	   contact.type = CollisionType::Right;
    }
    else if (ballTop < 0.0f)
    {
	   contact.type = CollisionType::Top;
	   contact.penetration = -ballTop;
    }
    else if (ballBottom > window_height)
    {
	   contact.type = CollisionType::Bottom;
	   contact.penetration = window_height - ballBottom;
    }

    return contact;
}
