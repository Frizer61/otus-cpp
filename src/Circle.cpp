#include "editor/Circle.h"

#include "editor/IRenderer.h"
#include "editor/Point.h"
#include "editor/Primitive.h"

namespace editor {

Circle::Circle(PrimitiveId id, const Point& center, double radius)
	: Primitive(id)
	, center_(center)
	, radius_(radius) {
}

void Circle::draw(IRenderer& renderer) const {
	renderer.drawCircle(center_, radius_);
}

} // namespace editor
