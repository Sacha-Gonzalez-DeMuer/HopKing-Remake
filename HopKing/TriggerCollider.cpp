#include "pch.h"
#include "TriggerCollider.h"
#include "utils.h"

TriggerCollider::TriggerCollider(const Rectf& collider)
	:m_Collider{collider}
{
}

bool TriggerCollider::OnTriggerEnter(const Point2f& targetPos) const
{
	return utils::IsPointInRect(targetPos, m_Collider);
}
