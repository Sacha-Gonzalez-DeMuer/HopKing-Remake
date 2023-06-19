#include "pch.h"
#include "MenuManager.h"
#include "Menu.h"
#include "HUD.h"
#include "MainMenu.h"
#include "OptionsMenu.h"
#include "InputSettings.h"
#include "KeyRebindingMenu.h"
#include <iostream>
#include "SoundManager.h"


MenuManager::MenuManager(const Window& window)
	: m_GameWindow{window}
	, m_pMenus{}
	, m_pActiveMenus{}
	, m_SelectSF{"select.mp3"}
{
	Initialize();
}

MenuManager::~MenuManager()
{
	std::cout << "Menu Manager Destructor\n";
	for (std::pair<MenuType, Menu*> itr : m_pMenus) {
		delete itr.second;
		itr.second = nullptr;
	}

	m_pMenus.clear();
}

void MenuManager::Initialize()
{
	SoundManager::Get()->AddSFX(m_SelectSF);


	m_pMenus.insert(std::pair<MenuType, Menu*>(MenuType::HUD, new HUD(m_GameWindow, this)));
	m_pMenus.insert(std::pair<MenuType, Menu*>(MenuType::MainMenu, new MainMenu(m_GameWindow, this)));
	m_pMenus.insert(std::pair<MenuType, Menu*>(MenuType::OptionsMenu, new OptionsMenu(m_GameWindow, this)));
	m_pMenus.insert(std::pair<MenuType, Menu*>(MenuType::InputSettings, new InputSettings(m_GameWindow, this)));
	m_pMenus.insert(std::pair<MenuType, Menu*>(MenuType::KeyRebindingMenu, new KeyRebindingMenu(m_GameWindow, this)));
}

void MenuManager::Update(float elapsedSec)
{
	for (int i = 0; i <= int(MenuType::LENGTH); ++i)
	{
		if (m_pMenus.at(MenuType(i))->IsActive())
		{
			m_pMenus.at(MenuType(i))->Update(elapsedSec);
		}
	}
}

void MenuManager::Draw() const
{
	for (int i = 0; i <= int(MenuType::LENGTH); ++i)
	{
		if (m_pMenus.at(MenuType(i))->IsActive())
		{
			m_pMenus.at(MenuType(i))->Draw();
		}
	}
}

void MenuManager::OpenMenu(const MenuType& menu)
{
	if (m_pMenus.contains(menu))
	{
		if (!m_pActiveMenus.empty())
		{
			m_pMenus.at(menu)->SetPosition(m_pActiveMenus.top()->GetPosition());
		}
		m_pMenus.at(menu)->Toggle();
		m_pActiveMenus.push(m_pMenus.at(menu));
	}
	else
	{
		std::cerr << "Menu type not found\n";
	}
}

void MenuManager::Select(Game& game)
{
	if (!m_pActiveMenus.empty())
	{
		m_pActiveMenus.top()->Select(game);
		SoundManager::Get()->PlaySFX(m_SelectSF, 0);
	}
	else
	{
		std::cerr << "MenuManager Error: No active menu to select\n";
	}
}

void MenuManager::SwitchSelection(bool down)
{
	if (!m_pActiveMenus.empty())
	{
		m_pActiveMenus.top()->SwitchSelection(down);
	}
	else
	{
		std::cerr << "MenuManager Error: No active menu to select\n";
	}
}

void MenuManager::Close()
{
	m_pActiveMenus.top()->Close();
	m_pActiveMenus.pop();
}

void MenuManager::Close(bool force)
{
	if (m_pActiveMenus.top()->IsCloseable() || !m_pActiveMenus.top()->IsCloseable() && force)
	{
		m_pActiveMenus.top()->Close();
		m_pActiveMenus.pop();
	}
}
