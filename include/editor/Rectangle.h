#pragma once

#include "editor/Point.h"
#include "editor/Primitive.h"

namespace editor {

class IRenderer;

/**
 * @brief Прямоугольник, position — левый верхний угол.
 */
class Rectangle final : public Primitive {
public:
	Rectangle(PrimitiveId id, const Point& position, double width, double height);

	void draw(IRenderer& renderer) const override;

private:
	Point position_;
	double width_;
	double height_;
};

} // namespace editor
