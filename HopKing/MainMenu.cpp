#include "pch.h"
#include "MainMenu.h"
#include "Game.h"
#include "Button.h"
#include "Texture.h"
#include "utils.h"
#include "MenuManager.h"
#include "TextureManager.h"
#include "SaveManager.h"
#include "SoundManager.h"
#include "SpriteAnimator.h"


MainMenu::MainMenu(const Window& window, MenuManager* manager)
	: Menu{window, manager, 3 }
	, m_pPlayBtn{ new Button(Point2f(m_MenuRect.left + m_pMenuStyle->GetPadding(), m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() - (m_pMenuStyle->GetFontSize() / 1.33f)), "CONTINUE", m_pMenuStyle) }
	, m_pNewGameBtn{ new Button(Point2f(m_MenuRect.left + m_pMenuStyle->GetPadding(), m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() * 2 - (m_pMenuStyle->GetFontSize() / 1.33f) * 2), "NEW GAME", m_pMenuStyle) }
	, m_pOptionsBtn{ new Button(Point2f(m_MenuRect.left + m_pMenuStyle->GetPadding(), m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() * 3 - (m_pMenuStyle->GetFontSize() / 1.33f) * 3), "OPTIONS", m_pMenuStyle)  }
	, m_pQuitBtn { new Button(Point2f(m_MenuRect.left + m_pMenuStyle->GetPadding(), m_MenuRect.bottom + m_MenuRect.height - m_pMenuStyle->GetPadding() * 4 - (m_pMenuStyle->GetFontSize() / 1.33f) * 4), "QUIT", m_pMenuStyle) }
	, m_MenuMusic{"MenuTheme.mp3"}
	, m_PlaySound{"startGame.mp3"}
	, m_pFloatingBanjo{ }
	, m_FloatingBanjoPos{ utils::GenerateNegativeRandomRange(0.0f, 1920.0f), utils::GenerateNegativeRandomRange( 0.0f, 1080.0f) }
	, m_FloatingBanjoAngle{0}
	, m_FloatingBanjoRotationSpeed{10}
	, m_FloatingBanjoVelocity{utils::GenerateRandomRange(200.0f, 300.0f), utils::GenerateRandomRange(200.0f, 300.0f)}
{
	Initialize();
}

MainMenu::~MainMenu()
{
	m_pPlayBtn = nullptr;
	m_pOptionsBtn = nullptr;
	m_pQuitBtn = nullptr;

	delete m_pFloatingBanjo;
	m_pFloatingBanjo = nullptr;
}

void MainMenu::Update(float elapsedSec)
{
	m_pFloatingBanjo->Update(elapsedSec, Point2f(m_FloatingBanjoPos.x - m_pFloatingBanjo->GetClipRect().width / 2, m_FloatingBanjoPos.y - m_pFloatingBanjo->GetClipRect().height / 2));
	m_FloatingBanjoPos += m_FloatingBanjoVelocity * elapsedSec;
	

	float windowWidth{ 1920 + 300 };
	float windowHeight{ 1080 + 300 };
	m_FloatingBanjoPos.x = float(int(m_FloatingBanjoPos.x + int(windowWidth)) % int(windowWidth));
	m_FloatingBanjoPos.y = float(int(m_FloatingBanjoPos.y + int(windowHeight)) % int(windowHeight));


	m_FloatingBanjoAngle += m_FloatingBanjoRotationSpeed * elapsedSec;
}

void MainMenu::Initialize()
{
	TextureManager::Get()->AddTexture("BanjoBreathing", new Texture("Resources/Banjo/breathing.png"));
	m_pFloatingBanjo = new SpriteAnimator(TextureManager::Get()->GetTexture("BanjoBreathing"), 4, 6, 256, 256);

	int negativeX{ utils::GenerateRandomRange(0,1) };
	int negativeY{ utils::GenerateRandomRange(0,1) };
	negativeX == 1 ? m_FloatingBanjoVelocity.x = utils::GenerateNegativeRandomRange(-100.0f, -200.0f)
		: m_FloatingBanjoVelocity.x = utils::GenerateRandomRange(100.0f, 200.0f);

	negativeY == 1 ? m_FloatingBanjoVelocity.y = utils::GenerateNegativeRandomRange(-100.0f, -200.0f)
		: m_FloatingBanjoVelocity.y = utils::GenerateRandomRange(100.0f, 200.0f);


	AddButton(m_pPlayBtn);
	AddButton(m_pNewGameBtn);
	AddButton(m_pOptionsBtn);
	AddButton(m_pQuitBtn);

	SoundManager::Get()->AddSoundStream(m_MenuMusic);
	SoundManager::Get()->AddSFX(m_PlaySound);

	m_pPlayBtn->Select();
	m_IsCloseable = false;

	TextureManager::Get()->AddTexture("Background.png", new Texture("Resources/Menu/Background.png"));
}

void MainMenu::DrawAdditionalElements() const
{

	glPushMatrix();

	glScalef(utils::g_WindowScaleFactor, utils::g_WindowScaleFactor, 0);
	TextureManager::Get()->GetTexture("Background.png")->Draw();


	glTranslatef(m_pFloatingBanjo->GetPosition().x, m_pFloatingBanjo->GetPosition().y, 0);
	glRotatef(m_FloatingBanjoAngle, 0, 0, 1);
	glTranslatef(-m_pFloatingBanjo->GetPosition().x, -m_pFloatingBanjo->GetPosition().y, 0);
	m_pFloatingBanjo->Draw();

	glPopMatrix();
}

void MainMenu::Toggle()
{
	m_IsActive = !m_IsActive;

	if (m_IsActive)
	{
		SoundManager::Get()->PlaySoundStream(m_MenuMusic, true);
	}
	else
	{
		SoundManager::Get()->StopSoundStream(m_MenuMusic);
	}
}

void MainMenu::Close()
{
	m_IsActive = false;
	SoundManager::Get()->StopSoundStream(m_MenuMusic);
}

void MainMenu::Select(Game& game)
{
	if (IsActive())
	{
		if (m_pPlayBtn->IsSelected())
		{
			SoundManager::Get()->StopSoundStream(m_MenuMusic);
			Play(game);
			SoundManager::Get()->PlaySFX(m_PlaySound, 0);
		}
		else if (m_pNewGameBtn->IsSelected())
		{
			SaveManager::Get()->DeleteData();
			SoundManager::Get()->StopSoundStream(m_MenuMusic);
			Play(game);
		}
		else if (m_pOptionsBtn->IsSelected())
		{
			m_pMenuManager->OpenMenu(MenuType::OptionsMenu);
		} 
		else if (m_pQuitBtn->IsSelected()) {
			SaveManager::Get()->SaveData(*game.GetPlayer());
			game.QuitGame();
		}
	}
}

void MainMenu::Play(Game& game) const
{
	SaveManager::Get()->LoadData(*game.GetPlayer());
	m_pMenuManager->Close(true);
	game.UnPause();
	game.ChangeGameState(GameState::Game);
}
