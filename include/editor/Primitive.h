#pragma once

#include <cstddef>

namespace editor {

class IRenderer;

/**
 * @brief Идентификатор фигуры.
 *
 * Не является индексом в коллекции и не меняется в течение жизни фигуры.
 */
using PrimitiveId = std::size_t;

/**
 * @brief Базовый класс графического примитива.
 */
class Primitive {
public:
	explicit Primitive(PrimitiveId id)
		: id_(id) {
	}

	/**
	 * @brief Виртуальный деструктор: документ удаляет фигуры
	 * через std::unique_ptr<Primitive>.
	 */
	virtual ~Primitive() = default;

	[[nodiscard]] PrimitiveId id() const {
		return id_;
	}

	/**
	 * @brief Передаёт данные фигуры в renderer.
	 */
	virtual void draw(IRenderer& renderer) const = 0;

private:
	PrimitiveId id_;
};

} // namespace editor
