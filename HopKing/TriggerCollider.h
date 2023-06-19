#pragma once
class TriggerCollider final
{
public:
	TriggerCollider(const Rectf& collider);

	bool OnTriggerEnter(const Point2f& targetPos) const;
private:
	const Rectf m_Collider;
};

