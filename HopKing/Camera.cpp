#include "pch.h"
#include "Camera.h"

Camera::Camera(float width, float height)
	: m_Width{1920}
	, m_Height{1080}
{
}

void Camera::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	m_LevelBoundaries = levelBoundaries;
}

void Camera::Transform(const Rectf& target)
{
	m_CamPos = Track(target);
	Rectf camRect{
		m_CamPos.x,
		m_CamPos.y,
		m_Width,
		m_Height
	};

	glTranslatef(0, -m_CamPos.y, 0);
}

Point2f Camera::Track(const Rectf& target)
{
	Point2f targetPos{ target.left, target.bottom };

	if (targetPos.y >= m_Height + m_CamPos.y)
	{
		return Point2f(0, m_CamPos.y += m_Height);
	}
	else if (targetPos.y <= m_CamPos.y)
	{
		return Point2f(0, m_CamPos.y -= m_Height);
	}

	return Point2f(0, m_CamPos.y);
}

