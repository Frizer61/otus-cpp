#include "editor/EditorView.h"

#include "editor/Document.h"
#include "editor/IRenderer.h"
#include "editor/Primitive.h"

namespace editor {

EditorView::EditorView(IRenderer& renderer)
	: renderer_(renderer) {
}

void EditorView::display(const Document& document) {
	document.forEachPrimitive([this](const Primitive& primitive) {
		primitive.draw(renderer_);
	});
}

} // namespace editor
