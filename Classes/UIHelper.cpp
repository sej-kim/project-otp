
#include "UIHelper.h"
#include "Define.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"


USING_NS_CC;
using namespace ui;

bool SetUILabelText(cocos2d::Node* pRoot, std::string strName, int nIndex)
{
	return SetUILabelText(pRoot, strName, OTP_TEXT(nIndex));
}

bool SetUILabelText(cocos2d::Node* pBase, std::string strRoot, std::string strName, int nIndex)
{
	if (pBase == nullptr)
		return false;

	Node* pRoot = pBase->getChildByName(strRoot);
	if (pRoot == nullptr)
		return false;

	return SetUILabelText(pRoot, strName, nIndex);
}

bool SetUILabelText(cocos2d::Node* pRoot, std::string strName, std::string strText)
{
	if (pRoot == nullptr)
	{
		return false;
	}

	Text* pLabel = dynamic_cast<Text*>(pRoot->getChildByName(strName));
	if (pLabel == nullptr)
	{
		CCAssert(false, "pLabel == nullptr");
		return false;
	}

	pLabel->setString(strText);

	return true;
}

bool SetUIButtonText(cocos2d::Node* pRoot, std::string strName, int nIndex)
{
	return SetUIButtonText(pRoot, strName, OTP_TEXT(nIndex));
}

bool SetUIButtonText(cocos2d::Node* pRoot, std::string strName, std::string strText)
{
	if (pRoot == nullptr)
	{
		return false;
	}

	Button* pButton = dynamic_cast<Button*>(pRoot->getChildByName(strName));
	if (pButton == nullptr)
	{
		CCAssert(false, "pButton == nullptr");
		return false;
	}

	pButton->setTitleText(strText);

	return true;
}