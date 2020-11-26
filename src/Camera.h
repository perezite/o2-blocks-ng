#pragma once
#include "Vector2.h"
#include "Transform.h"

namespace sb 
{
    class Window;

	class Camera
	{
	public:
		Camera(Window& window)
			: m_parentWindow(window), m_rotation(0), m_transformNeedsUpdate(true)
		{ }

		Transform& getTransform();

		inline const sb::Vector2f& getPosition() const { return m_position; }

		inline float getRotation() const { return m_rotation; }

        inline float getWidth() const { return m_size.x; }

        inline float getHeight() const { return m_size.y; }

        inline float getAspectRatio() const { return getWidth() / getHeight(); }

        inline float getInverseAspectRatio() const { return getHeight() / getWidth(); }

		void setPosition(const sb::Vector2f& position);

		void setRotation(float rotation);
		
        void requestSize(float width, float height);

		inline void translate(const sb::Vector2f& translation) { setPosition(getPosition() + translation); }

		inline void rotate(float angle) { setRotation(getRotation() + angle); }

	protected:
		void updateTransform();

	private:
        Window& m_parentWindow;

		sb::Vector2f m_position;

        sb::Vector2f m_size;

		float m_rotation;

		Transform m_transform;

		bool m_transformNeedsUpdate;
	};
}