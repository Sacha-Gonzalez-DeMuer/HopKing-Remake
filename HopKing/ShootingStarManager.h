#pragma once
class SpriteAnimator;

class ShootingStarManager final
{
public:
	ShootingStarManager();
	~ShootingStarManager();

	ShootingStarManager(const ShootingStarManager& other) = delete;
	ShootingStarManager(ShootingStarManager&& other) = delete;
	ShootingStarManager& operator=(const ShootingStarManager& other) = delete;
	ShootingStarManager& operator=(ShootingStarManager&& other) = delete;


	void Draw() const;
	void UpdateShootingStars(float elapsedSec, const int percentChanceToStop);
	void TriggerShootingStars(const int percentChanceToTrigger, Rectf spawnRect);

private:
	SpriteAnimator* m_pShootingStars;
	bool m_ShootStars;
};

