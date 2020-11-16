#include "Paddle.h"
#include "Engine.h"
#include "Walls.h"

Paddle::Paddle() : redBrush(NULL) {
	position.x = (float) (RIGHT_EDGE / 2) - (PADDLE_WIDTH / 2);
	position.y = (float) RESOLUTION_Y - 20;

	paddleSpeed = 0.4f;
}

Paddle::~Paddle() {
	redBrush->Release();
}

void Paddle::InitializeD2D(ID2D1HwndRenderTarget* renderTarget) {
	// Creates a red brush for drawing
	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Red),
		&redBrush
	);
}

void Paddle::DrawPaddle(ID2D1HwndRenderTarget* renderTarget) {
	D2D1_RECT_F paddle = D2D1::RectF(
		position.x, position.y,
		position.x + PADDLE_WIDTH, position.y + PADDLE_THICKNESS
	);
	renderTarget->FillRectangle(&paddle, redBrush);
}

Coord Paddle::GetPosition() {
	return position;
}

void Paddle::GoLeft() {
	position.x -= paddleSpeed;

	if (LeftWallCollision()) {
		position.x += paddleSpeed;
		return;
	}
}

void Paddle::GoRight() {
	position.x += paddleSpeed;

	if (RightWallCollision()) {
		position.x -= paddleSpeed;
		return;
	}
}

bool Paddle::LeftWallCollision() {
	if (position.x < LEFT_EDGE) {
		return true;
	}
	return false;
}

bool Paddle::RightWallCollision() {
	if ((position.x + PADDLE_WIDTH) > RIGHT_EDGE) {
		return true;
	}
	return false;
}