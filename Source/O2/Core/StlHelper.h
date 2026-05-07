#pragma once
#include <algorithm>

namespace o2
{
	template<class T, class U> bool contains(const T& container, const U& elem)
	{
		return std::find(container.begin(), container.end(), elem) != container.end();
	}
}