#pragma once

class AtlasEntry {
public:
	explicit AtlasEntry(int firstFrame, int frameCount = 1);

	int firstFrame() const;

	int frameCount() const;

private:
	int _firstFrame;
	int _frameCount;
};