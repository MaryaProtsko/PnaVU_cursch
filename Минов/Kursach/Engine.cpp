#include "Engine.hpp"

Engine::Engine() : 
	m_window(sf::VideoMode(FileManager::Get().m_def_win_w, FileManager::Get().m_def_win_h), "Tetris")
{
	m_window.setKeyRepeatEnabled(false);
}

void Engine::Run()
{
	m_current_scene = SceneFactory::MakeScene(SceneLoadProp{SceneLoadProp::menu});
	MainLoop();
}

void Engine::MainLoop()
{
	float dt = m_clock.restart().asSeconds();

	SceneLoadProp s_p{SceneLoadProp::menu};
	sf::Event event;
	while (s_p.next_scene != s_p.finished && m_window.isOpen())
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
		if (m_current_scene->HasFinished(&s_p) && s_p.next_scene != s_p.finished)
			m_current_scene = SceneFactory::MakeScene(s_p);
		dt = m_clock.restart().asSeconds();
	}
}

void Engine::HandleEngineEvents(sf::Event event)
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