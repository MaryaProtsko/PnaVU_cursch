#pragma once
#include "IScene.hpp"
#include "GameProcess.hpp"
#include "Additional.hpp"

class GameScene : public IScene
{
private:
	const SceneLoadProp m_start_prop;
	SceneLoadProp m_exit_prop;
	GameProcess m_gp;
	InputField m_in;
	bool m_scene_finished;
	bool HasFinished(SceneLoadProp* exit_prop) override;
public:
	GameScene(SceneLoadProp prop);
	void HanldeEvents(const sf::Event event) override;
	void Update(const float dt) override;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
};