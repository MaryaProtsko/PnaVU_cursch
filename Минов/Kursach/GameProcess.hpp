#pragma once
#include "Tile.hpp"
#include "FileManager.hpp"
#include <random>
#include <chrono>


class GPFields
{
protected:
	GPFields() : m_scale(1.f), m_game_over(false) {};
	const sf::Vector2i m_field_size = { 10, 20 };
	const float m_scale = 1.f;
	const float m_wall_thickness = 1;
	const int m_top_paddind = 2;
	const float m_soft_drop_multiplier = 0.2f;
	const float m_coyote_time = 0.14f;

	const float m_soft_drop_time = 0.05f;

	float m_gravity_timer = 0.f;
	const float m_spawn_time = -0.15f;
	bool m_game_over;

	std::vector<int> m_rows_cleared;


	Figure m_fig;
	Tile* m_field;
	sf::Vector2f m_pos = {20, 20};

	int m_lines_cleared = 0;

public:
	bool IsGameOver()
	{
		return m_game_over;
	}

};
class GPLogic : public virtual GPFields
{
protected:
	std::mt19937 m_random_generator;

	bool CollionOccured()
	{
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
			{
				if (!m_fig.m_grid[y][x])
					continue;
				
				if (m_fig.m_x + x < 0)
					return true;
				if (m_fig.m_x + x >= m_field_size.x)
					return true;
				if (m_fig.m_y + y >= m_field_size.y + m_top_paddind)
					return true;
				int arr_index = m_field_size.x * (m_fig.m_y + y - m_top_paddind) + m_fig.m_x + x;
				if (arr_index < 0 || arr_index >= m_field_size.x * m_field_size.y)
					throw("");
				if (m_field[arr_index].active)
					return true;
			}
		return false;
	}
	bool TryMove(int dx, int dy)
	{
		m_fig.m_x += dx;
		m_fig.m_y += dy;

		if (!CollionOccured())
			return true;

		m_fig.m_x -= dx;
		m_fig.m_y -= dy;
		return false;
	}
	bool TryRotate(int dir)
	{
		m_fig.Rotate(dir);
		if (CollionOccured())
		{
			m_fig.Rotate(-dir);
			return false;
		}
		return true;
	}
	void LandFigure()
	{
		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
			{
				if (!m_fig.m_grid[y][x])
					continue;
				int arr_index = m_field_size.x * (m_fig.m_y + y - m_top_paddind) + m_fig.m_x + x;
				if (m_field[arr_index].active)
					throw("");
				m_field[arr_index].m_color_id = m_fig.m_color_id;
				m_field[arr_index].m_tex_id = m_fig.m_tex_id;
				m_field[arr_index].active = true;
			}
		ResetFigure();
		if (CollionOccured())
			m_game_over = true;

		CheckClearedLines();
		DropLines();

	}
	void ResetFigure()
	{
		std::uniform_int_distribution<int> range(0, 6);
		m_fig.SetFigure(range(m_random_generator));
		m_fig.m_x = m_field_size.x / 2 - 2;
		m_fig.m_y = m_top_paddind;

		m_gravity_timer = m_spawn_time;
	}
	void CheckClearedLines()
	{
		m_rows_cleared.clear();
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
	void DropLines()
	{
		if (m_rows_cleared.empty())
			return;
		m_lines_cleared += m_rows_cleared.size();

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

				if (arr_index_to > m_field_size.x * m_field_size.y)
					throw("");

				m_field[arr_index_to] = m_field[arr_index_from];
				m_field[arr_index_from].active = false;
			}
		}
	}
	void HardDrop()
	{
		while (TryMove(0, 1));
		LandFigure();
	}
};
class GPRender : public virtual GPFields
{
protected:
	void DrawField(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::Texture* tex = FileManager::Get().GetTexture();
		int size = tex->getSize().y / 2.f;
		sf::RectangleShape rect(sf::Vector2f{ (float)size * m_scale, (float)size * m_scale});
		rect.setTexture(tex);

		for (int y = m_top_paddind; y < m_field_size.y + m_top_paddind; y++)
			for(int x = 0; x < m_field_size.x; x++)
			{
				int arr_index = m_field_size.x * (y - m_top_paddind) + x;
				if (!m_field[arr_index].active)
					continue;
		
				int tx = (int)size * (m_field[arr_index].m_tex_id % 4);
				int ty = (int)size * (m_field[arr_index].m_tex_id / 4);
				rect.setTextureRect({ tx, ty, size, size});
				sf::Glsl::Vec4 color = m_field[arr_index].GetColor();
				(*FileManager::Get().GetShader()).setUniform("color", color);
				rect.setPosition(m_pos + sf::Vector2f{ (x + m_wall_thickness) * size * m_scale, y * size * m_scale});
				target.draw(rect, FileManager::Get().GetShader());
			}
	}
	void DrawFigure(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::Texture* tex = FileManager::Get().GetTexture();
		int size = tex->getSize().y / 2.f;

		sf::RectangleShape rect(sf::Vector2f{ (float)size * m_scale, (float)size * m_scale});
		rect.setTexture(tex);

		int tx = (int)size * (m_fig.m_tex_id % 4);
		int ty = (int)size * (m_fig.m_tex_id / 4);
		rect.setTextureRect({ tx, ty, size, size});
		(*FileManager::Get().GetShader()).setUniform("color", (sf::Glsl::Vec4)m_fig.GetColor());

		for (int y = 0; y < 4; y++)
			for (int x = 0; x < 4; x++)
			{
				if (!m_fig.m_grid[y][x])
					continue;
				rect.setPosition(m_pos + sf::Vector2f{ 
					(x + m_wall_thickness + m_fig.m_x) * size * m_scale, 
					(y + m_fig.m_y) * size * m_scale});
				target.draw(rect, FileManager::Get().GetShader());
			}
	}
	void DrawDecorations(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::Texture* tex = FileManager::Get().GetTexture();
		int size = tex->getSize().y / 2.f;

		sf::RectangleShape rect(sf::Vector2f{ (float)size * m_scale, (float)size * m_scale});
		rect.setTexture(tex);

		(*FileManager::Get().GetShader()).setUniform("color", sf::Glsl::Vec4{sf::Color::White});
		rect.setTextureRect({ size, size, size, size });
		for (int y = m_top_paddind; y < m_field_size.y + m_top_paddind; y++)
			for(int x = 0; x < m_wall_thickness; x++)
			{
				rect.setPosition(m_pos + sf::Vector2f{ x * size * m_scale, y * size * m_scale});
				target.draw(rect, FileManager::Get().GetShader());
			}
		for (int y = m_top_paddind; y < m_field_size.y + m_top_paddind; y++)
			for(int x = 0; x < m_wall_thickness; x++)
			{
				rect.setPosition(m_pos + sf::Vector2f{ (x + m_wall_thickness + m_field_size.x) * size * m_scale, y * size * m_scale});
				target.draw(rect, FileManager::Get().GetShader());
			}
	}
};


class GameProcess : public sf::Drawable, public GPLogic, public GPRender
{

	float m_fall_time = .5f;
	float m_move_timer = 0.f;
	float m_move_time = 0.1f;
public:
	
	GameProcess() 
	{
		m_field = new Tile[m_field_size.x * m_field_size.y];
		m_fig.m_tex_id = 4;
		m_fig.m_color_id = 1;
		std::random_device device;
		m_random_generator.seed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
		ResetFigure();
	}
	void SetPos(sf::Vector2f pos)
	{
		m_pos = pos;
	}
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		DrawDecorations(target, states);
		DrawField(target, states);
		DrawFigure(target, states);
	}
	void Update(const float dt)
	{
		//float gravity_multiplier = 1.f;

		bool soft = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);

			//gravity_multiplier *= m_soft_drop_multiplier;

		if(!m_game_over)
		{
			m_gravity_timer += dt;
			if (m_gravity_timer >= m_fall_time || (soft && m_gravity_timer >= m_soft_drop_time))
			{
				if (!TryMove(0, 1))
				{
					if (m_gravity_timer >= (m_fall_time + m_coyote_time))
					{
						m_gravity_timer = 0.f;
						LandFigure();
					}
				}
				else
					m_gravity_timer = 0.f;
			}
			m_move_timer += dt;
			if (m_move_timer > m_move_time)
			{
				if (TryMove(m_dir.x, 0))
					m_move_timer = 0;
			}
		}
	}

	sf::Vector2i m_dir;

	void HanldeEvents(const sf::Event event) 
	{
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Numpad0)
			TryRotate(1);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::RControl)
			TryRotate(-1);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
			m_fig.SetFigure((m_fig.m_fig_id + 1) % 7);

		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Left)
			m_dir.x = -1;
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Left && m_dir.x == -1)
			m_dir.x = 0;
			//TryMove(-1, 0);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Right)
			m_dir.x = 1;
		if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Right && m_dir.x == 1)
			m_dir.x = 0;
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::S)
			TryMove(0, 1);
		if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up)
			HardDrop();


	}
	~GameProcess()
	{
		if (m_field)
			delete[] m_field;
	}
};