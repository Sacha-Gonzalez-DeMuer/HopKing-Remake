#pragma once
#include <vector>

class TriggerCollider;
class Game;
class SpriteAnimator;
class TypeWriter;

class GameEnd final
{
public:
	GameEnd(const Window& window, const Rectf& triggerCollider);
	~GameEnd();

	GameEnd(const GameEnd& other) = delete;
	GameEnd(GameEnd&& other) = delete;
	GameEnd& operator=(const GameEnd& other) = delete;
	GameEnd& operator=(GameEnd&& other) = delete;

	void LoadScript();
	void Update(const Point2f& playerPos, Game* game, float elapsedSec);
	void DrawEndingSequence() const;

	bool IsEnding() const;
	void Continue();

private: 
	TriggerCollider* m_pTriggerCollider;
	TypeWriter* m_pTypewriter;
	const Window m_Window;
	std::vector<std::string> m_Script;
	int m_ScriptIdx;
	
	Color4f m_BackgroundColor;
	const float m_FadeInSpeed;

	bool m_EndingStarted;
	int m_NrFalls;
	int m_NrJumps;

	void Exit();
	bool m_Exiting;


	//sound
	std::string m_EndMusic;
};

