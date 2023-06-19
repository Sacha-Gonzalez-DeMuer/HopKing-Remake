#pragma once
#include "Vector2f.h"
#include <vector>

class Texture;
class Player;
class RepelSwarmManager;
class SpriteAnimator;
class AdjustedGravityZone;
class PullPhysicZone;
class PhysicModifierZone;
class ShootingStarManager;
class GameEnd;
class Game;
class TriggerCollider;

class Level final
{
public:
	Level(const Window& window);
	~Level();
	Level(const Level& other) = delete;
	Level(Level&& other) = delete;
	Level& operator=(const Level& other) = delete;
	Level& operator=(Level&& other) = delete;


	void Initialize();
	void Update(float elapsedSec, const Point2f& playerPos, Game* pGame);

	void DrawBackground(const Point2f& playerPos) const;
	void DrawMiddleground(const Point2f& playerPos) const;
	void DrawForeground() const;
	void DrawOnTop() const;

	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, Vector2f& actorAcceleration, float elapsedSec) const;
	bool IsOnGround(const Rectf& actorShape);
	Rectf GetBoundaries() const;

	void ApplyModifiers(Player& player);

	void ProcessKeyDownEvent();
	bool IsLevelEnding() const;

private:
	//DATA MEMBERS
	std::vector<std::vector<Point2f>> m_Vertices;
	std::vector<std::vector<Point2f>> m_SlipperyVertices;
	void HandleSlipperySurfaces(Rectf& actorShape, Vector2f& actorVelocity, Vector2f& actorAcceleration, float elapsedSec) const;

	const Point2f m_OldLevelPos;
	const Rectf m_Boundaries;

	AdjustedGravityZone* m_pLowGZone;
	ShootingStarManager* m_pShootingStars;
	RepelSwarmManager* m_pAsteroidSwarm;

	PullPhysicZone* m_pBlackholeZone;
	SpriteAnimator* m_pBlackHoleDistortion;

	GameEnd* m_pGameEnd;

	std::vector<PhysicModifierZone*> m_ModifierZones;

	const float m_BounceFactor;

	//files
	const std::string m_ImageFolderPath;
	const std::string m_LevelBackgroundFileName;
	const std::string m_OldLevelFileName;
	const std::string m_LevelForegroundFileName;
	const std::string m_PlatformsFileName;

	//sound
	const std::string m_ForestMusic;
	bool m_ForestMusicPlaying;
	const std::string m_BlackholeMusic;
	bool m_BlackholeMusicPlaying;


};

