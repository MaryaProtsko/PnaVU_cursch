#pragma once

struct SceneLoadProp
{
	enum Scene
	{
		menu,
		game,
	};

	Scene scene;
	bool exit;
};