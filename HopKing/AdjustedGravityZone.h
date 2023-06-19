#pragma once
#include "Vector2f.h"
#include "PhysicModifierZone.h"

class AdjustedGravityZone final : public PhysicModifierZone 
{
public:
	AdjustedGravityZone(const Rectf& zone, const Vector2f& adjustedGravity);

	virtual void ApplyAccelerationModifier(const Point2f& actorPos, Vector2f& actorAccel) override;

private:
	Vector2f m_AdjustedGravity;
};
