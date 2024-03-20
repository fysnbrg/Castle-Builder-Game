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

#include "HelloWorldScene.h"
#include "WorkshopScene.h"
#include "DataManager.h"
#include "SceneManager.h"
#include "ui\CocosGUI.h"
#include "XMLParser.h"
#include "IsometricTile.h"
#include "ShopItem.h"


USING_NS_CC;

Scene* WorldScene::createScene()
{
    return WorldScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool WorldScene::init()
{
    //////////////////////////////
    // 1. super init first
    if (!Scene::init())
    {
        return false;
    }

    //Init Map
    XMLParser::getInstance().parseXML("..\\Resources\\MapDetails.xml");
    m_map = XMLParser::getInstance().LoadMap();
    addChild(m_map);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    cocos2d::Director::getInstance()->setDisplayStats(false);

    //Init shop
    auto shopImage = MenuItemImage::create("shop.png", "shop.png", CC_CALLBACK_1(WorldScene::ShopButton, this));
    shopImage->setScale(.2f);
    shopImage->setPosition(Point(visibleSize.width - shopImage->getBoundingBox().size.width, shopImage->getBoundingBox().size.height));

    //Init zoom in
    auto zoomIn = MenuItemImage::create("zoomin.png", "zoomin.png", CC_CALLBACK_1(WorldScene::ZoomIn, this));
    zoomIn->setScale(.2f);
    zoomIn->setPosition(Point(zoomIn->getBoundingBox().size.width, visibleSize.height - zoomIn->getBoundingBox().size.height));

    //Init zoom out
    auto zoomOut = MenuItemImage::create("zoomout.png", "zoomout.png", CC_CALLBACK_1(WorldScene::ZoomOut, this));
    zoomOut->setScale(.2f);
    zoomOut->setPosition(Point(zoomOut->getBoundingBox().size.width * 3, visibleSize.height - zoomOut->getBoundingBox().size.height));

    //Init Switch grid
    auto button = ui::Button::create("Button.png", "Button.png", "Button.png");
    button->setScale(.3f);
    button->setTitleText("Turn grid on/off");
    button->setTitleFontName("MarkerFelt.ttf");
    button->setTitleFontSize(20.0f);
    button->setPosition(Vec2(origin.x + button->getBoundingBox().size.width / 2, visibleSize.height / 2 + origin.y));
    button->addClickEventListener(CC_CALLBACK_1(WorldScene::SwitchGrid, this));
    this->addChild(button);

    //Init Recalibrate
    auto button2 = ui::Button::create("Button.png", "Button.png", "Button.png");
    button2->setScale(.3f);
    button2->setTitleText("Recalibrate Map");
    button2->setTitleFontName("MarkerFelt.ttf");
    button2->setTitleFontSize(20.0f);
    button2->setPosition(Vec2(origin.x + button->getBoundingBox().size.width / 2, visibleSize.height / 2 + origin.y + 20.0f));
    button2->addClickEventListener(CC_CALLBACK_1(WorldScene::ReCalibrateMap, this));
    this->addChild(button2);

    //Init New Map loader button
    auto button3 = ui::Button::create("Button.png", "Button.png", "Button.png");
    button3->setScale(.3f);
    button3->setTitleText("Load New Map");
    button3->setTitleFontName("MarkerFelt.ttf");
    button3->setTitleFontSize(20.0f);
    button3->setPosition(Vec2(origin.x + button->getBoundingBox().size.width / 2, visibleSize.height / 2 + origin.y + 40.0f));
    button3->addClickEventListener(CC_CALLBACK_1(WorldScene::LoadNewMap, this));
    this->addChild(button3);

    InitTiles();
    InitFriends();

    //Init image menu
    m_imageMenu = Menu::create(shopImage, zoomIn, zoomOut, NULL); 
    m_imageMenu->setPosition(Point(0, 0));
    this->addChild(m_imageMenu);

    //Init font menu
    m_fontMenu = Menu::create();
    m_fontMenu->setPosition(Point(0, 0));
    this->addChild(m_fontMenu);

    //getting previous map details of shop items
    XMLParser::getInstance().parseXML("..\\Resources\\ShopItemDetails.xml");
    XMLParser::getInstance().ReadShopItemData(m_shopItems,m_imageMenu,this);

    //Init event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);

    listener->onTouchBegan = CC_CALLBACK_2(WorldScene::OnTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(WorldScene::OnTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(WorldScene::OnTouchEnded, this);

    //Init Mouse listener
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(WorldScene::OnMouseMoved, this);

    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    return true;
}


void WorldScene::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();
}

//Creating an isometric grid
void WorldScene::InitTiles()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    IsometricTile* tile = new IsometricTile(Point((visibleSize.width / 2) + origin.x - 20, (visibleSize.height / 2) + origin.y + 60),DataManager::getInstance()->m_scaleFactor);
    auto offset = tile->GetTileSprite()->getBoundingBox().size;

    int key = 0;
    for (int i = 1; i < 6; i++)
    {
        for (int j = 1; j < 8; j++)
        {
            IsometricTile* tile = new IsometricTile(Point((visibleSize.width / 2) + origin.x - 135 + offset.width * i, (visibleSize.height / 2) + origin.y + 65 - offset.height * j), DataManager::getInstance()->m_scaleFactor);
            this->addChild(tile->GetTileSprite());
            tile->SetKey(key);  //key is a unique identifier for each tile
            m_grid.push_back(tile);
            key++;
        }
    }

    for (int i = 1; i < 5; i++)
    {
        for (int j = 1; j < 8; j++)
        {
            IsometricTile* tile = new IsometricTile(Point((visibleSize.width / 2) + origin.x - 135 + offset.width / 2 + offset.width * i, (visibleSize.height / 2) + origin.y + 65 - offset.height * j - offset.height / 2), DataManager::getInstance()->m_scaleFactor);
            this->addChild(tile->GetTileSprite());
            tile->SetKey(key);
            m_grid.push_back(tile);
            key++;
        }
    }
}

//Adding friends sprites and details
void WorldScene::InitFriends()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    std::vector<std::string> names{ "Sageer","Zihaan","Hrishi" };
    std::vector<std::string> levels{ "Level 10","Level 8","Level 9" };

    for (int i = 0; i < 3; i++)
    {
        auto profile = Sprite::create("profile.png");
        profile->setScale(.3f);
        profile->setPosition(Point(origin.x + visibleSize.width / 2 +i*30, origin.y + profile->getBoundingBox().size.height));
        addChild(profile);

        auto name = Label::createWithTTF(names[i], "fonts/arial.ttf", 5);
        name->setPosition(Vec2(profile->getPositionX(), profile->getPositionY() - 13));
        name->setColor(Color3B::BLACK);
        this->addChild(name);

        auto level = Label::createWithTTF(levels[i], "fonts/arial.ttf", 5);
        level->setPosition(Vec2(profile->getPositionX(), profile->getPositionY() - 17));
        level->setColor(Color3B::BLACK);
        this->addChild(level);
    }
}

//Collision checker on the isometric grid
IsometricTile* WorldScene::CheckIsometricCollision(cocos2d::Touch* touch)
{
    //creating an array of all nearest tiles
    std::vector<IsometricTile*> nearbyTiles;
    for (IsometricTile* tile : m_grid)
    {
        if (tile->GetTileSprite()->getBoundingBox().containsPoint(Vec2(touch->getLocation().x, touch->getLocation().y)))
        {
            nearbyTiles.push_back(tile);
        }
    }

    if (nearbyTiles.size() == 0) //if outside grid
        return nullptr;

    IsometricTile* result = nearbyTiles[0];
    Vec2 point1 = Vec2(touch->getLocation().x, touch->getLocation().y);
    float minDistance = point1.distance(Vec2(result->GetTileSprite()->getPosition()));

    //finding the closest tile to the touch point
    for (IsometricTile* tile : nearbyTiles)
    {
        Vec2 point2 = cocos2d::Vec2(tile->GetTileSprite()->getPosition());
        float distance = point1.distance(point2);
        if (distance < minDistance)
        {
            minDistance = distance;
            result = tile;
        }
    }

    if (!result->IsOccupied()) //returns the tile pointer if it's not occupied already
        return result;

    return nullptr;
}

//Callback for when touch starts
bool WorldScene::OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
    m_initPos = Vec2(touch->getLocation().x, touch->getLocation().y); //stores the initial touch point to be used in OnTouchMoved
    return true;
}

//Callback for when mouse is just moved
bool WorldScene::OnMouseMoved(cocos2d::EventMouse* event)
{
    if (DataManager::getInstance()->m_isFloating) //if floating has been enabled (image selected from shop)
    {
        auto img = DataManager::getInstance()->m_floatingImage;

        if (!m_floatSprite) //creating the sprite if it's not there 
        {
            m_floatSprite = Sprite::create(img);
            this->addChild(m_floatSprite);
        }

        m_floatSprite->setPosition(event->getCursorX(), event->getCursorY()); //moving the sprite based on the cursor
        m_floatSprite->setScale(.5f);
    }

    return true;
}

//Callback for when mouse it touched and moved simultaneously - for panning
bool WorldScene::OnTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float delta = touch->getLocation().x - m_initPos.x;
    float zoomFactor = DataManager::getInstance()->m_scaleFactor;
    
    CCLOG("delta : %f", delta);

    //Map, grid and shop items are moved back if panned towards right threshold
    if (m_map->getPosition().x >= (visibleSize.width / 2 + origin.x) * zoomFactor)
    {
        m_map->setPositionX(m_map->getPositionX() - .1f);

        for (auto& tile : m_grid)
        {
            tile->GetTileSprite()->setPositionX(tile->GetTileSprite()->getPositionX() - .1f);
        }

        for (auto& shopItem : m_shopItems)
        {
            shopItem->GetImage()->setPositionX(shopItem->GetImage()->getPositionX() - .1f);
        }
    }
    //Map, grid and shop items are moved forward if panned towards left threshold
    else if (m_map->getPosition().x <= (visibleSize.width / 2 + origin.x) / zoomFactor)
    {
        m_map->setPositionX(m_map->getPositionX() + .1f);

        for (auto& tile : m_grid)
        {
            tile->GetTileSprite()->setPositionX(tile->GetTileSprite()->getPositionX() + .1f);
        }

        for (auto& shopItem : m_shopItems)
        {
            shopItem->GetImage()->setPositionX(shopItem->GetImage()->getPositionX() + .1f);
        }
    }
    //Panning
    else
    {
        m_map->setPositionX(m_map->getPosition().x + delta);

        for (auto& tile : m_grid)
        {
            tile->GetTileSprite()->setPositionX(tile->GetTileSprite()->getPositionX() + delta);
        }

        for (auto& shopItem : m_shopItems)
        {
            shopItem->GetImage()->setPositionX(shopItem->GetImage()->getPositionX() + delta);
        }
    }
    
    m_initPos = touch->getLocation(); //last touched position becomes initPos to calculate new delta
    return true;
}

//Callback for when touch is ended
bool WorldScene::OnTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
    IsometricTile* tile = CheckIsometricCollision(touch); // getting pointer of the corresponding tile based on touch. Returns nullptr if already occupied or outside grid

    if (DataManager::getInstance()->m_isFloating && tile) //if touch is ended while placing a 
    {
        tile->SetOccupied(true);
        Vec2 offset = cocos2d::Vec2(0.0f, 5.0f); //offset so that shop item is in centre

        std::string imgLocation = DataManager::getInstance()->m_floatingImage;
        auto shopItem = MenuItemImage::create(imgLocation, imgLocation, CC_CALLBACK_1(WorldScene::ShopItems, this));
        shopItem->setPosition(tile->GetTileSprite()->getPosition() + offset);
        shopItem->setScale(.4f);
        shopItem->setName(imgLocation);

        ShopItem* item = new ShopItem(shopItem, DataManager::getInstance()->m_scaleFactor);
        item->SetTileKey(tile->GetKey());
        tile->m_myShopItem = item;

        m_imageMenu->addChild(shopItem); //shopItem is added to the map
        m_floatSprite->removeFromParent();

        m_shopItems.push_back(item);

        DataManager::getInstance()->m_isFloating = false;
        m_floatSprite = nullptr;
    }

    return true;
}

//Switches visibilty of grid on/off
void WorldScene::SwitchGrid(Ref* pSender)
{
    CCLOG("buttton prrr");

    for (IsometricTile* tile : m_grid)
    {
        tile->GetTileSprite()->setVisible(!tile->GetTileSprite()->isVisible());
    }
}

//Recalibrates the map to zero zoom and initial position
void WorldScene::ReCalibrateMap(Ref* pSender)
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    //setting map to original scale and position
    m_map->setScale(DataManager::getInstance()->m_mapDimensions.x / m_map->getContentSize().width, DataManager::getInstance()->m_mapDimensions.y / m_map->getContentSize().height);
    m_map->setPosition(DataManager::getInstance()->m_mapPosition);
    
    
    for (auto& tile : m_grid)
    {
        //updating each tile to the initial state
        tile->GetTileSprite()->setPosition(tile->GetInitPos());
        tile->GetTileSprite()->setScale(tile->GetTileSprite()->getScale() / DataManager::getInstance()->m_scaleFactor);

        int shopItemIndex = 0;
        while (shopItemIndex < m_shopItems.size())
        {
            if (tile->GetKey() == m_shopItems[shopItemIndex]->GetTileKey())
            {
                tile->SetOccupied(true);
                tile->m_myShopItem = m_shopItems[shopItemIndex];
                break;
            }
            shopItemIndex++;
        }
    }

    //setting each shop item to it's corresponding tile position based on the key
    for (auto& shopItem : m_shopItems)
    {
        int tileKey = shopItem->GetTileKey();
        Vec2 initPos = m_grid[tileKey]->GetInitPos();
        shopItem->GetImage()->setPosition(initPos.x, initPos.y + 5);
        shopItem->GetImage()->setScale(shopItem->GetImage()->getScale() / DataManager::getInstance()->m_scaleFactor);
    }

    DataManager::getInstance()->m_scaleFactor = 1.0f; //changing scale factor back to 1
}

//CallBack for Loading new Map button - clears the exisiting map of all the shop items
void WorldScene::LoadNewMap(Ref* pSender)
{
    ReCalibrateMap(pSender);

    //changing grid structure
    for (auto& tile : m_grid)
    {
        if (tile->IsOccupied())
        {
            tile->SetOccupied(false);
            tile->m_myShopItem->GetImage()->removeFromParent();
            tile->m_myShopItem = nullptr;
        }
    }

    m_shopItems.clear();
}

//CallBack for when the cross is clicked - deletes the corresponding shop item
void WorldScene::DeleteShopItem(Ref* pSender)
{
    auto cross = static_cast<MenuItemFont*>(pSender); //getting the cross pointer
    for (auto& tile : m_grid)
    {
        if (tile->m_myShopItem)
        {
            if (tile->m_myShopItem->GetCloseMarker() == cross)  //checking which tile's shop-item has the cross
            {
                tile->m_myShopItem = nullptr;
                tile->SetOccupied(false);
            }
        }
    }

    for (int i = 0; i < m_shopItems.size(); i++)
    {
        if (m_shopItems[i]->GetCloseMarker() == cross)
        {
            m_shopItems[i]->GetImage()->removeFromParent();  //deleting the corresponding shop item from the array
            m_shopItems.erase(m_shopItems.begin() + i);
        }       
    }

    cross->removeFromParent();
}

//Callback for shop image - goes inside the shop to workshop scene
void WorldScene::ShopButton(Ref* pSpender)
{
    /*if (!SceneManager::getInstance().workshopScene)
        SceneManager::getInstance().workshopScene = WorkshopScene::create();*/
    Director::getInstance()->pushScene(WorkshopScene::create());
}

//Callback for whenever a shop item in the map is clicked - 
void WorldScene::ShopItems(Ref* pSpender)
{
    auto menuItemImage = static_cast<MenuItemImage*>(pSpender);

    for (auto shopItem : m_shopItems)
    {
        if (shopItem->GetImage() == menuItemImage)
        {
            shopItem->SetXMark(m_fontMenu, this);
        }
    }
}

//Callback for zooming in
void WorldScene::ZoomIn(Ref* pSpender)
{
    //zooms in if scale is less than a threshold
    if (m_map->getScaleX() < .75f)
    {
        DataManager::getInstance()->m_scaleFactor *= 1.1f;  

        m_map->setScale(m_map->getScaleX() * 1.1f, m_map->getScaleY() * 1.1f);

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 midPoint = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

        //scaling and repositioning the tiles based on the new zoomed map
        for (IsometricTile* tile : m_grid)
        {
            tile->GetTileSprite()->setScale(tile->GetTileSprite()->getScale() * 1.1f);

            Vec2 tilePos = tile->GetTileSprite()->getPosition();
            
            tilePos = tilePos - midPoint;
            tilePos *= 1.1f;
            tilePos = tilePos + midPoint;

            tile->GetTileSprite()->setPosition(tilePos);
        }

        //scaling and repositioning the shop items in the map based on the new zoomed map
        for (auto shopItem : m_shopItems)
        {
            shopItem->GetImage()->setScale(shopItem->GetImage()->getScale() * 1.1f);

            Vec2 itemPos = shopItem->GetImage()->getPosition();

            itemPos = itemPos - midPoint;
            itemPos *= 1.1f;
            itemPos = itemPos + midPoint;

            shopItem->GetImage()->setPosition(itemPos);
        }
    }
}

//Callback for zooming out - works similarly to ZoomIn
void WorldScene::ZoomOut(Ref* pSpender)
{
    if (m_map->getScaleX() > .675f)
    {
        DataManager::getInstance()->m_scaleFactor /= 1.1f;

        m_map->setScale(m_map->getScaleX() / 1.1f, m_map->getScaleY() / 1.1f);

        auto visibleSize = Director::getInstance()->getVisibleSize();
        Vec2 origin = Director::getInstance()->getVisibleOrigin();
        Vec2 midPoint = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);

        for (IsometricTile* tile : m_grid)
        {
            tile->GetTileSprite()->setScale(tile->GetTileSprite()->getScale() / 1.1f);

            Vec2 pos = tile->GetTileSprite()->getPosition();
            pos = pos - midPoint;
            pos = pos/1.1f;

            pos = pos + midPoint;
            tile->GetTileSprite()->setPosition(pos);
        }

        for (auto shopItem : m_shopItems)
        {
            shopItem->GetImage()->setScale(shopItem->GetImage()->getScale() / 1.1f);

            Vec2 itemPos = shopItem->GetImage()->getPosition();

            itemPos = itemPos - midPoint;
            itemPos = itemPos / 1.1f;
            itemPos = itemPos + midPoint;

            shopItem->GetImage()->setPosition(itemPos);
        }
    }
}

WorldScene::~WorldScene()
{
    XMLParser::getInstance().WriteShopItemData(m_shopItems,m_grid); //writing all the shop item data to xml

    for (IsometricTile* tile : m_grid)
        delete tile;

    for (ShopItem* shopItem : m_shopItems)
        delete shopItem;
}




