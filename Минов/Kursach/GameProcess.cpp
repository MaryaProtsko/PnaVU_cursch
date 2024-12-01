#include "GameProcess.hpp"

GameProcess::GameProcess(enum ConrolsMode cm, int x, int y)
{
	m_control_mode = cm;

	m_field_size = { x, y };
	m_field = new Tile[m_field_size.x * m_field_size.y];

	InitRender();

	m_random_generator.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
	m_state = playing;
	UpdateTextPos();
	ResetFigure();
}

void GameProcess::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	DrawDecorations(target, states);
	DrawField(target, states);
	DrawFigure(target, states, m_ghost_fig);
	DrawFigure(target, states, m_fig);
	DrawSide(target, states);
}

void GameProcess::Update(const float dt)
{
	if(m_state == playing)
		PlayingUpdate(dt);
	if (m_state == animation)
		AnimationUpdate(dt);
	UpdateInfo();
}

void GameProcess::HanldeEvents(const sf::Event event) 
{
	if (m_state == playing)
		HandleControlsEvent(event);
	if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		m_state = m_state == playing ? paused : (m_state == paused ? playing : m_state);
}