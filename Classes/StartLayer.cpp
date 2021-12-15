#include "StartLayer.h"
#include "Define.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UIHelper.h"

USING_NS_CC;
using namespace ui;

bool StartLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	Node* pRoot = CSLoader::createNode("StartLayer.csb");

	Button* pStartMenu = static_cast<Button*>(pRoot->getChildByName("StartButton"));
	if (pStartMenu == nullptr)
	{
		return false;
	}
	pStartMenu->addClickEventListener(CC_CALLBACK_1(StartLayer::menuCallback_Start, this));

	addChild(pRoot);

	Button* pLoginButton = static_cast<Button*>(pRoot->getChildByName("LoginButton"));
	if (pLoginButton == nullptr)
	{
		return false;
	}

	pLoginButton->addClickEventListener(CC_CALLBACK_1(StartLayer::menuCallback_Login, this));


	SetUILabelText(pRoot, "Title", 1000);
	SetUILabelText(pRoot, "Text_2", 1001);
	SetUILabelText(pRoot, "Text_3", 1003);

	SetUIButtonText(pRoot, "StartButton", 1002);
	SetUIButtonText(pRoot, "LoginButton", 1004);
	return true;
}

void StartLayer::menuCallback_Start(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_ID_REGISTER_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void StartLayer::menuCallback_Login(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_LOGIN_LAYER_EVNET);
	runAction(RemoveSelf::create());
}
