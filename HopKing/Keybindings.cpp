#include "pch.h"
#include "Keybindings.h"
#include "Keybinding.h"
#include <iostream>
#include "SaveManager.h"

Keybindings::Keybindings()
	: m_Keybindings{}
{
}

Keybindings::Keybindings(std::vector<Keybinding*> keybindings)
	:m_Keybindings{ keybindings }
{
}

Keybindings::~Keybindings()
{
	for (int i = 0; i < m_Keybindings.size(); i++)
	{
		delete m_Keybindings[i];
		m_Keybindings[i] = nullptr;
	}

	m_Keybindings.clear();
}

void Keybindings::AddKeybinding(Keybinding* keybinding)
{
	m_Keybindings.push_back(keybinding);
}


void Keybindings::InitializeDefaultBinds()
{
	if (m_Keybindings.size() > 0)
	{
		m_Keybindings.clear();
	}

	Keybinding* jumpKey{ new Keybinding(KeybindingAction::JUMP, SDLK_SPACE, SDL_SCANCODE_SPACE) };
	Keybinding* leftKey{ new Keybinding(KeybindingAction::LEFT, SDLK_a, SDL_SCANCODE_A) };
	Keybinding* rightKey{ new Keybinding(KeybindingAction::RIGHT, SDLK_d, SDL_SCANCODE_D) };
	Keybinding* upKey{ new Keybinding(KeybindingAction::UP, SDLK_w, SDL_SCANCODE_W) };
	Keybinding* downKey{ new Keybinding(KeybindingAction::DOWN, SDLK_s,SDL_SCANCODE_S) };
	Keybinding* confirmKey{ new Keybinding(KeybindingAction::CONFIRM, SDLK_RETURN, SDL_SCANCODE_RETURN) };
	Keybinding* cancelKey{ new Keybinding(KeybindingAction::CANCEL, SDLK_ESCAPE, SDL_SCANCODE_ESCAPE) };
	Keybinding* pauseKey{ new Keybinding(KeybindingAction::PAUSE, SDLK_p, SDL_SCANCODE_P) };

	AddKeybinding(jumpKey);
	AddKeybinding(leftKey);
	AddKeybinding(rightKey);
	AddKeybinding(upKey);
	AddKeybinding(downKey);
	AddKeybinding(confirmKey);
	AddKeybinding(cancelKey);
	AddKeybinding(pauseKey);
}


std::vector<Keybinding*> Keybindings::GetKeybindings()
{
	if (m_Keybindings.size() > 0)
	{
		return m_Keybindings;
	}
	else
	{
		throw std::runtime_error("Keybindings.cpp: no keybindings set\n");
		InitializeDefaultBinds();
		SaveManager::Get()->SaveData(this);
		return std::vector<Keybinding*>();
	}
}

void Keybindings::Load(Keybinding keybinding)
{
	Keybinding* keybind{ new Keybinding(keybinding) };
	m_Keybindings.push_back(keybind);
}
