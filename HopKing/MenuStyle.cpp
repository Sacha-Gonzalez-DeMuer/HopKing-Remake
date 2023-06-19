#include "pch.h"
#include "MenuStyle.h"

MenuStyle::MenuStyle()
	: m_TextColor{Color4f(1.0f,1.0f,1.0f,1.0f)}
	, m_FontSize{ 50 }
	, m_Font{ "Resources/Fonts/Fixedsys.ttf" }
	, m_Padding{100.0f}
	, m_WindowBackgroundColor{ Color4f(0.0f,0.0f,0.0f,1.0f) }
	, m_WindowBorderColor{ Color4f(1.0f,1.0f,1.0f,1.0f) }
{
}

MenuStyle::MenuStyle(const Window& window)
	: m_TextColor{ Color4f(1.0f,1.0f,1.0f,1.0f) }
	, m_FontSize{ 50 }
	, m_Font{ "Resources/Fonts/Fixedsys.ttf" }
	, m_Padding{ window.width/100.0f }
	, m_WindowBackgroundColor{ Color4f(0.0f,0.0f,0.0f,1.0f) }
	, m_WindowBorderColor{ Color4f(1.0f,1.0f,1.0f,1.0f) }
{
}

MenuStyle::MenuStyle(const Color4f& textColor, const int fontSize, const float padding)
	: m_TextColor{textColor}
	, m_FontSize{fontSize}
	, m_Font{ "Resources/Fonts/Fixedsys.ttf" }
	, m_Padding{padding}
	, m_WindowBackgroundColor{ Color4f(0.0f,0.0f,0.0f,1.0f) }
	, m_WindowBorderColor{ Color4f(1.0f,1.0f,1.0f,1.0f) }
{
}

Color4f MenuStyle::GetWindowBackgroundColor() const
{
	return m_WindowBackgroundColor;
}

Color4f MenuStyle::GetWindowBorderColor() const
{
	return m_WindowBorderColor;
}

Color4f MenuStyle::GetTextColor() const
{
	return m_TextColor;
}

int MenuStyle::GetFontSize() const
{
	return m_FontSize;
}

float MenuStyle::GetPadding() const
{
	return m_Padding;
}

std::string MenuStyle::GetFont() const
{
	return m_Font;
}
