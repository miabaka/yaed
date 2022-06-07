#pragma once

#include <memory>
#include "ICustomData.hpp"

class CustomDataContainer {
public:
	virtual ~CustomDataContainer() = default;

	template<typename T>
	T &customData() {
		return *dynamic_cast<T *>(_customData.get());
	}

	template<typename T>
	const T &customData() const {
		return *dynamic_cast<T *>(_customData.get());
	}

	void setCustomData(std::unique_ptr<ICustomData> customData);

private:
	std::unique_ptr<ICustomData> _customData;
};