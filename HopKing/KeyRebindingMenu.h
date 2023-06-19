#pragma once
#include "Menu.h"
#include <unordered_map>

class InputManager;
class Texture;

class KeyRebindingMenu final : public Menu
{
public:
	KeyRebindingMenu(const Window& window, MenuManager* menuManager);
	~KeyRebindingMenu();

	KeyRebindingMenu(const KeyRebindingMenu& other) = delete;
	KeyRebindingMenu(KeyRebindingMenu&& other) = delete;
	KeyRebindingMenu& operator=(const KeyRebindingMenu& other) = delete;
	KeyRebindingMenu& operator=(KeyRebindingMenu&& other) = delete;

	virtual void Select(Game& game) override;
	virtual void DrawAdditionalElements() const override;

	void CreateActionTextDictionary();
	void BindButtons(InputManager* inputManager);

private:
	Texture* m_pUpText;
	Texture* m_pDownText;
	Texture* m_pLeftText;
	Texture* m_pRightText;
	Texture* m_pJumpText;
	Texture* m_pConfirmText;
	Texture* m_pCancelText;
	Texture* m_pPauseText;

	std::unordered_map<int, Texture*> m_ActionTexts;

	int m_KeybindIdx;
	
};

