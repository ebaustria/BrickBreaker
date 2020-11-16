#pragma once
#include "Coord.h"
#include <d2d1.h>

#define PADDLE_WIDTH 40.0f
#define PADDLE_THICKNESS 10.0f
#define PADDLE_Y 580.0f

class Paddle {
public:
	Paddle();
	~Paddle();

	void InitializeD2D(ID2D1HwndRenderTarget* renderTarget);
	void GoLeft();
	void GoRight();
	void DrawPaddle(ID2D1HwndRenderTarget* renderTarget);

	bool LeftWallCollision();
	bool RightWallCollision();

	Coord GetPosition();

private:
	Coord position;

	ID2D1SolidColorBrush* redBrush;

	float paddleSpeed;
};