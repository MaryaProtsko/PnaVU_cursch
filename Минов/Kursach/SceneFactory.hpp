#pragma once
#include <memory>
#include "IScene.hpp"
#include "MenuScene.hpp"
#include "GameScene.hpp"
#include "MultiplayerScene.hpp"

class SceneFactory
{
public:
	static std::unique_ptr<IScene> MakeScene(SceneLoadProp prop)
	{
		switch (prop.next_scene)
		{
		case SceneLoadProp::menu:
			return std::make_unique<MenuScene>(prop);
		case SceneLoadProp::default_singleplayer:
			return std::make_unique<GameScene>(prop);
		case SceneLoadProp::multiplayer:
			return std::make_unique<MultiplayerScene>(prop);
		default:
			break;
		}
	}
};