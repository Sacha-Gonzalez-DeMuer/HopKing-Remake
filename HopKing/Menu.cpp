#include "pch.h"
#include "Menu.h"
#include "Texture.h"
#include "utils.h"
#include "Button.h"
#include "Game.h"
#include "enums.h"

Menu::Menu(const Window& window, MenuManager* menuManager, const int nrBtns)
	: m_pMenuManager{menuManager}
	, m_pMenuStyle{ new MenuStyle(window) }
	, m_MenuRect
		{ 
			0, 
			0, 
			(window.width/3),
			(m_pMenuStyle->GetFontSize() * 1.33f * (nrBtns)) + (m_pMenuStyle->GetPadding() * (nrBtns))
		}
	, m_pButtons{}
	, m_IsActive{}
	, m_SelectIdx{0}
	, m_DrawAdditionalOnTop{}
	, m_IsCloseable{true}
{
	CorrectMenuRect(window);
}

Menu::~Menu()
{
	for (Button* pBtn : m_pButtons) {
		delete pBtn;
		pBtn = nullptr;
	}

	delete m_pMenuStyle;
	m_pMenuStyle = nullptr;
}

void Menu::Update(float elapsedSec)
{
}

void Menu::Draw() const
{
	if (IsActive())
	{
		if (!m_DrawAdditionalOnTop)
		{
			DrawAdditionalElements();
		}

		utils::SetColor(m_pMenuStyle->GetWindowBackgroundColor());
		utils::FillRect(m_MenuRect);

		utils::SetColor(m_pMenuStyle->GetWindowBorderColor());
		utils::DrawRect(m_MenuRect);

		for (const Button* btn : m_pButtons) {
			btn->Draw();
		}

		if (m_DrawAdditionalOnTop)
		{
			DrawAdditionalElements();
		}
	}
}

void Menu::DrawAdditionalElements() const
{
}


Point2f Menu::GetPosition() const
{
	return Point2f(m_MenuRect.left, m_MenuRect.bottom);
}

void Menu::SetPosition(const Point2f& pos)
{
	m_MenuRect.left = pos.x;
	m_MenuRect.bottom = pos.y;
}

void Menu::AddButton(Button* btn)
{
	m_pButtons.push_back(btn);
}

void Menu::SwitchSelection(bool down)
{
	if (IsActive() && m_pButtons.size() > 0)
	{
		m_pButtons[m_SelectIdx]->Deselect();
		down ? ++m_SelectIdx : --m_SelectIdx;
		m_SelectIdx = (m_SelectIdx + m_pButtons.size()) % int(m_pButtons.size());
		m_pButtons[m_SelectIdx]->Select();
	}
}

void Menu::Toggle()
{
	m_IsActive = !m_IsActive;
}

std::vector<Button*> Menu::GetButtons() const
{
	return m_pButtons;
}

void Menu::SetDrawAdditionalOnTop(bool isTrue)
{
	m_DrawAdditionalOnTop = isTrue;
}

void Menu::CorrectMenuRect(const Window& window)
{
	if (m_MenuRect.bottom + m_MenuRect.height > window.height)
	{
		m_MenuRect.bottom = window.height - m_MenuRect.height - m_pMenuStyle->GetPadding();
	}
	else if (m_MenuRect.bottom < m_pMenuStyle->GetPadding())
	{
		m_MenuRect.bottom = m_pMenuStyle->GetPadding();
	}

	if (m_MenuRect.left + m_MenuRect.width > window.width)
	{
		m_MenuRect.left = window.width - m_MenuRect.width - m_pMenuStyle->GetPadding();
	}
	else if (m_MenuRect.left < m_pMenuStyle->GetPadding()) {
		m_MenuRect.left = m_pMenuStyle->GetPadding();
	}
}

bool Menu::IsCloseable() const
{
	return m_IsCloseable;
}

bool Menu::IsActive() const
{
	return m_IsActive;
}


void Menu::Close()
{
	/*Menu* activeSub{ IsInSubMenu() };
	if (activeSub)
	{
		activeSub->PropagateClose();
	}
	else
	{
		Toggle();
	}*/

	//if (IsInSubMenu())
	//{
	//	GetTopActiveSubMenu()->Toggle();
	//}
	//else
	//{
	//	Toggle();
	//}
	m_IsActive = false;
}
