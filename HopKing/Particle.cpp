#include "pch.h"
#include "Particle.h"
#include "Texture.h"
#include "utils.h"
#include <iostream>


Particle::Particle(float startSize, float endSize, float lifeTime, const Vector2f& velocity)
	: m_Size{ 0 }
	, m_Particle{ 0,0, startSize, startSize }
	, m_InitialSize{ startSize }
	, m_EndSize{ endSize }
	, m_LifeTime{ lifeTime }
	, m_LifeTimer{ 0 }
	, m_InitialVelocity{ velocity }
	, m_Velocity{ velocity }
	, m_IsAlive{ false }
	, m_HasVariableVelocity{false}
{
}

Particle::Particle(float startSize, float endSize, float lifeTime, const Vector2f& initVelocity, const Vector2f& endVelocity)
	: m_Size{ 0 }
	, m_Particle{ 0,0, startSize, startSize }
	, m_InitialSize{ startSize }
	, m_EndSize{ endSize }
	, m_LifeTime{ lifeTime }
	, m_LifeTimer{ 0 }
	, m_InitialVelocity{ initVelocity }
	, m_EndVelocity{ endVelocity }
	, m_HasVariableVelocity{true}
	, m_Velocity{ initVelocity }
	, m_IsAlive{ false }
{
}


void Particle::Instantiate(const Point2f& pos)
{
	m_IsAlive = true;
	m_Size = m_InitialSize;
	m_Particle.left = pos.x;
	m_Particle.bottom = pos.y;
}

void Particle::Instantiate(const Point2f& pos, const Vector2f& vel)
{
	m_IsAlive = true;
	m_Size = m_InitialSize;
	m_Particle.left = pos.x;
	m_Particle.bottom = pos.y;
	m_Velocity = vel;
}

void Particle::Instantiate(const Point2f& pos, const Vector2f& startVel, const Vector2f& endVel)
{
	m_IsAlive = true;
	m_Size = m_InitialSize;
	m_Particle.left = pos.x;
	m_Particle.bottom = pos.y;
	m_HasVariableVelocity = true;
	m_InitialVelocity = startVel;
	m_EndVelocity = endVel;
}


void Particle::Update(float elapsedSec)
{
	if (m_IsAlive)
	{
		m_Size = std::lerp(m_InitialSize, m_EndSize, m_LifeTimer / m_LifeTime);

		m_Particle.width = m_Size;
		m_Particle.height = m_Size;

		if (m_HasVariableVelocity)
		{
			m_Velocity.x = std::lerp(m_InitialVelocity.x, m_EndVelocity.x, m_LifeTimer / m_LifeTime);
			m_Velocity.y = std::lerp(m_InitialVelocity.y, m_EndVelocity.y, m_LifeTimer / m_LifeTime);
		}
		

		m_Particle.left += m_Velocity.x * elapsedSec;
		m_Particle.bottom += m_Velocity.y * elapsedSec;

		m_LifeTimer += elapsedSec;
		if (m_LifeTimer > m_LifeTime)
		{
			Reset();
		}
	}
}

void Particle::Draw() const
{
	if (m_IsAlive)
	{
		utils::SetColor(Color4f(1, 1, 1, 1));
		utils::FillRect(m_Particle);
	}
}

void Particle::Reset()
{
	m_IsAlive = false;
	m_Velocity = m_InitialVelocity;
	m_LifeTimer = 0;
}

bool Particle::IsAlive() const
{
	return m_IsAlive;
}
