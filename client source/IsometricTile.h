#pragma once
#include "cocos2d.h"

class ShopItem;

class IsometricTile
{
public:
	ShopItem* m_myShopItem;  //pointer of corresponding shop item the tile holds

private:
	cocos2d::Sprite* m_tileSprite; //sprite of tile
	int m_key;  //unique identifier of each tile
	bool m_isOccupied; //check occupation status of tile
	cocos2d::Vec2 m_initPos; //initial position of the tile; required for reaclibration

public:
	IsometricTile(cocos2d::Point point, float scale);

	inline int GetKey() { return m_key; }
	inline bool IsOccupied() { return m_isOccupied; }
	inline cocos2d::Sprite* GetTileSprite() { return m_tileSprite; }
	inline cocos2d::Vec2 GetInitPos() { return m_initPos; }

	inline void SetKey(int key) { m_key = key; }
	inline void SetOccupied(bool occupied) { m_isOccupied = occupied; }
};

