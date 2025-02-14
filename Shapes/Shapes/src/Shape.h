#include <string>
#include <array>

#include "SFML/Graphics.hpp"

#ifndef SHAPE_H
#define SHAPE_H

class Shape {

public:
	Shape(sf::CircleShape* circ, sf::RectangleShape* rec, char* name, float p_x,
		float p_y, float v_x, float v_y, bool draw_shape, bool draw_text, int circle_segments,
		std::array<float, 2> dimensions, float r, float gr, float b);

	~Shape();

	sf::CircleShape* getCirc();
	sf::RectangleShape* getRect();
	char* getName();
	float getPx();
	float getPy();
	float* getVx();
	float* getVy();
	bool* getDrawShape();
	bool* getDrawText();
	int* getSegments();
	float* getRGB();
	std::array<float, 2>* getDim();
	void printShape();

private:
	sf::CircleShape* circleShape;
	sf::RectangleShape* rectangleShape;
	char displayName[255];
	float px, py, vx, vy;
	bool drawShape, drawText;
	int circleSegments;
	std::array<float, 2> dims;
	float rgb[3];
};

#endif