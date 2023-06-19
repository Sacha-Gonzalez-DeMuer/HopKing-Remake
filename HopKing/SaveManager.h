#pragma once
#include <string>
#include "enums.h"
#include <vector>
#include "Vector2f.h"
#include <chrono>
#include <ctime>

class Player;
class Keybindings;

class SaveManager final
{
public:
	SaveManager();
	~SaveManager();

	SaveManager(const SaveManager& other) = delete;
	SaveManager(SaveManager&& other) = delete;
	SaveManager& operator=(const SaveManager& other) = delete;
	SaveManager& operator=(SaveManager&& other) = delete;

	static SaveManager* Get();

	void SaveData(const Player& player) const;
	void SaveDefaultPlayerData() const;

	void SaveData(Keybindings* pKeybindings) const;

	void LoadData(Player& player);
	void LoadData(Keybindings& keybindings);

	void DeleteData();
	
	int FetchTimesJumped() const;
	int FetchTimesFallen() const;
	int FetchTimePlayed() const;
	std::chrono::time_point<std::chrono::system_clock> FetchStartTime() const;

private:
	static SaveManager* m_Instance;

	std::string m_FilePath;

	std::string GetClassData(const std::string& className) const;
	std::string GetDataExcludingClass( const std::string& classToExclude) const ;

	std::string GetKeybindingData() const;

	std::vector<std::string> GetObjectsData(const std::string& data);
	std::string GetAttributeValue(const std::string& attrName, const std::string& element) const;
	Point2f ToPoint2f(const std::string& point2fStr) const;
	Vector2f ToVector2f(const std::string& vector2fStr) const;
	bool ToBool(const std::string& boolStr) const;
	KeybindingAction ToKeybindingAction(const std::string& keybindingActionStr) const;
	std::chrono::time_point<std::chrono::system_clock> ToTime(const std::string& timeStr) const;
};

