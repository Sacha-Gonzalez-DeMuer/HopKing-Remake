#include "pch.h"
#include "Level.h"
#include "utils.h"
#include "SVGParser.h"
#include "Texture.h"
#include "Player.h"
#include "RepelSwarmManager.h"
#include "SpriteAnimator.h"
#include "PullPhysicZone.h"
#include "AdjustedGravityZone.h"
#include "TextureManager.h"
#include "ShootingStarManager.h"
#include "GameEnd.h"
#include "SoundManager.h"

Level::Level(const Window& window)
	: m_Vertices{}
	, m_Boundaries{0,0,1920,6480}
	, m_BounceFactor{ -.5f }
	, m_OldLevelPos{ 0, 4323 }
	, m_pLowGZone{ new AdjustedGravityZone(Rectf(0,1080 * 3, 1920,1080),  Vector2f(0.0f, -981.0f / 1.5f)) }
	, m_pBlackholeZone{ new PullPhysicZone( Rectf(0, 1080*4, 1920,1080), Point2f(440, 4830), 900) }
	, m_pAsteroidSwarm{ new RepelSwarmManager(15, "Resources/Level/Asteroids/", Rectf(500, 6480/6 * 3, 800, 1370), 100)}
	, m_pBlackHoleDistortion{ }
	, m_pShootingStars{new ShootingStarManager()}
	, m_pGameEnd{new GameEnd(window, Rectf(1486.0f, 6480.0f - 720.0f, 1000.0f, 720.0f))}
	, m_ForestMusic{"boggMusic.mp3"}
	, m_BlackholeMusic{"blackholeMusic.mp3"}
	, m_ForestMusicPlaying{false}
	, m_BlackholeMusicPlaying{false}
	, m_ImageFolderPath{"Resources/Level/"}
	, m_LevelBackgroundFileName{ "LevelBackground.png"}
	, m_OldLevelFileName{"OldLevel.png"}
	, m_LevelForegroundFileName{ "LevelForeground.png" }
	, m_PlatformsFileName{ "Platforms.png" }
{
	Initialize();
}

Level::~Level()
{
	delete m_pAsteroidSwarm;
	m_pAsteroidSwarm = nullptr;

	delete m_pLowGZone;
	m_pLowGZone = nullptr;

	delete m_pBlackholeZone;
	m_pBlackholeZone = nullptr;

	delete m_pBlackHoleDistortion;
	m_pBlackHoleDistortion = nullptr;

	delete m_pShootingStars;
	m_pShootingStars = nullptr;

	delete m_pGameEnd;
	m_pGameEnd = nullptr;
}

void Level::Initialize()
{
	//initialize blackhole distortion animation
	TextureManager::Get()->AddTexture("BlackHoleDistortion", new Texture("Resources/Level/BlackholeDistortion.png"));
	m_pBlackHoleDistortion = new SpriteAnimator(TextureManager::Get()->GetTexture("BlackHoleDistortion"), 6, 60, 610, 610, m_pBlackholeZone->GetPullPos());

	//load level colliders
	SVGParser::GetVerticesFromSvgFile("Resources/Level/normalColliders.svg", m_Vertices);
	SVGParser::GetVerticesFromSvgFile("Resources/Level/slipperyColliders.svg", m_SlipperyVertices);

	//push modifierzones
	m_ModifierZones.push_back(m_pBlackholeZone);
	m_ModifierZones.push_back(m_pLowGZone);

	TextureManager::Get()->GetFileTexture(m_ImageFolderPath, m_LevelBackgroundFileName);
	TextureManager::Get()->GetFileTexture(m_ImageFolderPath, m_OldLevelFileName);
	TextureManager::Get()->GetFileTexture(m_ImageFolderPath, m_LevelForegroundFileName);
	TextureManager::Get()->GetFileTexture(m_ImageFolderPath, m_PlatformsFileName);

	SoundManager::Get()->AddSoundStream(m_ForestMusic);
	SoundManager::Get()->AddSoundStream(m_BlackholeMusic);
}

void Level::Update(float elapsedSec, const Point2f& playerPos, Game* pGame)
{
	if (!IsLevelEnding())
	{
		m_pAsteroidSwarm->Update(elapsedSec, playerPos);
		m_pBlackHoleDistortion->Update(elapsedSec, m_pBlackHoleDistortion->GetPosition());

		m_pShootingStars->TriggerShootingStars(1, m_pLowGZone->GetZone());
		m_pShootingStars->UpdateShootingStars(elapsedSec, 5);


		if (playerPos.y > m_pBlackholeZone->GetZone().bottom)
		{
			if (m_ForestMusicPlaying)
			{
				SoundManager::Get()->StopSoundStream(m_ForestMusic);
				m_ForestMusicPlaying = false;
			}
			if (!m_BlackholeMusicPlaying)
			{
				SoundManager::Get()->PlaySoundStream(m_BlackholeMusic, true);
				m_BlackholeMusicPlaying = true;
			}
		}
		else
		{
			if (m_BlackholeMusicPlaying)
			{
				SoundManager::Get()->StopSoundStream(m_BlackholeMusic);
				m_BlackholeMusicPlaying = false;
			}
			
			if (!m_ForestMusicPlaying)
			{

				SoundManager::Get()->PlaySoundStream(m_ForestMusic, true);
				m_ForestMusicPlaying = true;
			}
		}
	}

	m_pGameEnd->Update(playerPos, pGame, elapsedSec );
}

void Level::DrawBackground(const Point2f& playerPos) const
{
	TextureManager::Get()->GetTexture("OldLevel.png")->DrawTextureWithParalax(m_OldLevelPos, .03f, playerPos);
	
	TextureManager::Get()->GetTexture("LevelBackground.png")->Draw();

	m_pShootingStars->Draw();
}

void Level::DrawMiddleground(const Point2f& playerPos) const {

	m_pAsteroidSwarm->Draw();
	TextureManager::Get()->GetTexture("Platforms.png")->Draw();
}

void Level::DrawForeground() const
{
	m_pBlackHoleDistortion->Draw();
	utils::SetColor(Color4f(1, 1, 1, 1));

	TextureManager::Get()->GetTexture("LevelForeground.png")->Draw();
}

void Level::DrawOnTop() const
{
	if (m_pGameEnd->IsEnding())
	{
		m_pGameEnd->DrawEndingSequence();
	}
}

void Level::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity, Vector2f& actorAcceleration, float elapsedSec) const
{
	HandleSlipperySurfaces(actorShape, actorVelocity, actorAcceleration, elapsedSec);
	const float errorMargin{ 1 };
	Point2f actorCenter{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };

	Point2f actorTop{ actorCenter };
	actorTop.y += actorShape.height / 2;

	Point2f actorBot{ actorCenter };
	actorBot.y -= actorShape.height / 2;

	Point2f actorLeft{ actorCenter };
	actorLeft.x -= actorShape.width / 2;

	Point2f actorRight{ actorCenter };
	actorRight.x += actorShape.width / 2;
	
	Point2f actorBotCenter{ actorCenter };
	actorBotCenter.y -= actorShape.height / 2;
	actorBotCenter.y += errorMargin;

	Point2f actorTopCenter{ actorCenter };
	actorTopCenter.y += actorShape.height / 2;

	Point2f actorLeftCenter{ actorCenter };
	actorLeftCenter.x -= actorShape.width / 2;

	Point2f actorRightCenter{ actorCenter };
	actorRightCenter.x += actorShape.width / 2;

	Point2f actorBotR{ actorCenter };
	actorBotR.x += actorShape.width / 2;
	actorBotR.y -= actorShape.height / 2;
	actorBotR.y += errorMargin;
	

	Point2f actorTopR{ actorCenter };
	actorTopR.y += actorShape.height / 2;
	actorTopR.x += actorShape.width / 2;

	Point2f actorBotL{ actorShape.left, actorShape.bottom };
	actorBotL.y += errorMargin;

	Point2f actorTopL{ actorBotL };
	actorTopL.y += actorShape.height;

	utils::HitInfo hit{};

	//mesh collision
	for (unsigned int i = 0; i < m_Vertices.size(); ++i)
	{
		
		//right
		if (utils::Raycast(m_Vertices[i], actorCenter, actorRight, hit)
			|| utils::Raycast(m_Vertices[i], actorTopCenter, actorTopR, hit)
			|| utils::Raycast(m_Vertices[i], actorBotCenter, actorBotR, hit))
		{
			if (hit.intersectPoint.y > actorBot.y + 1)
			{
				actorShape.left = hit.intersectPoint.x - actorShape.width - errorMargin;
				actorVelocity.x *= m_BounceFactor;

			}
		}

		//left
		if (utils::Raycast(m_Vertices[i], actorCenter, actorLeft, hit)
			|| utils::Raycast(m_Vertices[i], actorTopCenter, actorTopL, hit)
			|| utils::Raycast(m_Vertices[i], actorBotCenter, actorBotL, hit))
		{
			if (hit.intersectPoint.y >actorBot.y + 1)
			{
				actorShape.left = hit.intersectPoint.x + errorMargin;
				actorVelocity.x *= m_BounceFactor;


			}
		}

		//top
		if (utils::Raycast(m_Vertices[i], actorCenter, actorTop, hit)
			|| utils::Raycast(m_Vertices[i], actorRightCenter, actorTopR, hit)
			|| utils::Raycast(m_Vertices[i], actorLeftCenter, actorTopL, hit) && actorVelocity.y > 0)
		{
			if (hit.intersectPoint.y <= actorShape.bottom + actorShape.height)
			{
				actorShape.bottom = hit.intersectPoint.y - actorShape.height - errorMargin * 2;
			}
			actorVelocity.y = 0;
			actorVelocity.x = 0;
		}

		//bottom collision
		if (utils::Raycast(m_Vertices[i], actorCenter, actorBot, hit)
			|| utils::Raycast(m_Vertices[i], actorLeftCenter, actorBotL, hit)
			|| utils::Raycast(m_Vertices[i], actorRightCenter, actorBotR, hit)
			&& actorVelocity.y < 0)
		{
			actorShape.bottom = hit.intersectPoint.y;
			actorAcceleration.x = 1000.0f;

		}
	}

	//boundaries collision
	if (actorShape.left <= 0)
	{
		actorShape.left = 0;
		actorVelocity.x *= m_BounceFactor;
	}
	else if (actorShape.left + actorShape.width >= m_Boundaries.width)
	{
		actorShape.left = m_Boundaries.width - actorShape.width;
		actorVelocity.x *= m_BounceFactor;
	}
	if (actorShape.bottom <= 0)
	{
		actorShape.bottom = 0;
	}
	else if (actorShape.bottom >= m_Boundaries.height)
	{
		actorShape.bottom = m_Boundaries.height;
	}

}

void Level::HandleSlipperySurfaces(Rectf& actorShape, Vector2f& actorVelocity, Vector2f& actorAcceleration, float elapsedSec) const
{
	const float errorMargin{ 1 };

	Point2f actorCenter{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height / 2 };
	Point2f actorTop{ actorCenter };
	actorTop.y += actorShape.height / 2;
	Point2f actorBot{ actorCenter };
	actorBot.y -= actorShape.height / 2;
	Point2f actorLeft{ actorCenter };
	actorLeft.x -= actorShape.width / 2;
	Point2f actorRight{ actorCenter };
	actorRight.x += actorShape.width / 2;
	Point2f actorBotCenter{ actorCenter };
	actorBotCenter.y -= actorShape.height / 2;
	actorBotCenter.y += errorMargin;
	Point2f actorTopCenter{ actorCenter };
	actorTopCenter.y += actorShape.height / 2;
	Point2f actorLeftCenter{ actorCenter };
	actorLeftCenter.x -= actorShape.width / 2;
	Point2f actorRightCenter{ actorCenter };
	actorRightCenter.x += actorShape.width / 2;
	Point2f actorBotR{ actorCenter };
	actorBotR.x += actorShape.width / 2;
	actorBotR.y -= actorShape.height / 2;
	actorBotR.y += errorMargin;
	Point2f actorTopR{ actorCenter };
	actorTopR.y += actorShape.height / 2;
	actorTopR.x += actorShape.width / 2;
	Point2f actorBotL{ actorShape.left, actorShape.bottom };
	actorBotL.y += errorMargin;
	Point2f actorTopL{ actorBotL };
	actorTopL.y += actorShape.height;
	utils::HitInfo hit{};


	//mesh collision
	for (unsigned int i = 0; i < m_SlipperyVertices.size(); ++i)
	{
		//right
		if (utils::Raycast(m_SlipperyVertices[i], actorCenter, actorRight, hit)
			|| utils::Raycast(m_SlipperyVertices[i], actorTopCenter, actorTopR, hit)
			|| utils::Raycast(m_SlipperyVertices[i], actorBotCenter, actorBotR, hit))
		{
			if (hit.intersectPoint.y > actorBot.y +1)
			{
				actorShape.left = hit.intersectPoint.x - actorShape.width - errorMargin;
				actorVelocity.x *= m_BounceFactor;
			}
		}

		//left
		if (utils::Raycast(m_SlipperyVertices[i], actorCenter, actorLeft, hit)
			|| utils::Raycast(m_SlipperyVertices[i], actorTopCenter, actorTopL, hit)
			|| utils::Raycast(m_SlipperyVertices[i], actorBotCenter, actorBotL, hit))
		{
			if (hit.intersectPoint.y > actorBot.y + 1 )
			{
				actorShape.left = hit.intersectPoint.x + errorMargin;
				actorVelocity.x *= m_BounceFactor;
			}
		}

		//top
		if (utils::Raycast(m_SlipperyVertices[i], actorCenter, actorTop, hit)
			|| utils::Raycast(m_SlipperyVertices[i], actorRightCenter, actorTopR, hit)
			|| utils::Raycast(m_SlipperyVertices[i], actorLeftCenter, actorTopL, hit) && actorVelocity.y > 0)
		{
			if (hit.intersectPoint.y <= actorShape.bottom + actorShape.height && (hit.intersectPoint.x > actorShape.left && hit.intersectPoint.x < actorShape.left + actorShape.width))
			{
				actorShape.bottom = hit.intersectPoint.y - actorShape.height - errorMargin * 2;
			}
			actorVelocity.y = 0;
		}

		//bottom collision
		if (utils::Raycast(m_SlipperyVertices[i], actorCenter, actorBot, hit)
			|| utils::Raycast(m_SlipperyVertices[i], actorLeftCenter, actorBotL, hit)
			|| utils::Raycast(m_SlipperyVertices[i], actorRightCenter, actorBotR, hit)
			&& actorVelocity.y < 0)
		{
			actorShape.bottom = hit.intersectPoint.y;
			actorAcceleration.x = 0.8f;

			float angleWithUp{ hit.normal.AngleWith(Vector2f(0,1))}; 
			if (angleWithUp >= 0.1f || angleWithUp <= -0.1f ) //if incline
			{
				actorVelocity += actorVelocity * 5 * elapsedSec;
				if (hit.normal.CrossProduct(actorVelocity) > 0 && actorVelocity.x > 0 || hit.normal.CrossProduct(actorVelocity) < 0 && actorVelocity.x < 0) //if falling in opposite direction of incline
				{
					actorVelocity.x = -actorVelocity.x;
				}
			}
		}
	}
}

bool Level::IsOnGround(const Rectf& actorShape)
{
	const int errorMargin{ 1 };
	const Point2f startPos{ actorShape.left + actorShape.width / 2, actorShape.bottom + actorShape.height};
	const Point2f endPos{ startPos.x, actorShape.bottom };

	const Point2f startPosL{ startPos.x - actorShape.width /2 + errorMargin, startPos.y };
	const Point2f endPosL{ endPos.x - actorShape.width / 2 + errorMargin, endPos.y };

	const Point2f startPosR{ startPos.x + actorShape.width / 2 - errorMargin, startPos.y };
	const Point2f endPosR{ endPos.x + actorShape.width / 2 - errorMargin, endPos.y };

	utils::HitInfo hitInfo;


	for (unsigned int i = 0; i < m_Vertices.size(); ++i)
	{
		if (utils::Raycast(m_Vertices[i], startPos, endPos, hitInfo)
			|| utils::Raycast(m_Vertices[i], startPosL, endPosL, hitInfo)
			|| utils::Raycast(m_Vertices[i], startPosR, endPosR, hitInfo))
		{
			return true;
		}
	}

	for (unsigned int i = 0; i < m_SlipperyVertices.size(); ++i)
	{
		if (utils::Raycast(m_SlipperyVertices[i], startPos, endPos, hitInfo)
			|| utils::Raycast(m_SlipperyVertices[i], startPosL, endPosL, hitInfo)
			|| utils::Raycast(m_SlipperyVertices[i], startPosR, endPosR, hitInfo))
		{
			return true;
		}
	}
	return false;
}

Rectf Level::GetBoundaries() const
{
	return m_Boundaries;
}

void Level::ApplyModifiers(Player& player)
{
	if (!IsOnGround(player.GetPlayerShape()))
	{
		for (unsigned int i = 0; i < m_ModifierZones.size(); ++i)
		{
			m_ModifierZones[i]->ApplyAccelerationModifier(player.GetPlayerPos(), player.GetAcceleration());
		}
	}

	bool isPlayerInModifierZone{ false };
	bool hasDefaultAcceleration{ (player.GetAcceleration().y == -981.0f && player.GetAcceleration().x == 10.0f) };

	if (!hasDefaultAcceleration)
	{
		for (unsigned int i = 0; i < m_ModifierZones.size(); ++i)
		{
			if (utils::IsPointInRect(player.GetPlayerPos(), m_ModifierZones[i]->GetZone()))
			{
				isPlayerInModifierZone = true;
			}
		}

		if (!isPlayerInModifierZone)
		{
			player.SetAcceleration(Vector2f(10.0f, -981.0f));
		}
	}
}

void Level::ProcessKeyDownEvent()
{
	if (IsLevelEnding())
	{
		m_pGameEnd->Continue();
	}
}

bool Level::IsLevelEnding() const
{
	return m_pGameEnd->IsEnding();
}
	