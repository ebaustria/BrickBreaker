#include "BricksArray.h"

BricksArray::BricksArray() : greenBrush(NULL), purpleBrush(NULL) {
	cells = new Matrix(STACK_WIDTH, STACK_HEIGHT);
}

BricksArray::~BricksArray() {
	delete cells;
	greenBrush->Release();
	purpleBrush->Release();
}

void BricksArray::InitializeD2D(ID2D1HwndRenderTarget* renderTarget) {
	// Creates a red brush for drawing
	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Green),
		&greenBrush
	);

	renderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::Purple),
		&purpleBrush
	);
}

Matrix* BricksArray::GetCells() {
	return cells;
}

void BricksArray::DrawBricks(ID2D1HwndRenderTarget* renderTarget) {

	for (int i = 0; i < STACK_HEIGHT; i++) {
		for (int j = 0; j < STACK_WIDTH; j++) {
			if (cells->GetBrick(j, i) == true) {
				float pos_x = (j + 1) * HORIZONTAL_GAP + BRICK_WIDTH;
				float pos_y = (i + 1) * VERTICAL_GAP + BRICK_HEIGHT + 10;
				D2D1_RECT_F brick = D2D1::RectF(
					pos_x + j * BRICK_WIDTH, pos_y + i * BRICK_HEIGHT,
					pos_x + (j + 1) * BRICK_WIDTH, pos_y + (i + 1) * BRICK_HEIGHT
				);
				if (cells->GetPurple(j, i)) {
					renderTarget->FillRectangle(&brick, purpleBrush);
				}
				else {
					renderTarget->FillRectangle(&brick, greenBrush);
				}
			}
		}
	}


}