#pragma once

class Texture;

class TypeWriter final
{
public:
	TypeWriter(const std::string& text, const Rectf& window);
	~TypeWriter();

	TypeWriter(const TypeWriter& other) = delete;
	TypeWriter(TypeWriter&& other) = delete;
	TypeWriter& operator=(const TypeWriter& other) = delete;
	TypeWriter& operator=(TypeWriter&& other) = delete;

	void StartTyping();
	void StopTyping();
	void SkipTyping();
	void Reset();

	void UpdateText(const std::string& newText);

	bool IsFinished() const;

	void Update(float elapsedSec);
	void Draw() const;
	void Draw(const Point2f& pos) const;

private:
	bool m_Typing;

	std::string m_Text;

	int m_FontSize;
	Rectf m_Window;
	float m_TypeDelay;
	float m_DelayTimer;
	int m_TextIdx;
	Texture* m_pTexture;

	const std::string m_TypingSF;
};

