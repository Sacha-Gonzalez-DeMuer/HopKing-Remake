#pragma once
#include <map>
#include <stack>
#include "enums.h"
class Menu;
class Game;

class MenuManager final
{
public:
	MenuManager(const Window& window);
	~MenuManager();

	MenuManager(const MenuManager& other) = delete;
	MenuManager(MenuManager&& other) = delete;
	MenuManager& operator=(const MenuManager& other) = delete;
	MenuManager& operator=(MenuManager&& other) = delete;

	void Initialize();

	void Update(float elapsedSec);
	void Draw() const;

	void OpenMenu(const MenuType& menu);
	void Select(Game& game);
	void SwitchSelection(bool down);
	void Close();
	void Close(bool force);

private:
	const Window m_GameWindow;
	std::map<MenuType, Menu*> m_pMenus;
	std::stack<Menu*> m_pActiveMenus;

	//sound
	std::string m_SelectSF;
};

