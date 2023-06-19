#pragma once
class MenuStyle final
{
public:
	MenuStyle();
	MenuStyle(const Window& window);
	MenuStyle(const Color4f& textColor, const int fontSize, const float padding);

	Color4f GetWindowBackgroundColor() const;
	Color4f GetWindowBorderColor() const;
	Color4f GetTextColor() const;
	int GetFontSize() const;
	float GetPadding() const;
	std::string GetFont() const;

private:	
	//text
	const Color4f m_TextColor;
	const int m_FontSize;
	const float m_Padding;
	const std::string m_Font;

	//window
	const Color4f m_WindowBackgroundColor;
	const Color4f m_WindowBorderColor;
};

