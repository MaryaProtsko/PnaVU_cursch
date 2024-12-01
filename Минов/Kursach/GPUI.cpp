#include "GPUI.hpp"

void GPUI::UpdateTextPos()
{
	m_paused_text.setPosition(sf::Vector2f{ (GetSize().x - m_paused_text.getGlobalBounds().width) * 0.5f, 
		GetSize().y * 0.4f } + GetPos());
	m_game_over_text.setPosition(sf::Vector2f{ (GetSize().x - m_game_over_text.getGlobalBounds().width) * 0.5f, 
		GetSize().y * 0.4f } + GetPos());
	m_guide_text2.setPosition({ m_pos.x, m_pos.y + GetSize().y + 2});
	m_guide_text1.setPosition({ m_pos.x + m_guide_text2.getGlobalBounds().width + 8, m_pos.y + GetSize().y + 2});
	sf::Vector2f side_pos = m_pos + m_side_space + sf::Vector2f{ GetSize().x, 0 };

	sf::Vector2f text_h_shift = {0, m_paused_text.getGlobalBounds().height * 1.f};
	sf::Vector2f text_between_h_shift = {0, m_paused_text.getGlobalBounds().height * 2.f};
	
	int i = 0;
	for (auto text : std::vector<sf::Text*>{ &m_score_text, &m_score_num, &m_lines_text,
		&m_lines_num, &m_level_text, &m_level_num })
	{
		text->setPosition(side_pos);
		side_pos += (text_h_shift + float(i % 2) * text_between_h_shift);
		i++;
	}
}

GPUI::GPUI()
{
	const std::vector<sf::Text*> to_set_font = { 
		&m_paused_text, &m_game_over_text, &m_guide_text1, &m_guide_text2,
		&m_score_num, &m_lines_num, &m_level_num, &m_score_text, &m_lines_text, &m_level_text};

	for (sf::Text* t : to_set_font)
		t->setFont(*FileManager::Get().GetFont());

	m_paused_text.setScale(1.5f, 1.5f);
	m_paused_text.setOutlineThickness(3.f);

	m_game_over_text.setScale(1.5f, 1.5f);
	m_game_over_text.setFillColor(sf::Color::Red);
	m_game_over_text.setOutlineThickness(3.f);

	m_guide_text1.setScale(0.6f, 0.6f);
	m_guide_text2.setScale(0.6f, 0.6f);

	for (sf::Text& text : std::vector{m_score_text, m_lines_text, m_level_text})
		text.setStyle(sf::Text::Underlined);

	const std::vector names = { "PAUSED", "GAME OVER", "R - restart   ESC - to menu", "SPACE - pause", "SCORE", "LINES", "LEVEL" };
	int i = 0;
	for (auto& text : std::vector<sf::Text*>{ &m_paused_text, &m_game_over_text, &m_guide_text1, 
		&m_guide_text2, &m_score_text, &m_lines_text, &m_level_text })
			text->setString(names[i++]);
}

void GPUI::UpdateInfo()
{
	std::ostringstream oss; 
	const auto set_text = [&oss](int* from, sf::Text* to)
	{
		oss << std::setw(6) << std::setfill('0') << *from;
		to->setString(oss.str());
		oss.str("");
	};
	set_text(&m_game_scores.score, &m_score_num);
	set_text(&m_game_scores.lines, &m_lines_num);
	set_text(&m_game_scores.level, &m_level_num);
}

void GPUI::DrawSide(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_state == paused)
		target.draw(m_paused_text, states);
	if(m_state == over)
		target.draw(m_game_over_text, states);
	target.draw(m_guide_text2, states);
	if (m_state == paused || m_state == over)
		target.draw(m_guide_text1, states);

	for (const auto text : std::vector<const sf::Text*>{ &m_score_text, &m_lines_text, &m_level_text,
		&m_score_num, &m_lines_num, &m_level_num })
			target.draw(*text, states);

	Figure next_fig;
	next_fig.SetFigure(m_next_fig_id);
	next_fig.m_x = m_field_size.x + 2 * m_wall_thickness + 1;
	next_fig.m_y = m_field_size.y - 6;
	DrawFigure(target, states, next_fig);
}