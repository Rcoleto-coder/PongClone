#include "Ball.h"

void Ball::Draw(SDL_Renderer* renderer)
{
    rect.x = static_cast<int>(position.x);
    rect.y = static_cast<int>(position.y);
    SDL_RenderFillRect(renderer, &rect);
}

void Ball::Update(float dt)
{
    position += velocity * dt;
}

void Ball::CollideWithPaddle(Contact const& contact, const float ball_speed)
{
    position.x += contact.penetration;
    velocity.x = -velocity.x;

    if (contact.type == CollisionType::Top)
    {
	   velocity.y = -.75f * ball_speed;
    }
    else if (contact.type == CollisionType::Bottom)
    {
	   velocity.y = 0.75f * ball_speed;
    }
}

void Ball::CollideWithWall(Contact const& contact, const int window_width, const int window_height, const float ball_speed)
{
    if ((contact.type == CollisionType::Top)
	   || (contact.type == CollisionType::Bottom))
    {
	   position.y += contact.penetration;
	   velocity.y = -velocity.y;
    }
    else if (contact.type == CollisionType::Left)
    {
	   position.x = window_width / 2.0f;
	   position.y = window_height / 2.0f;
	   velocity.x = ball_speed;
	   velocity.y = 0.75f * ball_speed;
    }
    else if (contact.type == CollisionType::Right)
    {
	   position.x = window_width / 2.0f;
	   position.y = window_height / 2.0f;
	   velocity.x = -ball_speed;
	   velocity.y = 0.75f * ball_speed;
    }
}


