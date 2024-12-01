#include "Tile.hpp"

sf::Color Tile::GetColor() const
{
	if (ghost)
		return sf::Color(0x33333399);
	switch (m_color_id)// 0o 1i 2s 3z 4l 5j 6t
	{
	case 0:
		return sf::Color(0xC76D60FF);
	case 1:
		return sf::Color(0xCDDBBFFF);
	case 2:
		return sf::Color(0xB9C58FFF);
	case 3:
		return sf::Color(0x69BAF8FFF);	
	case 4:
		return sf::Color(0xDCDCFFFF);
	case 5:
		return sf::Color(0xFFC0CBFF);
	case 6:
		return sf::Color(0xBDCABFFF);
	case 7:
		return sf::Color(0x999999FF);
	case 8:
		return sf::Color::White;
	default:
		return sf::Color::White;
		break;
	}
}

const bool* Figure::GetFigureGrid(char fig, char rot) 
{
	if (rot > 3)
		throw std::runtime_error("Invalid figure rotation");
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
		throw std::runtime_error("Invalid figure id");
		break;
	}
}

void Figure::SetFigure(char fig)// 0o 1i 2s 3z 4l 5j 6t
{
	m_rot = 0;
	m_fig_id = fig;
	memcpy(m_grid, GetFigureGrid(m_fig_id, m_rot), sizeof(m_grid));
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

void Figure::Rotate(int dir)
{
	m_rot += dir;
	if (m_rot < 0)
		m_rot = 3;
	if(m_rot > 3)
		m_rot = 0;
	memcpy(m_grid, GetFigureGrid(m_fig_id, m_rot), sizeof(m_grid));
}