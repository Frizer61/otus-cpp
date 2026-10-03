#pragma once

#include "editor/Primitive.h"

#include <functional>
#include <memory>
#include <vector>

namespace editor {

/**
 * @brief Векторный документ (Model). Единолично владеет фигурами.
 */
class Document {
public:
	/**
	 * @brief Добавляет фигуру.
	 *
	 * Владение переходит к методу в момент вызова: при отказе фигура
	 * уничтожается и вызывающему коду не возвращается.
	 *
	 * @return false для nullptr или повторяющегося идентификатора.
	 */
	bool addPrimitive(std::unique_ptr<Primitive> primitive);

	/**
	 * @return false, если фигуры с таким идентификатором нет.
	 */
	bool removePrimitive(PrimitiveId id);

	/**
	 * @brief Обходит фигуры только для чтения.
	 */
	void forEachPrimitive(const std::function<void(const Primitive&)>& visitor) const;

private:
	std::vector<std::unique_ptr<Primitive>> primitives_;
};

} // namespace editor
