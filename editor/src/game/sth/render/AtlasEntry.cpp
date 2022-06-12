#include "AtlasEntry.hpp"

AtlasEntry::AtlasEntry(int firstFrame, int frameCount)
		: _firstFrame(firstFrame),
		  _frameCount(frameCount) {}

int AtlasEntry::firstFrame() const {
	return _firstFrame;
}

int AtlasEntry::frameCount() const {
	return _frameCount;
};