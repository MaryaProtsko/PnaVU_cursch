#pragma once
#include "GPRender.hpp"

class GPUI : public virtual GPRender
{
private:
	sf::Text m_paused_text;
	sf::Text m_game_over_text;
	sf::Text m_guide_text1;
	sf::Text m_guide_text2;
	sf::Text m_score_text;
	sf::Text m_lines_text;
	sf::Text m_level_text;
	sf::Text m_score_num;
	sf::Text m_lines_num;
	sf::Text m_level_num;
public:
	void UpdateTextPos();
	sf::Vector2f m_side_space = {0 , 0};
protected:
	GPUI();
	void UpdateInfo();
	void DrawSide(sf::RenderTarget& target, sf::RenderStates states) const;
};