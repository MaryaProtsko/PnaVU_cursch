#include "GPRender.hpp"

void GPRender::InitRender()
{
	sf::Texture* tex = FileManager::Get().GetMainTexture();
	m_tex_size = tex->getSize().y / 2.f;
}

void GPRender::DrawField(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Texture* tex = FileManager::Get().GetMainTexture();
	
	sf::RectangleShape rect(sf::Vector2f{ (float)m_tex_size * m_scale, (float)m_tex_size * m_scale});
	rect.setTexture(tex);

	for (int y = 0; y < m_field_size.y; y++)
		for(int x = 0; x < m_field_size.x; x++)
		{
			int arr_index = m_field_size.x * y + x;
			if (!m_field[arr_index].active)
				continue;
			int tx = (int)m_tex_size * (m_field[arr_index].m_tex_id % 4);
			int ty = (int)m_tex_size * (m_field[arr_index].m_tex_id / 4);
			rect.setTextureRect({ tx, ty, m_tex_size, m_tex_size});
			sf::Glsl::Vec4 color = m_field[arr_index].GetColor();
			(*FileManager::Get().GetShader()).setUniform("color", color);
			rect.setPosition(m_pos + sf::Vector2f{ (x + m_wall_thickness) * m_tex_size * m_scale, y * m_tex_size * m_scale});
			target.draw(rect, FileManager::Get().GetShader());
		}
}

void GPRender::DrawFigure(sf::RenderTarget& target, sf::RenderStates states, const Figure& fig) const
{
	if (!fig.active)
		return;
	sf::Texture* tex = FileManager::Get().GetMainTexture();
	sf::RectangleShape rect(sf::Vector2f{ (float)m_tex_size * m_scale, (float)m_tex_size * m_scale});
	rect.setTexture(tex);

	int tx = (int)m_tex_size * (fig.m_tex_id % 4);
	int ty = (int)m_tex_size * (fig.m_tex_id / 4);
	rect.setTextureRect({ tx, ty, m_tex_size, m_tex_size});

	FileManager::Get().GetShader()->setUniform("color", (sf::Glsl::Vec4)fig.GetColor());

	for (int y = 0; y < 4; y++)
		for (int x = 0; x < 4; x++)
		{
			if (!fig.m_grid[y][x])
				continue;
			rect.setPosition(m_pos + sf::Vector2f{ 
				(x + m_wall_thickness + fig.m_x) * m_tex_size * m_scale, 
				(y + fig.m_y) * m_tex_size * m_scale});
			target.draw(rect, FileManager::Get().GetShader());
		}
}

void GPRender::DrawDecorations(sf::RenderTarget& target, sf::RenderStates states) const
{
	sf::Texture* tex = FileManager::Get().GetMainTexture();

	sf::RectangleShape rect(sf::Vector2f{ (float)m_tex_size * m_scale, (float)m_tex_size * m_scale});
	rect.setTexture(tex);

	FileManager::Get().GetShader()->setUniform("color", sf::Glsl::Vec4{sf::Color::White});
	rect.setTextureRect({ m_tex_size, m_tex_size, m_tex_size, m_tex_size });
	for (int y = 0; y < m_field_size.y; y++)
		for(int x = 0; x < m_wall_thickness; x++)
		{
			rect.setPosition(m_pos + sf::Vector2f{ x * m_tex_size * m_scale, y * m_tex_size * m_scale});
			target.draw(rect, FileManager::Get().GetShader());
		}
	for (int y = 0; y < m_field_size.y; y++)
		for(int x = 0; x < m_wall_thickness; x++)
		{
			rect.setPosition(m_pos + sf::Vector2f{ (x + m_wall_thickness + m_field_size.x) * m_tex_size * m_scale, y * m_tex_size * m_scale});
			target.draw(rect, FileManager::Get().GetShader());
		}
}