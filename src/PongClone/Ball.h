#ifndef _BALL_H_
#define _BALL_H_
#include <SDL.h>
#include "Vec2.h"
#include "Contact.h"

class Ball
{
    

public:
    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect{};

    Ball(int ball_height, int ball_width, Vec2 position, Vec2 velocity)
	   : position(position), velocity(velocity)
    {
	   rect.x = static_cast<int>(position.x);
	   rect.y = static_cast<int>(position.y);
	   rect.w = ball_width;
	   rect.h = ball_height;
    }
    // Default destructor
    ~Ball() = default;

    //Function definition
    void Draw(SDL_Renderer* renderer);
    void Update(float dt);
    void CollideWithPaddle(Contact const& contact, const float ball_speed);
    void CollideWithWall(Contact const& contact, const int window_width, const int window_height, const float ball_speed);
    
    
  
};
#endif
