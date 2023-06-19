#include "pch.h"
#include "HUD.h"
#include "Button.h"
#include "utils.h"
#include "Game.h"
#include "SaveManager.h"
#include "MenuManager.h"
#include <iostream>
#include "TextureManager.h"
#include "Texture.h"

HUD::HUD(const Window& window, MenuManager* menuManager)
	: Menu{ window, menuManager, 3 }
	, m_DataRect{
		m_pMenuStyle->GetPadding(),
		window.height - m_pMenuStyle->GetFontSize() * 1.33f * 4 - m_pMenuStyle->GetPadding(),
		window.width / 3.5f, m_pMenuStyle->GetFontSize() * 1.33f * 4 }
	, m_pJumpTxt{
		new Texture("Jumps: " + std::to_string(SaveManager::Get()->FetchTimesJumped()),
		"Resources/Fonts/Fixedsys.ttf", m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor()) }

	, m_pFallTxt{ 
		new Texture("Falls: " + std::to_string(SaveManager::Get()->FetchTimesFallen()),
		"Resources/Fonts/Fixedsys.ttf", m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor())
	}
	, m_TotalSecondsPlayed{SaveManager::Get()->FetchTimePlayed()}
	, m_pTimePlayedTxt{ new Texture("Time played: ", "Resources/Fonts/Fixedsys.ttf", m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor()) }
	, m_pTimeTxt{ 
			new Texture(
				(std::to_string((m_TotalSecondsPlayed / 60) / 60) + "H "
				+ std::to_string((m_TotalSecondsPlayed / 60) % 60) + "M "
				+ std::to_string(m_TotalSecondsPlayed % 60) + "S"),
				"Resources/Fonts/Fixedsys.ttf", m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor())
	}
	, m_pResumeBtn { new Button( 
			Point2f(
				m_MenuRect.left + m_pMenuStyle->GetPadding(),
				m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() - (m_pMenuStyle->GetFontSize() / 1.33f)), //1pt = 1.33px
			"RESUME",
			m_pMenuStyle)
		}
	, m_pOptionsBtn { new Button( 
			Point2f(
				m_MenuRect.left + m_pMenuStyle->GetPadding(),
				m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() * 2 - (m_pMenuStyle->GetFontSize() / 1.33f) * 2),
			"OPTIONS",
			m_pMenuStyle)
		}
	, m_pSaveExitBtn{ new Button(
			Point2f(
				m_MenuRect.left + m_pMenuStyle->GetPadding(),
				m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() * 3 - (m_pMenuStyle->GetFontSize() / 1.33f) * 3),
			"SAVE & EXIT",
			m_pMenuStyle)
	}
	, m_pQuitBtn{ new Button(
			Point2f(
				m_MenuRect.left + m_pMenuStyle->GetPadding(),
				m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() * 4 - (m_pMenuStyle->GetFontSize() / 1.33f) * 4),
			"QUIT",
			m_pMenuStyle) }
{
	AddButton(m_pResumeBtn);
	AddButton(m_pOptionsBtn);
	AddButton(m_pSaveExitBtn);
	AddButton(m_pQuitBtn);

	m_IsCloseable = false;

	m_pResumeBtn->Select();
}

HUD::~HUD()
{
	m_pResumeBtn = nullptr;
	m_pOptionsBtn = nullptr;
	m_pSaveExitBtn = nullptr;
	m_pQuitBtn = nullptr;

	delete m_pJumpTxt;
	m_pJumpTxt = nullptr;

	delete m_pFallTxt;
	m_pFallTxt = nullptr;

	delete m_pTimePlayedTxt;
	m_pTimePlayedTxt = nullptr;

	delete m_pTimeTxt;
	m_pTimeTxt = nullptr;
}

void HUD::UpdateData()
{
	delete m_pJumpTxt;
	delete m_pFallTxt;
	delete m_pTimeTxt;
	
	m_pJumpTxt = new Texture("Jumps: " + std::to_string(SaveManager::Get()->FetchTimesJumped()),
		"Resources/Fonts/Fixedsys.ttf", m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor());

	m_pFallTxt = new Texture("Falls: " + std::to_string(SaveManager::Get()->FetchTimesFallen()),
		"Resources/Fonts/Fixedsys.ttf", m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor());

	m_TotalSecondsPlayed = SaveManager::Get()->FetchTimePlayed();
	m_pTimeTxt =
		new Texture(
			(std::to_string(m_TotalSecondsPlayed / 3600) + "H "
				+ std::to_string((m_TotalSecondsPlayed / 60) % 60) + "M "
				+ std::to_string(m_TotalSecondsPlayed % 60) + "S"),
			"Resources/Fonts/Fixedsys.ttf", m_pMenuStyle->GetFontSize(), m_pMenuStyle->GetTextColor());
}

void HUD::Select(Game& game)
{
	if (IsActive())
	{
		int nrSelected{};

		for (Button* btn : GetButtons()) {
			if (btn->IsSelected())
			{
				++nrSelected;
			}
		}

		if (nrSelected == 1)
		{
			if (m_pResumeBtn->IsSelected())
			{
				game.UnPause();
				m_pMenuManager->Close(true);
			}
			else if (m_pSaveExitBtn->IsSelected())
			{
				m_pMenuManager->Close();
				SaveManager::Get()->SaveData(*game.GetPlayer());
				m_pMenuManager->OpenMenu(MenuType::MainMenu);
				game.ChangeGameState(GameState::Menu);
			}
			else if (m_pOptionsBtn->IsSelected()) {
				m_pMenuManager->OpenMenu(MenuType::OptionsMenu);
			}
			else if (m_pQuitBtn->IsSelected()) {
				SaveManager::Get()->SaveData(*game.GetPlayer());
				game.QuitGame();
			}
		}
		else
		{
			std::cerr << "HUD Error: More/less than 1 button selected";
		}
	}
}

void HUD::DrawAdditionalElements() const
{
	utils::SetColor(m_pMenuStyle->GetWindowBackgroundColor());
	utils::FillRect(m_DataRect);
	utils::SetColor(m_pMenuStyle->GetWindowBorderColor());
	utils::DrawRect(m_DataRect);

	m_pJumpTxt->Draw(Point2f(m_DataRect.left + m_pMenuStyle->GetPadding(), m_DataRect.bottom + m_pMenuStyle->GetFontSize() * 3 + m_pMenuStyle->GetPadding() * 4) );
	m_pFallTxt->Draw(Point2f(m_DataRect.left + m_pMenuStyle->GetPadding(), m_DataRect.bottom + m_pMenuStyle->GetFontSize() * 2 + m_pMenuStyle->GetPadding() * 3));
	m_pTimePlayedTxt->Draw(Point2f(m_DataRect.left + m_pMenuStyle->GetPadding(), m_DataRect.bottom + m_pMenuStyle->GetFontSize() + m_pMenuStyle->GetPadding() * 2));
	m_pTimeTxt->Draw(Point2f(m_DataRect.left + m_pMenuStyle->GetPadding(), m_DataRect.bottom + m_pMenuStyle->GetPadding()));
}

void HUD::Toggle()
{
	m_IsActive = !m_IsActive;
	UpdateData();
}
