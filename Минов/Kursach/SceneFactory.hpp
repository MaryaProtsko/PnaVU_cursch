#pragma once
#include <memory>
#include "SceneLoadProp.hpp"
#include "IScene.hpp"

#include "MenuScene.hpp"
#include "GameScene.hpp"

class SceneFactory
{
public:
	static std::unique_ptr<IScene> MakeScene(SceneLoadProp prop)
	{
		switch (prop.scene)
		{
		case SceneLoadProp::menu:
			return std::make_unique<MenuScene>(prop);
		case SceneLoadProp::game:
			return std::make_unique<GameScene>(prop);
		default:
			break;
		}
	}
};