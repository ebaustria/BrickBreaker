#include "Ball.h"
#include "Walls.h"
#include "Engine.h"
#include "Paddle.h"

Ball::Ball(Paddle* paddle, Matrix* brickCells) : yellowBrush(NULL) {
	startingPos = 300.0f;

	position.x = (float) LEFT_EDGE;
	position.y = startingPos;

	direction.x = 0.3f;
	direction.y = 0.3f;

	active = false;

	ballPaddle = paddle;
	brickMatrix = brickCells;
}

Ball::~Ball() {
}

void Ball::InitializeD2D(ID2D1HwndRenderTarget* renderTarget) {
	// Creates a yellow brush for drawing
	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Yellow),
		&yellowBrush
	);
}

bool Ball::LeftCollision(float xLeft, float xRight, float yTop, float yBottom) {
	if (position.x < xRight && position.x + BALL_SIZE > xRight && position.y + BALL_SIZE > yTop && position.y < yBottom) {
		return true;
	}
	return false;
}

bool Ball::RightCollision(float xLeft, float xRight, float yTop, float yBottom) {
	if (position.x + BALL_SIZE > xLeft && position.x < xLeft && position.y + BALL_SIZE > yTop && position.y < yBottom) {
		return true;
	}
	return false;
}

bool Ball::TopCollision(float xLeft, float xRight, float yTop, float yBottom) {
	if (position.y < yBottom && position.y + BALL_SIZE > yBottom && position.x + BALL_SIZE > xLeft && position.x < xRight) {
		return true;
	}
	return false;
}

bool Ball::BottomCollision(float xLeft, float xRight, float yTop, float yBottom) {
	if (position.y + BALL_SIZE > yTop && position.y < yTop && position.x + BALL_SIZE > xLeft && position.x < xRight) {
		return true;
	}
	return false;
}

int Ball::BrickCollision(Matrix* brickCells) {
	for (int i = 0; i < brickCells->GetYSize(); i++) {
		for (int j = 0; j < brickCells->GetXSize(); j++) {
			if (brickCells->GetBrick(j, i)) {
				float x_pos = ((j + 1) * HORIZONTAL_GAP + BRICK_WIDTH) + j * BRICK_WIDTH;
				float y_pos = ((i + 1) * VERTICAL_GAP + BRICK_HEIGHT + 10) + i * BRICK_HEIGHT;

				if (TopCollision(x_pos, x_pos + BRICK_WIDTH, y_pos, y_pos + BRICK_HEIGHT)) {
					brickCells->UpdateBrick(j, i);
					return 0;
				}

				if (RightCollision(x_pos, x_pos + BRICK_WIDTH, y_pos, y_pos + BRICK_HEIGHT)) {
					brickCells->UpdateBrick(j, i);
					return 1;
				}

				if (BottomCollision(x_pos, x_pos + BRICK_WIDTH, y_pos, y_pos + BRICK_HEIGHT)) {
					brickCells->UpdateBrick(j, i);
					return 2;
				}

				if (LeftCollision(x_pos, x_pos + BRICK_WIDTH, y_pos, y_pos + BRICK_HEIGHT)) {
					brickCells->UpdateBrick(j, i);
					return 3;
				}
			}
		}
	}
	return -1;
}

void Ball::MoveBall(int* score) {

	position.x += direction.x;
	position.y += direction.y;

	if (TopCollision(LEFT_EDGE, RIGHT_EDGE, 10.0f, TOP_EDGE) ||
		BottomCollision(ballPaddle->GetPosition().x, ballPaddle->GetPosition().x + PADDLE_WIDTH, PADDLE_Y, PADDLE_Y + PADDLE_THICKNESS))
	{
		position.y -= direction.y;
		direction.y = -direction.y;
		return;
	}

	if (LeftCollision(10, LEFT_EDGE, TOP_EDGE, RESOLUTION_Y + 50.0f) ||
		RightCollision(RIGHT_EDGE, 10.0f, TOP_EDGE, RESOLUTION_Y + 50.0f) ||
		LeftCollision(ballPaddle->GetPosition().x, ballPaddle->GetPosition().x + PADDLE_WIDTH, PADDLE_Y, PADDLE_Y + PADDLE_THICKNESS) ||
		RightCollision(ballPaddle->GetPosition().x, ballPaddle->GetPosition().x + PADDLE_WIDTH, PADDLE_Y, PADDLE_Y + PADDLE_THICKNESS))
	{
		position.x -= direction.x;
		direction.x = -direction.x;
		return;
	}

	int bc = BrickCollision(brickMatrix);

	if (bc == 0 || bc == 2) {
		position.y -= direction.y;
		direction.y = -direction.y;
		*score += 100;
		return;
	}

	if (bc == 3 || bc == 1) {
		position.x -= direction.x;
		direction.x = -direction.x;
		*score += 100;
		return;
	}
}

void Ball::Activate() {
	active = true;
}

void Ball::Reset(bool advanceLevel) {
	position.x = LEFT_EDGE;
	if (advanceLevel) {
		startingPos += 2.0f;
		direction.y = 0.3f;
	}
	position.y = startingPos;
	active = false;
}

bool Ball::isActive() {
	return active;
}

Coord Ball::GetPosition() {
	return position;
}

void Ball::DrawBall(ID2D1HwndRenderTarget* renderTarget) {
	D2D1_RECT_F ball = D2D1::RectF(
		position.x, position.y,
		position.x + BALL_SIZE, position.y + BALL_SIZE
	);
	renderTarget->FillRectangle(&ball, yellowBrush);
}