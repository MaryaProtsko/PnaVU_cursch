#include "GameScene.hpp"

GameScene::GameScene(SceneLoadProp prop) : m_start_prop(prop), m_exit_prop(prop)
{
	m_gp.m_side_space = { 200, 0 };
	m_gp.m_pos = { 60, 40 };
	m_gp.UpdateTextPos();
	m_in.m_entering = true;
	FileManager::Get().GetMainTheme()->setVolume(0.f);
	FileManager::Get().GetMainTheme()->play();
	FileManager::Get().GetMainTheme()->setVolume(1.f);
}

void GameScene::HanldeEvents(const sf::Event event)
{
	if(m_in.HanldeEvents(event, &m_gp))
		return;
	
	m_gp.HanldeEvents(event);
	if (m_gp.GetCurrentState() == GameProcess::paused)
		FileManager::Get().GetMainTheme()->setVolume(3.f);
	else
		FileManager::Get().GetMainTheme()->setVolume(10.f);
	bool key_p = event.type == sf::Event::KeyPressed;
	if (key_p && event.key.code == sf::Keyboard::Escape && 
		(m_gp.GetCurrentState() == GameProcess::over
		|| m_gp.GetCurrentState() == GameProcess::paused))
	{
		m_exit_prop.next_scene = SceneLoadProp::menu;
		m_scene_finished = true;
	}
	if (key_p && event.key.code == sf::Keyboard::R && 
		(m_gp.GetCurrentState() == GameProcess::over
		|| m_gp.GetCurrentState() == GameProcess::paused))
	{
		m_scene_finished = true;
	}
}

void GameScene::Update(const float dt)  
{
	if (m_gp.GetCurrentState() == GameProcess::over)
		FileManager::Get().GetMainTheme()->stop();
	
	m_gp.Update(dt);
	m_in.Update(&m_gp);
};

void GameScene::draw(sf::RenderTarget& target, sf::RenderStates states) const 
{	
	target.draw(m_gp);
	target.draw(m_in);
};

bool GameScene::HasFinished(SceneLoadProp* exit_prop)
{
	if (!m_scene_finished)
		return false;
	FileManager::Get().GetMainTheme()->stop();
	FileManager::Get().GetMainTheme()->setPlayingOffset(sf::Time(sf::seconds(0)));
	*exit_prop = m_exit_prop;
	return true;
}