#pragma once
#include "pugixml/pugixml.hpp"
#include <vector>
#include "cocos2d.h"
#include "Decoration.h"
#include "Workshop.h"

class WorldScene;
class ShopItem;
class IsometricTile;

//Singleton XML parser class
class XMLParser 
{
private:
    pugi::xml_document doc;

    XMLParser() {}

public:
    static XMLParser& getInstance();

    bool parseXML(const std::string& filename);

    void ReadShopItemData(std::vector<ShopItem*>& shopItems, cocos2d::Menu* menu, WorldScene* obj);
    void WriteShopItemData(std::vector<ShopItem*>& shopItems, std::vector<IsometricTile*>& grid);
    cocos2d::Sprite* LoadMap();

    void GetWorkshopData(std::vector<Workshop>& workshopData);
    void GetDecorationData(std::vector<Decoration>& decoraionsData);
};


