#pragma once

#include "Matrix.h"
#include "Coord.h"
#include <d2d1.h>

#define STACK_WIDTH 10
#define STACK_HEIGHT 10
#define VERTICAL_GAP 15.0f
#define HORIZONTAL_GAP 15.0f
#define BRICK_WIDTH 30.0f
#define BRICK_HEIGHT 8.0f

class BricksArray {
public:
	BricksArray();
	~BricksArray();

	void InitializeD2D(ID2D1HwndRenderTarget* renderTarget);
	void DrawBricks(ID2D1HwndRenderTarget* renderTarget);

	Matrix* GetCells();

private:
	Matrix* cells;

	ID2D1SolidColorBrush* greenBrush;
	ID2D1SolidColorBrush* purpleBrush;
};