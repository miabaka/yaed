#include <cstdint>
#include <memory>
#include <string>
#include <fstream>
#include "Tilemap.hpp"
#include "LayerTemplate.hpp"
#include "Layer.hpp"
#include "Level.hpp"

class CrappyTestBinaryReader {
public:
	CrappyTestBinaryReader(void *buf, size_t bufSize)
			: _buf((uint8_t *) buf),
			  _bufCur(_buf),
			  _bufEnd(_buf + bufSize) {}

	template<typename T>
	T read();

	template<typename T>
	void readArrayInto(size_t count, T *data) {
		for (typeof(count) i = 0; i < count; i++)
			*(data++) = read<T>();
	}

	void skip(size_t count) {
		if (_bufCur >= _bufEnd)
			return;

		_bufCur += std::min((size_t) (_bufEnd - _bufCur), count);
	}

private:
	uint8_t *_buf;
	uint8_t *_bufEnd;
	uint8_t *_bufCur;
};

template<>
uint8_t CrappyTestBinaryReader::read() {
	if (_bufCur >= _bufEnd)
		return 0;

	return *(_bufCur++);
}

template<>
uint16_t CrappyTestBinaryReader::read() {
	auto a = read<uint8_t>();
	auto b = read<uint8_t>();

	return (b << 8) | a;
}

template<>
uint32_t CrappyTestBinaryReader::read() {
	auto a = read<uint8_t>();
	auto b = read<uint8_t>();
	auto c = read<uint8_t>();
	auto d = read<uint8_t>();

	return (d << 24) | (c << 16) | (b << 8) | a;
}

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
	auto mainLayerTemplate = std::make_shared<LayerTemplate>("Main");
	auto gemLayerTemplate = std::make_shared<LayerTemplate>("Gems");

	std::vector<char> data;

	// Pls don't kill me :O
	{
		std::ifstream file("data/0.thp", std::ios::binary);

		if (!file.is_open())
			return 1;

		file.seekg(0, std::ios_base::end);

		size_t fileSize = file.tellg();

		file.seekg(0, std::ios_base::beg);

		data.resize(fileSize);
		file.read(data.data(), (std::streamsize) data.size());
	}

	CrappyTestBinaryReader reader(data.data(), data.size());

	if (reader.read<uint32_t>() != 0x4854)
		return 2;

	reader.skip(4);

	auto levelCount = reader.read<uint32_t>();

	for (typeof(levelCount) i = 0; i < levelCount; i++) {
		auto skinIndex = reader.read<uint16_t>();

		reader.skip(14);

		glm::ivec2 size = {
				reader.read<uint16_t>(),
				reader.read<uint16_t>()
		};

		auto name = std::to_string(i + 1);

		auto mainLayer = std::make_shared<Layer>(mainLayerTemplate, size);
		auto gemLayer = std::make_shared<Layer>(gemLayerTemplate, size);

		Tilemap &mainLayerMap = mainLayer->tilemap();
		Tilemap &gemLayerMap = gemLayer->tilemap();

		static_assert(std::is_same<Tilemap::tile_t, uint16_t>());

		reader.readArrayInto(mainLayerMap.tileCount(), mainLayerMap.data());
		reader.readArrayInto(gemLayerMap.tileCount(), gemLayerMap.data());

		Level level(name, skinIndex, {gemLayer, mainLayer});

		std::printf("\nLevel %03d\n\n", i + 1);

		printTilemap(mainLayerMap);

		std::putchar('\n');

		printTilemap(gemLayerMap);
	}

	return 0;
}