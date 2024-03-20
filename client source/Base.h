#pragma once
#include<string>

//Base structure of each shop item to store details from XML
struct Item
{
public:
	 std::string m_name;
	 std::string m_imageLocation;
	 std::string m_description;
	 int m_levelCap;
	 int m_goldCost;

	 Item();
};

