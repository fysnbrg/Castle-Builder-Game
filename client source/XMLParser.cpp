#include "XMLParser.h"
#include "HelloWorldScene.h"
#include "DataManager.h"
#include "ShopItem.h"
#include "IsometricTile.h"

XMLParser& XMLParser::getInstance()
{
    static XMLParser instance;
    return instance;
}

bool XMLParser::parseXML(const std::string& filename)
{
    pugi::xml_parse_result result = doc.load_file(filename.c_str());
    if (!result) {
        return false;
    }
    return true;
}

//Reads the shop item data from XML
void XMLParser::ReadShopItemData(std::vector<ShopItem*>& shopItems, cocos2d::Menu* ImageMenu, WorldScene* obj)
{
    pugi::xml_node root = doc.child("MenuImageItems");

    for (pugi::xml_node itemNode : root.children("Item")) {
        // Get the attribute values
        std::string imgLocation = itemNode.attribute("ImagePath").as_string();
        auto shopItem = cocos2d::MenuItemImage::create(imgLocation, imgLocation, CC_CALLBACK_1(WorldScene::ShopItems, obj));
        shopItem->setPosition(itemNode.attribute("PositionX").as_float(), itemNode.attribute("PositionY").as_float());
        shopItem->setScale(itemNode.attribute("Scale").as_float());
        shopItem->setName(imgLocation);

        ShopItem* item = new ShopItem(shopItem, DataManager::getInstance()->m_scaleFactor);
        item->SetTileKey(itemNode.attribute("Key").as_int());

        ImageMenu->addChild(shopItem);
        shopItems.push_back(item);
    }
}

//Write Shop Item data to XML
void XMLParser::WriteShopItemData(std::vector<ShopItem*>& shopItems, std::vector<IsometricTile*>& grid)
{
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("MenuImageItems");

    for (const auto& item : shopItems) {

        cocos2d::Vec2 itemPos; 
        
        itemPos = grid[item->GetTileKey()]->GetInitPos();
        float scale = item->GetImage()->getScale() / DataManager::getInstance()->m_scaleFactor;

        pugi::xml_node itemNode = root.append_child("Item");
        itemNode.append_attribute("ImagePath").set_value(item->GetImage()->getName().c_str());
        itemNode.append_attribute("PositionX").set_value(itemPos.x);
        itemNode.append_attribute("PositionY").set_value(itemPos.y +  5.0f);
        itemNode.append_attribute("Scale").set_value(scale);
        itemNode.append_attribute("Key").set_value(item->GetTileKey());
    }

    doc.save_file("..\\Resources\\ShopItemDetails.xml");
}

//Get Map Details
cocos2d::Sprite* XMLParser::LoadMap()
{
    cocos2d::Sprite* map;
    pugi::xml_node root = doc.child("MapDetails");
    for (pugi::xml_node itemNode : root.children("Item")) 
    {
        map = cocos2d::Sprite::create(itemNode.attribute("ImagePath").as_string());
        map->setScale(itemNode.attribute("Width").as_float() / map->getContentSize().width, itemNode.attribute("Height").as_float() / map->getContentSize().height);
        map->setPosition(itemNode.attribute("PositionX").as_float(), itemNode.attribute("PositionY").as_float());

        DataManager::getInstance()->m_mapDimensions.x = itemNode.attribute("Width").as_float();
        DataManager::getInstance()->m_mapDimensions.y = itemNode.attribute("Height").as_float();

        DataManager::getInstance()->m_mapPosition.x = itemNode.attribute("PositionX").as_float();
        DataManager::getInstance()->m_mapPosition.y = itemNode.attribute("PositionY").as_float();

        return map;
    }
    
    return nullptr;
}

void XMLParser::GetWorkshopData(std::vector<Workshop>& workshopData)
{
    pugi::xml_node workshopsNode = doc.child("shop").child("workshops");
    for (pugi::xml_node workshopNode : workshopsNode.children("workshop")) {
        Workshop workshop;
        workshop.m_name = workshopNode.child_value("name");
        workshop.m_imageLocation = workshopNode.child_value("image");
        workshop.m_goldCost = std::stoi(workshopNode.child_value("gold_cost"));
        workshopData.push_back(workshop);
    }
}

void XMLParser::GetDecorationData(std::vector<Decoration>& decorationsData)
{
    pugi::xml_document doc;
    doc.load_file("..\\Resources\\ShopList.xml");
    pugi::xml_node decorationsNode = doc.child("shop").child("decorations");
    for (pugi::xml_node decorationNode : decorationsNode.children("decoration")) {
        Decoration decoration;
        decoration.m_name = decorationNode.child_value("name");
        decoration.m_imageLocation = decorationNode.child_value("image");
        decoration.m_goldCost = std::stoi(decorationNode.child_value("gold_cost"));
        decorationsData.push_back(decoration);
    }
}
