#include "pch.h"
#include "SaveManager.h"
#include "Player.h"
#include "Keybindings.h"
#include "Keybinding.h"
#include <direct.h>
#include <filesystem>
#include <iostream>
#include <fstream>

SaveManager* SaveManager::m_Instance{ new SaveManager() };

SaveManager::SaveManager()
	: m_FilePath{"Resources/Saves/Save.txt"}
{
}

SaveManager::~SaveManager()
{
	
}

SaveManager* SaveManager::Get()
{
	return m_Instance;
}



void SaveManager::SaveData(const Player& player) const
{
	std::chrono::time_point<std::chrono::system_clock> startTime{ FetchStartTime()};
	int timePlayed{ FetchTimePlayed() };
	int timeDiffStartToNow{
		int(std::chrono::duration_cast<std::chrono::seconds>
		(std::chrono::system_clock::now() - player.m_SessionStartTime).count()) };



	std::string existingData{ GetDataExcludingClass("Player") };
	std::ofstream ofs{ m_FilePath, std::ios::out };
	ofs << existingData;
	ofs.close();

	std::ofstream outputFileStream{m_FilePath, std::ios::app};

	if (outputFileStream.is_open())
	{
		outputFileStream << "<Player Data\n";
		outputFileStream << "\tPlayerPos:" << player.m_Shape.left << ',' << player.m_Shape.bottom << '\n';
		outputFileStream << "\tVelocity:" << player.m_Velocity.x << ',' << player.m_Velocity.y << '\n';
		outputFileStream << "\tJumps:" << player.m_TimesJumped << '\n';
		outputFileStream << "\tFalls:" << player.m_TimesFallen << '\n';
		outputFileStream << "\tStartTime:" << startTime << '\n';
		outputFileStream << "\tTimeplayed:" << (timePlayed + timeDiffStartToNow) << '\n';
		outputFileStream << ">\n";

	}
}

void SaveManager::SaveDefaultPlayerData() const
{
	std::string existingData{ GetDataExcludingClass("Player") };
	std::ofstream ofs{ m_FilePath, std::ios::out };
	ofs << existingData;
	ofs.close();

	std::ofstream outputFileStream{ m_FilePath, std::ios::app };

	if (outputFileStream.is_open())
	{
		outputFileStream << "<Player Data\n";
		outputFileStream << "\tPlayerPos:" << 1000 << ',' << 400 << '\n';
		outputFileStream << "\tVelocity:" << 0 << ',' << 0 << '\n';
		outputFileStream << "\tJumps:" << 0 << '\n';
		outputFileStream << "\tFalls:" << 0 << '\n';
		outputFileStream << "\tStartTime:" << std::chrono::system_clock::now() << '\n';
		outputFileStream << "\tTimeplayed:" << 0 << '\n';
		outputFileStream << ">\n";
	}
}

void SaveManager::SaveData(Keybindings* pKeybindings) const
{
	std::string existingData{ GetDataExcludingClass("Keybind") };
	std::ofstream ofs{ m_FilePath, std::ios::out };
	ofs << existingData;
	ofs.close();

	std::ofstream outputFileStream{ m_FilePath, std::ios::app };


	if (outputFileStream.is_open())
	{
		outputFileStream << "<Keybind Data\n";
		for (size_t i = 0; i < pKeybindings->GetKeybindings().size(); i++)
		{
			outputFileStream << "\tAction:" << KeybindingActionToString[int(pKeybindings->GetKeybindings()[i]->GetAction())] << '\n';
			outputFileStream << "\tKey:" << int(pKeybindings->GetKeybindings()[i]->GetKey()) << '\n';
			outputFileStream << "\tScancode:" << int(pKeybindings->GetKeybindings()[i]->GetScancode()) << '\n';
			outputFileStream << ".\n";
		}

		outputFileStream << ">\n";

	}
}

void SaveManager::LoadData(Player& player)
{
	std::ifstream ifs{ m_FilePath };
	const std::string playerData{ GetClassData("Player") };


	if (!ifs.fail() && playerData != "0")
	{
		Point2f playerPos{ ToPoint2f(GetAttributeValue("PlayerPos", playerData)) };
		Vector2f playerVel{ ToVector2f(GetAttributeValue("Velocity", playerData)) };
		int timesJumped{ std::stoi(GetAttributeValue("Jumps", playerData)) };
		int timesFallen{ std::stoi(GetAttributeValue("Falls", playerData)) };
		std::chrono::time_point<std::chrono::system_clock> startTime{ ToTime(GetAttributeValue("StartTime", playerData)) };


		player.Load(playerPos, playerVel, timesJumped, timesFallen);
	}
	else
	{
		std::cerr << "Error loading player data: Initializing default data\n";
		SaveDefaultPlayerData();
		LoadData(player);
	}
}

void SaveManager::LoadData(Keybindings& keybindings)
{
	std::ifstream ifs{ m_FilePath };
	std::cout << "loading keybindData\n";
	const std::string keybindData{ GetClassData("Keybind") };

	if (!ifs.fail() || keybindData != "0")
	{
		std::vector<std::string> objectsDatas{ GetObjectsData(keybindData) };

		for (unsigned int i = 0; i < objectsDatas.size(); ++i)
		{
			Keybinding pKeybinding
 			{
				ToKeybindingAction(GetAttributeValue("Action", objectsDatas[i])),
				SDL_Keycode(std::stoi(GetAttributeValue("Key", objectsDatas[i]))),
				SDL_Scancode(std::stoi(GetAttributeValue("Scancode", objectsDatas[i])))
			};

			keybindings.Load(pKeybinding);
		}
	}
	else
	{
		std::cerr << "Failed loading data, initializing default bindings\n";
		keybindings.InitializeDefaultBinds();
		SaveData(&keybindings);
	}

	//KeybindingAction action{ ToKeybindingAction(GetAttributeValue("Action", keybindData)) };
	//SDL_Keycode{ SDL_Keycode( std::stoi(GetAttributeValue("Key", keybindData))) };
	//SDL_Scancode{ SDL_Scancode(std::stoi(GetAttributeValue("Scancode", keybindData))) };
}

void SaveManager::DeleteData()
{
	remove(m_FilePath.c_str());
}

int SaveManager::FetchTimesJumped() const
{
	std::ifstream ifs{ m_FilePath };
	const std::string playerData{ GetClassData("Player") };

	if (!ifs.fail() && playerData != "0")
	{
		return std::stoi(GetAttributeValue("Jumps", playerData));
	}
	else
	{
		std::cerr << "Error loading player data: Couldn't fetch times jumped\n";
	}
	return 0;
}

int SaveManager::FetchTimesFallen() const
{
	std::ifstream ifs{ m_FilePath };
	const std::string playerData{ GetClassData("Player") };

	if (!ifs.fail() && playerData != "0")
	{
		return std::stoi(GetAttributeValue("Falls", playerData));
	}
	else
	{
		std::cerr << "Error loading player data: Couldn't fetch times fallen\n";
	}
	return 0;
}

int SaveManager::FetchTimePlayed() const
{
	std::ifstream ifs{ m_FilePath };
	const std::string playerData{ GetClassData("Player") };

	if (!ifs.fail() && playerData != "0")
	{
		return std::stoi(GetAttributeValue("Timeplayed", playerData));
	}
	else
	{
		std::cerr << "Error loading player data: Couldn't fetch times jumped\n";
	}
	return 0;
}

std::chrono::time_point<std::chrono::system_clock> SaveManager::FetchStartTime() const
{
	std::ifstream ifs{ m_FilePath };
	const std::string playerData{ GetClassData("Player") };

	if (!ifs.fail() || playerData != "0")
	{
		return ToTime(GetAttributeValue("StartTime", playerData));
	}
	else
	{
		std::cerr << "Error loading player data: Couldn't fetch start time\n";
	}

	return std::chrono::system_clock::now();
}


//std::string SaveManager::GetPlayerData() const
//{
//	std::ifstream inputFileStream{ m_FilePath, std::ios::in };
//	std::string line{};
//	std::string playerData{};
//	bool dataFound{};
//
//	while (inputFileStream.good() && inputFileStream.peek() != EOF)
//	{
//		std::getline(inputFileStream, line);
//		
//		if (dataFound)
//		{
//			playerData += line;
//		}
//
//		if (line.find("<Player Data") != std::string::npos)
//		{
//			dataFound = true;
//		}
//	}
//
//	if (playerData.size() > 0)
//	{
//		return playerData;
//	}
//	else
//	{
//		std::cerr << "Error loading player data";
//		return 0;
//	}
//}

std::string SaveManager::GetClassData(const std::string& className) const
{
	std::ifstream inputFileStream{ m_FilePath, std::ios::in };
	std::string line{};
	std::string data{};
	bool dataFound{};
	bool dataRead{};

	while (inputFileStream.good() && inputFileStream.peek() != EOF && !dataRead)
	{
		std::getline(inputFileStream, line);

		if (dataFound)
		{
			data += line + '\n';
		}

		if (line.find("<" + className + " Data") != std::string::npos)
		{
			dataFound = true;
		}

		if (dataFound && line.find(">") != std::string::npos)
		{
			dataRead = true;
		}
	}

	if (data.size() > 0)
	{
		return data;
	}
	else
	{
		std::cerr << "Error loading " + className + " data\n";
		return "0";
	}
	
	return data;
}

std::string SaveManager::GetDataExcludingClass(const std::string& classToExclude) const
{
	std::ifstream ifs{ m_FilePath };
	std::string existingData{};
	std::string storage;
	bool ownDataFound{};

	while (ifs.good() && ifs.peek() != EOF)
	{
		std::getline(ifs, storage);
		if (storage.find("<" + classToExclude + " Data") != std::string::npos)
		{
			ownDataFound = true;
		}

		if (!ownDataFound)
		{
			existingData += storage + '\n';
		}

		if (ownDataFound && storage.find(">") != std::string::npos)
		{
			ownDataFound = false;
		}
	}

	return existingData;
}

std::string SaveManager::GetKeybindingData() const
{
	std::ifstream inputFileStream{ m_FilePath, std::ios::in };
	std::string line{};
	std::string keybindingData{};
	bool dataFound{};


	return std::string();
}

std::vector<std::string> SaveManager::GetObjectsData(const std::string& data)
{
	std::vector<std::string> objectsDatas{};
	std::string storageString{};
	std::stringstream ss{ data };
	std::string objData{};

	while (ss.good() && ss.peek() != EOF)
	{
		std::getline(ss, storageString);
		objData += storageString + '\n';

		if (storageString.find('.') != std::string::npos)
		{
			objectsDatas.push_back(objData);
			objData.clear();
		}
	}


	return objectsDatas;
}

std::string SaveManager::GetAttributeValue(const std::string& attrName, const std::string& element) const
{
	std::string attribute{};
	std::string storageString{};
	std::stringstream ss{ element };
	while (ss.good() && ss.peek() != EOF)
	{
		std::getline(ss, storageString, ':');

		if (storageString.find(attrName) != std::string::npos)
		{
			std::getline(ss, storageString, '\n');

			attribute = storageString;
		}	
	}

	return attribute;
}


Point2f SaveManager::ToPoint2f(const std::string& point2fStr) const
{
	std::stringstream ss{ point2fStr };
	std::string str{};
	int idx{ 0 };
	Point2f point;

	while (ss.good() && ss.peek() != EOF)
	{
		std::getline(ss, str, ',');
		switch (idx)
		{
		case 0:
			point.x = std::stof(str);
			break;
		case 1:
			point.y = std::stof(str);
			break;
		default:
			std::cerr << "Error converting point2fStr to Point2f\n";
			break;
		}
		++idx;
	}

	return point;
}

Vector2f SaveManager::ToVector2f(const std::string& vector2fStr) const
{
	std::stringstream ss{ vector2fStr };
	std::string str{};
	int idx{ 0 };
	Vector2f vec;

	while (ss.good() && ss.peek() != EOF)
	{
		std::getline(ss, str, ',');
		switch (idx)
		{
		case 0:
			vec.x = std::stof(str);
			break;
		case 1:
			vec.y = std::stof(str);
			break;
		default:
			std::cerr << "Error converting vector2fStr to Vector2f\n";
			break;
		}
		++idx;
	}

	return vec;
}

bool SaveManager::ToBool(const std::string& boolStr) const
{
	std::stringstream ss(boolStr);
	bool isTrue{};
	ss >> std::boolalpha >> isTrue;

	return isTrue;
}

KeybindingAction SaveManager::ToKeybindingAction(const std::string& keybindingActionStr) const
{
	KeybindingAction tmp{};

	for (int i = 0; i <= int(KeybindingAction::LENGTH); i++)
	{

		if (keybindingActionStr == KeybindingActionToString[i])
		{
			tmp = KeybindingAction(i);
		}
	}
	return tmp;
}

std::chrono::time_point<std::chrono::system_clock> SaveManager::ToTime(const std::string& timeStr) const
{
	std::tm tm = {};
	std::stringstream ss(timeStr);
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S"); //parses to tm struct (Structure holding a calendar date and time broken down into its components.)

	std::chrono::time_point<std::chrono::system_clock> tp = std::chrono::system_clock::from_time_t(std::mktime(&tm)); //casts tm to time_t, and then casts to system clock

	return tp + std::chrono::hours(1); //hour gets lost in tp cast, idk why, bad fix but does the job
}

