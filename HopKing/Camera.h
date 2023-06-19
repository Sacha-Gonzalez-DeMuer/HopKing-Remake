#pragma once
class Camera final
{
public:
	Camera(float width, float height);


	void SetLevelBoundaries(const Rectf& levelBoundaries);
	void Transform(const Rectf& target);

private:
	//DATA MEMBERS
	const float m_Width; 
	const float m_Height;
	Point2f m_CamPos;
	Rectf m_LevelBoundaries;

	//FUNCTIONS
	Point2f Track(const Rectf& target);
};

