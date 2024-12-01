#pragma once

#include <fstream>
#include <stdexcept>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <algorithm>  

class ResourceLoadException : public std::runtime_error
{
public:
    ResourceLoadException(const std::string& message)
        : std::runtime_error(message) {}
};
class FileSaveException : public std::runtime_error
{
public:
    FileSaveException(const std::string& message)
        : std::runtime_error(message) {}
};

class FileManager
{
private:
    sf::Texture m_tile;
    sf::Texture m_logo;
    sf::Shader m_shader;
    sf::Font m_font;
    sf::Music m_main_theme;
    sf::SoundBuffer m_line_cleared_buff;
    sf::Sound m_line_cleared;
    sf::SoundBuffer m_game_over_buff;
    sf::Sound m_game_over;

    const std::string m_texture_path = "resources/TileMap1Res2.png";
    const std::string m_logo_path = "resources/logo.png";
    const std::string m_font_path = "resources/JockeyOne-Regular.ttf";
    const std::string m_leader_list_path = "leaders.txt";
    const std::string m_music_path = "resources/Tetris.ogg";
    const std::string m_line_cleared_sound_path = "resources/line_cleared.ogg";
    const std::string m_game_over_path = "resources/game_over.ogg";
    const std::string m_shader_path = "resources/shader.frag";
    const std::string m_error_log_path = "log.txt";

    FileManager();
    void CleanLog();

    template <typename Resource, typename... Args>
    bool TryLoadResource(Resource& resource, const std::string& path, Args... args);

    struct Result
    {
        std::string name;
        int score = 0;
    };

public:
    std::vector<Result> m_leader_board;
    const float m_def_win_w = 1300;
    const float m_def_win_h = 800;

    void LoadLeaderBoard();
    bool IsToAddScore(int score);
    void PushScore(int score, const std::string& name);
    void PushErrorLog(const std::string& log);
    static FileManager& Get()
    {
        static FileManager fm;
        return fm;
    }
    sf::Texture* GetMainTexture()
    {
        return &m_tile;
    }
    sf::Texture* GetLogoTexture()
    {
        return &m_logo;
    }
    sf::Shader* GetShader()
    {
        return &m_shader;
    }
    sf::Font* GetFont()
    {
        return &m_font;
    }
    sf::Music* GetMainTheme()
    {
        return &m_main_theme;
    }
    sf::Sound* GetLineClearedSound()
    {
        return &m_line_cleared;
    }
    sf::Sound* GetGameOverSound()
    {
        return &m_game_over;
    }
};