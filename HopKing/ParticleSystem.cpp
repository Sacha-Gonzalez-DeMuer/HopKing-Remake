#include "pch.h"
#include "ParticleSystem.h"
#include "Particle.h"
#include "utils.h"

ParticleSystem::ParticleSystem(int nrParticles, float systemSize, float particleStartSize, float particleEndSize, const Vector2f& maxParticleVel, const Vector2f& minParticleVel, float minLifeTime, float maxLifeTime)
	: m_NrParticles{ nrParticles }
	, m_ParticleStartSize { particleStartSize }
	, m_ParticleEndSize { particleEndSize }
	, m_SystemSize{ systemSize }
	, m_MinParticleVelocity{ minParticleVel }
	, m_MaxParticleVelocity{ maxParticleVel }
	, m_MinParticleLifeTime{ minLifeTime }
	, m_MaxParticleLifeTime{ maxLifeTime }
{
	InstantiateParticlePool();
}

ParticleSystem::ParticleSystem(int nrParticles, float systemSize, float particleStartSize, float particleEndSize, float minLifeTime, float maxLifeTime)
	: m_NrParticles{ nrParticles }
	, m_ParticleStartSize{ particleStartSize }
	, m_ParticleEndSize{ particleEndSize }
	, m_SystemSize{ systemSize }
	, m_MinParticleVelocity{ 0,0 }
	, m_MaxParticleVelocity{ 0,0 }
	, m_MinParticleLifeTime{ minLifeTime }
	, m_MaxParticleLifeTime{ maxLifeTime }
{
	InstantiateParticlePool();
}

ParticleSystem::~ParticleSystem()
{
	for (int i = 0; i < m_pParticles.size(); i++)
	{
		delete m_pParticles[i];
		m_pParticles[i] = nullptr;
	}
	m_pParticles.clear();
}

void ParticleSystem::UpdateParticles(float elapsedSec)
{
	for (int i = 0; i < m_pParticles.size(); i++)
	{
		m_pParticles[i]->Update(elapsedSec);
	}
}

void ParticleSystem::InstantiateParticlePool()
{
	for (int i = 0; i < m_NrParticles; i++)
	{
		m_pParticles.push_back(new Particle(m_ParticleStartSize, m_ParticleEndSize, utils::GenerateRandomRange(m_MinParticleLifeTime, m_MaxParticleLifeTime),
			Vector2f(utils::GenerateNegativeRandomRange(m_MinParticleVelocity.x, m_MaxParticleVelocity.x), 
				utils::GenerateNegativeRandomRange(m_MinParticleVelocity.y, m_MaxParticleVelocity.y))));
	}
}

void ParticleSystem::InstantiateParticles(const Point2f& pos)
{
	for (int i = 0; i < m_pParticles.size(); i++)
	{
		Point2f randomPos{ pos };
		randomPos.x += utils::GenerateNegativeRandomRange(-m_SystemSize, m_SystemSize);
		randomPos.y += utils::GenerateNegativeRandomRange(-m_SystemSize, m_SystemSize);

		m_pParticles[i]->Instantiate(randomPos);
	}
}

void ParticleSystem::InstantiateParticles(const Point2f& pos, const Vector2f& vel)
{
	for (int i = 0; i < m_pParticles.size(); i++)
	{
		Point2f randomPos{ pos };
		randomPos.x += utils::GenerateNegativeRandomRange(-m_SystemSize/2, m_SystemSize/2);
		randomPos.y += utils::GenerateNegativeRandomRange(-m_SystemSize/2, m_SystemSize/2);

		m_pParticles[i]->Instantiate(randomPos, vel);
	}
}

void ParticleSystem::InstantiateParticles(const Point2f& pos, const Vector2f& startVel, const Vector2f& endVel)
{
	for (int i = 0; i < m_pParticles.size(); i++)
	{
		Point2f randomPos{ pos };
		randomPos.x += utils::GenerateNegativeRandomRange(-m_SystemSize / 2, m_SystemSize / 2);
		randomPos.y += utils::GenerateNegativeRandomRange(-m_SystemSize / 2, m_SystemSize / 2);

		m_pParticles[i]->Instantiate(randomPos, startVel, endVel);
	}
}

void ParticleSystem::Draw() const
{
	for (int i = 0; i < m_pParticles.size(); i++)
	{
		m_pParticles[i]->Draw();
	}
}
