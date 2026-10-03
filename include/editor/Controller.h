#pragma once

#include "editor/Document.h"
#include "editor/Primitive.h"

#include <memory>
#include <string>

namespace editor {

class EditorView;
class IDocumentIO;

/**
 * @brief Контроллер редактора (Controller).
 *
 * Владеет активным документом, представлением и файловым слоем не владеет.
 */
class Controller {
public:
	Controller(EditorView& view, IDocumentIO& documentIO);

	void createDocument();
	bool importDocument(const std::string& path);
	bool exportDocument(const std::string& path);
	bool addPrimitive(std::unique_ptr<Primitive> primitive);
	bool removePrimitive(PrimitiveId id);

private:
	void refreshView();

	EditorView& view_;
	IDocumentIO& documentIO_;
	std::unique_ptr<Document> document_;
};

} // namespace editor
