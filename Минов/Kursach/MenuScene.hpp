#pragma once
#include "Iscene.hpp"

class MenuScene : public IScene
{
public:
	MenuScene(SceneLoadProp prop) : m_start_prop(prop), m_scene_finished(false), cursor_pos(0)
	{
		m_options[0].setString("Start game");
		m_options[1].setString("Credits");
		m_options[2].setString("Exit");

		for (int i = 0; i < menu_options_count; i++)
		{
			m_options[i].setFont(*FileManager::Get().GetFont());
			m_options[i].setPosition(0, i * 40 + 30);
		}
		for (int i = 0; i < menu_options_count; i++)
		{
			m_options[i].setFillColor(sf::Color::White);
		}
		m_options[cursor_pos].setFillColor(sf::Color::Yellow);

	}

	void HanldeEvents(const sf::Event event) override 
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down)
		{
			cursor_pos++;
			cursor_pos %= menu_options_count;

			for (int i = 0; i < menu_options_count; i++)
			{
				m_options[i].setFillColor(sf::Color::White);
			}
			m_options[cursor_pos].setFillColor(sf::Color::Yellow);
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
		{
			cursor_pos--;
			cursor_pos = cursor_pos >= 0 ? cursor_pos : menu_options_count - 1;
			for (int i = 0; i < menu_options_count; i++)
			{
				m_options[i].setFillColor(sf::Color::White);
			}
			m_options[cursor_pos].setFillColor(sf::Color::Yellow);
		}
		if (event.type == sf::Event::KeyPressed && (event.key.code == sf::Keyboard::Enter || event.key.code == sf::Keyboard::Space))
		{
			switch (cursor_pos)
			{
			case 0:
			{
				m_exit_prop.exit = false;
				m_exit_prop.scene = SceneLoadProp::game;
				m_scene_finished = true;
			}
			break;
			case 2:
			{
				m_exit_prop.exit = true;
				m_scene_finished = true;
			}
			break;
			default:
				break;
			}
		}

	};
	void Update(float dt) override {};
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override 
	{
		for (int i = 0; i < menu_options_count; i++)
		{
			target.draw(m_options[i]);
		}

	};
private:
	bool m_scene_finished;
	const SceneLoadProp m_start_prop;
	static const int menu_options_count = 3;
	SceneLoadProp m_exit_prop;
	sf::Text m_options[menu_options_count];
	int cursor_pos;

	bool HasFinished(SceneLoadProp* exit_prop) override
	{
		
		if (!m_scene_finished)
			return false;
		*exit_prop = m_exit_prop;
		return true;
	}
};