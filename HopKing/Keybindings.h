#pragma once
#include "enums.h"
#include <vector>
class Keybinding;


class Keybindings final
{
public:
	Keybindings();
	Keybindings(std::vector<Keybinding*> keybindings);
	~Keybindings();

	Keybindings(const Keybindings& other) = delete;
	Keybindings& operator=(const Keybindings& other) = delete;
	Keybindings(Keybindings&& other) = delete;
	Keybindings& operator=(const Keybindings&& other) = delete;

	void AddKeybinding(Keybinding* keybinding);
	void InitializeDefaultBinds();

	std::vector<Keybinding*> GetKeybindings();
	void Load(Keybinding keybinding);

private:
	std::vector<Keybinding*> m_Keybindings;
};

