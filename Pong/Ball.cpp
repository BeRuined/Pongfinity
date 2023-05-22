#include "Ball.h"


Ball::Ball(Game* g)
{
	game = g;
	speed = INITIAL_SPEED;

	
	pos.x = SCREEN_WIDTH / 2 - size / 2;
	pos.y = SCREEN_HEIGHT / 2 - size / 2;

	vel.x = -1;
	vel.y = 1;

	rect.x = (int)pos.x;
	rect.y = (int)pos.y;
	rect.w = size;
	rect.h = size;

	sound = Mix_LoadWAV("Assets/hitSound.wav");
}

void Ball::Update(Paddle* lPaddle, Paddle* rPaddle)
{
	pos.x = pos.x + vel.x * speed;
	pos.y = pos.y + vel.y * speed;

	if (pos.x < 0) 
	{
		rPaddle->IncScore();
		Reset();
	}
	else if (pos.x > SCREEN_WIDTH - size) 
	{
		lPaddle->IncScore();
		Reset();
	}


	HandleCollision(lPaddle);
	HandleCollision(rPaddle);



	//keep ball in borders
	if (pos.y < 0)
	{
		pos.y = 0;
		vel.y *= -1;
	}
	else if (pos.y + size > SCREEN_HEIGHT)
	{
		pos.y = SCREEN_HEIGHT - size;
		vel.y *= -1;
	}

	

	

	rect.x = (int)pos.x;
	rect.y = (int)pos.y;
}

bool Ball::BottomCollision(Paddle* paddle)
{
	bool passedBoundry = false; //Checking wheter or not we have passed the boundary or the position on the x-axis
	bool collided = false;

	if (paddle->GetId() == 0) {
		passedBoundry = pos.x < size;
	}
	else if (paddle->GetId() == 1) {
		passedBoundry = pos.x > SCREEN_WIDTH - size;
	}

	if (passedBoundry &&												// Passed outer boundry of paddle
		pos.y <= paddle->GetRect()->y + paddle->GetRect()->h &&			// Top of ball is inside bottom of paddle
		pos.y + size > paddle->GetRect()->y + paddle->GetRect()->h &&	// Bottom of ball is not inside bottom of paddle
		vel.y < 0) {													// Ball is moving up
		pos.y = paddle->GetRect()->y + paddle->GetRect()->h + 1;		// Give a buffer of 1px so doesn't intersect immediately
		vel.y *= -1;
		rect.y = pos.y;

		collided = true;
	}

	return collided;
}

bool Ball::TopCollision(Paddle* paddle)
{
	bool passedBoundry = false;
	bool collided = false;

	if (paddle->GetId() == 0) {
		passedBoundry = pos.x < size;
	}
	else if (paddle->GetId() == 1) {
		passedBoundry = pos.x > SCREEN_WIDTH - size;
	}

	if (passedBoundry &&												// Passed outer boundry of paddle
		pos.y + size >= paddle->GetRect()->y &&							// Bottom of ball is inside top of paddle
		pos.y < paddle->GetRect()->y &&									// Top of ball is not inside top of paddle
		vel.y > 0) {													// Ball is moving down
		pos.y = paddle->GetRect()->y - size - 1;						// Give a buffer of 1px so doesn't intersect immediately
		vel.y *= -1;
		rect.y = pos.y;

		collided = true;
	}

	return collided;
}

void Ball::HandleCollision(Paddle* paddle)
{
	if (BottomCollision(paddle) || TopCollision(paddle))
	{
		Mix_PlayChannel(-1, sound, 0);
		return;
	}

	if (pos.x + size >= paddle->GetPos().x &&						 //Right ball intersect left paddle
		pos.x <= paddle->GetPos().x + paddle->GetRect()->w &&		 //Left ball intersect right paddle
		pos.y + size >= paddle->GetPos().y &&							 //Bottom ball intersect top paddle
		pos.y <= paddle->GetPos().y + paddle->GetRect()->h)			//Top ball intersect bottom paddle
	{
		vel.x *= -1;

		// Bounce ball differently depending on where it hits the paddle
		int middlePaddleY = paddle->GetPos().y + paddle->GetRect()->h / 2;
		int middleBallY = pos.y + size / 2;
		int offsetY = middlePaddleY - middleBallY;

		vel.y -= offsetY * 1.5 / 100.f;

		if (paddle->GetId() == 0)
		{
			pos.x = paddle->GetPos().x + paddle->GetRect()->w;
		}
		else if (paddle->GetId() == 1)
		{
			pos.x = paddle->GetPos().x - size;
		}


		Mix_PlayChannel(-1, sound, 0);

		if (speed < MAX_SPEED)
		{
			speed++;
		}

	}

}

SDL_Rect* Ball::GetRect() 
{
	return &rect;
}

void Ball::Shutdown()
{
	Mix_FreeChunk(sound);
}

void Ball::Reset()
{
	speed = INITIAL_SPEED - 1.5f;


	pos.x = SCREEN_WIDTH / 2 - size / 2;
	pos.y = SCREEN_HEIGHT / 2 - size / 2;

	rect.x = (int)pos.x;
	rect.y = (int)pos.y;

	game->SetGamestatePending();
}