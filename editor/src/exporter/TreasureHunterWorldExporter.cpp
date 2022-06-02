#include <fstream>
#include "TreasureHunterWorldExporter.hpp"

namespace fs = std::filesystem;

const std::string &TreasureHunterWorldExporter::name() const {
    static std::string name = "TreasureHunterWorldExporter";
    return name;
}

std::set<std::string> TreasureHunterWorldExporter::supportedGamesIds() const {
    return {"sth"};
}

void TreasureHunterWorldExporter::save(const World &world, const std::filesystem::path &path) const {
    std::ofstream file(path, std::ios::binary);

    char header[12] = {'T', 'H', 0};
    *((uint32_t *) &header[8]) = world.levels().size();
    file.write(header, sizeof(header));

    char level_header[20] = {0};
    for (const auto &level: world.levels()) {
        *((uint16_t *) &level_header[0]) = level->skinIndex();

        glm::ivec2 size = level->layers()[0]->tilemap().size();
        *((uint16_t *) &level_header[16]) = size.x;
        *((uint16_t *) &level_header[18]) = size.y;
        file.write(level_header, sizeof(level_header));

        static_assert(std::is_same<Tilemap::tile_t, uint16_t>());

        for (auto i = level->layers().rbegin(); i != level->layers().rend(); i++) {
            const auto &layer = *i;
            const auto &tilemap = layer->tilemap();
            file.write(reinterpret_cast<const char *>(tilemap.data()), sizeof(Tilemap::tile_t) * tilemap.tileCount());
        }
    }

    file.flush();
}