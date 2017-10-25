#include <Matrix2D.h>

Matrix2D::Matrix2D()
{
	matrix.at(0) = Vector2D(1, 0);
	matrix.at(1) = Vector2D(0, 1);
}

void Matrix2D::setRotMat(float rot)
{
	matrix.at(0) = Vector2D(cosf(Deg2Rad(rot)), -sinf(Deg2Rad(rot)));
	matrix.at(1) = Vector2D(sinf(Deg2Rad(rot)), cosf(Deg2Rad(rot)));
}

void Matrix2D::setTransMat(float x_In, float y_In)
{
	matrix.at(0) = Vector2D(x_In, y_In);
	matrix.at(1) = Vector2D(0, 1);
}

Vector2D Matrix2D::getRow(int ID)
{
	return matrix.at(ID);
}