#include "XmlParser.h"
#include "Define.h"

static XmlParser* gs_pXmlPaser = nullptr;

USING_NS_CC;
using namespace tinyxml2;
XmlParser* XmlParser::Create()
{
	if (gs_pXmlPaser != nullptr)
	{
		XmlParser::Destroy();
	}

	gs_pXmlPaser = new XmlParser();
	gs_pXmlPaser->InitTextXml();
	return gs_pXmlPaser;
}

void XmlParser::Destroy()
{
	CC_SAFE_DELETE(gs_pXmlPaser);
}

XmlParser* XmlParser::GetInstance()
{
	return gs_pXmlPaser;
}

bool XmlParser::InitNationXml()
{
	tinyxml2::XMLDocument	xmlDocument;
	if (xmlDocument.LoadFile("nation.xml") != XML_SUCCESS)
	{
		return false;
	}

	XMLHandle docHandle(&xmlDocument);

	XMLElement* entry = docHandle.FirstChildElement("root").ToElement();
	for (tinyxml2::XMLNode *node = entry->FirstChildElement(); node; node = node->NextSibling())
	{
		tinyxml2::XMLElement *pElement = node->ToElement();
		XMLNode* pNode = pElement->FirstChild();

		int nCode = atoi(pNode->FirstChild()->Value());
		pNode = pNode->NextSibling();
		std::string strName = pNode->FirstChild()->Value();

		m_mapReligion[nCode] = strName;
	}

	return true;
}

bool XmlParser::InitReligionXml()
{
	tinyxml2::XMLDocument	xmlDocument;
	if (xmlDocument.LoadFile("religion.xml") != XML_SUCCESS)
	{
		return false;
	}

	XMLHandle docHandle(&xmlDocument);

	XMLElement* entry = docHandle.FirstChildElement("root").ToElement();
	for (tinyxml2::XMLNode *node = entry->FirstChildElement(); node; node = node->NextSibling())
	{
		tinyxml2::XMLElement *pElement = node->ToElement();
		XMLNode* pNode		= pElement->FirstChild();

		int nCode			= atoi(pNode->FirstChild()->Value());
		pNode				= pNode->NextSibling();
		std::string strName = pNode->FirstChild()->Value();

		m_mapNation[nCode] = strName;
	}

	return true;
}

bool XmlParser::InitRegistPlaceXml()
{
	tinyxml2::XMLDocument	xmlDocument;
	if (xmlDocument.LoadFile("registPlace.xml") != XML_SUCCESS)
	{
		return false;
	}
	
	XMLHandle docHandle(&xmlDocument);

	XMLElement* entry = docHandle.FirstChildElement("root").ToElement();
	for (tinyxml2::XMLNode *node = entry->FirstChildElement(); node; node = node->NextSibling())
	{
		tinyxml2::XMLElement *pElement = node->ToElement();
		XMLNode* pNode = pElement->FirstChild();

		int nCode = atoi(pNode->FirstChild()->Value());
		pNode = pNode->NextSibling();
		std::string strName = pNode->FirstChild()->Value();

		m_mapRegistPlace[nCode] = strName;;
	}

	return true;
}

bool XmlParser::InitTextXml()
{
	std::string fullPath = CCFileUtils::getInstance()->fullPathForFilename("text.xml");
	Data xmlData = CCFileUtils::getInstance()->getDataFromFile(fullPath);


	cocos2d::log("filepath:%s", fullPath.c_str());
	tinyxml2::XMLDocument	xmlDocument;
	XMLError erorr = xmlDocument.Parse((const char*)xmlData.getBytes(), xmlData.getSize());
	if (erorr != XML_SUCCESS)
	{
		cocos2d::log("InitTextXml() == false erorr[%d]", erorr);
		return false;
	}
	
	XMLHandle docHandle(&xmlDocument);
	XMLElement* entry = docHandle.FirstChildElement("root").ToElement();
	for (tinyxml2::XMLNode *node = entry->FirstChildElement(); node; node = node->NextSibling())
	{
		tinyxml2::XMLElement *pElement = node->ToElement();
		XMLNode* pNode = pElement->FirstChild();

		int nCode = atoi(pNode->FirstChild()->Value());
		pNode = pNode->NextSibling();
		std::string strName = pNode->FirstChild()->Value();

		m_mapText[nCode] = strName;;
	}

	return true;
}

bool XmlParser::GetNationList(std::vector< std::string > *Out)
{
	if (Out == nullptr)
	{
		return false;
	}

 	std::map< int, std::string >::iterator it = m_mapNation.begin();
	while (it != m_mapNation.end())
	{
		Out->push_back(it->second);
		++it;
	}

	return true;
}

bool XmlParser::GetReligionList(std::vector< std::string > *Out)
{
	if (Out == nullptr)
	{
		return false;
	}

	std::map< int, std::string >::iterator it = m_mapReligion.begin();
	while (it != m_mapReligion.end())
	{
		Out->push_back(it->second);
		++it;
	}

	return true;
}

bool XmlParser::GetRegistPlace(std::vector< std::string > * Out)
{
	if (Out == nullptr)
	{
		return false;
	}

	std::map< int, std::string >::iterator it = m_mapRegistPlace.begin();
	while (it != m_mapRegistPlace.end())
	{
		Out->push_back(it->second);
		++it;
	}

	return true;
}

std::string XmlParser::GetText(int nIndex)
{
	std::map< int, std::string >::iterator it = m_mapText.find( nIndex );
	if (it == m_mapText.end())
	{
		return "";
	}

	return it->second;
}