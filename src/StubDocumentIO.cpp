#include "editor/StubDocumentIO.h"

#include "editor/Circle.h"
#include "editor/Document.h"
#include "editor/Point.h"
#include "editor/Rectangle.h"

#include <iostream>
#include <memory>
#include <string>

namespace editor {

std::unique_ptr<Document> StubDocumentIO::load(const std::string& path) {
	std::cout << "[STUB IO] имитация импорта из \"" << path << "\", файл не читался\n";

	auto document = std::make_unique<Document>();
	document->addPrimitive(std::make_unique<Circle>(10, Point{50.0, 50.0}, 25.0));
	document->addPrimitive(std::make_unique<Rectangle>(11, Point{100.0, 20.0}, 40.0, 60.0));
	return document;
}

bool StubDocumentIO::save(const Document& /*document*/, const std::string& path) {
	std::cout << "[STUB IO] имитация экспорта в \"" << path << "\", файл не создавался\n";
	return true;
}

} // namespace editor
