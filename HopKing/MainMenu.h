#pragma once
#include <vector>
#include "Menu.h"
#include "Vector2f.h"

class Game;
class Button;
class OptionsMenu;
class SpriteAnimator;

class MainMenu final : public Menu
{
public:
	MainMenu(const Window& window, MenuManager* manager);
	~MainMenu();
	MainMenu(const MainMenu& other) = delete;
	MainMenu(MainMenu&& other) = delete;
	MainMenu& operator=(const MainMenu other) = delete;
	MainMenu& operator=(MainMenu&& other) = delete;

	virtual void Update(float elapsedSec) override;

	void Initialize();
	virtual void DrawAdditionalElements() const override;
	virtual void Toggle() override;
	virtual void Close() override;

	virtual void Select(Game& game) override;

private:
	Point2f m_MousePos;

	Button* m_pPlayBtn;
	Button* m_pNewGameBtn;
	Button* m_pOptionsBtn;
	Button* m_pQuitBtn;

	SpriteAnimator* m_pFloatingBanjo;
	const float m_FloatingBanjoRotationSpeed;
	Vector2f m_FloatingBanjoVelocity;
	Point2f m_FloatingBanjoPos;
	float m_FloatingBanjoAngle;

	//sound
	const std::string m_MenuMusic;
	const std::string m_PlaySound;

	void Play(Game& game) const;
};

