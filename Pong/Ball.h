#pragma once
#include "Paddle.h"
#include <SDL_mixer.h>
#include "Game.h"

class Ball
{
public:
	Ball(class Game* g);
	void Update(Paddle* lPaddle, Paddle* rPaddle);
	void HandleCollision(Paddle* paddle);
	bool TopCollision(Paddle* paddle);
	bool BottomCollision(Paddle* paddle);
	void Shutdown();
	void Reset();

	SDL_Rect* GetRect();

private:
	
	int dir = 0;
	int size = 20;
	float speed;
	const int INITIAL_SPEED = 3.f;
	const int MAX_SPEED = 5.5f;

	Vec2 pos;
	Vec2 vel;
	SDL_Rect rect;

	Mix_Chunk* sound = nullptr;

	Game* game;
};

