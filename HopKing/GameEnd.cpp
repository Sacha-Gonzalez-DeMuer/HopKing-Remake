#include "pch.h"
#include "GameEnd.h"
#include "TriggerCollider.h"
#include "Game.h"
#include "TypeWriter.h"
#include "utils.h"
#include "SaveManager.h"
#include "SoundManager.h"

GameEnd::GameEnd(const Window& window, const Rectf& triggerCollider)
	: m_pTriggerCollider{ new TriggerCollider(triggerCollider) }
	, m_BackgroundColor{1.0f,1.0f,1.0f,0.0f}
	, m_FadeInSpeed{0.7f}
	, m_EndingStarted{false}
	, m_NrFalls{}
	, m_NrJumps{}
	, m_pTypewriter{new TypeWriter("", Rectf(0,0, window.width, window.height))}
	, m_ScriptIdx{0}
	, m_Window{window}
	, m_Exiting{false}
	, m_EndMusic{ "gameEndMusic.mp3" }
{
	SoundManager::Get()->AddSoundStream(m_EndMusic);
}

GameEnd::~GameEnd()
{
	delete m_pTriggerCollider;
	delete m_pTypewriter;

	m_pTriggerCollider = nullptr;
	m_pTypewriter = nullptr;
}

void GameEnd::LoadScript()
{
	m_Script.push_back("Hey there!");
	m_Script.push_back("Congratulations! You've reached the top!");

	int totalSeconds{ SaveManager::Get()->FetchTimePlayed() };
	int minutes{ (totalSeconds / 60) };
	int hours = minutes / 60;
	int seconds = totalSeconds % 60;
	std::string timeTxt{ "It took you " + 
		std::to_string(hours) + "H " + 
		std::to_string(minutes % 60) + "M " +
		std::to_string(seconds) + "S" + "!"};
	m_Script.push_back(timeTxt);

	if (totalSeconds <= 500)
	{
		m_Script.push_back("What an insane run!");
	}
	if (totalSeconds > 500 && totalSeconds <= 1800)
	{
		m_Script.push_back("You should consider becoming a speedrunner!");
	}
	else if (hours <= 1)
	{
		m_Script.push_back("That's pretty fast!");
	}
	else if (hours >= 2 && hours <= 3) {
		m_Script.push_back("Not Bad!");
	}
	else if (hours > 3) {
		m_Script.push_back("I hope you're okay, that must've been frustrating.");
	}

	bool didCheat{ false };
	int timesJumped{ SaveManager::Get()->FetchTimesJumped() };
	if (timesJumped <= 5)
	{
		std::string jumpTxt{  };
		m_Script.push_back("You barely even jumped..");
		m_Script.push_back("Literally cheating. Kinda weird.");
		m_Script.push_back("Thanks for playing i guess");
		didCheat = true;
	}
	else
	{
		std::string jumpTxt{ "You jumped " + std::to_string(SaveManager::Get()->FetchTimesJumped()) + " times!" };
		m_Script.push_back(jumpTxt);
	}
	

	
	if (!didCheat)
	{
		int timesFallen{ SaveManager::Get()->FetchTimesFallen() };

		switch (timesFallen) {
			case 0:
				m_Script.push_back("And didn't even fall once?!");
				m_Script.push_back("You're an absolute gaming TITAN.");
				m_Script.push_back("Getting this good must've taken a lot of dedication.");
				break;
			case 1:
				m_Script.push_back("And only fell once?!");
				m_Script.push_back("Getting this good must've taken a lot of dedication.");
				break;
			default:
				std::string fallTxt{};
				if (timesFallen < 20)
				{
					fallTxt = "And only fell " + std::to_string(timesFallen) + " times,";
					m_Script.push_back(fallTxt);
					m_Script.push_back("Getting this good must've taken a lot of dedication.");
				}
				else
				{
					fallTxt= "And although you fell " + std::to_string(timesFallen) + " times..";
					m_Script.push_back(fallTxt);
					m_Script.push_back("..you never gave up.");
				}
				break;
		}

		m_Script.push_back("That's how you make it.");
		m_Script.push_back("Keep that attitude.");
		m_Script.push_back("Thanks for playing <3");
	}

	m_pTypewriter->UpdateText(m_Script[m_ScriptIdx]);
}

void GameEnd::Update(const Point2f& playerPos, Game* pGame, float elapsedSec)
{
	if (m_pTriggerCollider->OnTriggerEnter(playerPos) && !m_EndingStarted)
	{
		m_EndingStarted = true;
		SoundManager::Get()->PlaySoundStream(m_EndMusic, true);
		LoadScript();
	}

	if (m_EndingStarted)
	{
		if (m_BackgroundColor.a < 1)
		{
			m_BackgroundColor.a += m_FadeInSpeed * elapsedSec;
		}
		else
		{
			if (m_ScriptIdx == 0)
			{
				m_pTypewriter->StartTyping();
			}

			if (!m_pTypewriter->IsFinished())
			{
				m_pTypewriter->Update(elapsedSec);
			}
		}

		if (m_Exiting)
		{
			m_Exiting = false;
			pGame->ReturnToMainMenu();
			m_EndingStarted = false;
		}
	}
}

void GameEnd::DrawEndingSequence() const
{
	if (m_EndingStarted)
	{
		utils::SetColor(m_BackgroundColor);
		utils::FillRect(0, 0, m_Window.width, m_Window.height);

		if (m_BackgroundColor.a >= 1)
		{
			//start typing
			m_pTypewriter->Draw(Point2f(m_Window.width / 2, m_Window.height / 2));
		}
	}
}

bool GameEnd::IsEnding() const
{
	return m_EndingStarted;
}

void GameEnd::Continue()
{
	if (m_pTypewriter->IsFinished())
	{
		++m_ScriptIdx;

		if (m_ScriptIdx >= int(m_Script.size()))
		{
			Exit();
		}
		else
		{
			m_pTypewriter->UpdateText(m_Script[m_ScriptIdx]);
			m_pTypewriter->Reset();
			m_pTypewriter->StartTyping();
		}
	}
	else
	{
		m_pTypewriter->SkipTyping();
	}
}

void GameEnd::Exit()
{
	m_Exiting = true;
	m_ScriptIdx = 0;
	m_pTypewriter->Reset();

}
