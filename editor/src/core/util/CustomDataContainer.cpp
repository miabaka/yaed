#include "CustomDataContainer.hpp"

void CustomDataContainer::setCustomData(std::unique_ptr<ICustomData> customData) {
	_customData = std::move(customData);
}