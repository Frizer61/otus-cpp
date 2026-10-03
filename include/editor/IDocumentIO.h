#pragma once

#include <memory>
#include <string>

namespace editor {

class Document;

/**
 * @brief Интерфейс импорта и экспорта документа.
 */
class IDocumentIO {
public:
	virtual ~IDocumentIO() = default;

	/**
	 * @return Загруженный документ или nullptr при ошибке.
	 */
	virtual std::unique_ptr<Document> load(const std::string& path) = 0;

	/**
	 * @return true при успешном сохранении.
	 */
	virtual bool save(const Document& document, const std::string& path) = 0;
};

} // namespace editor
