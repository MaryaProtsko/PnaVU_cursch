#pragma once
#include "GPLogic.hpp"

void GPLogic::UpdateGhost() 
{
	m_ghost_fig = m_fig;
	m_ghost_fig.ghost = true;
	while (TryMove(0, 1, m_ghost_fig));
}

bool GPLogic::CollionOccured(Figure& fig) const 
{
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
		{
			if (!fig.m_grid[y][x])
				continue;
			
			if (fig.m_x + x < 0)
				return true;
			if (fig.m_x + x >= m_field_size.x)
				return true;
			if (fig.m_y + y >= m_field_size.y)
				return true;
			int arr_index = m_field_size.x * (fig.m_y + y) + fig.m_x + x;
			if (arr_index < 0 || arr_index >= m_field_size.x * m_field_size.y)
				throw("");
			if (m_field[arr_index].active)
				return true;
		}
	return false;
}

bool GPLogic::TryMove(int dx, int dy, Figure& fig) const
{
	fig.m_x += dx;
	fig.m_y += dy;

	if (!CollionOccured(fig))
		return true;

	fig.m_x -= dx;
	fig.m_y -= dy;
	return false;
}

bool GPLogic::TryRotate(int dir, Figure& fig) const 
{
	fig.Rotate(dir);
	if (CollionOccured(fig))
	{
		fig.Rotate(-dir);
		return false;
	}
	return true;
}

void GPLogic::EndGame()
{
	m_state = over;
	const int death_color_id = 7;
	
	for (int y = 0; y < m_field_size.y; y++)
	{
		for (int x = 0; x < m_field_size.x; x++)
		{
			int arr_index = m_field_size.x * y + x;
			if(m_field[arr_index].active)
				m_field[arr_index].m_color_id = death_color_id;
		}
	}
	m_fig.m_color_id = death_color_id;
	FileManager::Get().GetGameOverSound()->play();
}

void GPLogic::StartAnimation()
{
	if (m_rows_cleared.empty())
		return;
	m_state = animation;
	FileManager::Get().GetMainTheme()->pause();
	FileManager::Get().GetLineClearedSound()->setPlayingOffset(sf::seconds(0));
	FileManager::Get().GetLineClearedSound()->play();
	m_animation_timer = m_animation_time;
}

void GPLogic::EndAnimation()
{
	m_state = playing;
	DropLines();
	FileManager::Get().GetMainTheme()->play();
	m_rows_cleared.clear();
}

void GPLogic::LandFigure(Figure& fig)
{
	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
		{
			if (!fig.m_grid[y][x])
				continue;
			int arr_index = m_field_size.x * (fig.m_y + y) + fig.m_x + x;
			m_field[arr_index].m_color_id = fig.m_color_id;
			m_field[arr_index].m_tex_id = fig.m_tex_id;
			m_field[arr_index].active = true;
		}
	ResetFigure();
	if (CollionOccured(fig))
	{
		EndGame();
		return;
	}

	CheckClearedLines();
	StartAnimation();
}

void GPLogic::ResetFigure()
{
	std::uniform_int_distribution<int> range(0, 6);
	if (m_next_fig_id == -1)
		m_fig.SetFigure(range(m_random_generator));
	else
		m_fig.SetFigure(m_next_fig_id);
	m_fig.m_x = m_field_size.x / 2 - 2;
	m_fig.m_y = 0;

	m_gravity_timer = m_spawn_time;

	m_next_fig_id = range(m_random_generator);
	UpdateGhost();
}

void GPLogic::CheckClearedLines()
{
	for (int y = 0; y < m_field_size.y; y++)
	{
		bool clean = true;
		for (int x = 0; x < m_field_size.x; x++)
		{
			int arr_index = m_field_size.x * y + x;
			if (!m_field[arr_index].active)
				clean = false;
		}
		if (clean)
			m_rows_cleared.push_back(y);
	}
}

void GPLogic::DropLines()
{
	if (m_rows_cleared.empty())
		return;
	int count = m_rows_cleared.size();
	m_game_scores.lines += count;
	const int scores[4] = {100, 300, 500, 800};
	m_game_scores.score += scores[count - 1] * m_game_scores.level;
	m_game_scores.level = (m_game_scores.lines / 10) + 1;

	for (int y = m_field_size.y - 1; y >= 0; y--)
	{
		int height = 0;
		for (int i = 0; i < m_rows_cleared.size(); i++)
		{
			if (m_rows_cleared[i] > y)
				height++;
			if (m_rows_cleared[i] == y)
			{
				height = -1;
				break;
			}
		}
		if (height <= 0)
			continue;
		for (int x = 0; x < m_field_size.x; x++)
		{
			int arr_index_from = m_field_size.x * (y) + x;
			int arr_index_to = m_field_size.x * (y + height) + x;
			m_field[arr_index_to] = m_field[arr_index_from];
		}
	}
}

void GPLogic::HardDrop()
{
	int h = 0;
	while (TryMove(0, 1, m_fig))
		h++;
	m_game_scores.score += 2 * h;
	LandFigure(m_fig);
}

void GPLogic::HandleControlsEvent(const sf::Event event)
{
	bool key_p = event.type == sf::Event::KeyPressed;
	bool key_r = event.type == sf::Event::KeyReleased;
	if (m_control_mode == right || m_control_mode == both)
	{
		if (key_p && event.key.code == sf::Keyboard::Numpad0)
			TryRotate(1, m_fig);
		if (key_p && event.key.code == sf::Keyboard::RControl)
			TryRotate(-1, m_fig);

		if (key_p && event.key.code == sf::Keyboard::Left)
		{
			m_move_timer = m_move_time * 0.5f;
			m_move_dir.x = -1;
		}
		if (key_r && event.key.code == sf::Keyboard::Left && m_move_dir.x == -1)
			m_move_dir.x = 0;
			
		if (key_p && event.key.code == sf::Keyboard::Right)
		{
			m_move_timer = m_move_time * 0.5f;
			m_move_dir.x = 1;
		}
		if (key_r && event.key.code == sf::Keyboard::Right && m_move_dir.x == 1)
			m_move_dir.x = 0;

		if (key_p && event.key.code == sf::Keyboard::Down)
			m_move_dir.y = 1;
		if (key_r && event.key.code == sf::Keyboard::Down)
			m_move_dir.y = 0;

		if (key_r && event.key.code == sf::Keyboard::Up)
			HardDrop();
	}
	if(m_control_mode == left || m_control_mode == both)
	{
		if (key_p && (event.key.code == sf::Keyboard::E || event.key.code == sf::Keyboard::F))
			TryRotate(1, m_fig);
		if (key_p && event.key.code == sf::Keyboard::Q)
			TryRotate(-1, m_fig);

		if (key_p && event.key.code == sf::Keyboard::A)
		{
			m_move_timer = m_move_time * 0.5f;
			m_move_dir.x = -1;
		}
		if (key_r && event.key.code == sf::Keyboard::A && m_move_dir.x == -1)
			m_move_dir.x = 0;
			
		if (key_p && event.key.code == sf::Keyboard::D)
		{
			m_move_timer = m_move_time * 0.5f;
			m_move_dir.x = 1;
		}
		
		if (key_r && event.key.code == sf::Keyboard::D && m_move_dir.x == 1)
			m_move_dir.x = 0;
		if (key_p && event.key.code == sf::Keyboard::S)
			m_move_dir.y = 1;
		if (key_r && event.key.code == sf::Keyboard::S)
			m_move_dir.y = 0;

		if (key_r && event.key.code == sf::Keyboard::W)
			HardDrop();
	}
	UpdateGhost();
}

void GPLogic::PlayingUpdate(float dt)
{
	m_fall_time = m_game_scores.level <= 16 ? (float)m_frames_per_tile[m_game_scores.level - 1] / 60.f : 2.f / 60.f;
	m_gravity_timer += dt;
	if (m_gravity_timer >= m_fall_time || (m_move_dir.y && m_gravity_timer >= m_soft_drop_time))
	{
		if (!TryMove(0, 1, m_fig))
		{
			if (m_gravity_timer >= (m_fall_time + m_coyote_time))
			{
				m_gravity_timer = 0.f;
				LandFigure(m_fig);
			}
		}
		else
		{
			m_gravity_timer = 0.f;
			if (m_move_dir.y)
				m_game_scores.score++;
		}
	}
	m_move_timer += dt;
	if (m_move_timer > m_move_time && TryMove(m_move_dir.x, 0, m_fig))
	{
		m_move_timer = 0;
		UpdateGhost();
	}
}

void GPLogic::AnimationUpdate(float dt)
{
	m_animation_timer -= dt;
	if (m_animation_timer <= 0)
		EndAnimation();

	for (const auto row : m_rows_cleared)
		for (int x = 0; x < m_field_size.x; x++)
		{
			int index = m_field_size.x * row + x;
			if ((m_animation_timer / m_animation_time) < ((float)(m_field_size.x - x + 1) / m_field_size.x))
				m_field[index].active = false;
			else if((m_animation_timer / m_animation_time) < ((float)(m_field_size.x - x + 4) / m_field_size.x))
				m_field[index].m_color_id = 8;
		}
}

void GPLogic::SetSeed(unsigned int seed)
{
	m_next_fig_id = -1;
	m_random_generator.seed(seed);
	ResetFigure();
}	

bool GPLogic::IsNewRecord(int& record_out)
{
	if (m_state != over)
		return false;
	if(!FileManager::Get().IsToAddScore(m_game_scores.score))
		return false;
	record_out = m_game_scores.score;
	return true;
}