#include "pch.h"
#include "InputManager.h"
#include "Keybindings.h"
#include "Keybinding.h"
#include "enums.h"
#include "SaveManager.h"

InputManager* InputManager::Get()
{
	static InputManager* i = new InputManager();

	return i;
}

InputManager::InputManager()
	: m_pKeybindings{ new Keybindings() }
	, m_IsRebinding{}

{
	InitializeKeybindings();
}

InputManager::~InputManager()
{
	delete m_pKeybindings;
	m_pKeybindings = nullptr;
}

void InputManager::InitializeKeybindings()
{
	SaveManager::Get()->LoadData(*m_pKeybindings);
}

void InputManager::InitializeDefaultKeybindings()
{
	m_pKeybindings->InitializeDefaultBinds();
	SaveManager::Get()->SaveData(m_pKeybindings);
}

void InputManager::Cleanup()
{
	delete m_pKeybindings;
	m_pKeybindings = nullptr;
}

SDL_Keycode InputManager::GetKeyForAction(KeybindingAction action) const
{
	SDL_Keycode key{};

	for (const Keybinding* keybinding : m_pKeybindings->GetKeybindings()) {
		if (keybinding->GetAction() == action)
		{
			key = keybinding->GetKey();
		}

	}
	return key;
}

SDL_Scancode InputManager::GetScancodeForAction(KeybindingAction action) const
{
	SDL_Scancode scancode{};

	for (const Keybinding* keybindings : m_pKeybindings->GetKeybindings()) {
		if (keybindings->GetAction() == action)
		{
			scancode = keybindings->GetScancode();
		}
	}

	return scancode;
}

bool InputManager::GetKey(KeybindingAction action) const
{
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);

	if (pStates[GetScancodeForAction(action)])
	{
		return true;
	}

	return false;
}

KeybindingAction InputManager::GetActionForKey(SDL_Keycode key) const
{
	KeybindingAction action{};

	for (const Keybinding* keybinding : m_pKeybindings->GetKeybindings()) {
		if (keybinding->GetKey() == key)
		{
			action = keybinding->GetAction();
		}
	}

	return action;
}


void InputManager::BindButton(KeybindingAction action)
{
	for (unsigned int i = 0; i < m_pKeybindings->GetKeybindings().size(); i++)
	{
		if (m_pKeybindings->GetKeybindings()[i]->GetAction() == action)
		{
			m_pKeybindings->GetKeybindings()[i]->SetKey(m_LastKeyDown);
			m_pKeybindings->GetKeybindings()[i]->SetScancode(m_LastKey);
		}
	}
}

void InputManager::SaveLastKeyDown(SDL_Keycode key, SDL_Scancode scanCode)
{
	m_LastKeyDown = key;
	m_LastKey = scanCode;
}

SDL_Keycode InputManager::GetLastKeyDown() const
{
	return m_LastKeyDown;
}

void InputManager::StartRebinding()
{
	m_IsRebinding = true;
}

void InputManager::EndRebinding()
{
	SaveManager::Get()->SaveData(m_pKeybindings);
	m_IsRebinding = false;
}

bool InputManager::IsRebinding()
{
	return m_IsRebinding;
}
