#include "pch.h"
#include "ShootingStarManager.h"
#include "utils.h"
#include "SpriteAnimator.h"
#include "Texture.h"
#include "TextureManager.h"

ShootingStarManager::ShootingStarManager()
	:m_ShootStars{}
	, m_pShootingStars{ new SpriteAnimator(
		TextureManager::Get()->GetFileTexture("Resources/Level/", "Shootingstars.png"),
		6, 30, 668, 788, Point2f(800, 6480 - 2700)) }
{
	m_pShootingStars->SetScale(.5f);
}

ShootingStarManager::~ShootingStarManager()
{
	delete m_pShootingStars;
	m_pShootingStars = nullptr;
}

void ShootingStarManager::Draw() const
{
	if (m_ShootStars)
	{
		m_pShootingStars->Draw();
	}
}

void ShootingStarManager::UpdateShootingStars(float elapsedSec, const int percentChanceToStop)
{
	if (m_ShootStars)
	{
		m_pShootingStars->Update(elapsedSec, m_pShootingStars->GetPosition());

		if (utils::GenerateRandomRange(0, 100) > 100 - percentChanceToStop)
		{
			m_ShootStars = false;
		}
	}
}

void ShootingStarManager::TriggerShootingStars(const int percentChanceToTrigger, Rectf spawnRect)
{
	if (utils::GenerateRandomRange(0, 100) < percentChanceToTrigger)
	{
		m_ShootStars = true;

		float rnd1 = utils::GenerateRandomRange(spawnRect.left, spawnRect.width);
		float rnd2 = utils::GenerateRandomRange(spawnRect.bottom, spawnRect.bottom + spawnRect.height);
		m_pShootingStars->SetPosition(Point2f(rnd1, rnd2));
	}
}
