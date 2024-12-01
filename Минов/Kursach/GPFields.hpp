#pragma once
#include "Tile.hpp"
#include "FileManager.hpp"
#include <random>
#include <chrono>
#include <sstream>

class GPFields
{
public:
	sf::Vector2f m_pos = {0, 0};
	enum GameState
	{
		playing,
		paused,
		over,
		new_record,
		animation,
	};
	virtual ~GPFields()
	{
		if (m_field)
			delete[] m_field;
	}
	GPFields() : m_control_mode(left){}
protected:
	sf::Vector2i m_field_size = { 10, 21 };
	const float m_scale = 1.f;
	const float m_wall_thickness = 1;
	const float m_soft_drop_multiplier = 0.2f;
	const float m_coyote_time = 0.14f;
	const float m_spawn_time = -0.25f;
	const float m_soft_drop_time = 0.05f;
	const float m_animation_time = 0.7f;

	float m_gravity_timer = 0.f;
	float m_animation_timer = 0.f;
	std::vector<int> m_rows_cleared;

	sf::Vector2i m_move_dir;
	Figure m_fig;
	Figure m_ghost_fig;
	Tile* m_field;
	GameState m_state;

	int m_tex_size;
	int m_next_fig_id = -1;
	struct GameScores
	{
		int score = 0;
		int lines = 0;
		int level = 1;
	}m_game_scores;
	
public:
	enum ConrolsMode
	{
		left,
		right,
		both,
	}m_control_mode;
	void SetPos(sf::Vector2f pos)
	{
		m_pos = pos;
	}
	sf::Vector2f GetPos()
	{
		return m_pos;
	}
	sf::Vector2f GetSize()
	{
		return m_tex_size * m_scale * sf::Vector2f{ (float)m_field_size.x + (int)m_wall_thickness * 2, (float)m_field_size.y};
	}
	GameState GetCurrentState()
	{
		return m_state;
	}
};