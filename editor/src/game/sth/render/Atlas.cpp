#include "Atlas.hpp"

#include <fstream>
#include <unordered_set>

#include <nlohmann/json.hpp>

namespace fs = std::filesystem;

struct InputEntry {
	glm::ivec2 position;
	glm::ivec2 frameSize;
	int frameCount;
	int framesPerLine;
	int firstFrameOffset;

	void computeOffsets(std::vector<glm::vec2> &out, glm::ivec2 atlasSize) const {
		const glm::vec2 baseScale = 1.f / glm::vec2(atlasSize);

		const glm::vec2 baseOffset = glm::vec2(position) * baseScale;
		const glm::vec2 scaledFrameSize = glm::vec2(frameSize) * baseScale;

		out.reserve(frameCount);

		for (int i = firstFrameOffset; i < (frameCount + firstFrameOffset); i++) {
			const glm::vec2 offset = scaledFrameSize * glm::vec2(i % framesPerLine, i / framesPerLine);
			out.emplace_back(baseOffset + offset);
		}
	}
};

Atlas::Entry::Entry(uint16_t firstFrame, uint16_t frameCount)
		: _firstFrame(firstFrame),
		  _frameCount(frameCount) {}

uint16_t Atlas::Entry::firstFrame() const {
	return _firstFrame;
}

uint16_t Atlas::Entry::frameCount() const {
	return _frameCount;
};

static nlohmann::json readJson(const fs::path &path) {
	std::ifstream file("data/sth_renderer/atlas.json");

	if (!file)
		return {};

	nlohmann::json json;
	file >> json;

	return json;
}

static glm::ivec2 ivec2FromJson(const nlohmann::json &json, glm::ivec2 defaultValue = {}) {
	if (!(json.is_array() && json.size() == 2))
		return defaultValue;

	auto &x = json[0];
	auto &y = json[1];

	if (!(x.is_number_integer() && y.is_number_integer()))
		return defaultValue;

	return {x, y};
}

static int intFromJson(const nlohmann::json &json, int defaultValue = {}) {
	if (!json.is_number_integer())
		return defaultValue;

	return json;
}

std::vector<glm::vec2> Atlas::loadFromFile(const std::filesystem::path &path) {
	const auto data = readJson("data/sth_renderer/atlas.json");

	const auto &itSize = data.find("size");
	const auto &itEntries = data.find("entries");

	if (itSize == data.cend() || itEntries == data.cend())
		return {};

	_entries.clear();

	const glm::ivec2 size = ivec2FromJson(*itSize);
	std::vector<glm::vec2> frameOffsets;

	int framesWritten = 0;

	for (auto &[_, rawEntry]: (*itEntries).items()) {
		const auto &itPosition = rawEntry.find("position");
		const auto &itFrameSize = rawEntry.find("frame_size");
		const auto &itFrameCount = rawEntry.find("frame_count");
		const auto &itFramesPerLine = rawEntry.find("frames_per_line");
		const auto &itFirstFrameOffset = rawEntry.find("first_frame_offset");
		const auto &itTiles = rawEntry.find("tiles");

		InputEntry input{};

		if (itPosition != rawEntry.cend())
			input.position = ivec2FromJson(*itPosition);

		if (itFrameSize != rawEntry.cend())
			input.frameSize = ivec2FromJson(*itFrameSize);

		if (itFrameCount == rawEntry.cend())
			input.frameCount = 1;
		else
			input.frameCount = intFromJson(*itFrameCount, 1);

		if (itFramesPerLine == rawEntry.cend())
			input.framesPerLine = input.frameCount;
		else
			input.framesPerLine = intFromJson(*itFramesPerLine, input.frameCount);

		if (itFirstFrameOffset != rawEntry.cend())
			input.firstFrameOffset = intFromJson(*itFirstFrameOffset);

		input.computeOffsets(frameOffsets, size);

		if (itTiles != rawEntry.cend()) {
			uint16_t firstFrame = (input.frameSize.x > 40) << 15 | framesWritten & 0x7fff;

			Entry entry(firstFrame, input.frameCount);

			for (Tilemap::tile_t tile: *itTiles)
				_entries.insert({tile, entry});
		}

		framesWritten += input.frameCount;
	}

	return frameOffsets;
}

const Atlas::Entry &Atlas::findEntryForTile(Tilemap::tile_t tile) const {
	auto it = _entries.find(tile);

	if (it == _entries.end())
		return _defaultEntry;

	return it->second;
}