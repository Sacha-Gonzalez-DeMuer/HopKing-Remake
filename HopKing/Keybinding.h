#pragma once
#include "enums.h"
#include <SDL_keycode.h>

class Keybinding final
{
public:
	Keybinding(KeybindingAction action, SDL_Keycode key, SDL_Scancode scanCode);

	void SetKey(SDL_Keycode key);
	void SetScancode(SDL_Scancode scancode);

	KeybindingAction GetAction() const;
	SDL_Keycode GetKey() const;
	SDL_Scancode GetScancode() const;


private:
	KeybindingAction m_Action;
	SDL_Keycode m_Key;
	SDL_Scancode m_Scan;
};

