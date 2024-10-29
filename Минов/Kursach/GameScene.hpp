#pragma once
#include "Iscene.hpp"
#include "GameProcess.hpp"

class GameScene : public IScene
{
public:
	GameScene(SceneLoadProp prop) : m_start_prop(prop), m_exit_prop(prop), m_paused(false)
	{
		m_texts.m_main_title.setFont(*FileManager::Get().GetFont());
		m_texts.m_guide.setFont(*FileManager::Get().GetFont());
	}
	void HanldeEvents(const sf::Event event) override 
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Num1 && (m_paused || m_gp.IsGameOver()))
		{
			m_exit_prop.scene = SceneLoadProp::menu;
			m_scene_finished = true;
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && !m_gp.IsGameOver())
		{
			m_paused = !m_paused;
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R && m_gp.IsGameOver())
		{
			m_scene_finished = true;
		}
		if (m_paused || m_gp.IsGameOver())
			return;
		m_gp.HanldeEvents(event);
	}
	void Update(const float dt) override 
	{
		if (m_paused || m_gp.IsGameOver())
			return;

		m_gp.Update(dt);
	};
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override 
	{		
		target.draw(m_gp);
	};
private:
	GameProcess m_gp;
	struct
	{
		sf::Text m_main_title;
		sf::Text m_guide;
	}m_texts;
	bool m_paused;

	bool HasFinished(SceneLoadProp* exit_prop) override
	{
		if (!m_scene_finished)
			return false;

		*exit_prop = m_exit_prop;
		return true;
	}
	bool m_scene_finished;
	const SceneLoadProp m_start_prop;
	SceneLoadProp m_exit_prop;
};