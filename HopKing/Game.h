#pragma once
#include "enums.h"

class Level;
class Player;
class Camera;
class MainMenu;
class SoundEffects;
class HUD;
class InputManager;
class MenuManager;
class TextureManager;

class ParticleSystem;

class Game final
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& rhs) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& rhs) = delete;
	~Game();

	void Update( float elapsedSec );
	void Draw( ) const;

	void Pause();
	void UnPause();
	Player* GetPlayer() const;

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e );
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e );
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e );
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e );
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e );
	void ChangeGameState(const GameState toState);
	void ReturnToMainMenu();
	void QuitGame();

	bool GetQuitGame() const;

	InputManager* GetInputManager();

private:
	// DATA MEMBERS
	const Window m_Window;
	GameState m_GameState;
	Level* m_pLevel;
	Player* m_pPlayer;
	Camera* m_pCamera;
	InputManager* m_pInputManager;
	MenuManager* m_pMenuManager;

	bool m_IsPaused;
	bool m_QuitGame;

	//sound
	const std::string m_PauseSound;
	
	ParticleSystem* m_pParticleSysTest;

	// FUNCTIONS
	void Initialize( );
	void Cleanup( );
	void ClearBackground( ) const;
};