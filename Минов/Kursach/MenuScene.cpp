#include "MenuScene.hpp"

MenuScene::MenuScene(SceneLoadProp prop) : m_start_prop(prop), m_scene_finished(false), cursor_pos(0)
{
	const std::vector names = { "Start game", "Two players", "Credits", "Exit" };
	for(int i = 0; i < names.size(); i++)
		m_options[i].setString(names[i]);

	for (auto& text : m_options)
		text.setScale(1.2f, 1.2f);

	m_logo.setTexture(*FileManager::Get().GetLogoTexture());
	m_logo.setScale(2.f, 2.f);
	m_logo.setPosition(300, 0);

	for (int i = 0; i < menu_options_count; i++)
	{
		m_options[i].setFont(*FileManager::Get().GetFont());
		m_options[i].setPosition(10, i * 80 + 300);
	}
	UpdateOptionsStyle();
	m_lb.m_text.setPosition(800, 400);
}

void MenuScene::UpdateOptionsStyle()
{
	for (int i = 0; i < menu_options_count; i++)
	{
		m_options[i].setFillColor(sf::Color::White);
		m_options[i].setStyle(sf::Text::Regular);
	}
	m_options[cursor_pos].setFillColor(sf::Color::Yellow);
	m_options[cursor_pos].setStyle(sf::Text::Underlined);
}

void MenuScene::HanldeEvents(const sf::Event event) 
{
	bool key_p = event.type == sf::Event::KeyPressed;
	if (key_p && (event.key.code == sf::Keyboard::Down || event.key.code == sf::Keyboard::S))
	{
		cursor_pos++;
		cursor_pos %= menu_options_count;
		UpdateOptionsStyle();
	}
	if (key_p && (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::W))
	{
		cursor_pos--;
		cursor_pos = cursor_pos >= 0 ? cursor_pos : menu_options_count - 1;
		UpdateOptionsStyle();
	}
	if (key_p && (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space))
		switch (cursor_pos)
		{
		case 0:
		{
			m_exit_prop.next_scene = SceneLoadProp::default_singleplayer;
			m_scene_finished = true;
		}
		break;
		case 1:
		{
			m_exit_prop.next_scene = SceneLoadProp::multiplayer;
			m_scene_finished = true;
		}
		break;
		case 3:
		{
			m_exit_prop.next_scene = SceneLoadProp::finished;
			m_scene_finished = true;
		}
		break;
		default:
			break;
		}
};

void MenuScene::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{
	for (int i = 0; i < menu_options_count; i++)
		target.draw(m_options[i]);
	target.draw(m_lb.m_text, states);
	target.draw(m_logo, states);
};

bool MenuScene::HasFinished(SceneLoadProp* exit_prop)
{
	if (!m_scene_finished)
		return false;
	*exit_prop = m_exit_prop;
	return true;
}