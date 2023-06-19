#pragma once
#include <vector>
#include "enums.h"
#include "MenuStyle.h"

class Texture;
class Game;
class Button;
class MenuManager;

class Menu
{
public:
	Menu(const Window& window, MenuManager* menuManager, const int nrBtns);
	virtual ~Menu();
	Menu(const Menu& other) = delete;
	Menu(Menu&& other) = delete;
	Menu& operator=(const Menu rhs) = delete;
	Menu& operator=(Menu&& other) = delete;

	virtual void Update(float elapsedSec);

	void Draw() const;
	virtual void DrawAdditionalElements() const;
	virtual void Select(Game& game) = 0;

	Point2f GetPosition() const;
	void SetPosition(const Point2f& pos);

	void AddButton(Button* btn);
	void SwitchSelection(bool down);
	virtual void Toggle();
	virtual void Close();

	bool IsCloseable() const;
	bool IsActive() const;

protected:
	MenuManager* m_pMenuManager;
	MenuStyle* m_pMenuStyle;
	bool m_IsActive;

	Rectf m_MenuRect;
	bool m_IsCloseable;

	void SetDrawAdditionalOnTop(bool isTrue);
	std::vector<Button*> GetButtons() const;


private:
	int m_SelectIdx;
	bool m_DrawAdditionalOnTop;

	std::vector<Button*> m_pButtons;


	void CorrectMenuRect(const Window& window);

};

