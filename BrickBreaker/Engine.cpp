#include "Framework.h"
#include "Engine.h"
#include "Matrix.h"
#include "Paddle.h"
#include "BricksArray.h"
#include "Ball.h"
#include "Walls.h"

#pragma comment(lib, "d2d1")
#pragma comment(lib, "dwrite")
#pragma comment(lib, "Windowscodecs.lib")

Engine::Engine() : direct2dFactory(NULL), renderTarget(NULL) {

	pScore = &score;

	bricks = new BricksArray();
	paddle = new Paddle();
	walls = new Walls();
	ball = new Ball(paddle, bricks->GetCells());
}

Engine::~Engine() {

	SafeRelease(&direct2dFactory);
	SafeRelease(&renderTarget);

	delete ball;
	delete bricks;
	delete paddle;
	delete walls;
}

HRESULT Engine::InitializeD2D(HWND hwnd) {

	D2D1_SIZE_U size = D2D1::SizeU(RESOLUTION_X, RESOLUTION_Y);
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &direct2dFactory);
	direct2dFactory->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(hwnd, size, D2D1_PRESENT_OPTIONS_IMMEDIATELY),
		&renderTarget
	);

	InitializeTextAndScore();
	ball->InitializeD2D(renderTarget);
	bricks->InitializeD2D(renderTarget);
	paddle->InitializeD2D(renderTarget);
	walls->InitializeD2D(renderTarget);

	return S_OK;
}

void Engine::KeyUp(WPARAM wParam) {
	if (wParam == VK_LEFT) {
		leftPressed = false;
	}

	if (wParam == VK_RIGHT) {
		rightPressed = false;
	}

	if (wParam == VK_SPACE) {
		spacePressed = false;
	}
}

void Engine::KeyDown(WPARAM wParam) {
	if (wParam == VK_LEFT) {
		leftPressed = true;
	}

	if (wParam == VK_RIGHT) {
		rightPressed = true;
	}

	if (wParam == VK_SPACE) {
		spacePressed = true;
	}

	if (wParam == 0x50) {
		if (isPaused) {
			isPaused = false;
		}
		else {
			isPaused = true;
		}
	}
}

void Engine::Logic(double elapsedTime) {

	if (!running || isPaused || victory) {
		return;
	}

	if (spacePressed) {
		ball->Activate();
	}

	if (leftPressed) {
		paddle->GoLeft();
	}
	if (rightPressed) {
		paddle->GoRight();
	}
	if (ball->isActive()) {
		ball->MoveBall(pScore);
	}

	if (LevelComplete()) {
		level++;
		if (level > 11) {
			level = 11;
			victory = true;
			return;
		}
		greenHeight--;
		ball->Reset(true);
		for (int i = 0; i < STACK_HEIGHT; i++) {
			for (int j = 0; j < STACK_WIDTH; j++) {
				if (i > greenHeight) {
					bricks->GetCells()->ResetPurple(j, i);
				}
				else {
					bricks->GetCells()->ResetGreen(j, i);
				}
				bricks->GetCells()->SetBrick(j, i, true);
			}
		}
	}

	if (ball->GetPosition().y > RESOLUTION_Y) {
		lives--;
		ball->Reset(false);
	}

	if (lives < 0) {
		running = false;
	}
}

void Engine::UpdateMovement() {
	if (leftPressed) {
		paddle->GoLeft();
	}
	if (rightPressed) {
		paddle->GoRight();
	}
	if (ball->isActive()) {
		ball->MoveBall(pScore);
	}
}

bool Engine::LevelComplete() {
	for (int i = 0; i < STACK_HEIGHT; i++) {
		for (int j = 0; j < STACK_WIDTH; j++) {
			if (bricks->GetCells()->GetBrick(j, i)) {
				return false;
			}
		}
	}
	return true;
}

HRESULT Engine::Render() {

	HRESULT hr;

	renderTarget->BeginDraw();
	renderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	renderTarget->Clear(D2D1::ColorF(D2D1::ColorF::Black));

	DrawTextAndScore();
	if (!running) {
		DrawGameOver();
	}
	if (victory) {
		DrawVictory();
	}
	if (isPaused) {
		DrawPause();
	}
	ball->DrawBall(renderTarget);
	walls->DrawWalls(renderTarget);
	bricks->DrawBricks(renderTarget);
	paddle->DrawPaddle(renderTarget);

	hr = renderTarget->EndDraw();

	return S_OK;
}

void Engine::InitializeTextAndScore() {
	DWriteCreateFactory(
		DWRITE_FACTORY_TYPE_SHARED,
		__uuidof(dWriteFactory),
		reinterpret_cast<IUnknown**>(&dWriteFactory)
	);

	dWriteFactory->CreateTextFormat(
		L"OCR A Extended",
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		20,
		L"",
		&textFormat
	);

	textFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	textFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		&whiteBrush
	);
}

void Engine::DrawVictory() {
	float middleX = (RIGHT_EDGE / 2);
	float middleY = (RESOLUTION_Y - TOP_EDGE) / 2;

	D2D1_RECT_F youWin = D2D1::RectF(middleX - 200, middleY, middleX + 200, middleY + 300);
	renderTarget->DrawText(
		L"YOU WIN",
		7,
		textFormat,
		youWin,
		whiteBrush
	);
}

void Engine::DrawGameOver() {
	float middleX = (RIGHT_EDGE / 2);
	float middleY = (RESOLUTION_Y - TOP_EDGE) / 2;

	D2D1_RECT_F gameOver = D2D1::RectF(middleX - 200, middleY, middleX + 200, middleY + 300);
	renderTarget->DrawText(
		L"GAME OVER",
		9,
		textFormat,
		gameOver,
		whiteBrush
	);
}

void Engine::DrawPause() {
	float middleX = (RIGHT_EDGE / 2);
	float middleY = (RESOLUTION_Y - TOP_EDGE) / 2;

	D2D1_RECT_F pause = D2D1::RectF(middleX - 200, middleY, middleX + 200, middleY + 300);
	renderTarget->DrawText(
		L"PAUSED",
		6,
		textFormat,
		pause,
		whiteBrush
	);
}

void Engine::DrawTextAndScore() {
	// Text and score
	float padding = (RESOLUTION_Y - (20 + 1) * 20) / 2;
	float centerRight = RESOLUTION_X - (RESOLUTION_X - padding - (20 + 2) * 20) / 2;

	D2D1_RECT_F stage = D2D1::RectF(centerRight - 200, padding, centerRight + 200, padding + 50);
	WCHAR levelStr[64];
	swprintf_s(levelStr, L"Level: %d ", level);
	renderTarget->DrawText(
		levelStr,
		9,
		textFormat,
		stage,
		whiteBrush
	);

	D2D1_RECT_F livesRect = D2D1::RectF(centerRight - 200, padding + 50, centerRight + 200, padding + 100);
	WCHAR livesStr[64];
	swprintf_s(livesStr, L"Lives: %d ", lives);
	if (lives < 0) {
		swprintf_s(livesStr, L"Lives: %d ", 0);
	}
	renderTarget->DrawText(
		livesStr,
		8,
		textFormat,
		livesRect,
		whiteBrush
	);

	D2D1_RECT_F score1 = D2D1::RectF(centerRight - 200, padding + 100, centerRight + 200, padding + 150);
	renderTarget->DrawText(
		L"Score:",
		6,
		textFormat,
		score1,
		whiteBrush
	);


	D2D1_RECT_F score2 = D2D1::RectF(centerRight - 200, padding + 150, centerRight + 200, padding + 200);
	WCHAR scoreStr[64];
	swprintf_s(scoreStr, L"%d        ", score);
	renderTarget->DrawText(
		scoreStr,
		7,
		textFormat,
		score2,
		whiteBrush
	);
}