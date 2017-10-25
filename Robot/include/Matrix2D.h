#pragma once

#include <array>
#include <Vector2D.h>
#define Deg2Rad(x)  (x * 3.14) / 180

class Matrix2D
{
public:
	//Constructor
	Matrix2D();

	//The actual matrix
	std::array<Vector2D, 2> matrix;

	//Setup rotate matrix
	void setRotMat(float rot);

	//Setup translation matrix
	void setTransMat(float x_In, float y_In);

	//Get the particular row of the matrix
	Vector2D getRow(int ID);
};