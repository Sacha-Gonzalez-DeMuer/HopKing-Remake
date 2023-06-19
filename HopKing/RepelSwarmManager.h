#pragma once
#include <vector>

class RepellingObject;

class RepelSwarmManager final
{
public:
	RepelSwarmManager(const int nrObjects, const std::string& spritesLocation, const Rectf& swarmZone, const float repelDistance);
	~RepelSwarmManager();
	RepelSwarmManager(RepelSwarmManager&& other) = delete;
	RepelSwarmManager& operator=(const RepelSwarmManager& other) = delete;
	RepelSwarmManager& operator=(RepelSwarmManager&& other) = delete;

	void Initialize();
	void Update(float elapsedSec, const Point2f& repellantPos);
	void Draw() const;

private:
	const Rectf m_SwarmZone;
	const int m_NrObjects;
	const float m_RepellingDistance;
	const std::string m_SpritesLocation;
	std::vector<RepellingObject*> m_RepellingObjects;
	std::vector<Point2f> m_RepelObjPositions;

	void UpdateRepelantPositions();
	void UpdateRepelantPositions(const Point2f& playerRepellant);

};

