#ifndef _PADDLE_H_
#define _PADDLE_H_
#include "Vec2.h"
#include <SDL.h>


class Paddle
{
    
public:
    Vec2 position;
    Vec2 velocity;
    SDL_Rect rect{};
    
    Paddle(const int paddle_height, const int paddle_width, Vec2 position, Vec2 velocity)
	   : position(position), velocity(velocity)
    {
	   rect.x = static_cast<int>(position.x);
	   rect.y = static_cast<int>(position.y);
	   rect.w = paddle_width;
	   rect.h = paddle_height;
    }
    // Default destructor
    ~Paddle() = default;

    // Function definition
    void Draw(SDL_Renderer* renderer);
    void Update(float dt, int window_height, int paddle_height);
    
    void setVelocity(float v);
    Vec2 getVelocity();
    

};

#endif //_PADDLE_H_