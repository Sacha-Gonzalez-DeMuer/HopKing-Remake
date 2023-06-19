#include "pch.h"
#include "OptionsMenu.h"
#include "Button.h"
#include "MenuManager.h"
#include "Game.h"

OptionsMenu::OptionsMenu(const Window& window, MenuManager* menuManager)
	: Menu{ window, menuManager, 1 }
	, m_pControlsBtn
		{ new Button{ Point2f(
			m_MenuRect.left + m_pMenuStyle->GetPadding(),
			m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() - (m_pMenuStyle->GetFontSize() / 1.33f)),
			"CONTROLS",
			m_pMenuStyle }
		}
{
	AddButton(m_pControlsBtn);
	m_pControlsBtn->Select();
}

OptionsMenu::~OptionsMenu()
{
	m_pControlsBtn = nullptr;
}

void OptionsMenu::Select(Game& game)
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
			if (m_pControlsBtn->IsSelected())
			{
				m_pMenuManager->OpenMenu(MenuType::InputSettings);
			}
		}
	}
}
