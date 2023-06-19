#pragma once
#include "Vector2f.h"
#include "enums.h"

class Texture;

class SpriteAnimator final
{
public:
	SpriteAnimator(Texture* pSpritesheet, int nrOfFrames, int framesPerSec, float clipHeight, float clipWidth);
	SpriteAnimator(Texture* pSpritesheet, int nrOfFrames, int framesPerSec, float clipHeight, float clipWidth, const Point2f& pos);
	~SpriteAnimator();
	SpriteAnimator& operator=(const SpriteAnimator& other) = delete;
	SpriteAnimator(SpriteAnimator&& other) = delete;
	SpriteAnimator& operator=(SpriteAnimator&& other) = delete;
	
	void Update(float elapsedSec, const Point2f& pos);
	void Draw() const;
	void DrawPlayer(const ActionState& state, const Rectf& shape);

	float GetScale() const;
	void SetScale(float scale);
	void SetPosition(const Point2f& pos);
	Point2f GetPosition() const;
	Rectf GetClipRect() const;

private:
	Texture* m_pSpriteSheet;
	Point2f m_Pos;
	Rectf m_ClipRect;

	const int m_NrFramesPerSec;
	const int m_NrOfFrames;

	const int m_NrHorizontalFrames;
	const int m_NrVerticalFrames;

	int m_AnimFrame;
	float m_AnimTime;
	int m_HorIdx;
	int m_VerIdx;

	float m_Scale;

	void UpdateAnimationFrame();

};

