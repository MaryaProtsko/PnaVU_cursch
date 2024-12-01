#pragma once
#include "Iscene.hpp"
#include "GameProcess.hpp"

class MultiplayerScene : public IScene
{
private:
	GameProcess m_gp1, m_gp2;
	bool m_scene_finished = false;
	const SceneLoadProp m_start_prop;
	SceneLoadProp m_exit_prop;
	bool HasFinished(SceneLoadProp* exit_prop) override;
public:
	MultiplayerScene(SceneLoadProp prop);
	void HanldeEvents(const sf::Event event) override;
	void Update(const float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};