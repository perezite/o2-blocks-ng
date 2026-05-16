#pragma once
#include <string>
#include <vector>

namespace o2
{
	std::vector<std::string> split(const std::string& str, const std::string& separator)
	{
		if (separator.empty())
			throw std::runtime_error("separator must not be empty");

		size_t pos = 0;
		size_t newpos = 0;
		std::vector<std::string> parts;
		while(true) {
			newpos = str.find(separator, pos);
			if (newpos == std::string::npos)
				break;

			parts.push_back(str.substr(pos, newpos - pos));
			pos = newpos + separator.length();
		}

		parts.push_back(str.substr(pos));
		return parts;
	}
}