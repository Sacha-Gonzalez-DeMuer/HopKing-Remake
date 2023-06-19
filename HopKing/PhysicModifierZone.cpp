#include "pch.h"
#include "PhysicModifierZone.h"


PhysicModifierZone::PhysicModifierZone(const Rectf& zone)
	:m_ZoneRect{zone}
{
}

Rectf PhysicModifierZone::GetZone() const
{
	return m_ZoneRect;
}
