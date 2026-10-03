#include "editor/Rectangle.h"

#include "editor/IRenderer.h"
#include "editor/Point.h"
#include "editor/Primitive.h"

namespace editor {

Rectangle::Rectangle(PrimitiveId id, const Point& position, double width, double height)
	: Primitive(id)
	, position_(position)
	, width_(width)
	, height_(height) {
}

void Rectangle::draw(IRenderer& renderer) const {
	renderer.drawRectangle(position_, width_, height_);
}

} // namespace editor
