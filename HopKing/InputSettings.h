#pragma once
#include "Menu.h"

class Button;
class KeyRebindingMenu;
class Texture;

class InputSettings final : public Menu
{
public:
	InputSettings(const Window& window, MenuManager* menuManager);
	~InputSettings();

	InputSettings(const InputSettings& other) = delete;
	InputSettings(InputSettings&& other) = delete;
	InputSettings& operator=(const InputSettings& other) = delete;
	InputSettings& operator=(InputSettings&& other) = delete;


	virtual void Select(Game& game) override;
	virtual void DrawAdditionalElements() const override;
	virtual void Toggle() override;

	void UpdateCurrentKeybindingWindow();

private:
	const Rectf m_CurrentBindsRect;
	std::vector<Texture*> m_pBindsTextures;

	Button* m_pBindBtn;
	Button* m_pDefaultBtn;
};