#pragma once

struct SceneLoadProp
{
	enum Scene
	{
		finished,
		menu,
		default_singleplayer,
		multiplayer,
	}next_scene;
};