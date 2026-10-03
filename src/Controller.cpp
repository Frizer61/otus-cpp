#include "editor/Controller.h"

#include "editor/Document.h"
#include "editor/EditorView.h"
#include "editor/IDocumentIO.h"
#include "editor/Primitive.h"

#include <memory>
#include <string>
#include <utility>

namespace editor {

Controller::Controller(EditorView& view, IDocumentIO& documentIO)
	: view_(view)
	, documentIO_(documentIO) {
}

void Controller::createDocument() {
	document_ = std::make_unique<Document>();
	refreshView();
}

bool Controller::importDocument(const std::string& path) {
	auto loadedDocument = documentIO_.load(path);
	if (!loadedDocument) {
		return false;
	}

	document_ = std::move(loadedDocument);
	refreshView();
	return true;
}

bool Controller::exportDocument(const std::string& path) {
	return document_ && documentIO_.save(*document_, path);
}

bool Controller::addPrimitive(std::unique_ptr<Primitive> primitive) {
	if (!document_ || !document_->addPrimitive(std::move(primitive))) {
		return false;
	}

	refreshView();
	return true;
}

bool Controller::removePrimitive(PrimitiveId id) {
	if (!document_ || !document_->removePrimitive(id)) {
		return false;
	}

	refreshView();
	return true;
}

void Controller::refreshView() {
	view_.display(*document_);
}

} // namespace editor
