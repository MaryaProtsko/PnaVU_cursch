#include "Additional.hpp"

Leaderboard::Leaderboard()
{
	m_text.setFont(*FileManager::Get().GetFont());
	m_text.setScale(0.8f, 0.8f);
	std::string list;
	for (int i = 0; i < 10; i++)
	{
		if (i >= FileManager::Get().m_leader_board.size())
			break;
		list += (std::to_string(i + 1) + ") " + FileManager::Get().m_leader_board[i].name + "        "
			+ std::to_string(FileManager::Get().m_leader_board[i].score)) + "\n";
	}
	m_text.setString(list);
};

InputField::InputField()
{
	m_text.setFont(*FileManager::Get().GetFont());
	m_pretext.setFont(*FileManager::Get().GetFont());
	m_pretext.setString("Enter your name : ");
	m_text.setPosition(m_pretext.getPosition() + sf::Vector2f{ m_pretext.getGlobalBounds().width + 2, 0 });
}

void InputField::Update(GameProcess* gp)
{
	int res = 0;
	if (m_entering && gp->IsNewRecord(res))
		m_visible = true;
}

bool InputField::HanldeEvents(const sf::Event event, GameProcess* gp)
{
	int res = 0;
	if (m_visible && m_entering)
	{
		HanldeKeyEvents(event);
		if (!m_entering)
		{
			gp->IsNewRecord(res);
			try
			{
				FileManager::Get().PushScore(res, m_str);
			}
			catch (FileSaveException& e)
			{
				FileManager::Get().PushErrorLog(e.what());
				throw(e);
			}
		}
		return true;
	}
	return false;
}

void InputField::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!m_visible)
		return;
	target.draw(m_pretext, states);
	target.draw(m_text, states);
}

bool InputField::CheckChar(char simbol)
{
	if (simbol >= 'a' && simbol <= 'z')
		return true;
	if (simbol >= 'A' && simbol <= 'Z')
		return true;
	if (simbol >= '0' && simbol <= '9')
		return true;
	if (simbol == '_')
		return true;
	return false;
}

void InputField::HanldeKeyEvents(const sf::Event event)
{
	if (event.type == sf::Event::TextEntered)
	{
		if (event.text.unicode < 128 && m_str.size() <= 32)
		{
			char sim = static_cast<char>(event.text.unicode);
			if (CheckChar(sim))
				m_str += sim;
		}
		else if (m_str.size() <= 32)
			m_str += '?';
	}
	bool key_p = event.type == sf::Event::KeyPressed;
	if (key_p && event.key.code == sf::Keyboard::Space)
		m_str += '_';
	if (key_p && event.key.code == sf::Keyboard::BackSpace && !m_str.empty())
		m_str = m_str.erase(m_str.size() - 1);
	if (key_p && event.key.code == sf::Keyboard::Enter)
	{
		m_entering = false;
		m_text.setFillColor(sf::Color::Green);
	}
	m_text.setString(m_str);
}