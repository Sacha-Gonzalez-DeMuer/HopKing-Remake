#include "pch.h"
#include "Game.h"
#include "Level.h"
#include "Player.h"
#include "Camera.h"
#include "utils.h"
#include "SaveManager.h"
#include "SoundManager.h"
#include "InputManager.h"
#include "MenuManager.h"
#include "TextureManager.h"
#include <iostream>
#include "Core.h"


Game::Game( const Window& window ) 
	: m_Window{ window }
	, m_pLevel{new Level(window)}
	, m_pCamera{new Camera(m_Window.width,m_Window.height)}
	, m_GameState{ GameState::Menu }
	, m_pInputManager{ InputManager::Get() }
	, m_pPlayer{ new Player() }
	, m_IsPaused{}
	, m_pMenuManager{new MenuManager(window)}
	, m_QuitGame{false}
	, m_PauseSound{"pause.mp3"}
{
	Initialize( );
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	SoundManager::Get()->Initialize();
	SoundManager::Get()->AddSFX(m_PauseSound);
	m_pMenuManager->OpenMenu(MenuType::MainMenu);
}

void Game::Cleanup( )
{
	delete TextureManager::Get();

	delete m_pLevel;
	delete m_pPlayer;
	delete m_pCamera;
	delete m_pMenuManager;

	m_pLevel = nullptr;
	m_pPlayer = nullptr;
	m_pCamera = nullptr;
	m_pInputManager = nullptr;
	m_pMenuManager = nullptr;

	delete SoundManager::Get();
	delete SaveManager::Get();
	delete InputManager::Get();
}

void Game::Update( float elapsedSec )
{
	m_pMenuManager->Update(elapsedSec);
	switch (m_GameState) {
		case GameState::Game:
			if (!m_IsPaused)
			{
				if (!m_pLevel->IsLevelEnding()) m_pPlayer->Update(elapsedSec, m_pLevel);
				m_pLevel->Update(elapsedSec, m_pPlayer->GetPlayerPos(), this);

			}
			break;
	}
}

void Game::Draw( ) const
{
	ClearBackground( );

	switch (m_GameState)
	{
		case GameState::Game:

			glPushMatrix();

			glScalef(utils::g_WindowScaleFactor, utils::g_WindowScaleFactor, 0);
			m_pCamera->Transform(m_pPlayer->GetPlayerShape());

			m_pLevel->DrawBackground(m_pPlayer->GetPlayerPos());
			m_pLevel->DrawMiddleground(m_pPlayer->GetPlayerPos());

			m_pPlayer->Draw();

			m_pLevel->DrawForeground();

			glPopMatrix();

			m_pLevel->DrawOnTop();
			break;
	}

	m_pMenuManager->Draw();
}

void Game::Pause()
{
	m_IsPaused = !m_IsPaused;
	SoundManager::Get()->PlaySFX(m_PauseSound, 0);
}

void Game::UnPause()
{
	m_IsPaused = false;
}

Player* Game::GetPlayer() const
{
	return m_pPlayer;
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	KeybindingAction action{ m_pInputManager->GetActionForKey(e.keysym.sym) };

	if (m_GameState == GameState::Game)
	{
		if (!m_pInputManager->IsRebinding())
		{
			switch (action) {
			case KeybindingAction::PAUSE:
				m_pMenuManager->OpenMenu(MenuType::HUD);
				Pause();
			}

			if (m_pLevel->IsLevelEnding())
			{
				m_pLevel->ProcessKeyDownEvent();
			}
		}

		if (m_IsPaused)
		{
			if (!m_pInputManager->IsRebinding())
			{
				switch (action) {
				case KeybindingAction::UP:
					m_pMenuManager->SwitchSelection(false);
					break;

				case KeybindingAction::DOWN:
					m_pMenuManager->SwitchSelection(true);
					break;

				case KeybindingAction::CONFIRM:
					m_pMenuManager->Select(*this);
					break;
				case KeybindingAction::CANCEL:
					m_pMenuManager->Close(false);
				}
			}
			else
			{
				m_pInputManager->SaveLastKeyDown(e.keysym.sym, e.keysym.scancode);
				m_pMenuManager->Select(*this);
			}
		}
	}
	else if (m_GameState == GameState::Menu)
	{
		if (!m_pInputManager->IsRebinding())
		{
			switch (action) {
			case KeybindingAction::CONFIRM:
				m_pMenuManager->Select(*this);
				break;

			case KeybindingAction::UP:
				m_pMenuManager->SwitchSelection(false);
				break;

			case KeybindingAction::DOWN:
				m_pMenuManager->SwitchSelection(true);
				break;
			case KeybindingAction::CANCEL:
				m_pMenuManager->Close(false);
				break;

			}
		}
		else
		{
			m_pInputManager->SaveLastKeyDown(e.keysym.sym, e.keysym.scancode);
			m_pMenuManager->Select(*this);
		}
		
	}

	if (e.keysym.sym == SDLK_i)
	{
		for (int i = 0; i < int(KeybindingAction::LENGTH); i++)
		{
			std::cout << KeybindingActionToString[i] << ": " << SDL_GetKeyName(InputManager::Get()->GetKeyForAction(KeybindingAction(i))) << std::endl;
		}
	}
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{

}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	if (m_GameState == GameState::Game)
	{
		m_pPlayer->SetPositionCheat(Point2f(float(e.x), float(e.y)));
	}
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{

}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::ChangeGameState(const GameState toState)
{
	m_GameState = toState;
}

void Game::ReturnToMainMenu()
{
	m_pMenuManager->OpenMenu(MenuType::MainMenu);
	ChangeGameState(GameState::Menu);
}

void Game::QuitGame()
{
	m_QuitGame = true;
}

bool Game::GetQuitGame() const
{
	return m_QuitGame;
}

InputManager* Game::GetInputManager()
{
	return m_pInputManager;
}

 