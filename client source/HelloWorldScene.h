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

#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class IsometricTile;
class ShopItem;

class WorldScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(WorldScene);

    cocos2d::Sprite* m_map; //map sprite
    cocos2d::Sprite* m_floatSprite = nullptr; //sprite for the image selected from the shop

    cocos2d::Menu* m_imageMenu; //menu for image items
    cocos2d::Menu* m_fontMenu;  //menu for font items

    std::vector<ShopItem*> m_shopItems; //shop items array
    std::vector<IsometricTile*> m_grid; 

    cocos2d::Vec2 m_initPos;

    void InitTiles();
    IsometricTile* CheckIsometricCollision(cocos2d::Touch* touch);
    void InitFriends();

    bool OnTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    bool OnMouseMoved(cocos2d::EventMouse* event);
    bool OnTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
    bool OnTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

    void SwitchGrid(Ref* pSender);
    void ReCalibrateMap(Ref* pSender);
    void LoadNewMap(Ref* pSender);

    void DeleteShopItem(Ref* pSender);
    void ShopButton(Ref* pSpender);
    void ShopItems(Ref* pSpender);

    void ZoomIn(Ref* pSpender);
    void ZoomOut(Ref* pSpender);

    ~WorldScene();
};

#endif // __HELLOWORLD_SCENE_H__
