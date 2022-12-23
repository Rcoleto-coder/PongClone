#ifndef _PLAYERSCORE_H_
#define _PLAYERSCORE_H_
#include <SDL_ttf.h>
#include <iostream>
#include "Vec2.h"

class PlayerScore
{
private: 
    SDL_Renderer* renderer = nullptr;
    TTF_Font* font = nullptr;
    SDL_Surface* surface= nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Rect rect{};
    int textWidth{ 0 }, textHeight{ 0 };

public:
    //Constructor
    PlayerScore(Vec2 position, SDL_Renderer* renderer, TTF_Font* font)
	   : renderer(renderer), font(font)
    {
	   surface = TTF_RenderText_Solid(font, "0", { 0, 0xFF, 0, 0xFF });
	   texture = SDL_CreateTextureFromSurface(renderer, surface);

	   
	   SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);

	   rect.x = static_cast<int>(position.x);
	   rect.y = static_cast<int>(position.y);
	   rect.w = textWidth;
	   rect.h = textHeight;
    }
    //Destructor
    ~PlayerScore()
    {
	   SDL_FreeSurface(surface);
	   SDL_DestroyTexture(texture);
    }

    //Function definition
    void Draw();
    void SetScore(int score);
    

    
};


#endif 
