/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "DecorationScene.h"
#include "XMLParser.h"
#include "cocos2d.h"
#include "DataManager.h"
#include "WorkshopScene.h"
#include "SceneManager.h"

USING_NS_CC;

Scene* DecorationScene::createScene()
{
    return DecorationScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool DecorationScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    ParseData();
    DisplayMenu();

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    auto bb = Sprite::create("button.png");
    bb->setPosition(Point(visibleSize.width / 2 + 100.0f, visibleSize.height));
    bb->setScale(.3f);
    addChild(bb);

    auto label = Label::createWithTTF("Workshops", "fonts/arial.ttf", 10);
    label->setPosition(Point(visibleSize.width / 2 - 100.0f, visibleSize.height));
    addChild(label);

    auto dec = Label::createWithTTF("Decorations", "fonts/arial.ttf", 10);
    dec->setPosition(Point(visibleSize.width / 2 + 100.0f, visibleSize.height));
    dec->enableBold();
    addChild(dec);
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(DecorationScene::OnTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(DecorationScene::OnTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(DecorationScene::OnTouchEnded, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}


void DecorationScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}


bool DecorationScene::OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    CCLOG("OnTouchBegan  x=%f y=%f", touch->getLocation().x, touch->getLocation().y);
    return true;
}

bool DecorationScene::OnTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    CCLOG("OnTouchMoved  x=%f y=%f", touch->getLocation().x, touch->getLocation().y);
    return true;
}

bool DecorationScene::OnTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    CCLOG("OnTouchEnded  x=%f y=%f", touch->getLocation().x, touch->getLocation().y);
    return true;
}

void DecorationScene::DisplayMenu()
{
    int i = 0;
    auto* menu = Menu::create();
    for (auto& decoration : m_decorationList)
    {
        auto menuItem = MenuItemImage::create(decoration.m_imageLocation, decoration.m_imageLocation, CC_CALLBACK_1(DecorationScene::ClickButton, this));
        menuItem->setPosition(Point(100.0f+i, 200));
        menuItem->setScale(.5f);
        menuItem->setName(decoration.m_imageLocation);
        m_menuItems.push_back(menuItem);

        menu->addChild(menuItem);
        auto label = Label::createWithTTF(decoration.m_name, "fonts/Marker Felt.ttf", 8);
        label->setPosition(Vec2(menuItem->getPositionX(), menuItem->getPositionY()+25));
        this->addChild(label);

        auto coin = Sprite::create("coin.png");
        coin->setPosition(Vec2(menuItem->getPositionX(), menuItem->getPositionY() - 20));
        coin->setScale(.4f);
        addChild(coin);

        auto label2 = Label::createWithTTF(std::to_string(decoration.m_goldCost), "fonts/arial.ttf", 6);
        label2->setPosition(Vec2(coin->getPositionX() + 10, coin->getPositionY()));
        this->addChild(label2);

        i += 75;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();

    auto prevPage = MenuItemImage::create("previous.png", "previous.png", CC_CALLBACK_1(DecorationScene::PrevPage, this));
    prevPage->setScale(2.0f);
    prevPage->setPosition(Point(prevPage->getBoundingBox().size.width, visibleSize.height / 2));
    menu->addChild(prevPage);

    auto closeSceneImage = MenuItemImage::create("close.png", "close.png", CC_CALLBACK_1(DecorationScene::GoBack, this));
    closeSceneImage->setPosition(Point(420.0f, 30.0f));
    menu->addChild(closeSceneImage);

    menu->setPosition(Point(0, 0));
    this->addChild(menu);

}

void DecorationScene::ParseData()
{
    XMLParser::getInstance().parseXML("..\\Resources\\ShopList.xml");
    XMLParser::getInstance().GetDecorationData(m_decorationList);
}


void DecorationScene::ClickButton(Ref* pSender)
{
    MenuItemImage* image = dynamic_cast<MenuItemImage*>(pSender);
    DataManager::getInstance()->m_isFloating = true;
    DataManager::getInstance()->m_floatingImage = image->getName();

    Director::getInstance()->popToRootScene();    
}

void DecorationScene::PrevPage(Ref* pSender)
{
    /*if (!SceneManager::getInstance().workshopScene)
        SceneManager::getInstance().workshopScene = WorkshopScene::create();*/
    Director::getInstance()->pushScene(TransitionFade::create(1.0f, WorkshopScene::createScene()));
}

void DecorationScene::GoBack(Ref* pSender)
{
    Director::getInstance()->popToRootScene();
}

