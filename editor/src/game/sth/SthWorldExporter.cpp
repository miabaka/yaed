#include "SthWorldExporter.hpp"

#include <fstream>

const std::string &SthWorldExporter::name() const {
	static std::string name = "SthWorldExporter";
	return name;
}

std::set<std::string> SthWorldExporter::supportedGameIds() const {
	return {"sth"};
}

// bruh gal wtf
// FIXME: basically unreadable, unsafe and endian-dependent
void SthWorldExporter::save(const World &world, const std::filesystem::path &path) const {
	std::ofstream file(path, std::ios::binary);

	char header[12] = {'T', 'H', 0};
	*((uint32_t *) &header[8]) = world.levels().size();
	file.write(header, sizeof(header));

	char level_header[20] = {0};
	for (const auto &level: world.levels()) {
		*((uint16_t *) &level_header[0]) = level->skin()->id();

		glm::ivec2 size = level->layers()[0]->tilemap().size();
		*((uint16_t *) &level_header[16]) = size.x;
		*((uint16_t *) &level_header[18]) = size.y;
		file.write(level_header, sizeof(level_header));

		static_assert(std::is_same<Tilemap::tile_t, uint16_t>());

		for (auto i = level->layers().rbegin(); i != level->layers().rend(); i++) {
			const auto &layer = *i;
			const auto &tilemap = layer->tilemap();

			const auto &tiles = tilemap.tiles();

			file.write(
					reinterpret_cast<const char *>(tiles.data()),
					static_cast<std::streamsize>(tiles.size_bytes())
			);
		}
	}

	file.flush();
}