#include "pch.h"
#include "Keybinding.h"

Keybinding::Keybinding(KeybindingAction action, SDL_Keycode key, SDL_Scancode scanCode)
	:m_Action{action}
	,m_Key{key}
	,m_Scan{ scanCode }
{
}

void Keybinding::SetKey(SDL_Keycode key)
{
	m_Key = key;
}

void Keybinding::SetScancode(SDL_Scancode scancode)
{
	m_Scan = scancode;
}

KeybindingAction Keybinding::GetAction() const
{
	return m_Action;
}

SDL_Keycode Keybinding::GetKey() const
{
	return m_Key;
}

SDL_Scancode Keybinding::GetScancode() const
{
	return m_Scan;
}
