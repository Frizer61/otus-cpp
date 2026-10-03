#pragma once

#include "editor/Point.h"

namespace editor {

/**
 * @brief Интерфейс отрисовки примитивов.
 */
class IRenderer {
public:
	virtual ~IRenderer() = default;

	virtual void drawCircle(const Point& center, double radius) = 0;
	virtual void drawRectangle(const Point& position, double width, double height) = 0;
};

} // namespace editor
