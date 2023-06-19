#include "pch.h"
#include "Button.h"
#include "utils.h"
#include "Texture.h"
#include "TextureManager.h"
#include "MenuStyle.h"

Button::Button(const Point2f& pos, const std::string& text, MenuStyle* menuStyle)
	: m_BtnText { text }
	, m_Pos{pos}
	, m_SelectedIcon{ pos.x, pos.y + menuStyle->GetFontSize() / 2, float(menuStyle->GetFontSize() / 5)}
	, m_Selected{}
{
	TextureManager::Get()->GetTextTexture(m_BtnText, menuStyle);
}

void Button::Draw() const
{
	utils::SetColor(Color4f(1, 1, 1, 1));
	if (m_Selected)
	{
		utils::FillEllipse(Ellipsef(m_SelectedIcon.center, m_SelectedIcon.radius, m_SelectedIcon.radius));
		TextureManager::Get()->GetTexture(m_BtnText)->Draw(Point2f(m_Pos.x + m_SelectedIcon.radius * 2, m_Pos.y));
	}
	else
	{
		TextureManager::Get()->GetTexture(m_BtnText)->Draw(m_Pos);
	}
}

bool Button::IsSelected() const
{
	return m_Selected;
}

void Button::Select()
{
	m_Selected = true;
}

void Button::Deselect()
{
	m_Selected = false;
}

