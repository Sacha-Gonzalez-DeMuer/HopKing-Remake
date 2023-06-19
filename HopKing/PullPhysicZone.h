#pragma once
#include "PhysicModifierZone.h"
class PullPhysicZone final : public PhysicModifierZone
{
public:
	PullPhysicZone(const Rectf& zone, const Point2f& pullingPoint, const float pullForce);

	virtual void ApplyAccelerationModifier(const Point2f& actorPos, Vector2f& actorAccel) override;
	Point2f GetPullPos() const;

protected:
	const Point2f m_PullPos;
	const float m_PullForce;
};

