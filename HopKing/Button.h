#pragma once
#include <string>
class MenuStyle;

class Button final
{
public:
	Button(const Point2f& pos, const std::string& text, MenuStyle* menuStyle);

	void Draw() const;

	bool IsSelected() const;
	void Select();
	void Deselect();

private:
	const std::string m_BtnText;
	Point2f m_Pos;

	Circlef m_SelectedIcon;
	bool m_Selected;
};

