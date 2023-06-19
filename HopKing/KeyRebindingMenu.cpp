#include "pch.h"
#include "KeyRebindingMenu.h"
#include "InputManager.h"
#include "Game.h"
#include "Texture.h"
#include "TextureManager.h"
#include "MenuManager.h"
#include "SaveManager.h"

KeyRebindingMenu::KeyRebindingMenu(const Window& window, MenuManager* menuManager)
	:Menu{ window, menuManager, 2 }
	,m_KeybindIdx{0}
	,m_pUpText{new Texture("PRESS UP", m_pMenuStyle->GetFont(), m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor())}
	,m_pDownText{new Texture("PRESS DOWN", m_pMenuStyle->GetFont(), m_pMenuStyle->GetFontSize(),  m_pMenuStyle->GetTextColor())}
	,m_pLeftText{new Texture("PRESS LEFT", m_pMenuStyle->GetFont(), m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor())}
	,m_pRightText{new Texture("PRESS RIGHT", m_pMenuStyle->GetFont(), m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor())}
	,m_pJumpText{new Texture("PRESS JUMP", m_pMenuStyle->GetFont(), m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor())}
	,m_pConfirmText{new Texture("PRESS CONFIRM", m_pMenuStyle->GetFont(), m_pMenuStyle->GetFontSize(),  m_pMenuStyle->GetTextColor())}
	,m_pCancelText{new Texture("PRESS CANCEL", m_pMenuStyle->GetFont(), m_pMenuStyle->GetFontSize(),  m_pMenuStyle->GetTextColor())}
	,m_pPauseText{new Texture("PRESS PAUSE", m_pMenuStyle->GetFont(), m_pMenuStyle->GetFontSize(),  m_pMenuStyle->GetTextColor())}
	,m_ActionTexts{}
{
	CreateActionTextDictionary();
	SetDrawAdditionalOnTop(true);
}

KeyRebindingMenu::~KeyRebindingMenu()
{
	delete m_pUpText;
	m_pUpText = nullptr;

	delete m_pDownText;
	m_pDownText = nullptr;

	delete m_pLeftText;
	m_pLeftText = nullptr;

	delete m_pRightText;
	m_pRightText = nullptr;

	delete m_pJumpText;
	m_pJumpText = nullptr;

	delete m_pConfirmText;
	m_pConfirmText = nullptr;

	delete m_pCancelText;
	m_pCancelText = nullptr;

	delete m_pPauseText;
	m_pPauseText = nullptr;

}

void KeyRebindingMenu::Select(Game& game)
{
	if (m_KeybindIdx < int(KeybindingAction::LENGTH) && IsActive())
	{
		BindButtons(game.GetInputManager());
	}
	else
	{
		game.GetInputManager()->EndRebinding();
		m_pMenuManager->Close();
		m_KeybindIdx = 0;
	}
}

void KeyRebindingMenu::DrawAdditionalElements() const
{
	m_ActionTexts.at(m_KeybindIdx)->Draw(Point2f(m_MenuRect.left + m_pMenuStyle->GetPadding(), m_MenuRect.bottom + m_pMenuStyle->GetPadding()));
}


void KeyRebindingMenu::CreateActionTextDictionary()
{
	m_ActionTexts.insert(std::pair<int, Texture*>(int(KeybindingAction::UP), m_pUpText));
	m_ActionTexts.insert(std::pair<int, Texture*>(int(KeybindingAction::DOWN), m_pDownText));
	m_ActionTexts.insert(std::pair<int, Texture*>(int(KeybindingAction::LEFT), m_pLeftText));
	m_ActionTexts.insert(std::pair<int, Texture*>(int(KeybindingAction::RIGHT), m_pRightText));
	m_ActionTexts.insert(std::pair<int, Texture*>(int(KeybindingAction::JUMP), m_pJumpText));
	m_ActionTexts.insert(std::pair<int, Texture*>(int(KeybindingAction::CONFIRM), m_pConfirmText));
	m_ActionTexts.insert(std::pair<int, Texture*>(int(KeybindingAction::CANCEL), m_pCancelText));
	m_ActionTexts.insert(std::pair<int, Texture*>(int(KeybindingAction::PAUSE), m_pPauseText));
}

void KeyRebindingMenu::BindButtons(InputManager* inputManager)
{
	inputManager->BindButton(KeybindingAction(m_KeybindIdx));
	++m_KeybindIdx;
} 