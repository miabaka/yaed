#include "SthWorldImporter.hpp"

#include <cstring>
#include <fstream>

#include "../../core/game/BaseGame.hpp"

namespace fs = std::filesystem;

static const char THP_MAGIC[] = {'T', 'H', '\0', '\0'};

const std::string &SthWorldImporter::name() const {
	static std::string name = "SthWorldImporter";
	return name;
}

bool SthWorldImporter::probe(const fs::path &path) const {
	std::ifstream file(path, std::ios::binary);

	char magicBuf[sizeof(THP_MAGIC)];
	file.read(magicBuf, sizeof(magicBuf));

	if (!file.good())
		return false;

	return std::memcmp(magicBuf, THP_MAGIC, sizeof(THP_MAGIC)) == 0;
}

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
		while (count--)
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

std::shared_ptr<World>
SthWorldImporter::load(
		const std::filesystem::path &path, const GameManager &gameManager,
		const WorldFactoryManager &worldFactoryManager) const {
	std::shared_ptr<IGame> game = gameManager.findGameById("sth");
	std::shared_ptr<IWorldFactory> worldFactory = worldFactoryManager.findFactoryById("sth");

	std::shared_ptr<World> world = worldFactory->createWorld(gameManager, {});

	world->setPath(path);

	std::vector<char> data;

	{
		std::ifstream file(path, std::ios::binary | std::ios::ate);

		std::streamsize size = file.tellg();

		file.seekg(0, std::ios_base::beg);

		data.resize(size);
		file.read(data.data(), size);
	}

	CrappyTestBinaryReader reader(data.data(), data.size());

	if (reader.read<uint32_t>() != 0x4854)
		return world;

	reader.skip(4);

	auto levelCount = std::min(reader.read<uint32_t>(), 1'000u);

	for (int i = 0; i < levelCount; i++) {
		std::shared_ptr<LevelSkin> skin = game->findLevelSkinById(reader.read<uint16_t>());

		reader.skip(14);

		glm::ivec2 size = {
				reader.read<uint16_t>(),
				reader.read<uint16_t>()
		};

		if (size != glm::ivec2(40, 30))
			throw std::runtime_error("Wrong level dimensions");

		auto name = std::to_string(i + 1);

		auto level = worldFactory->createLevel(gameManager, name, skin);

		Tilemap &gemLayerMap = level->layers()[0]->tilemap();
		Tilemap &mainLayerMap = level->layers()[1]->tilemap();

		static_assert(std::is_same<Tilemap::tile_t, uint16_t>());

		reader.readArrayInto(mainLayerMap.tileCount(), mainLayerMap.data());
		reader.readArrayInto(gemLayerMap.tileCount(), gemLayerMap.data());

		world->addLevel(level);
		level->setWorld(world);
	}

	return world;
}