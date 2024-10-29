#pragma once

#include <fstream>
#include <SFML/Graphics.hpp>


class FileManager
{
private:
	sf::Texture m_tile;
	sf::Shader m_shader;
	sf::Font m_font;
	std::string m_texture_path = "resources/TileMap1Res2.png";
	std::string m_font_path = "resources/JockeyOne-Regular.ttf";
	
	const std::string c_error_log_path = "log.txt";	
	bool m_load_exeption_occured;

	FileManager() : m_load_exeption_occured(0)
	{
		if (!m_shader.loadFromFile("resources/shader.frag", sf::Shader::Fragment))
			HandleLoadError("resources/shader.frag");
		
		if (!m_tile.loadFromFile(m_texture_path))
			HandleLoadError(m_texture_path);
		if(!m_font.loadFromFile(m_font_path))
			HandleLoadError(m_font_path);

		PushErrorLog();

		m_shader.setUniform("texture", sf::Shader::CurrentTexture);
	}

	void HandleLoadError(const std::string& path)
	{
		std::ofstream out(c_error_log_path, std::ios::app);
		out << "Cannot load file : \"" << path << "\"\n";
		out.close();
		m_load_exeption_occured = 1;
	}
	void PushErrorLog()
	{
		if (!m_load_exeption_occured)
			return;
		std::ofstream out(c_error_log_path, std::ios::app);
		out << "_______________________________________________\n\n";
		out.close();
		throw std::exception("");
	}

public:
	const float def_win_w = 1280;
	const float def_win_h = 720;

	static FileManager& Get()
	{
		static FileManager fm;
		return fm;
	}
	
	sf::Texture* GetTexture()
	{
		return &m_tile;
	}
	sf::Shader* GetShader()
	{
		return &m_shader;
	}
	sf::Font* GetFont()
	{
		return &m_font;
	}
};