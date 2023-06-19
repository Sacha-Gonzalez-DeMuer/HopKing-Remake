#pragma once
#include "Vector2f.h"
#include "enums.h"
#include <chrono>
#include "SaveManager.h"

class Texture;
class Level;
class SpriteAnimator;
class InputManager;
class Game;
class InputManager;
class ParticleSystem;

class Player final
{
public:
	friend void SaveManager::SaveData(const Player& player) const;

	Player();
	~Player();
	Player(Player&& other) = delete;
	Player& operator=(const Player& other) = delete;
	Player& operator=(Player&& other) = delete;


	void Initialize();
	void Update(float elapsedSec, Level* level);
	void Draw() const;

	Rectf GetPlayerShape() const;
	Point2f GetPlayerPos() const;
	Vector2f GetPlayerVelocity() const;
	void SetAcceleration(const Vector2f& vel);
	Vector2f GetAcceleration() const;
	Vector2f& GetAcceleration();

	int GetTimesJumped() const;
	int GetTimesFallen() const;

	void Save();
	void Load(Point2f pos, Vector2f vel, int nrJumps, int nrFalls);
	void LoadDefaultData();


	//hack (debug purposes)
	void SetPositionCheat(const Point2f& pos);
	

private:
	InputManager* m_pInputManager;

	ActionState m_ActionState;

	Rectf m_Shape;
	Vector2f m_Velocity;
	Vector2f m_Acceleration;
	float m_TerminalVelocity;
	int m_HorScale;
	int m_TimesJumped;
	int m_TimesFallen;
	std::chrono::time_point<std::chrono::system_clock> m_SessionStartTime;

	//animation
	SpriteAnimator* m_pSpriteAnimator;
	ParticleSystem* m_pLandingParticles;
	ParticleSystem* m_pJumpingParticles;

	//gameplay
	const float m_ChargeSpeedMultiplier;
	const float m_MinimumJumpSpeed;
	float m_HorSpeed;
	float m_JumpSpeed;
	float m_MaxJumpSpeed;
	const float m_DownwardForceMultiplier;
	const float m_UpwardForceMultiplier;
	bool m_JumpPressed;
	int m_JumpDir;
	int m_JumpedDir;
	float m_FallTime;
	const float m_LongFallThreshold;
	bool m_Landed;

	//sound
	const std::string m_LandSound;
	const std::string m_JumpSound;
	const std::string m_FlatFallSound;
	const std::string m_BounceSound;

	bool IsGrounded(Level* pLevel) const;
	void ProcessInput(Level* pLevel, float elapsedSec);
	void UpdateMovement(float elapsedSec, Level* pLevel);
	void HandleCollision(Level* pLevel, float elapsedSec);
	void SwitchActionState(const ActionState& toState);
	void Jump();
	void ResetJump();
	void UpdateJump(float elapsedSec);
	void UpdateActionState(Level* pLevel);
};

