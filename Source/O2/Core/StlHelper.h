#pragma once
#include <vector>
#include <algorithm>
#include <memory>

namespace o2
{
    template<class T, typename Pred> T* singleOrNull(const std::vector<std::unique_ptr<T>>& vec, Pred pred)
    {
        T* result = nullptr;

        for (const auto& item : vec) {
            if (pred(*item)) {
                if (result != nullptr)
                    throw std::runtime_error("More than one element found");

                result = item.get();
            }
        }

        return result;
    }

	template<class T, class U> bool contains(const T& container, const U& elem)
	{
		return std::find(container.begin(), container.end(), elem) != container.end();
	}
}