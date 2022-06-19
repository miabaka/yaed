#include "UniqueTileAllocator.hpp"

using namespace TilemapDefs;

UniqueTileAllocator::UniqueRange::UniqueRange(tile_range_t range)
		: range(range) {}

// TODO: proper existence check
void UniqueTileAllocator::registerUniqueRange(tile_range_t range) {
	if (range.inclusiveLength() > UniqueRange::MAX_LENGTH)
		return;

	_ranges.insert({range.start(), range});

	for (tile_t tile = range.start(); tile <= range.stop(); tile++)
		_rangeFirstTiles[tile] = range.start();
}

// TODO: rewrite this scary thingy
std::optional<tile_t> UniqueTileAllocator::alloc(tile_t oldTile, tile_t tile) {
	const auto itRangeStart = _rangeFirstTiles.find(tile);

	if (itRangeStart == _rangeFirstTiles.end())
		return tile;

	const tile_t rangeStart = itRangeStart->second;

	auto itRange = _ranges.find(rangeStart);

	if (itRange == _ranges.end())
		return {};

	UniqueRange &range = itRange->second;

	if (range.range.inclusiveContains(oldTile))
		return {};

	for (int i = 0; i < range.range.inclusiveLength(); i++) {
		int &usageCount = range.usageCount[i];

		if (usageCount <= 0) {
			usageCount++;
			return rangeStart + i;
		}
	}

	return {};
}

void UniqueTileAllocator::incrementCounterFor(TilemapDefs::tile_t tile) {
	const auto itRangeStart = _rangeFirstTiles.find(tile);

	if (itRangeStart == _rangeFirstTiles.end())
		return;

	const tile_t rangeStart = itRangeStart->second;

	auto itRange = _ranges.find(rangeStart);

	if (itRange == _ranges.end())
		return;

	UniqueRange &range = itRange->second;

	if (!range.range.inclusiveContains(tile))
		return;

	range.usageCount[tile - rangeStart]++;
}

void UniqueTileAllocator::free(TilemapDefs::tile_t tile) {
	const auto itRangeStart = _rangeFirstTiles.find(tile);

	if (itRangeStart == _rangeFirstTiles.end())
		return;

	const tile_t rangeStart = itRangeStart->second;

	auto itRange = _ranges.find(rangeStart);

	if (itRange == _ranges.end())
		return;

	UniqueRange &range = itRange->second;

	if (!range.range.inclusiveContains(tile))
		return;

	range.usageCount[tile - range.range.start()] -= 1;
}

void UniqueTileAllocator::resetUsage() {
	for (auto &range: _ranges)
		range.second.resetUsage();
}

void UniqueTileAllocator::UniqueRange::resetUsage() {
	for (auto &counter: usageCount)
		counter = 0;
}