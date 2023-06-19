#include "pch.h"
#include "InputSettings.h"
#include "Button.h"
#include "MenuManager.h"
#include "Game.h"
#include "InputManager.h"
#include "Texture.h"
#include "utils.h"

InputSettings::InputSettings(const Window& window, MenuManager* menuManager)
	:Menu{window, menuManager, 2}
	, m_pBindBtn
		{ new Button{ Point2f(
			m_MenuRect.left + m_pMenuStyle->GetPadding(),
			m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() - (m_pMenuStyle->GetFontSize() / 1.33f)),
			"Bind buttons",
			m_pMenuStyle }
		}
	, m_pDefaultBtn
		{ new Button{ Point2f(
			m_MenuRect.left + m_pMenuStyle->GetPadding(),
			m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() * 2 - (m_pMenuStyle->GetFontSize() / 1.33f) * 2),
			"Default",
			m_pMenuStyle }
		}
	, m_CurrentBindsRect
		{ 
			window.width - window.width/3, 
			m_pMenuStyle->GetPadding(), 
			window.width/3, 
			m_pMenuStyle->GetFontSize() / 1.33f * int(KeybindingAction::LENGTH) + m_pMenuStyle->GetPadding() * (int(KeybindingAction::LENGTH) + 1)
		}
{
	AddButton(m_pBindBtn);
	AddButton(m_pDefaultBtn);
	m_pBindBtn->Select();
}

InputSettings::~InputSettings()
{
	m_pBindBtn = nullptr;
	m_pDefaultBtn = nullptr;

	for (int i = 0; i < m_pBindsTextures.size(); i++)
	{
		delete m_pBindsTextures[i];
		m_pBindsTextures[i] = nullptr;
	}

	m_pBindsTextures.clear();
}

void InputSettings::Select(Game& game)
{
	if (m_pBindBtn->IsSelected())
	{
		game.GetInputManager()->StartRebinding(); 
		m_pMenuManager->OpenMenu(MenuType::KeyRebindingMenu);
	}
	else if (m_pDefaultBtn->IsSelected()) {
		game.GetInputManager()->InitializeDefaultKeybindings();
		UpdateCurrentKeybindingWindow();
	}
}

void InputSettings::DrawAdditionalElements() const
{
	utils::SetColor(m_pMenuStyle->GetWindowBackgroundColor());
	utils::FillRect(m_CurrentBindsRect);
	utils::SetColor(m_pMenuStyle->GetWindowBorderColor());
	utils::DrawRect(m_CurrentBindsRect);

	Point2f currentPos{ m_CurrentBindsRect.left + m_pMenuStyle->GetPadding(), m_CurrentBindsRect.bottom + m_pMenuStyle->GetPadding() };
	for (int i = 0; i < m_pBindsTextures.size(); i++)
	{
		m_pBindsTextures[i]->Draw(currentPos);
		currentPos.y += m_pMenuStyle->GetFontSize() / 1.33f + m_pMenuStyle->GetPadding();
	}
}

void InputSettings::Toggle()
{
	m_IsActive = !m_IsActive;
	UpdateCurrentKeybindingWindow();
}

void InputSettings::UpdateCurrentKeybindingWindow()
{
	for (int i = 0; i < m_pBindsTextures.size(); i++)
	{
		delete m_pBindsTextures[i];
		m_pBindsTextures[i] = nullptr;
	}

	m_pBindsTextures.clear();

	for (int i = 0; i < int(KeybindingAction::LENGTH); i++)
	{
		std::string keyTxt{ SDL_GetKeyName(InputManager::Get()->GetKeyForAction(KeybindingAction(i))) };
		std::string actionTxt{ KeybindingActionToString[i] };

		m_pBindsTextures.push_back(
			new Texture(  (actionTxt + ": " + keyTxt)
				, m_pMenuStyle->GetFont()
				, m_pMenuStyle->GetFontSize()
				, m_pMenuStyle->GetTextColor()));
	}
}

