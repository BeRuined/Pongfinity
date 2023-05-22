#include "Game.h"


bool Game::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		return false;
	}
	window = SDL_CreateWindow("Pongfinity", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1270, 800, 0);
	if (!window)
	{
		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer)
	{
		return false;
	}


	if (TTF_Init() != 0)
	{
		return false;
	}


	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

	//Init of Paddles
	leftPaddle = new Paddle(0);
	rightPaddle = new Paddle(1);

	//Init of Ball
	ball = new Ball(this);

	font = TTF_OpenFont("Assets/font.ttf", 36); 


	return true;
}

void Game::SetGamestatePending()
{
	gamestate = Gamestates::Pending;
}

void Game::GameLoop() 
{
	while (isRunning) 
	{
		HandleEvent();
		Update();
		Draw();
	}
}

void Game::HandleEvent() 
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) 
	{
		if (event.type == SDL_QUIT) 
		{
			isRunning = false;
		}
	}

	const Uint8* keystates = SDL_GetKeyboardState(NULL);

	if (keystates[SDL_SCANCODE_ESCAPE])
	{
		isRunning = false;
	}

	if (gamestate == Gamestates::Pending)
	{
		if (keystates[SDL_SCANCODE_SPACE])
		{
			gamestate = Gamestates::Playing;
		}
		return;
	}


	//Left Paddle Movement
	leftPaddle->SetDir(0);
	if (keystates[SDL_SCANCODE_W])
	{
		leftPaddle->SetDir(-1);
	}
	if (keystates[SDL_SCANCODE_S])
	{
		leftPaddle->SetDir(+1);
	}
	//Right Paddle Movement
	rightPaddle->SetDir(0);
	if (keystates[SDL_SCANCODE_UP])
	{
		rightPaddle->SetDir(-1);
	}
	if (keystates[SDL_SCANCODE_DOWN])
	{
		rightPaddle->SetDir(+1);
	}
}

void Game::Update() 
{
	if (gamestate == Gamestates::Pending)
	{
		return;
	}


	leftPaddle->Update();
	rightPaddle->Update();

	

	ball->Update(leftPaddle, rightPaddle);
}

void Game::Draw() 
{

	SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);

	SDL_RenderClear(renderer);

	//Paddles Draw
	//left paddle
	SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
	SDL_RenderFillRect(renderer, leftPaddle->GetRect());

	//right paddle
	SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
	SDL_RenderFillRect(renderer, rightPaddle->GetRect());

	//ball draw
	SDL_SetRenderDrawColor(renderer, 255, 255, 40, 40);
	SDL_RenderFillRect(renderer, ball->GetRect());


	leftPaddle->ShowScore(renderer, font, fontColor);
	rightPaddle->ShowScore(renderer, font, fontColor);

	SDL_RenderPresent(renderer);
}

void Game::Shutdown() 
{
	ball->Shutdown();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}