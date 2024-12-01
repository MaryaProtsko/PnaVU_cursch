#pragma once
#include "IScene.hpp"
#include "Additional.hpp"

class MenuScene : public IScene
{
private:
	bool m_scene_finished;
	int cursor_pos;
	static const int menu_options_count = 4;
	const SceneLoadProp m_start_prop;
	SceneLoadProp m_exit_prop;
	Leaderboard m_lb;
	sf::Text m_options[menu_options_count];
	sf::Sprite m_logo;

	void UpdateOptionsStyle();
	bool HasFinished(SceneLoadProp* exit_prop) override;
public:
	MenuScene(SceneLoadProp prop);
	void HanldeEvents(const sf::Event event) override;
	void Update(float dt) override {};
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};