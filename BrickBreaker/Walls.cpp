#include "Walls.h"
#include "Engine.h"

Walls::Walls() : blueBrush(NULL) {
}

Walls::~Walls() {
	blueBrush->Release();
}

void Walls::InitializeD2D(ID2D1HwndRenderTarget* renderTarget) {
	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Blue),
		&blueBrush
	);
}

void Walls::DrawWalls(ID2D1HwndRenderTarget* renderTarget) {
	float buffer = 10.0f;

	D2D1_RECT_F leftWall = D2D1::RectF(
		buffer, buffer,
		buffer + WALL_THICKNESS, RESOLUTION_Y - 10
	);
	renderTarget->FillRectangle(&leftWall, blueBrush);

	D2D1_RECT_F ceiling = D2D1::RectF(
		buffer, buffer,
		RIGHT_EDGE, buffer + WALL_THICKNESS
	);
	renderTarget->FillRectangle(&ceiling, blueBrush);

	D2D1_RECT_F rightWall = D2D1::RectF(
		RIGHT_EDGE, buffer,
		RIGHT_EDGE + WALL_THICKNESS, RESOLUTION_Y - 10
	);
	renderTarget->FillRectangle(&rightWall, blueBrush);
}