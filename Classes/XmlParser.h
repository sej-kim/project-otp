#pragma once


#include "cocos2d.h"
#include "tinyxml2/tinyxml2.h"


class XmlParser : public cocos2d::Node
{
private:
	XmlParser(){}

public:
	static XmlParser* Create();
	static XmlParser* GetInstance();
	static void Destroy();


public:
	bool InitNationXml();
	bool InitReligionXml();
	bool InitRegistPlaceXml();
	bool InitTextXml();

public:
	bool GetNationList(std::vector< std::string > *Out);
	bool GetReligionList(std::vector< std::string > *Out);
	bool GetRegistPlace(std::vector< std::string > * Out);
	std::string GetText(int nIndex);

private:
	std::map< int, std::string > m_mapNation;
	std::map< int, std::string > m_mapReligion;
	std::map< int, std::string > m_mapRegistPlace;
	std::map< int, std::string > m_mapText;
};