#include "pch.h"
#include "RepellingObject.h"
#include "Texture.h"
#include "utils.h"
#include <iostream>


RepellingObject::RepellingObject(const Point2f& pos, Texture* pTexture, const float repelDist)
	:m_Pos{pos}
	,m_pTexture{pTexture}
	,m_RepellingDistance{repelDist}
	,m_RepellantForce{1000.0f}
	,m_Velocity{}
	,m_MaxVelocity{500}
{
	m_UpdatedPos = m_Pos;
}

RepellingObject::RepellingObject(const Point2f& pos, Texture* pTexture, const float repelForce, const float repelDist)
	: m_Pos{ pos }
	, m_pTexture{ pTexture }
	, m_RepellantForce{repelForce}
	, m_RepellingDistance{repelDist}
	, m_Velocity{}
	, m_MaxVelocity{500}
{
	m_UpdatedPos = m_Pos;
}

RepellingObject::~RepellingObject()
{
}

void RepellingObject::Draw() const
{
	glPushMatrix();

	glTranslatef(-m_pTexture->GetWidth()/2, -m_pTexture->GetHeight()/2, 0);
	m_pTexture->Draw(m_UpdatedPos);

	glPopMatrix();

}

void RepellingObject::UpdatePos(std::vector<Point2f> repellants, float elapsedSec)
{
	Vector2f repelDir{};

	if (m_Velocity.Length() > m_MaxVelocity)
	{
		m_Velocity = m_Velocity.Normalized() * m_MaxVelocity;
	}

	m_UpdatedPos.x += m_Velocity.x * elapsedSec;
	m_UpdatedPos.y += m_Velocity.y * elapsedSec;

	for (unsigned int i = 0; i < repellants.size(); ++i)
	{
		const Vector2f dirToRepellant{ repellants[i] - m_UpdatedPos };

		if (dirToRepellant.Length() <= m_RepellingDistance)
		{
			m_Velocity += -dirToRepellant * elapsedSec * m_RepellantForce;
		}
		else
		{
			float slowFactor{ 1.2f };
			m_Velocity -= (m_Velocity * slowFactor) * elapsedSec;
		}

		const Vector2f dirToDefaultPos{ m_Pos - m_UpdatedPos };
		if (dirToDefaultPos.Length() > m_RepellingDistance * 10)
		{
			m_Velocity += dirToDefaultPos * elapsedSec;
		}
	}

	if (repelDir.Length() > 0)
	{
		m_UpdatedPos -= repelDir * elapsedSec;
	}
}

Point2f RepellingObject::GetPosition() const
{
	return m_UpdatedPos;
}
