#include "Camera.h"
#include "Window.h"

namespace sb 
{
	Transform& Camera::getTransform()
	{
		if (m_transformNeedsUpdate)
			updateTransform();

		return m_transform;
	}
	
    void Camera::requestSize(float width, float height)
    {
        const float cameraAspect = width / height;
        const Vector2f& windowResolution = m_parentWindow.getResolution();
        const float windowAspect = windowResolution.x / windowResolution.y;

        if (windowAspect > cameraAspect) {
            float widthScale = windowAspect / cameraAspect;
            m_actualSize = Vector2f(widthScale * width, height);
        } else {
            float heightScale = cameraAspect / windowAspect;
            m_actualSize = Vector2f(width, heightScale * height);
        }
    }

    void Camera::setPosition(const sb::Vector2f & position)
	{
		m_position = position;
		m_transformNeedsUpdate = true;
	}

	void Camera::setWidth(float width)
	{
		m_width = width;
		m_transformNeedsUpdate = true;
	}

	void Camera::setHeight(float height)
	{
		setWidth(height * getAspectRatio());
	}

	void Camera::setAspectRatio(float aspect)
	{
		m_aspectRatio = aspect;
		m_inverseAspectRatio = 1 / aspect;
		m_transformNeedsUpdate = true;
	}

	void Camera::setRotation(float rotation)
	{
		m_rotation = rotation;
		m_transformNeedsUpdate = true;
	}

	void Camera::updateTransform()
	{
		float* m = m_transform.getMatrix();
		float cf = cosf(-m_rotation);
		float sf = sinf(-m_rotation);

		// sb::Vector2f inverseScale(2 / m_width, m_aspectRatio * 2 / m_width);
        sb::Vector2f inverseScale(2 / m_actualSize.x, 2 / m_actualSize.y);
		float a = cf * inverseScale.x;
		float b = -sf * inverseScale.x;
		float c = sf * inverseScale.y;
		float d = cf * inverseScale.y;
		float tx = -m_position.x;
		float ty = -m_position.y;

		m[0] = a;	m[3] = b;	m[6] = a * tx + b * ty;
		m[1] = c;	m[4] = d;	m[7] = c * tx + d * ty;
		m[2] = 0;	m[5] = 0;	m[8] = 1;
	}
}