#include "FileManager.hpp"

FileManager::FileManager()
{
    CleanLog();
    int errors = 0;
    errors += !TryLoadResource(m_tile, m_texture_path);
    errors += !TryLoadResource(m_logo, m_logo_path);
    errors += !TryLoadResource(m_shader, m_shader_path, sf::Shader::Fragment);
    errors += !TryLoadResource(m_font, m_font_path);
    errors += !TryLoadResource(m_main_theme, m_music_path);
    errors += !TryLoadResource(m_line_cleared_buff, m_line_cleared_sound_path);
    errors += !TryLoadResource(m_game_over_buff, m_game_over_path);

    if (errors)
        throw ResourceLoadException("Error loading " + std::to_string(errors) + " resources");

    m_line_cleared.setBuffer(m_line_cleared_buff);
    m_line_cleared.setVolume(50.f);
    m_game_over.setBuffer(m_game_over_buff);
    m_game_over.setVolume(50.f);

    m_shader.setUniform("texture", sf::Shader::CurrentTexture);
    try
    {
        LoadLeaderBoard();
    }
    catch (ResourceLoadException& e)
    {
        PushErrorLog(e.what());
        throw e;
    }
    m_main_theme.setLoop(true);
}

void FileManager::CleanLog()
{
    std::ofstream out(m_error_log_path);
    out.close();
}

void FileManager::LoadLeaderBoard()
{
    std::ifstream in(m_leader_list_path);
    if (!in.is_open())
        throw ResourceLoadException("Failed to open leaderboard file: " + m_leader_list_path);

    for (int i = 0; i < 10; i++)
    {
        Result res;
        in >> res.name >> res.score;
        if (in.eof())
            break;
        if (in.bad())
            throw std::runtime_error("Error reading leaderboard file: " + m_leader_list_path);
        m_leader_board.push_back(res);
    }
    in.close();
}

bool FileManager::IsToAddScore(int score)
{
    if (m_leader_board.size() < 10)
        return true;
    for (const auto& res : m_leader_board)
        if (res.score < score)
            return true;
    return false;
}

void FileManager::PushScore(int score, const std::string& name)
{
    auto cmpr = [](const Result& a, const Result& b) { return a.score > b.score; };
    Result res{name, score};
    m_leader_board.push_back(res);
    std::sort(m_leader_board.begin(), m_leader_board.end(), cmpr);

    std::ofstream out(m_leader_list_path);
    if (!out.is_open())
        throw FileSaveException("Failed to write leaderboard file: " + m_leader_list_path);

    for (int i = 0; i < std::min(10, static_cast<int>(m_leader_board.size())); ++i)
        out << m_leader_board[i].name << " " << m_leader_board[i].score << "\n";
    out.close();
}

void FileManager::PushErrorLog(const std::string& log)
{
    std::ofstream out(m_error_log_path, std::ios::app);
    out << log << "\n";
    out.close();
}

template <typename Resource, typename... Args>
bool FileManager::TryLoadResource(Resource& resource, const std::string& path, Args... args)
{
    try
    {
        bool success = false;
        if constexpr (std::is_same_v<Resource, sf::Music>)
            success = resource.openFromFile(path);
        else
            success = resource.loadFromFile(path, args...);
        if (!success)
            throw ResourceLoadException("Failed to load resource: " + path);
    }
    catch (const std::exception& e)
    {
        PushErrorLog(e.what());
        return false;
    }
    return true;
}