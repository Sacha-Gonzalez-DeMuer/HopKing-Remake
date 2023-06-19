#include "pch.h"
#include "RepelSwarmManager.h"
#include "utils.h"
#include "Texture.h"
#include "RepellingObject.h"
#include "TextureManager.h"
#include <iostream>

RepelSwarmManager::RepelSwarmManager(const int nrObjects, const std::string& spritesLocation, const Rectf& swarmZone, const float repelDistance)
	: m_RepellingDistance{repelDistance}
	, m_NrObjects{nrObjects}
	, m_SpritesLocation{spritesLocation}
	, m_SwarmZone{swarmZone}
{
	Initialize();
}

RepelSwarmManager::~RepelSwarmManager()
{
	for (RepellingObject* obj : m_RepellingObjects) {
		delete obj;
		obj = nullptr;
	}
}

void RepelSwarmManager::Initialize()
{
	TextureManager::Get()->AddTexture("0.png", new Texture(m_SpritesLocation + "0.png"));


	//create objects
	for (int i = 0; i < m_NrObjects; ++i)
	{
		const Point2f objectSpawn{
			utils::GenerateRandomRange(m_SwarmZone.left, m_SwarmZone.left + m_SwarmZone.width),
			utils::GenerateRandomRange(m_SwarmZone.bottom, m_SwarmZone.bottom + m_SwarmZone.height) };

		m_RepellingObjects.push_back(new RepellingObject(objectSpawn, TextureManager::Get()->GetTexture("0.png"), m_RepellingDistance));
	}
}

void RepelSwarmManager::Update(float elapsedSec, const Point2f& repellantPos)
{
	UpdateRepelantPositions(repellantPos);
	for (RepellingObject* obj : m_RepellingObjects) {

		obj->UpdatePos(m_RepelObjPositions, elapsedSec);
	}
}

void RepelSwarmManager::Draw() const {
	for (RepellingObject* obj : m_RepellingObjects) {
		obj->Draw();
	}
}

void RepelSwarmManager::UpdateRepelantPositions()
{
	m_RepelObjPositions.clear();

	for (RepellingObject* obj : m_RepellingObjects) {
		m_RepelObjPositions.push_back(obj->GetPosition());
	}
}

void RepelSwarmManager::UpdateRepelantPositions(const Point2f& playerRepellant)
{
	m_RepelObjPositions.clear();

	for (RepellingObject* obj : m_RepellingObjects) {
		m_RepelObjPositions.push_back(obj->GetPosition());
	}

	m_RepelObjPositions.push_back(playerRepellant);
}
