#include "HomeLayer.h"
#include "Define.h"
#include "UIHelper.h"
#include "DataManager.h"


USING_NS_CC;
using namespace ui;

bool HomeLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	m_pRoot = CSLoader::createNode("HomeLayer.csb");
	if (m_pRoot == nullptr)
	{
		return false;
	}

	addChild(m_pRoot);

	Button* pMenuButton = static_cast<Button*>(m_pRoot->getChildByName("MenuButton"));
	if (pMenuButton == nullptr)
	{
		return false;
	}

	pMenuButton->addClickEventListener(CC_CALLBACK_1(HomeLayer::menuCallback_Menu, this));

	Button* pRemittanceButton = static_cast<Button*>(m_pRoot->getChildByName("RemittanceButton"));
	if (pRemittanceButton == nullptr)
	{
		return false;
	}

	pRemittanceButton->addClickEventListener(CC_CALLBACK_1(HomeLayer::menuCallback_Remittance, this));

	Button* pTradeButton = static_cast<Button*>(m_pRoot->getChildByName("TradeListButton"));
	if (pTradeButton == nullptr)
	{
		return false;
	}

	pTradeButton->addClickEventListener(CC_CALLBACK_1(HomeLayer::menuCallback_Trade, this));


	m_pListView = dynamic_cast<ListView*>(m_pRoot->getChildByName("MenuListView"));
	if (m_pListView == nullptr)
	{
		return false;
	}

	Node* pItem = CSLoader::createNode("ListViewItem.csb");
	if (pItem == nullptr)
	{
		return false;
	}

	m_pListViewItem = dynamic_cast<Button*>(pItem->getChildByName("Button"));
	if (m_pListViewItem == nullptr)
	{
		return false;
	}

	AddListViewItem(m_pListView, OTP_TEXT(3006), eMENU_LIST::LOGOUT);
	AddListViewItem(m_pListView, OTP_TEXT(3007), eMENU_LIST::EXCHANGE);
	AddListViewItem(m_pListView, OTP_TEXT(3008), eMENU_LIST::EXCHANGE_LIST);

	sUSER_DATA sUserData;
	sBANKACCOUNT_DATA sBankAccountData;
	DataManager::GetInstance()->CopyUserData(sUserData);
	DataManager::GetInstance()->CopyBankAccountsData(sBankAccountData, 1);

	char szTemp[512] = { 0, };
	SetUILabelText(m_pRoot, "Text_1", 3000);
	sprintf(szTemp, OTP_TEXT(3001), sUserData.strUserName.c_str());
	SetUILabelText(m_pRoot, "UserInfoLabel", szTemp);
	SetUILabelText(m_pRoot, "Text_3", 3002);


	sprintf(szTemp, OTP_TEXT(3003), sBankAccountData.dAmt);
	SetUILabelText(m_pRoot, "BTCLabel", szTemp);

	SetUIButtonText(m_pRoot, "RemittanceButton", 3004);
	SetUIButtonText(m_pRoot, "TradeListButton", 3005);
	return true;
}

void HomeLayer::AddListViewItem(cocos2d::ui::ListView* pView, std::string strName, int nTag)
{
	if (pView == nullptr || m_pListViewItem == nullptr)
	{
		return;
	}

	Size size = pView->getCustomSize();

	Button* pButtonClon = dynamic_cast<Button*>(m_pListViewItem->clone());
	pButtonClon->setContentSize(Size(size.width, pButtonClon->getContentSize().height));
	pButtonClon->setTitleText(strName.c_str());
	pButtonClon->setTag(nTag);
	pButtonClon->addClickEventListener(CC_CALLBACK_1(HomeLayer::ListCallback_Item, this));

	pView->pushBackCustomItem(pButtonClon);
}

void HomeLayer::ListCallback_Item(cocos2d::Ref* sender)
{
	Button* pButton = dynamic_cast<Button*>(sender);
	if (pButton == nullptr)
	{
		return;
	}

	int nTag = pButton->getTag();
	switch (nTag)
	{
	case eMENU_LIST::LOGOUT:
	{
        DataManager::GetInstance()->ClearUserData();
        DataManager::GetInstance()->ClearBankAccountData();
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_START_LAYER_EVNET);
		runAction(RemoveSelf::create());
	}
		break;
	case eMENU_LIST::EXCHANGE:
	{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_EXCHANGE_LAYER_EVNET);
		runAction(RemoveSelf::create());
	}
		break;
	case eMENU_LIST::EXCHANGE_LIST:
	{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_EXCHANGE_INFO_LAYER_EVNET);
		runAction(RemoveSelf::create());
	}
		break;
	default:
		break;
	}
}

void HomeLayer::menuCallback_Menu(cocos2d::Ref* sender)
{
	if (m_pListView == nullptr)
	{
		return;
	}

	m_pListView->setVisible(!m_pListView->isVisible());
	m_pListView->setTouchEnabled(m_pListView->isVisible());
	m_pListView->setEnabled(m_pListView->isVisible());
}

void HomeLayer::menuCallback_Remittance(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_REMITTANCE_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void HomeLayer::menuCallback_Trade(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_TRADE_INO_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

bool HomeLayer::onTouchBegan(Touch *a_pTouch, Event *a_pEvent)
{
	if (m_pListView != nullptr)
	{
		m_pListView->setVisible(false);
		m_pListView->setTouchEnabled(false);
		m_pListView->setEnabled(false);
	}

	return true;
}

void HomeLayer::onEnter()
{
	Layer::onEnter();

	m_plistener = EventListenerTouchOneByOne::create();
	m_plistener->setSwallowTouches(true);
	m_plistener->onTouchBegan = CC_CALLBACK_2(HomeLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_plistener, this);
}

void HomeLayer::onExit()
{
	if (m_plistener != nullptr)
	{
		_eventDispatcher->removeEventListener(m_plistener);
	}

	Layer::onExit();
}
