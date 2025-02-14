#include <iostream>
#include <string>
#include "Shape.h"

Shape::Shape(sf::CircleShape* circ, sf::RectangleShape* rec, char* name, float p_x,
	         float p_y, float v_x, float v_y, bool draw_shape, bool draw_text, int circle_segments,
			 std::array<float, 2> dimensions, float r, float g, float b) : rgb{r, g, b}
{
	circleShape = circ;
	rectangleShape = rec;
	strncpy_s(displayName, 255, name, strlen(name));
	px = p_x;
	py = p_y;
	vx = v_x;
	vy = v_y;
	
	drawShape = draw_shape;
	drawText = draw_text;
	circleSegments = circle_segments;
	dims = dimensions;
}
Shape::~Shape()
{
	delete circleShape;
	delete rectangleShape;
}

sf::CircleShape* Shape::getCirc()
{
	return circleShape;
}

sf::RectangleShape* Shape::getRect()
{
	return rectangleShape;
}

char* Shape::getName()
{
	return displayName;
}

float Shape::getPx()
{
	return px;
}

float Shape::getPy()
{
	return py;
}

float* Shape::getVx()
{
	return &vx;
}

float* Shape::getVy()
{
	return &vy;
}

bool* Shape::getDrawShape()
{
	return &drawShape;
}

bool* Shape::getDrawText()
{
	return &drawText;
}

int* Shape::getSegments()
{
	return &circleSegments;
}

float* Shape::getRGB()
{
	return rgb;
}

std::array<float, 2>* Shape::getDim()
{
	return &dims;
}

void Shape::printShape()
{
	int dimMax = 2;
	if (getCirc() == nullptr) {
		std::cout << "Rectangle ";
	}
	else {
		dimMax = 1;
		std::cout << "Circle ";
	}
	std::cout << getName() << " ";
	std::cout << getPx() << " ";
	std::cout << getPy() << " ";
	std::cout << *getVx() << " ";
	std::cout << *getVy() << " ";
	float* rgb = getRGB();
	for (int i = 0; i < 3; i++) {
		std::cout << rgb[i] * 255 << " ";
	}
	std::array<float, 2> dim = *getDim();

	for (int j = 0; j < dimMax; j++) {
		std::cout << dim[j] << " ";
	}
	std::cout << std::endl;

}