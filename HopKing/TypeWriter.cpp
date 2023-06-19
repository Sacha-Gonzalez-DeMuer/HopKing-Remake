#include "pch.h"
#include "TypeWriter.h"
#include "Texture.h"
#include <string>
#include <iostream>
#include "utils.h"
#include "SoundManager.h"

TypeWriter::TypeWriter(const std::string& text, const Rectf& window)
	: m_Text{text}
	, m_FontSize{40}
	, m_DelayTimer{0}
	, m_TypeDelay{0.05f}
	, m_pTexture{new Texture(" ", "Resources/Fonts/Fixedsys.ttf", m_FontSize, Color4f(0, 0, 0, 1))}
	, m_Typing{false}
	, m_TextIdx{0}
	, m_TypingSF{ "typingSound.mp3" }
{
	SoundManager::Get()->AddSFX(m_TypingSF);
}

TypeWriter::~TypeWriter()
{
	delete m_pTexture;
	m_pTexture = nullptr;
}

void TypeWriter::StartTyping()
{
	m_Typing = true;
}

void TypeWriter::StopTyping()
{
	m_Typing = false;
}

void TypeWriter::SkipTyping()
{
	delete m_pTexture;
	m_TextIdx = int(m_Text.length());
	m_pTexture = new Texture(m_Text, "Resources/Fonts/Fixedsys.ttf", m_FontSize, Color4f(0, 0, 0, 1));
}

void TypeWriter::Reset()
{
	m_DelayTimer = 0;
	m_TextIdx = 0;
	m_Typing = false;
}

void TypeWriter::UpdateText(const std::string& newText)
{
	m_Text = newText;
}

bool TypeWriter::IsFinished() const
{
	return m_TextIdx > int(m_Text.length());
}

void TypeWriter::Update(float elapsedSec)
{
	if (m_Typing)
	{
		m_DelayTimer += elapsedSec;

		if (m_DelayTimer >= m_TypeDelay)
		{
			delete m_pTexture;
			m_pTexture = new Texture(m_Text.substr(0, m_TextIdx), "Resources/Fonts/Fixedsys.ttf", m_FontSize, Color4f(0, 0, 0, 1));
			SoundManager::Get()->PlaySFX(m_TypingSF, 0);
			if (!IsFinished())
			{
				++m_TextIdx;
			}
			else
			{
				StopTyping();
			}

			m_DelayTimer = 0;
		}
	}
}

void TypeWriter::Draw() const
{
	utils::SetColor(Color4f(0, 0, 0, 1));
	utils::DrawRect(m_Window);
	m_pTexture->Draw(Point2f(m_Window.width / 2 - m_pTexture->GetWidth() / 2, m_Window.height / 2 - m_pTexture->GetHeight() / 2));
}

void TypeWriter::Draw(const Point2f& pos) const
{
	utils::SetColor(Color4f(0, 0, 0, 1));
	utils::DrawRect(m_Window);
	m_pTexture->Draw(Point2f(pos.x - m_pTexture->GetWidth()/2, pos.y - m_pTexture->GetHeight()/2));
}
