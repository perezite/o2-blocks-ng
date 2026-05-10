#pragma once
#include <cstdint>

namespace o2
{
	class Color
	{
		public:
			std::uint8_t r, g, b, a;
			Color() : Color(0, 0, 0, 0) { }
			Color(std::uint8_t r_, std::uint8_t g_, std::uint8_t b_, std::uint8_t a_ = 255)
				: r(r_), g(g_), b(b_), a(a_) { }
	};
}