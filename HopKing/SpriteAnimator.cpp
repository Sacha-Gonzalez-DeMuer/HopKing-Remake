#include "pch.h"
#include "SpriteAnimator.h"
#include "Texture.h"


SpriteAnimator::SpriteAnimator(Texture* pSpritesheet, int nrOfFrames, int framesPerSec, float clipHeight, float clipWidth)
	: m_ClipRect{ 0,0, clipWidth, clipHeight }
	, m_NrFramesPerSec{framesPerSec}
	, m_NrOfFrames{nrOfFrames}
	, m_pSpriteSheet{pSpritesheet}
	, m_NrHorizontalFrames{int(m_pSpriteSheet->GetWidth() / clipWidth)}
	, m_NrVerticalFrames{int(m_pSpriteSheet->GetHeight() / clipHeight)}
	, m_HorIdx{}
	, m_VerIdx{}
	, m_Scale{1}
	, m_AnimFrame{}
	, m_AnimTime{}
	, m_Pos{}
{
}

SpriteAnimator::SpriteAnimator(Texture* pSpritesheet, int nrOfFrames, int framesPerSec, float clipHeight, float clipWidth, const Point2f& pos)
	: m_ClipRect{ 0,0, clipWidth, clipHeight }
	, m_NrFramesPerSec{ framesPerSec }
	, m_NrOfFrames{ nrOfFrames }
	, m_pSpriteSheet{ pSpritesheet }
	, m_NrHorizontalFrames{ int(m_pSpriteSheet->GetWidth() / clipWidth) }
	, m_NrVerticalFrames{ int(m_pSpriteSheet->GetHeight() / clipHeight) }
	, m_HorIdx{}
	, m_VerIdx{}
	, m_Scale{ 1 }
	, m_AnimFrame{}
	, m_AnimTime{}
	, m_Pos{pos}
{
}

SpriteAnimator::~SpriteAnimator()
{
	/*delete m_pSpriteSheet;
	m_pSpriteSheet = nullptr;*/
}

void SpriteAnimator::Update(float elapsedSec, const Point2f& pos)
{
	m_Pos = pos;
	m_AnimTime += elapsedSec;

	UpdateAnimationFrame();
}

void SpriteAnimator::Draw() const
{
	glPushMatrix();

	glTranslatef(m_Pos.x - m_ClipRect.width / 2, m_Pos.y - m_ClipRect.height / 2, 0);
	glScalef(m_Scale, m_Scale, 1);
	m_pSpriteSheet->Draw(Point2f(), m_ClipRect);

	glPopMatrix();
}

void SpriteAnimator::DrawPlayer(const ActionState& state, const Rectf& shape)
{
	m_ClipRect.bottom = m_ClipRect.height * (int(state) + 1);

	glPushMatrix();

	glTranslatef(shape.left - shape.width / 2, shape.bottom, 0);
	if (m_Scale < 0)
	{
		glTranslatef(-m_ClipRect.width * m_Scale, 0, 0);
	}
	glScalef(m_Scale, std::abs(m_Scale), 1);
	m_pSpriteSheet->Draw(Point2f(), m_ClipRect);

	glPopMatrix();
}

float SpriteAnimator::GetScale() const
{
	return m_Scale;
}

void SpriteAnimator::SetScale(float scale)
{
	m_Scale = scale;
}

void SpriteAnimator::SetPosition(const Point2f& pos)
{
	m_Pos = pos;
}

Point2f SpriteAnimator::GetPosition() const
{
	return m_Pos;
}

Rectf SpriteAnimator::GetClipRect() const
{
	return m_ClipRect;
}

void SpriteAnimator::UpdateAnimationFrame()
{
	float updateTime{ 1.0f / m_NrFramesPerSec };

	if (m_AnimTime >= updateTime)
	{
		++m_HorIdx;
		m_AnimFrame = m_AnimFrame % m_NrOfFrames;
		m_AnimTime = 0;
	}

	if (m_HorIdx > m_NrHorizontalFrames)
	{
		m_HorIdx = 0;
		++m_VerIdx;
	}

	m_ClipRect.left = m_ClipRect.width * m_HorIdx;
	m_ClipRect.bottom = m_ClipRect.height * m_VerIdx;
}
