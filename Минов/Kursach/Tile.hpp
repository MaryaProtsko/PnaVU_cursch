#pragma once

#include <SFML/Graphics.hpp>
#include <string.h>

struct Tile
{
	bool active = false;
	bool ghost = false;
	char m_tex_id = 7;
	char m_color_id = 0;
	sf::Color GetColor() const;
};
	
class Figure : public Tile
{
	static const bool* GetFigureGrid(char fig, char rot);

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
	void SetFigure(char fig);
	void Rotate(int dir);
};