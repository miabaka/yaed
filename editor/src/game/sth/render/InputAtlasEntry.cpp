#include "InputAtlasEntry.hpp"

void InputAtlasEntry::computeOffsets(std::vector<glm::vec2> &out, glm::ivec2 atlasSize) const {
	const glm::vec2 baseScale = 1.f / glm::vec2(atlasSize);

	const glm::vec2 baseOffset = glm::vec2(position) * baseScale;
	const glm::vec2 scaledFrameSize = glm::vec2(frameSize) * baseScale;

	out.reserve(frameCount);

	for (int i = 0; i < frameCount; i++) {
		const glm::vec2 offset = scaledFrameSize * glm::vec2(i % framesPerLine, i / framesPerLine);
		out.emplace_back(baseOffset + offset);
	}
}