#pragma once
#include "Menu.h"

class Button;

class OptionsMenu final : public Menu
{
public: 
	OptionsMenu(const Window& window, MenuManager* menuManager);
	~OptionsMenu();

	OptionsMenu(const OptionsMenu& other) = delete;
	OptionsMenu(OptionsMenu&& other) = delete;
	OptionsMenu& operator= (const OptionsMenu& other) = delete;
	OptionsMenu& operator= (OptionsMenu&& other) = delete;


	virtual void Select(Game& game) override;

private:
	Button* m_pControlsBtn;
};

