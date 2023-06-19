#pragma once
#include <vector>
#include "Vector2f.h"
class Particle;

class ParticleSystem final
{
public:
	ParticleSystem(int nrParticles, float systemSize, float particleStartSize, float particleEndSize, const Vector2f& maxParticleVel, const Vector2f& minParticleVel, float minLifeTime, float maxLifeTime);
	ParticleSystem(int nrParticles, float systemSize, float particleStartSize, float particleEndSize, float minLifeTime, float maxLifeTime);

	~ParticleSystem();

	ParticleSystem(const ParticleSystem& other) = delete;
	ParticleSystem(ParticleSystem&& other) = delete;
	ParticleSystem& operator=(const ParticleSystem& other) = delete;
	ParticleSystem& operator=(ParticleSystem&& other) = delete;

	void UpdateParticles(float elapsedSec);
	void InstantiateParticlePool();
	void InstantiateParticles(const Point2f& pos);
	void InstantiateParticles(const Point2f& pos, const Vector2f& vel);
	void InstantiateParticles(const Point2f& pos, const Vector2f& startVel, const Vector2f& endVel);

	void Draw() const;

private:
	const float m_SystemSize;
	const int m_NrParticles;
	const Vector2f m_MinParticleVelocity;
	const Vector2f m_MaxParticleVelocity;
	const float m_MinParticleLifeTime;
	const float m_MaxParticleLifeTime;
	const float m_ParticleStartSize;
	const float m_ParticleEndSize;

	std::vector<Particle*> m_pParticles;

};

