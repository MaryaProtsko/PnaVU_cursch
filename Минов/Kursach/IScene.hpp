#pragma once
#include <SFML/Graphics.hpp>
#include "SceneLoadProp.hpp"

class IScene : public sf::Drawable
{
public:
	virtual void HanldeEvents(const sf::Event event) = 0;
	virtual void Update(const float dt) = 0;
	virtual bool HasFinished(SceneLoadProp* exit_prop) = 0;
};