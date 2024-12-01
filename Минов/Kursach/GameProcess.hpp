#pragma once
#include "GPLogic.hpp"
#include "GPUI.hpp"

class GameProcess : public sf::Drawable, virtual public GPLogic, virtual public GPUI
{
public:
	GameProcess(enum ConrolsMode cm = both, int x = 10, int y = 22);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	void Update(const float dt);
	void HanldeEvents(const sf::Event event);
};