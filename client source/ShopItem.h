#pragma once
#include "cocos2d.h"

class WorldScene;
class IsometricTile;

class ShopItem
{
private:
	int m_tileKey; //holds the key for what tile it is situated on
	cocos2d::MenuItemFont* m_closeMarker; //marker for delete pop up
	cocos2d::MenuItemImage* m_itemImage;  //image holder item

public:

	ShopItem(cocos2d::MenuItemImage* itemImage, float scale);
	void SetXMark(cocos2d::Menu* fontMenu, WorldScene* obj);

	inline int GetTileKey() { return m_tileKey; }
	inline cocos2d::MenuItemFont* GetCloseMarker() { return m_closeMarker; }
	inline cocos2d::MenuItemImage* GetImage() { return m_itemImage; }

	inline void SetTileKey(int key) { m_tileKey = key; }
};

