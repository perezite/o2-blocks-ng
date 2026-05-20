#pragma once
#include <vector>
#include <algorithm>
#include <memory>

namespace o2
{
	template<class T, class U> bool contains(const T& container, const U& elem)
	{
		return std::find(container.begin(), container.end(), elem) != container.end();
	}

    template<class T> bool any(const T& container)
    {
        return !container.empty();
    }
}