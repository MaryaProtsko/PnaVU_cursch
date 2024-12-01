#include "MultiplayerScene.hpp"

MultiplayerScene::MultiplayerScene(SceneLoadProp prop) : m_start_prop(prop), m_exit_prop(prop), m_gp1(GameProcess::left), m_gp2(GameProcess::right)
{
	unsigned int seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	m_gp1.m_side_space = { 0, 0 };
	m_gp1.m_pos = { 5, 20 };
	m_gp1.UpdateTextPos();
	m_gp1.SetSeed(seed);
	m_gp2.m_side_space = { 0, 0 };
	m_gp2.m_pos = { 640, 20 };
	m_gp2.UpdateTextPos();
	m_gp2.SetSeed(seed);

	FileManager::Get().GetMainTheme()->setVolume(0.f);
	FileManager::Get().GetMainTheme()->play();
	FileManager::Get().GetMainTheme()->setVolume(20.f);
}

void MultiplayerScene::HanldeEvents(const sf::Event event) 
{
	for (GameProcess* gp : std::vector<GameProcess*>{ &m_gp1, &m_gp2 })
	{
		gp->HanldeEvents(event);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape && 
			(gp->GetCurrentState() == GameProcess::over
			|| gp->GetCurrentState() == GameProcess::paused))
		{
			m_exit_prop.next_scene = SceneLoadProp::menu;
			m_scene_finished = true;
		}
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R && 
			(gp->GetCurrentState() == GameProcess::over
			|| gp->GetCurrentState() == GameProcess::paused))
		{
			m_scene_finished = true;
		}
	}
}

void MultiplayerScene::Update(const float dt) 
{
	m_gp1.Update(dt);
	m_gp2.Update(dt);

	if (m_gp1.GetCurrentState() == GameProcess::over && m_gp2.GetCurrentState() == GameProcess::over)
	{
		FileManager::Get().GetMainTheme()->stop();
		FileManager::Get().GetMainTheme()->setPlayingOffset(sf::Time(sf::seconds(0)));
	}
};

void MultiplayerScene::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{	
	target.draw(m_gp1);
	target.draw(m_gp2);
};

bool MultiplayerScene::HasFinished(SceneLoadProp* exit_prop)
{
	if (!m_scene_finished)
		return false;

	*exit_prop = m_exit_prop;
	return true;
}