#include "pch.h"
#include "PullPhysicZone.h"
#include "utils.h"
#include "PhysicModifierZone.h"

PullPhysicZone::PullPhysicZone(const Rectf& zone, const Point2f& pullingPoint, const float pullForce)
	: PhysicModifierZone{ zone }
	, m_PullPos{pullingPoint}
	, m_PullForce{pullForce}
{
}

void PullPhysicZone::ApplyAccelerationModifier(const Point2f& actorPos, Vector2f& actorAccel)
{
	if (utils::IsPointInRect(actorPos, m_ZoneRect))
	{
		const Vector2f dirToBlackhole{ m_PullPos.x - actorPos.x, m_PullPos.y - actorPos.y };
	
		actorAccel = (dirToBlackhole.Normalized() * m_PullForce) + Vector2f(0.0f, -981.0f);
	}
}

Point2f PullPhysicZone::GetPullPos() const
{
	return m_PullPos;
}

