#include "Paddle.h"

void Paddle::Draw(SDL_Renderer* renderer)
{
    rect.y = static_cast<int>(position.y);

    SDL_RenderFillRect(renderer, &rect);
}

void Paddle::Update(float dt, int window_height, int paddle_height)
{
    position += velocity * dt;

    if (position.y < 0)
    {
	   // Restrict to top of the screen
	   position.y = 0;
    }
    else if (position.y > (window_height - paddle_height))
    {
	   // Restrict to bottom of the screen
	   position.y = window_height - paddle_height;
    }
}

void Paddle::setVelocity(float v) {
    velocity.y = v;
}

Vec2 Paddle::getVelocity()
{
    return velocity;
}
