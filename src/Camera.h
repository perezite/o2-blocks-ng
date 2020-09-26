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
			:   m_parentWindow(window), m_width(1), m_aspectRatio(1), 
                m_rotation(0), m_transformNeedsUpdate(true)
		{ }

		Transform& getTransform();

		inline const sb::Vector2f& getPosition() const { return m_position; }

		inline float getWidth() const { return m_width; }

		inline float getAspectRatio() const { return m_aspectRatio; }

		inline float getInverseAspectRatio() const { return m_inverseAspectRatio; }

		inline float getRotation() const { return m_rotation; }

        void requestSize(float width, float height);

		void setPosition(const sb::Vector2f& position);

		void setWidth(float width);

		void setHeight(float height);

		void setAspectRatio(float aspect);

		void setRotation(float rotation);
		
		inline void translate(const sb::Vector2f& translation) { setPosition(getPosition() + translation); }

		inline void rotate(float angle) { setRotation(getRotation() + angle); }

	protected:
		void updateTransform();

	private:
        Window& m_parentWindow;

		sb::Vector2f m_position;

        sb::Vector2f m_actualSize;

		float m_width;

		float m_aspectRatio;

		float m_inverseAspectRatio;

		float m_rotation;

		Transform m_transform;

		bool m_transformNeedsUpdate;
	};
}