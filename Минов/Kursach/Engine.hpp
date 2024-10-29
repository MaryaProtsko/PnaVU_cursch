#pragma once
#include "SceneFactory.hpp"

class Engine
{
public:
	Engine() : 
		m_window(sf::VideoMode(FileManager::Get().def_win_w, FileManager::Get().def_win_h), "Tetris")
	{
		m_window.setKeyRepeatEnabled(false);
	}
	void Run()
	{
		m_current_scene = SceneFactory::MakeScene(SceneLoadProp{});
		MainLoop();
	}
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	std::unique_ptr<IScene> m_current_scene;

	void MainLoop()
	{
		float dt = m_clock.restart().asSeconds();
	
		SceneLoadProp prop = SceneLoadProp{};
		sf::Event event;
		while (!prop.exit && m_window.isOpen())
		{
			while (m_window.pollEvent(event))
			{
				HandleEngineEvents(event);
				m_current_scene->HanldeEvents(event);
			}
			m_current_scene->Update(dt);

			m_window.clear();
			m_window.draw(*m_current_scene);
			m_window.display();
			
			if (m_current_scene->HasFinished(&prop) && !prop.exit)
				m_current_scene = SceneFactory::MakeScene(prop);
			dt = m_clock.restart().asSeconds();
		}
	}
	void HandleEngineEvents(sf::Event event)
	{
		if (event.type == sf::Event::Closed)
		{
		    m_window.close();
		}
		if (event.type == sf::Event::Resized)
		{
			sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
			m_window.setView(sf::View(visibleArea));
		}
	}
};