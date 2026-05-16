#pragma once
#include <vector>
#include <algorithm>
#include <memory>

namespace o2
{
    template<class T, typename Pred> T* singleOrNull(const std::vector<T*>& vec, Pred pred)
    {
        T* result = nullptr;
        for (const auto& item : vec) {
            if (pred(item)) {
                if (result != nullptr) throw std::runtime_error("More than one element found");
                result = item;
            }
        }

        return result;
    }

    template<class T> void deleteAll(const std::vector<T*>& vec)
    {
        for (auto* t : vec) {
            delete(t);
            t = nullptr;
        }
    }

	template<class T, class U> bool contains(const T& container, const U& elem)
	{
		return std::find(container.begin(), container.end(), elem) != container.end();
	}

    template<class T> bool any(const T& container)
    {
        return !container.empty();
    }
}