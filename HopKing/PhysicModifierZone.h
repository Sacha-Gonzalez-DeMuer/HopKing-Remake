#pragma once
#include "Vector2f.h"

class PhysicModifierZone
{
public:
	PhysicModifierZone(const Rectf& zone);

	virtual void ApplyAccelerationModifier(const Point2f& actorPos, Vector2f& actorAccel) = 0;

	Rectf GetZone() const;

protected:
	const Rectf m_ZoneRect;
};
