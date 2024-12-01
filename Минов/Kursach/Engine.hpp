#pragma once
#include "SceneFactory.hpp"
#include "SceneLoadProp.hpp"

class Engine
{
public:
	Engine();
	void Run();
private:
	sf::RenderWindow m_window;
	sf::Clock m_clock;
	std::unique_ptr<IScene> m_current_scene;

	void MainLoop();
	void HandleEngineEvents(sf::Event event);
};