#include "SceneManager.h"
#include "HelloWorldScene.h"
#include "WorkshopScene.h"
#include "DecorationScene.h"

SceneManager::SceneManager()
{
	Init();
}

void SceneManager::Init()
{
	helloWorldScene = WorldScene::create();
	workshopScene = nullptr;
	decorationScene = nullptr;
}
