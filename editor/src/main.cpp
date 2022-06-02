#include "core/Tilemap.hpp"
#include "Editor.hpp"

static void printTilemap(const Tilemap &map) {
	glm::ivec2 size = map.size();

	for (int y = 0; y < size.y; y++) {
		for (int x = 0; x < size.x; x++) {
			Tilemap::tile_t tile = map(x, y);

			if (tile)
				std::printf("%03hx ", tile);
			else
				std::printf("... ");
		}

		std::putchar('\n');
	}
}

int main() {
	Editor editor;

	std::shared_ptr<World> world = editor.openWorld("data/0.thp");
	editor.saveWorldAs(world, "exported.thp");

	for (auto &level: world->levels()) {
		std::printf("Level %s\n\n", level->name().c_str());

		for (auto &layer: level->layers()) {
			printTilemap(layer->tilemap());
			std::putchar('\n');
		}
	}

	return 0;
}