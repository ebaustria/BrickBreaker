#pragma once

#include "Resource.h"
#include "BricksArray.h"
#include "Paddle.h"
#include "Ball.h"
#include "Walls.h"
#include <dwrite.h>

#define RESOLUTION_X 1000
#define RESOLUTION_Y 600

class Engine {
public:
	Engine();
	~Engine();

	HRESULT InitializeD2D(HWND hwnd);
	HRESULT Render();

	void KeyUp(WPARAM wParam);
	void KeyDown(WPARAM wParam);
	void Logic(double elapsedTime);

private:
	ID2D1Factory* direct2dFactory;
	ID2D1HwndRenderTarget* renderTarget;

	IDWriteFactory* dWriteFactory;
	IDWriteTextFormat* textFormat;
	ID2D1SolidColorBrush* whiteBrush;

	void InitializeTextAndScore();
	void DrawTextAndScore();
	void UpdateMovement();
	void DrawGameOver();
	void DrawPause();
	void DrawVictory();
	bool LevelComplete();

	BricksArray* bricks;
	Paddle* paddle;
	Ball* ball;
	Walls* walls;

	bool running = true;

	bool leftPressed = false;
	bool rightPressed = false;
	bool spacePressed = false;
	bool isPaused = false;
	bool victory = false;

	int lives = 3;
	int score = 0;
	int level = 1;
	int* pScore;
	int greenHeight = 9;
};