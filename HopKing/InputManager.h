#pragma once
#include "enums.h"
class Keybindings;

class InputManager final
{
public:
	static InputManager* Get();
	InputManager();
	~InputManager();

	InputManager(const InputManager& other) = delete;
	InputManager(InputManager&& other) = delete;
	InputManager& operator=(const InputManager& other) = delete;
	InputManager& operator=(InputManager&& other) = delete;

	void InitializeKeybindings();
	void InitializeDefaultKeybindings();
	void Cleanup();

	SDL_Keycode GetKeyForAction(KeybindingAction action) const;
	SDL_Scancode GetScancodeForAction(KeybindingAction action) const;

	bool GetKey(KeybindingAction action) const;

	KeybindingAction GetActionForKey(SDL_Keycode key) const;

	void BindButton(KeybindingAction action);
	
	void SaveLastKeyDown(SDL_Keycode key, SDL_Scancode scanCode);
	SDL_Keycode GetLastKeyDown() const;
	void StartRebinding();
	void EndRebinding();
	bool IsRebinding();

private:
	Keybindings* m_pKeybindings;
	bool m_IsRebinding;
	SDL_Keycode m_LastKeyDown;
	SDL_Scancode m_LastKey;
};