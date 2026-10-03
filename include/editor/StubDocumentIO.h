#pragma once

#include "editor/IDocumentIO.h"

#include <memory>
#include <string>

namespace editor {

class Document;

/**
 * @brief STUB: заглушка файлового слоя, файлы не читаются и не создаются.
 */
class StubDocumentIO final : public IDocumentIO {
public:
	std::unique_ptr<Document> load(const std::string& path) override;
	bool save(const Document& document, const std::string& path) override;
};

} // namespace editor
