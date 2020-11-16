#include "Matrix.h"

Matrix::Matrix(int sizeX, int sizeY) {
	xSize = sizeX;
	ySize = sizeY;

    matrix = new Brick* [ySize];
    for (int i = 0; i < ySize; ++i) {
        matrix[i] = new Brick[xSize];
        for (int j = 0; j < xSize; ++j) {
            matrix[i][j].intact = true;
            matrix[i][j].health = 1;
            matrix[i][j].purple = false;
        }
    }
}

Matrix::~Matrix() {
    for (int i = 0; i < ySize; ++i)
    {
        delete[] matrix[i];
    }
    delete[] matrix;
}

bool Matrix::GetBrick(int x, int y) {
    return matrix[y][x].intact;
}

void Matrix::SetBrick(int x, int y, bool value) {
    matrix[y][x].intact = value;
}

void Matrix::ResetPurple(int x, int y) {
    matrix[y][x].purple = true;
    matrix[y][x].health = 2;
}

void Matrix::PurpleOff(int x, int y) {
    matrix[y][x].purple = false;
}

bool Matrix::GetPurple(int x, int y) {
    return matrix[y][x].purple;
}

void Matrix::DecreaseBrickHealth(int x, int y) {
    matrix[y][x].health--;
}

int Matrix::GetBrickHealth(int x, int y) {
    return matrix[y][x].health;
}

void Matrix::UpdateBrick(int x, int y) {
    if (GetPurple(x, y)) {
        PurpleOff(x, y);
    }
    DecreaseBrickHealth(x, y);
    if (GetBrickHealth(x, y) == 0) {
        SetBrick(x, y, false);
    }
}

void Matrix::ResetGreen(int x, int y) {
    matrix[y][x].health = 1;
}

int Matrix::GetXSize() {
    return xSize;
}

int Matrix::GetYSize() {
    return ySize;
}