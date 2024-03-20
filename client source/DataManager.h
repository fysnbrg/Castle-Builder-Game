#pragma once
#include<string>
#include<cocos2d.h>

//Singleton Data Manager class for holding required global data
class DataManager
{
public:
    static DataManager* getInstance();

    static std::string m_floatingImage;  //image selected from shop
    static bool m_isFloating;  //status of item being selected or not from shop
    static float m_scaleFactor; //how much zoom is applied on screen
    static cocos2d::Vec2 m_mapPosition; //map center
    static cocos2d::Vec2 m_mapDimensions; //map width & height

private:

    DataManager() {}
    static DataManager* instance;
  
};

