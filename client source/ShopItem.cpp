#include "ShopItem.h"
#include "HelloWorldScene.h"

ShopItem::ShopItem(cocos2d::MenuItemImage* itemImage,float scaleFactor)
{
	m_itemImage = itemImage;
    m_itemImage->setScale(m_itemImage->getScale() * scaleFactor);

    m_closeMarker = nullptr;
    m_tileKey = -1;
}

//Function for turning on and off X marker
void ShopItem::SetXMark(cocos2d::Menu* fontMenu, WorldScene* obj)
{
    //create X marker if it's nullptr
    if (!m_closeMarker)
    {
        m_closeMarker = cocos2d::MenuItemFont::create("X", CC_CALLBACK_1(WorldScene::DeleteShopItem, obj));
        m_closeMarker->setColor(cocos2d::Color3B::RED);
        m_closeMarker->setPosition(m_itemImage->getPosition().x + 7.0f, m_itemImage->getPosition().y);
        m_closeMarker->setScale(.2f);

        fontMenu->addChild(m_closeMarker);
    }
    //remove if clicked again
    else
    {
        m_closeMarker->removeFromParent();
        m_closeMarker = nullptr;
    }
}

