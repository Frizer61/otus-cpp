#pragma once

#include "editor/Point.h"
#include "editor/Primitive.h"

namespace editor {

class IRenderer;

/**
 * @brief Круг.
 */
class Circle final : public Primitive {
public:
	Circle(PrimitiveId id, const Point& center, double radius);

	void draw(IRenderer& renderer) const override;

private:
	Point center_;
	double radius_;
};

} // namespace editor
