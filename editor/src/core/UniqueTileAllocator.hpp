#pragma once

#include <optional>
#include <unordered_map>
#include "TilemapDefs.hpp"

class UniqueTileAllocator {
public:
	void registerUniqueRange(TilemapDefs::tile_range_t range);

	std::optional<TilemapDefs::tile_t> alloc(TilemapDefs::tile_t oldTile, TilemapDefs::tile_t tile);

	void incrementCounterFor(TilemapDefs::tile_t tile);

	void free(TilemapDefs::tile_t tile);

	void resetUsage();

private:
	struct UniqueRange {
		static const int MAX_LENGTH = 16;

		int usageCount[MAX_LENGTH]{};
		TilemapDefs::tile_range_t range;

		UniqueRange(TilemapDefs::tile_range_t range);

		void resetUsage();
	};

	std::unordered_map<TilemapDefs::tile_t, TilemapDefs::tile_t> _rangeFirstTiles;
	std::unordered_map<TilemapDefs::tile_t, UniqueRange> _ranges;
};