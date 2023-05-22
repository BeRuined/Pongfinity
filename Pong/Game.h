#pragma once


#include "Paddle.h"
#include "Ball.h"

enum Gamestates
{
	Pending,
	Playing
};

class Game
{
public:
	bool Init();
	void Shutdown();
	void GameLoop();

	void SetGamestatePending();
	

private:
	void HandleEvent();
	void Update();
	void Draw();
	
	int gamestate = Gamestates::Pending;
	bool isRunning = true;

	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;

	class Paddle* leftPaddle, * rightPaddle;
	class Ball* ball;

	TTF_Font* font = nullptr;
	SDL_Color fontColor = { 255, 255, 255 };
};

