#pragma once
#include "Drawable.h"
#include "Transformable.h"

namespace blocks {
	class Border : public sb::Drawable, public sb::Transformable {
	private:
		sb::Mesh _mesh;

	public:
		Border(float thickness, float height, bool addThicknessToSize,
			const sb::Color& innerColor, const sb::Color& outerColor);

		virtual void draw(sb::DrawTarget& target, sb::DrawStates states);
	};
}