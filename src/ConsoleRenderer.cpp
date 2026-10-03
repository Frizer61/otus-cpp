#include "editor/ConsoleRenderer.h"

#include "editor/Point.h"

#include <iostream>

namespace editor {

void ConsoleRenderer::drawCircle(const Point& center, double radius) {
	std::cout << "[VIEW] круг: центр (" << center.x << ", " << center.y << "), радиус " << radius << '\n';
}

void ConsoleRenderer::drawRectangle(const Point& position, double width, double height) {
	std::cout
		<< "[VIEW] прямоугольник: угол (" << position.x << ", " << position.y << ")"
		<< ", ширина " << width << ", высота " << height << '\n';
}

} // namespace editor
