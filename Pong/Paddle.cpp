#include "Paddle.h"


Paddle::Paddle(int i)
{
	id = i;
	speed = 6.f;

	if (id == 0)
	{
		pos.x = 0;
		
	}
	else if(id == 1)
	{
		pos.x = SCREEN_WIDTH - width;
	}

	pos.y = SCREEN_HEIGHT / 2  -  height / 2;

	

	rect.x = (int)pos.x;
	rect.y = (int)pos.y;
	rect.w = width;
	rect.h = height;
}

void Paddle::Update()
{
	//set initial y
	pos.y = pos.y + speed * dir;

	//keep y in borders
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	else if (pos.y + height > SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - height;
	}
	
	rect.y = pos.y;
}

void Paddle::SetDir(int d)
{
	dir = d;
}

void Paddle::IncScore()
{
	score++;
}

void Paddle::ShowScore(SDL_Renderer* renderer, TTF_Font* font, SDL_Color color)
 {
	scoreSurface = TTF_RenderText_Solid(font, to_string(score).c_str(), color);

	if (id == 0) {
		scoreRect.x = SCREEN_WIDTH / 2 - 100;
	}
	else if (id == 1) {
		scoreRect.x = SCREEN_WIDTH / 2 + 100;
	}
	scoreRect.y = 50;
	scoreRect.w = scoreSurface->w;
	scoreRect.h = scoreSurface->h;
	scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

	SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
}

int Paddle::GetId()
{
	return id;
}

Vec2 Paddle::GetPos()
{
	return pos;
}



SDL_Rect* Paddle::GetRect() 
{
	return &rect;
}