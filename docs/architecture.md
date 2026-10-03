# Диаграмма классов

- `*--` — владение через `std::unique_ptr`;
- `-->` — ссылка без владения;
- `..>` — зависимость через параметр метода;
- `<|--` — наследование, `<|..` — реализация интерфейса;
- `*` в конце метода — чисто виртуальный метод.

```mermaid
classDiagram
	class Point {
		<<struct>>
		+double x
		+double y
	}

	class Primitive {
		<<abstract>>
		-PrimitiveId id_
		+Primitive(PrimitiveId id)
		+id() PrimitiveId
		+draw(IRenderer& renderer) void*
	}

	class Circle {
		-Point center_
		-double radius_
		+Circle(PrimitiveId id, const Point& center, double radius)
		+draw(IRenderer& renderer) void
	}

	class Rectangle {
		-Point position_
		-double width_
		-double height_
		+Rectangle(PrimitiveId id, const Point& position, double width, double height)
		+draw(IRenderer& renderer) void
	}

	class Document {
		-vector~unique_ptr~Primitive~~ primitives_
		+addPrimitive(unique_ptr~Primitive~ primitive) bool
		+removePrimitive(PrimitiveId id) bool
		+forEachPrimitive(function visitor) void
	}

	class IRenderer {
		<<interface>>
		+drawCircle(const Point& center, double radius) void*
		+drawRectangle(const Point& position, double width, double height) void*
	}

	class ConsoleRenderer {
		+drawCircle(const Point& center, double radius) void
		+drawRectangle(const Point& position, double width, double height) void
	}

	class EditorView {
		-IRenderer& renderer_
		+EditorView(IRenderer& renderer)
		+display(const Document& document) void
	}

	class IDocumentIO {
		<<interface>>
		+load(const string& path) unique_ptr~Document~*
		+save(const Document& document, const string& path) bool*
	}

	class StubDocumentIO {
		<<stub>>
		+load(const string& path) unique_ptr~Document~
		+save(const Document& document, const string& path) bool
	}

	class Controller {
		-EditorView& view_
		-IDocumentIO& documentIO_
		-unique_ptr~Document~ document_
		+Controller(EditorView& view, IDocumentIO& documentIO)
		+createDocument() void
		+importDocument(const string& path) bool
		+exportDocument(const string& path) bool
		+addPrimitive(unique_ptr~Primitive~ primitive) bool
		+removePrimitive(PrimitiveId id) bool
		-refreshView() void
	}

	Primitive <|-- Circle
	Primitive <|-- Rectangle
	IRenderer <|.. ConsoleRenderer
	IDocumentIO <|.. StubDocumentIO

	Controller "1" *-- "0..1" Document
	Document "1" *-- "0..*" Primitive
	Controller --> EditorView
	Controller --> IDocumentIO
	EditorView --> IRenderer

	Primitive ..> IRenderer
	Circle ..> Point
	Rectangle ..> Point
	EditorView ..> Document
	IDocumentIO ..> Document
```

`PrimitiveId` — псевдоним `std::size_t` из `Primitive.h`.
Обработчики GUI (`onNewDocument`, `onImportDocument`, `onExportDocument`,
`onAddCircle`, `onAddRectangle`, `onRemovePrimitive`) — свободные функции
в `src/main.cpp`, они получают `Controller&` и вызывают его методы.
