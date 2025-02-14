#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <array>

#include "SFML/Graphics.hpp"
#include "imgui.h"
#include "imgui-SFML.h"
#include "Shape.h"

struct FontConfig
{
	std::string fileName;
	int size;
	int red;
	int green;
	int blue;
};

float findTextLength(sf::Text* text, sf::Font* font);
std::vector<Shape*> readInShapes(std::ifstream& inFile);
std::vector<char*> buildNameList(std::vector<Shape*> shapeArray);
void freeShapeMem(std::vector<Shape*>& shapeArray);
void printShapeArray(std::vector<Shape*>& shapeArray);

int main(int argc, char* argv[])
{
	// Open config file
	std::ifstream inFile("config.txt");
	if (!inFile) {
		std::cerr << "Could not open shape config file" << std::endl;
		exit(-1);
	}

	std::string skippedWord; // indicator word to skip over from infile
	int wWidth, wHeight;     // Window dimensions
	FontConfig fontConfig;   // Font object
	std::string configLine;  // store each line of the file

	// Get window size
	std::getline(inFile, configLine);
	std::istringstream wStream(configLine);
	wStream >> skippedWord >> wWidth >> wHeight;

	// Create font object
	std::getline(inFile, configLine);
	std::istringstream fStream(configLine);
	fStream >> skippedWord >> fontConfig.fileName >> fontConfig.size
		>> fontConfig.red >> fontConfig.green >> fontConfig.blue;

	// Read shape configs into an array
	std::vector<Shape*> shapes = readInShapes(inFile);
	std::vector<char*> shapeNames = buildNameList(shapes);

	inFile.close();

	// create a new window of size w*h pixels
	sf::RenderWindow window(sf::VideoMode(wWidth, wHeight), "Bouncing Shapes");
	window.setFramerateLimit(60);

	// iniitialize IMGUI and create a clock used for its internal timing
	ImGui::SFML::Init(window);
	sf::Clock deltaClock;

	// scale the imgui ui by a given factor, doesn't affect text size
	ImGui::GetStyle().ScaleAllSizes(1.0f);

	//load fonts
	sf::Font loadFont;

	if (!loadFont.loadFromFile(fontConfig.fileName)) {
		std::cerr << "Could not load font!\n";
		exit(-1);
	}

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			// pass event to imgui
			ImGui::SFML::ProcessEvent(window, event);

			// window closed
			if (event.type == sf::Event::Closed) {
				window.close();
			}
		}

		// update imgui for this frame with the time the last frame took
		ImGui::SFML::Update(window, deltaClock.restart());

		// draw the UI
		ImVec2 minSize = ImVec2(200, 200); // Minimum window size
		ImVec2 maxSize = ImVec2(600, 400); // Maximum window size

		// Apply the size constraints
		ImGui::SetNextWindowSizeConstraints(minSize, maxSize);
		ImGui::Begin("Shape Controller");

		static char* selectedItem = nullptr;
		static int totalNames = shapeNames.size();
		bool newSelection = false;

		if (ImGui::BeginCombo("Shape", selectedItem != nullptr ? selectedItem
			: "Select a Shape")) {
			for (char* name : shapeNames) {
				bool isSelected = (selectedItem == name);
				if (ImGui::Selectable(name, isSelected)) {
					selectedItem = name; // Update selected item
					newSelection = true;
				}
				if (isSelected) {
					ImGui::SetItemDefaultFocus(); // Auto-focus selected
				}
			}
			ImGui::EndCombo();
		}

		static char displayString[255];
		static char* shapeName;
		static std::string shapeType;
		static std::array<float, 2>* dims;
		static float* rgb;
		static sf::RectangleShape* rectangle;
		static sf::CircleShape* circle;
		window.clear();

		for (Shape* shape : shapes) {
			dims = shape->getDim();
			rgb = shape->getRGB();
			shapeName = shape->getName();

			sf::Text text(shapeName, loadFont, 24);

			if (shape->getCirc() == nullptr) {
				shapeType = "Rectangle";
				rectangle = shape->getRect();
			}
			else {
				circle = shape->getCirc();
				shapeType = "Circle";
			}
			if (shape->getName() == selectedItem) {
				if (newSelection) {
					if (strncpy_s(displayString, 255, shapeName, strlen(shapeName)) != 0) {
						std::cerr << "Error copying string from shape name\n";
						exit(-1);
					}
					newSelection = false;
				}
				ImGui::Text("Window text", shape->getName());
				ImGui::Checkbox("Draw Shape", shape->getDrawShape());
				ImGui::SameLine();
				ImGui::Checkbox("Draw Text", shape->getDrawText());
				if (shapeType == "Rectangle") {
					ImGui::SliderFloat("Length", &(*dims)[0], 0.0f, 300.0f);
					ImGui::SliderFloat("Width", &(*dims)[1], 0.0f, 300.0f);
				}
				else {
					ImGui::SliderFloat("Radius", &(*dims)[0], 0.0f, 300.0f);
					ImGui::SliderInt("Sides", shape->getSegments(), 3, 64);
				}
				ImGui::ColorEdit3("Color Circle", rgb);
				ImGui::InputText("Text", displayString, 255);
				if (ImGui::Button("Set Text")) {
					if (strncpy_s(shapeName, 255, displayString, strlen(displayString)) != 0) {
						std::cerr << "Error copying string to shape\n";
						exit(-1);
					}

					text.setString(shape->getName());
				}
				ImGui::SameLine();
				if (shapeType == "Rectangle") {
					if (ImGui::Button("Reset Rectangle")) {
						(*rectangle).setPosition(0, 0);
					}
				}
				else {
					if (ImGui::Button("Reset Circle")) {
						(*circle).setPosition(0, 0);
					}
				}
			}
			if (shapeType == "Rectangle") {
				const float width = (*shape->getDim())[0];
				const float length = (*shape->getDim())[1];
				sf::Vector2f recDims((*shape->getDim())[0], (*shape->getDim())[1]);
				(*rectangle).setFillColor(sf::Color(rgb[0] * 255, rgb[1] * 255, rgb[2] * 255));

				(*rectangle).setSize(recDims);

				// move each frame if it's still in frame
				int newxPos = (*rectangle).getPosition().x + *shape->getVx();
				int newyPos = (*rectangle).getPosition().y + *shape->getVy();
				if (newxPos < 0 || newxPos + width > wWidth) {
					*shape->getVx() = *shape->getVx() * -1;
					newxPos = (*rectangle).getPosition().x;
				}
				if (newyPos < 0 || newyPos + length > wHeight) {
					*shape->getVy() = *shape->getVy() * -1;
					newyPos = (*rectangle).getPosition().y;
				}
				(*rectangle).setPosition(newxPos, newyPos);
				float textWidth = findTextLength(&text, &loadFont);
					text.setPosition(newxPos + (width / 2) - (textWidth / 2), newyPos + (length / 2) - ((float)text.getCharacterSize() / 2));

				if (*shape->getDrawShape()) {
					window.draw(*rectangle);
				}
			}
			else {
				float radius = (*shape->getDim())[0];
				(*circle).setFillColor(sf::Color(rgb[0] * 255, rgb[1] * 255, rgb[2] * 255));

				(*circle).setPointCount(*shape->getSegments());
				(*circle).setRadius((*shape->getDim())[0]);

				// move each frame if it's still in frame
				int newxPos = (*circle).getPosition().x + *shape->getVx();
				int newyPos = (*circle).getPosition().y + *shape->getVy();

				if (newxPos < 0 || newxPos + radius > wHeight) {
					*shape->getVx() = *shape->getVx() * -1;
					newxPos = (*circle).getPosition().x;
				}
				if (newyPos < 0 || newyPos + 2 * radius > wHeight) {
					*shape->getVy() = *shape->getVy() * -1;
					newyPos = (*circle).getPosition().y;
				}
				float textWidth = findTextLength(&text, &loadFont);
				(*circle).setPosition((*circle).getPosition().x + *shape->getVx(), (*circle).getPosition().y + *shape->getVy());
				text.setPosition(newxPos + radius - (textWidth / 2), newyPos + radius - ((float)text.getCharacterSize() / 2));

				if (*shape->getDrawShape()) {
					window.draw(*circle);
				}
			}
			if (*shape->getDrawText()) {
				window.draw(text);
			}
		}
		ImGui::End();
		ImGui::SFML::Render(window); // ui drawn last = on top
		window.display();
	}
	freeShapeMem(shapes);
	return 0;
}

// Find the width in pixels of a specific text with a specific font and size
float findTextLength(sf::Text* text, sf::Font* font)
{
	std::string str = text->getString();
	unsigned int charSize = text->getCharacterSize();

	float width = 0.0f;
	for (char c : str) {
		width += font->getGlyph(c, charSize, false).advance; // Sum character widths
	}
	return width;
}

// Create Shape objects for every shape line in the config file and return them in an array
std::vector<Shape*> readInShapes(std::ifstream& inFile)
{
	std::string shapeConfig, shapeType;
	float px, py, vx, vy, r, g, b;
	char shapeName[255];
	std::array<float, 2> dim;

	std::vector<Shape*> shapeArray;

	// Divide each line into variables
	while (std::getline(inFile, shapeConfig)) {
		std::istringstream stream(shapeConfig);
		stream >> shapeType >> shapeName >> px >> py >> vx >> vy >> r >> g >> b;

		float dimension;
		int dimIndex = 0;
		while (stream >> dimension) {
			dim[dimIndex] = dimension;
			dimIndex++;
		}
		r /= 255;
		g /= 255;
		b /= 255;

		// Build a rectangle shape with the config options
		if (shapeType == "Rectangle") {
			sf::Vector2f recDims(dim[0], dim[1]);
			sf::RectangleShape* sfRectangle = new sf::RectangleShape(recDims);
			Shape* rectangleShape = new Shape(nullptr, sfRectangle, shapeName, px, py, vx, vy, true, true, -1, dim, r, b, b);
			sfRectangle->setPosition(px, py);
			shapeArray.push_back(rectangleShape);
		}
		// Build circle shape
		else {
			sf::CircleShape* sfCircle = new sf::CircleShape(dim[0], 32);
			sfCircle->setPosition(px, py);
			Shape* circleShape = new Shape(sfCircle, nullptr, shapeName, px, py, vx, vy, true, true, 32, dim, r, g, b);
			shapeArray.push_back(circleShape);
		}
	}

	return shapeArray;
}

// Create an array of the shape names to be shown on the gui dropdown 
std::vector<char*> buildNameList(std::vector<Shape*> shapeArray)
{
	std::vector<char*> nameList;
	for (Shape* shape : shapeArray) {
		nameList.push_back(shape->getName());
	}
	return nameList;
}

void freeShapeMem(std::vector<Shape*>& shapeArray)
{
	for (Shape* shape : shapeArray) {
		delete shape;
	}
	shapeArray.clear();
}

void printShapeArray(std::vector<Shape*>& shapeArray)
{
	for (Shape* shape : shapeArray) {
		shape->printShape();
	}
}