#pragma once
#include <vector>
#include "Vector2f.h"
class Texture;

class RepellingObject final
{
public:
	RepellingObject(const Point2f& pos, Texture* pTexture, const float repelDist);
	RepellingObject(const Point2f& pos, Texture* pTexture, const float repelForce, const float repelDist);
	~RepellingObject();
	RepellingObject& operator=(const RepellingObject& other) = delete;
	RepellingObject& operator=(RepellingObject&& other) = delete;
	RepellingObject(RepellingObject&& other) = delete;

	void Draw() const;
	void UpdatePos(std::vector<Point2f> repellants, float elapsedSec);


	Point2f GetPosition() const;

private:
	const float m_MaxVelocity;
	Point2f m_Pos;
	Point2f m_UpdatedPos;
	Point2f m_RepellantPos;
	Texture* m_pTexture;
	Vector2f m_Velocity;
	

	const float m_RepellantForce;
	const float m_RepellingDistance;
};

