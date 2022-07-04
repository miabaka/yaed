#pragma once

#include <vector>

#include "../core/palette/BrushSelection.hpp"
#include "../core/palette/Brush.hpp"
#include "../core/tool/BaseTool.hpp"
#include "../core/Tilemap.hpp"

class PencilTool : public BaseTool {
public:
	const std::string &name() const override;

	void handleMouseEnter() override;

	void handleMouseMove(glm::ivec2 position, glm::ivec2 tilePosition) override;

	void handleMousePress(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) override;

	void handleMouseRelease(MouseButton button, glm::ivec2 position, glm::ivec2 tilePosition) override;

	void update(Tilemap &tilemap, std::weak_ptr<BrushSelection> brushSelection);

private:
	std::vector<glm::ivec2> _pathTiles;
	glm::ivec2 _currentPosition{};
	bool _drawing = false;
	bool _mouseJustEntered = false;
	MouseButton _drawingButton{};
	std::shared_ptr<Brush> _drawingBrush;
	std::weak_ptr<BrushSelection> _brushSelection;

	void moveTo(glm::ivec2 position);

	void lineTo(glm::ivec2 position);

	void stroke(Tilemap &tilemap, const Brush &brush);

	void discard();
};