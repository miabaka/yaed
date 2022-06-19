#pragma once

#include "../../core/render/ITilemapRendererContext.hpp"
#include "../../core/tool/PencilTool.hpp"
#include "../../core/BaseEditor.hpp"
#include "../../core/Level.hpp"
#include "../BaseWindow.hpp"

class ViewportWindow : public BaseWindow {
public:
	ViewportWindow();

	void setLevel(BaseEditor &editor, std::shared_ptr<Level> level);

	void setBrushSelectionSource(std::weak_ptr<BrushSelectionManager> brushSelection);

protected:
	void onBeginPre() override;

	void onBeginPost() override;

	void onDraw() override;

	void onRender() override;

private:
	std::weak_ptr<Level> _level;
	std::shared_ptr<ITilemapRendererContext> _rendererContext;
	std::weak_ptr<BrushSelectionManager> _selectionManager;
	PencilTool _pencil;
	bool _drawing = false;

	void processEdits();
};