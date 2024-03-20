#include "IsometricTile.h"

IsometricTile::IsometricTile(cocos2d::Point point, float scaleFactor)
{
	m_isOccupied = false;
	m_myShopItem = nullptr;
	m_tileSprite = cocos2d::Sprite::create("tile.png");
	m_initPos = point;
	m_tileSprite->setPosition(point);
	m_tileSprite->setScale(.18f * scaleFactor);
}

