#pragma once
#include "Vector2f.h"

class Texture;

class Particle final
{
public:
	Particle(float startSize, float endSize, float lifeTime, const Vector2f& velocity);
	Particle(float startSize, float endSize, float lifeTime, const Vector2f& initVelocity, const Vector2f& endVelocity);


	void Instantiate(const Point2f& pos);
	void Instantiate(const Point2f& pos, const Vector2f& vel);
	void Instantiate(const Point2f& pos, const Vector2f& startVel, const Vector2f& endVel);
	void Update(float elapsedSec);
	void Draw() const;
	void Reset();

	bool IsAlive() const;

private:
	Rectf m_Particle;

	bool m_IsAlive;

	float m_LifeTime;
	float m_LifeTimer;
	float m_Size;

	Vector2f m_Velocity;

	float m_InitialSize;
	Vector2f m_InitialVelocity;

	const float m_EndSize;
	bool m_HasVariableVelocity;
	Vector2f m_EndVelocity;

};

