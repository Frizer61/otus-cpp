#pragma once

#include "editor/IRenderer.h"
#include "editor/Point.h"

namespace editor {

/**
 * @brief Демонстрационный renderer: печатает команды рисования в консоль.
 */
class ConsoleRenderer final : public IRenderer {
public:
	void drawCircle(const Point& center, double radius) override;
	void drawRectangle(const Point& position, double width, double height) override;
};

} // namespace editor
