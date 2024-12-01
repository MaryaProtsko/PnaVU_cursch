#pragma once
#include <SFML/Graphics.hpp>
#include "GameProcess.hpp"
#include "FileManager.hpp"

struct Leaderboard
{
	sf::Text m_text;
	Leaderboard();
};

class GameProcess;
struct InputField : public sf::Drawable
{
	bool m_entering;
	InputField();
	void Update(GameProcess* gp);

	bool HanldeEvents(const sf::Event event, GameProcess* gp);
	std::string GetSrtring()
	{
		return m_str;
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
private:
	bool m_visible = false;
	std::string m_str;
	sf::Text m_pretext;
	sf::Text m_text;
	bool CheckChar(char simbol);
	void HanldeKeyEvents(const sf::Event event);
};