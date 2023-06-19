#include "pch.h"
#include "AdjustedGravityZone.h"
#include "utils.h"

AdjustedGravityZone::AdjustedGravityZone(const Rectf& zone, const Vector2f& adjustedGravity)
	: PhysicModifierZone{zone}
	, m_AdjustedGravity{adjustedGravity}
{
}

void AdjustedGravityZone::ApplyAccelerationModifier(const Point2f& actorPos, Vector2f& actorAccel)
{
	if (utils::IsPointInRect(actorPos, m_ZoneRect))
	{
		actorAccel = m_AdjustedGravity;
	}
}
