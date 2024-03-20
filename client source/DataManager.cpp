#include "DataManager.h"
DataManager* DataManager::instance = 0;

std::string DataManager::m_floatingImage = "";
bool DataManager::m_isFloating = false;
float DataManager::m_scaleFactor = 1.0f;
cocos2d::Vec2 DataManager::m_mapDimensions = cocos2d::Vec2(0,0);
cocos2d::Vec2 DataManager::m_mapPosition = cocos2d::Vec2(0, 0);

DataManager* DataManager::getInstance() {
    if (!instance) {
        instance = new DataManager();
    }
    return instance;
}
