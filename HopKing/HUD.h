#pragma once
#include <vector>
#include "Menu.h"

class Button;
class Texture;
class Game;

class HUD final : public Menu
{
public:
	HUD(const Window& window, MenuManager* menuManager);
	~HUD();

	HUD(const HUD& other) = delete;
	HUD(HUD&& other) = delete;
	HUD& operator=(const HUD& other) = delete;
	HUD& operator=(HUD&& other) = delete;

	void UpdateData();
	virtual void Select(Game& game) override;
	virtual void DrawAdditionalElements() const override;
	virtual void Toggle() override;

private:
	const Rectf m_DataRect;
	int m_TotalSecondsPlayed;
	const Texture* m_pJumpTxt;
	const Texture* m_pFallTxt;
	const Texture* m_pTimePlayedTxt;
	const Texture* m_pTimeTxt;

	Button* m_pResumeBtn;
	Button* m_pOptionsBtn;
	Button* m_pSaveExitBtn;
	Button* m_pQuitBtn;
};

