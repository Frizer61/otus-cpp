#pragma once

namespace editor {

class Document;
class IRenderer;

/**
 * @brief Представление документа (View).
 */
class EditorView {
public:
	/**
	 * @param renderer Не принадлежит представлению и должен его пережить.
	 */
	explicit EditorView(IRenderer& renderer);

	void display(const Document& document);

private:
	IRenderer& renderer_;
};

} // namespace editor
