#pragma once

#include <SFML/Graphics.hpp>
#include <string.h>

struct Tile
{
	bool active = false;
	char m_tex_id = 7;
	char m_color_id = 0;

	sf::Color GetColor() const
	{
		switch (m_color_id)
		{
		case 0:
			return sf::Color::Yellow;
		case 1:
			return sf::Color(0x66FF66FF);
		case 2:
			return sf::Color::Green;
		case 3:
			return sf::Color::Red;	
		case 4:
			return sf::Color::Blue;
		case 5:
			return sf::Color(0xFFAAAAFF);
		case 6:
			return sf::Color(0xFF00FFFF);
		default:
			return sf::Color::White;
			break;
		}
	}
};
	
class Figure : public Tile
{
	static const bool* GetFigure(char fig, char rot) 
	{
		if (rot > 3)
			throw("");
		switch (fig)
		{
		case 0://o
		{
			static const bool o_fig[4][4] =
			{{0, 0, 0, 0 },
			{ 0, 1, 1, 0 },
			{ 0, 1, 1, 0 },
			{ 0, 0, 0, 0 }};
			return &o_fig[0][0];
		}
		case 1://i
		{
			static const bool i_fig[2][4][4] =
			{{{0, 0, 0, 0 },
			{  0, 0, 0, 0 },
			{  1, 1, 1, 1 },
			{  0, 0, 0, 0 }},
			   
			{{ 0, 1, 0, 0 },
			{  0, 1, 0, 0 },
			{  0, 1, 0, 0 },
			{  0, 1, 0, 0 }}};
			return &i_fig[rot % 2][0][0];
		}
		case 2://s
		{
			static const bool s_fig[2][4][4] =
			{{{0, 0, 0, 0 },
			{  0, 1, 1, 0 },
			{  1, 1, 0, 0 },
			{  0, 0, 0, 0}},
			   
			{{ 1, 0, 0, 0 },
			{  1, 1, 0, 0 },
			{  0, 1, 0, 0 },
			{  0, 0, 0, 0 }}};
			return &s_fig[rot % 2][0][0];
		}
		case 3://z
		{
			static const bool z_fig[2][4][4] =
			{{{0, 0, 0, 0 },
			{  1, 1, 0, 0 },
			{  0, 1, 1, 0 },
			{  0, 0, 0, 0 }},
			   
			{{ 0, 0, 1, 0 },
			{  0, 1, 1, 0 },
			{  0, 1, 0, 0 },
			{  0, 0, 0, 0 }}};
			return &z_fig[rot % 2][0][0];
		}
		case 4://L
		{
			static const bool l_fig[4][4][4] =
			{{{0, 0, 0, 0 },
			{  1, 1, 1, 0 },
			{  1, 0, 0, 0 },
			{  0, 0, 0, 0 }},
			   
			{{ 1, 1, 0, 0 },
			{  0, 1, 0, 0 },
			{  0, 1, 0, 0 },
			{  0, 0, 0, 0 }},

			{{ 0, 0, 1, 0 },
			{  1, 1, 1, 0 },
			{  0, 0, 0, 0 },
			{  0, 0, 0, 0 }},

			{{ 0, 1, 0, 0 },
			{  0, 1, 0, 0 },
			{  0, 1, 1, 0 },
			{  0, 0, 0, 0 }}};
			return &l_fig[rot][0][0];
		}
		case 5://J
		{
			static const bool j_fig[4][4][4] =
			{{{0, 0, 0, 0 },
			{  1, 1, 1, 0 },
			{  0, 0, 1, 0 },
			{  0, 0, 0, 0 }},
			   
			{{ 0, 1, 0, 0 },
			{  0, 1, 0, 0 },
			{  1, 1, 0, 0 },
			{  0, 0, 0, 0 }},

			{{ 1, 0, 0, 0 },
			{  1, 1, 1, 0 },
			{  0, 0, 0, 0 },
			{  0, 0, 0, 0 }},

			{{ 0, 1, 1, 0 },
			{  0, 1, 0, 0 },
			{  0, 1, 0, 0 },
			{  0, 0, 0, 0 }}};
			return &j_fig[rot][0][0];
		}
		case 6://T
		{
			static const bool t_fig[4][4][4] =
			{{{0, 0, 0, 0 },
			{  1, 1, 1, 0 },
			{  0, 1, 0, 0 },
			{  0, 0, 0, 0 }},
			   
			{{ 0, 1, 0, 0 },
			{  1, 1, 0, 0 },
			{  0, 1, 0, 0 },
			{  0, 0, 0, 0 }},

			{{ 0, 1, 0, 0 },
			{  1, 1, 1, 0 },
			{  0, 0, 0, 0 },
			{  0, 0, 0, 0 }},

			{{ 0, 1, 0, 0 },
			{  0, 1, 1, 0 },
			{  0, 1, 0, 0 },
			{  0, 0, 0, 0 }}};
			return &t_fig[rot][0][0];
		}
		default:
			throw("");
			break;
		}
	}

	int m_rot;
public:
	bool m_grid[4][4];

	char m_fig_id;
	int m_x, m_y;

	Figure() : m_rot(0), m_fig_id(0), m_x(2), m_y(0)
	{
		active = true;
	}

	Figure(char fig) : m_rot(0)
	{
		active = true;
		SetFigure(fig);
		


	}
	void SetFigure(char fig)// 0o 1i 2s 3z 4l 5j 6t
	{
		m_rot = 0;
		m_fig_id = fig;
		memcpy(m_grid, GetFigure(m_fig_id, m_rot), sizeof(m_grid));
		switch (m_fig_id)
		{
		case 0:
			m_tex_id = 5;
			m_color_id = 0;
			break;
		case 1:
			m_tex_id = 1;
			m_color_id = 1;
			break;
		case 2:
			m_tex_id = 6;
			m_color_id = 2;
			break;
		case 3:
			m_tex_id = 6;
			m_color_id = 3;
			break;
		case 4:
			m_tex_id = 3;
			m_color_id = 4;
			break;
		case 5:
			m_tex_id = 3;
			m_color_id = 5;
			break;
		case 6:
			m_tex_id = 2;
			m_color_id = 6;
			break;
		default:
			break;
		}
	}
	void Rotate(int dir)
	{
		m_rot += dir;
		if (m_rot < 0)
			m_rot = 3;
		if(m_rot > 3)
			m_rot = 0;
		memcpy(m_grid, GetFigure(m_fig_id, m_rot), sizeof(m_grid));
	}
};