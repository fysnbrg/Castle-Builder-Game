#pragma once
#include "cocos2d.h"

//Singleton Scene manager class
class SceneManager
{
public:
	static SceneManager& getInstance() {
		static SceneManager instance;
		return instance;
	}

	cocos2d::Scene* helloWorldScene;
	cocos2d::Scene* workshopScene;
	cocos2d::Scene* decorationScene;

	SceneManager();
	void Init();
};

