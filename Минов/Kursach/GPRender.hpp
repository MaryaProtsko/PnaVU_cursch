#pragma once
#include "GPFields.hpp"
#include "GPLogic.hpp"

class GPRender : public virtual GPFields
{
protected:
	void InitRender();
	void DrawField(sf::RenderTarget& target, sf::RenderStates states) const;
	void DrawFigure(sf::RenderTarget& target, sf::RenderStates states, const Figure& fig) const;
	void DrawDecorations(sf::RenderTarget& target, sf::RenderStates states) const;
};