#pragma once

#include "Coord.h"
#include "Matrix.h"
#include "Paddle.h"
#include <d2d1.h>
#include <string>

#define BALL_SIZE 10.0f

class Ball {
public:
	Ball(Paddle* paddle, Matrix* brickCells);
	~Ball();

	bool LeftCollision(float xLeft, float xRight, float yTop, float yBottom);
	bool RightCollision(float xLeft, float xRight, float yTop, float yBottom);
	bool TopCollision(float xLeft, float xRight, float yTop, float yBottom);
	bool BottomCollision(float xLeft, float xRight, float yTop, float yBottom);
	bool isActive();

	int BrickCollision(Matrix* brickCells);

	void MoveBall(int* score);
	void Activate();
	void Reset(bool advanceLevel);
	void InitializeD2D(ID2D1HwndRenderTarget* renderTarget);
	void DrawBall(ID2D1HwndRenderTarget* renderTarget);
	
	Coord GetPosition();

private:
	float startingPos;

	Coord position;
	Coord direction;

	Paddle* ballPaddle;
	Matrix* brickMatrix;

	bool active;

	ID2D1SolidColorBrush* yellowBrush;
};