#include "pch.h"
#include "Player.h"
#include "enums.h"
#include "Level.h"
#include "utils.h"
#include "Texture.h"
#include "SaveManager.h"
#include "SpriteAnimator.h"
#include "SoundManager.h"
#include "Game.h"
#include "InputManager.h"
#include "TextureManager.h"
#include "ParticleSystem.h"

Player::Player()
	: m_HorSpeed{ 300.0f }
	, m_MinimumJumpSpeed{ 200.0f }
	, m_JumpSpeed{ }
	, m_Velocity{ 0.0f,0.0f }
	, m_Acceleration{ 10.0f, -981.0f }
	, m_ActionState{ ActionState::idle }
	, m_Shape{ 1000,400,135,135 }
	, m_MaxJumpSpeed{ 1500.0f }
	, m_ChargeSpeedMultiplier{ 1500.0f }
	, m_JumpPressed{ }
	, m_DownwardForceMultiplier{ 5.0f } //was 3
	, m_UpwardForceMultiplier{ 2.0f } //was 2
	, m_JumpDir{ }
	, m_pSpriteAnimator { }
	, m_HorScale{ 1 }
	, m_FallTime{ }
	, m_LongFallThreshold{ .5f }
	, m_JumpedDir{ }
	, m_TerminalVelocity{-2000}
	, m_TimesJumped{}
	, m_TimesFallen{}
	, m_Landed{}
	, m_pInputManager{InputManager::Get()}
	, m_SessionStartTime{ std::chrono::system_clock::now() }
	, m_LandSound{"land.mp3"}
	, m_FlatFallSound{"flat.mp3"}
	, m_JumpSound{"jumpSound.mp3"}
	, m_BounceSound{"bounce.mp3"}
	, m_pLandingParticles{ new ParticleSystem(20, 1.0f, 10.0f, 1.0f,  Vector2f(400, 20), Vector2f(-400, -20), .2f, 1.0f) }
	, m_pJumpingParticles{ new ParticleSystem(20, m_Shape.width/2, 10.0f, 1.0f, .3f, .7f) }

{
	Initialize();

}

Player::~Player()
{
	delete m_pSpriteAnimator;
	m_pSpriteAnimator = nullptr;

	m_pInputManager = nullptr;

	delete m_pLandingParticles;
	m_pLandingParticles = nullptr;

	delete m_pJumpingParticles;
	m_pJumpingParticles = nullptr;
}

void Player::Initialize()
{
	TextureManager::Get()->AddTexture("BanjoSpritesheet", new Texture("Resources/Banjo/Spritesheet2.png"));
	m_pSpriteAnimator = new SpriteAnimator(TextureManager::Get()->GetTexture("BanjoSpritesheet"), 4, 8, 256, 256);

	SoundManager::Get()->AddSFX(m_LandSound);
	SoundManager::Get()->AddSFX(m_JumpSound);
	SoundManager::Get()->AddSFX(m_FlatFallSound);
	SoundManager::Get()->AddSFX(m_BounceSound);

	const float scale{ .5f };
	m_pSpriteAnimator->SetScale(scale);
	m_Shape.width *= scale;
	m_Shape.height *= scale;

}

void Player::Update(float elapsedSec, Level* level)
{
	m_pSpriteAnimator->Update(elapsedSec, Point2f(m_Shape.left, m_Shape.bottom));
	m_pSpriteAnimator->SetScale(std::abs(m_pSpriteAnimator->GetScale()) * m_HorScale);

	m_pJumpingParticles->UpdateParticles(elapsedSec);
	m_pLandingParticles->UpdateParticles(elapsedSec);

	UpdateActionState(level);

	HandleCollision(level, elapsedSec);
	ProcessInput(level, elapsedSec);
	UpdateMovement(elapsedSec, level);
}

void Player::Draw() const
{
	m_pLandingParticles->Draw();
	m_pSpriteAnimator->DrawPlayer(m_ActionState, m_Shape);
	m_pJumpingParticles->Draw();
}

Rectf Player::GetPlayerShape() const
{
	return m_Shape;
}

Point2f Player::GetPlayerPos() const
{
	return Point2f(m_Shape.left + m_Shape.width/2, m_Shape.bottom + m_Shape.height / 2);
}

Vector2f Player::GetPlayerVelocity() const
{
	return m_Velocity;
}

void Player::ProcessInput(Level* pLevel, float elapsedSec)
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (IsGrounded(pLevel) && m_ActionState != ActionState::jumping)
	{
		if (m_pInputManager->GetKey(KeybindingAction::JUMP))
		{
			m_JumpPressed = true;
			m_Landed = false;
			if (m_ActionState != ActionState::jumping)
			{
				m_Velocity.x = 0;
			}

			if (m_pInputManager->GetKey(KeybindingAction::RIGHT))
			{
				m_JumpDir = 1;
			}
			else if (m_pInputManager->GetKey(KeybindingAction::LEFT))
			{
				m_JumpDir = -1;
			}
		}

		else if (m_pInputManager->GetKey(KeybindingAction::LEFT) 
			|| m_pInputManager->GetKey(KeybindingAction::RIGHT) 
			&& !m_pInputManager->GetKey(KeybindingAction::JUMP))
		{
			SwitchActionState(ActionState::moving);
			m_JumpPressed = false;

			if (m_pInputManager->GetKey(KeybindingAction::RIGHT))
			{
				m_Velocity.x += m_pInputManager->GetKey(KeybindingAction::RIGHT) * m_HorSpeed * m_Acceleration.x * elapsedSec;
				m_HorScale = 1;
			}
			else if (m_pInputManager->GetKey(KeybindingAction::LEFT))
			{
				m_Velocity.x += m_pInputManager->GetKey(KeybindingAction::LEFT) * -m_HorSpeed * m_Acceleration.x * elapsedSec;
				m_HorScale = -1;
			}

			if (m_Velocity.x > 0 && m_Velocity.x > m_HorSpeed) m_Velocity.x = m_HorSpeed;
			if (m_Velocity.x < 0 && m_Velocity.x < -m_HorSpeed) m_Velocity.x = -m_HorSpeed;
		}

		else
		{
			/*if (!m_JumpPressed)
			{
				m_Velocity.x += -m_Velocity.x * m_Acceleration.x * elapsedSec;
				if (m_Velocity.x > -1 && m_Velocity.x < 1) m_Velocity.x = 0;
			}*/
			if(m_JumpPressed)
			{
				SwitchActionState(ActionState::jumping);
				m_JumpPressed = false;
			}
			else
			{
				m_Velocity.x += -m_Velocity.x * m_Acceleration.x * elapsedSec;
				if (m_Acceleration.x >= 100)
				{
					m_Velocity.x = 0;
				}
				if (m_Velocity.x > -1 && m_Velocity.x < 1) m_Velocity.x = 0;
			}
		}
	}
}

void Player::UpdateMovement(float elapsedSec, Level* pLevel)
{
	//m_Velocity.x = m_Velocity.x * m_Acceleration.x * elapsedSec;
	if (m_ActionState == ActionState::jumping || m_ActionState == ActionState::falling && !IsGrounded(pLevel))
	{
		pLevel->ApplyModifiers(*this);
	}

	if (m_Velocity.y <= m_TerminalVelocity)
	{
		m_Velocity.y = m_TerminalVelocity;
	}

	if (!pLevel->IsOnGround(m_Shape))
	{
		UpdateJump(elapsedSec);
	}
	else
	{
		if (m_Velocity.y < 0)
		{
			m_Velocity.y = 0;
			m_JumpedDir = 0; 
			if (m_FallTime >= m_LongFallThreshold)
			{
				if (m_ActionState != ActionState::flat)
				{
					m_pLandingParticles->InstantiateParticles(Point2f(m_Shape.left + m_Shape.width / 2, m_Shape.bottom));
				}
				SwitchActionState(ActionState::flat);

			}
			else
			{
				if (!m_Landed) //OnLand
				{
					Save();
					m_Landed = true;
					SoundManager::Get()->PlaySFX(m_LandSound, 0);
				}
				
			}
			m_FallTime = 0;

		}
	}

	m_Shape.left += m_Velocity.x * elapsedSec;
	m_Shape.bottom += m_Velocity.y * elapsedSec;

	if (m_ActionState == ActionState::charging)
	{
		if (m_JumpSpeed < m_MaxJumpSpeed)
		{ 
			m_JumpSpeed += elapsedSec * m_ChargeSpeedMultiplier;
		}
		else
		{
			m_JumpPressed = false; //force jump release
		}
	}

	if (m_JumpSpeed > 0 && !m_JumpPressed) //OnJumpRelease
	{
		Jump();
	}

}

void Player::HandleCollision(Level* pLevel, float elapsedSec)
{
	pLevel->HandleCollision(m_Shape, m_Velocity, m_Acceleration, elapsedSec);
}

bool Player::IsGrounded(Level* pLevel) const
{
	return pLevel->IsOnGround(m_Shape);
}

void Player::SwitchActionState(const ActionState& toState)
{
	m_ActionState = toState;

	if (toState == ActionState::flat)
	{
		++m_TimesFallen;
		SoundManager::Get()->PlaySFX(m_FlatFallSound, 0);
	}
}

void Player::Jump()
{
	SoundManager::Get()->PlaySFX(m_JumpSound, 0);
	++m_TimesJumped;
	SwitchActionState(ActionState::jumping);

	m_Velocity.y = m_MinimumJumpSpeed + m_JumpSpeed;
	m_Velocity.x = m_JumpDir * (m_MinimumJumpSpeed + m_JumpSpeed / 2) ;
	m_JumpedDir = m_JumpDir;
	if (m_JumpedDir != 0) m_HorScale = m_JumpedDir;

	m_pJumpingParticles->InstantiateParticles(Point2f(m_Shape.left + m_Shape.width/2, m_Shape.bottom), m_Velocity * 0.5f, m_Velocity * 0);

	Save();
	ResetJump();
}

void Player::ResetJump()
{
	m_JumpSpeed = 0;
	m_JumpDir = 0;
}

void Player::UpdateJump(float elapsedSec)
{
	m_Velocity += m_Acceleration * elapsedSec;

	if (m_Velocity.y < 0)
	{
		m_Velocity.y += m_Acceleration.y * m_DownwardForceMultiplier * elapsedSec;
		m_FallTime += elapsedSec;
	}
	else if (m_Velocity.y > 0)
	{
		m_Velocity.y += m_Acceleration.y * m_UpwardForceMultiplier * elapsedSec;
	}
}

void Player::UpdateActionState(Level* pLevel)
{
	if (IsGrounded(pLevel))
	{
		if (m_Velocity.y <= .1f && m_ActionState != ActionState::flat)
		{
			SwitchActionState(ActionState::idle);
		}

		if (m_JumpPressed)
		{
			SwitchActionState(ActionState::charging);
		}
	}
	else
	{
		if (m_Velocity.y < 0 && m_ActionState != ActionState::falling && m_ActionState != ActionState::bouncing)
		{
			SwitchActionState(ActionState::falling);
		}

		if (m_JumpedDir != 0 && m_JumpedDir > 0 && m_Velocity.x < 0 || m_JumpedDir < 0 && m_Velocity.x > 0)
		{
			if (m_ActionState != ActionState::bouncing)
			{
				SwitchActionState(ActionState::bouncing);
				SoundManager::Get()->PlaySFX(m_BounceSound, 0);
				m_HorScale = -m_HorScale;
			}
		}
	}
}

void Player::SetAcceleration(const Vector2f& vel)
{
	m_Acceleration = vel;
}

Vector2f Player::GetAcceleration() const
{
	return m_Acceleration;
}

Vector2f& Player::GetAcceleration()
{
	return m_Acceleration;
}

int Player::GetTimesJumped() const
{
	return m_TimesJumped;
}

int Player::GetTimesFallen() const
{
	return m_TimesFallen;
}

void Player::Save()
{
	SaveManager::Get()->SaveData(*this);
	m_SessionStartTime = std::chrono::system_clock::now();
}

void Player::Load(Point2f pos, Vector2f vel, int nrJumps, int nrFalls)
{
	m_Shape.left = pos.x;
	m_Shape.bottom = pos.y;

	m_Velocity = vel;

	m_TimesJumped = nrJumps;
	m_TimesFallen = nrFalls;
}

void Player::LoadDefaultData()
{
	m_Shape.left = 1000;
	m_Shape.bottom = 400;

	m_Velocity = Vector2f(0,0);

	m_TimesJumped = 0;
	m_TimesFallen = 0;
}

void Player::SetPositionCheat(const Point2f& pos)
{
	//player.role == teacher ? dev.AddToExamScore(1) : player.Ban(); 

	//m_Velocity *= 0;
	//m_Shape.left = pos.x + m_Shape.width;
	//m_Shape.bottom += pos.y; 
}


