#include "editor/Document.h"

#include "editor/Primitive.h"

#include <algorithm>
#include <functional>
#include <memory>
#include <utility>

namespace editor {

namespace {

auto hasId(PrimitiveId id) {
	return [id](const std::unique_ptr<Primitive>& primitive) {
		return primitive->id() == id;
	};
}

} // namespace

bool Document::addPrimitive(std::unique_ptr<Primitive> primitive) {
	if (!primitive || std::any_of(primitives_.begin(), primitives_.end(), hasId(primitive->id()))) {
		return false;
	}

	primitives_.push_back(std::move(primitive));
	return true;
}

bool Document::removePrimitive(PrimitiveId id) {
	const auto found = std::find_if(primitives_.begin(), primitives_.end(), hasId(id));
	if (found == primitives_.end()) {
		return false;
	}

	primitives_.erase(found);
	return true;
}

void Document::forEachPrimitive(const std::function<void(const Primitive&)>& visitor) const {
	for (const auto& primitive : primitives_) {
		visitor(*primitive);
	}
}

} // namespace editor
