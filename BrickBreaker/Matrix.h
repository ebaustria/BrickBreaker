#pragma once
#include "Brick.h"

class Matrix {
public:
	Matrix(int sizeX, int sizeY);
	~Matrix();

	bool GetBrick(int x, int y);
	bool GetPurple(int x, int y);

	void SetBrick(int x, int y, bool value);
	void ResetGreen(int x, int y);
	void ResetPurple(int x, int y);
	void PurpleOff(int x, int y);
	void DecreaseBrickHealth(int x, int y);
	void UpdateBrick(int x, int y);

	int GetBrickHealth(int x, int y);
	int GetXSize();
	int GetYSize();

private:
	int xSize;
	int ySize;

	Brick** matrix;
};