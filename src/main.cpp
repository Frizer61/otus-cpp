#include "editor/Circle.h"
#include "editor/ConsoleRenderer.h"
#include "editor/Controller.h"
#include "editor/EditorView.h"
#include "editor/Point.h"
#include "editor/Primitive.h"
#include "editor/Rectangle.h"
#include "editor/StubDocumentIO.h"

#include <exception>
#include <iostream>
#include <memory>
#include <string>

namespace {

//----------- GUI handlers -------------

void onNewDocument(editor::Controller& controller) {
	std::cout << "\n[GUI] Новый документ\n";
	controller.createDocument();
}

void onImportDocument(editor::Controller& controller, const std::string& path) {
	std::cout << "\n[GUI] Импорт документа\n";
	if (!controller.importDocument(path)) {
		std::cout << "[GUI] Импорт не выполнен\n";
	}
}

void onExportDocument(editor::Controller& controller, const std::string& path) {
	std::cout << "\n[GUI] Экспорт документа\n";
	if (!controller.exportDocument(path)) {
		std::cout << "[GUI] Экспорт не выполнен\n";
	}
}

void onAddCircle(editor::Controller& controller, editor::PrimitiveId id, const editor::Point& center, double radius) {
	std::cout << "\n[GUI] Добавить круг #" << id << '\n';
	if (!controller.addPrimitive(std::make_unique<editor::Circle>(id, center, radius))) {
		std::cout << "[GUI] Круг не добавлен\n";
	}
}

void onAddRectangle(editor::Controller& controller, editor::PrimitiveId id, const editor::Point& position, double width, double height) {
	std::cout << "\n[GUI] Добавить прямоугольник #" << id << '\n';
	if (!controller.addPrimitive(std::make_unique<editor::Rectangle>(id, position, width, height))) {
		std::cout << "[GUI] Прямоугольник не добавлен\n";
	}
}

void onRemovePrimitive(editor::Controller& controller, editor::PrimitiveId selectedId) {
	std::cout << "\n[GUI] Удалить фигуру #" << selectedId << '\n';
	if (!controller.removePrimitive(selectedId)) {
		std::cout << "[GUI] Фигура не найдена\n";
	}
}

} // namespace

int main() {
	try {
		// Всё, на что ссылается controller, создаётся раньше и живёт дольше него.
		editor::ConsoleRenderer renderer;
		editor::EditorView view(renderer);
		editor::StubDocumentIO documentIO;
		editor::Controller controller(view, documentIO);

		onNewDocument(controller);
		onAddCircle(controller, 1, editor::Point{10.0, 20.0}, 5.0);
		onAddRectangle(controller, 2, editor::Point{0.0, 0.0}, 30.0, 15.0);
		onRemovePrimitive(controller, 1);
		onExportDocument(controller, "drawing.vec");
		onImportDocument(controller, "sample.vec");
	}
	catch (const std::exception& error) {
		std::cerr << error.what() << '\n';
		return 1;
	}

	return 0;
}
