#include "ExchangeInfoListLayer.h"
#include "Define.h"


USING_NS_CC;
using namespace ui;

bool ExchangeInfoListLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

    scheduleUpdate();
    
	Node* pRoot = CSLoader::createNode("ExchangeInfoListLayer.csb");
	if (pRoot == nullptr)
	{
		return false;
	}

	addChild(pRoot);

	Button* pBackButton			= dynamic_cast<Button*>(pRoot->getChildByName("BackButton"));
	if (pBackButton == nullptr)
	{
		return false;
	}

	pBackButton->addClickEventListener(CC_CALLBACK_1(ExchangeInfoListLayer::menuCallback_Back, this));

	m_pNonDescText_1 = pRoot->getChildByName("NonDescText_1");
	m_pNonDescText_2 = pRoot->getChildByName("NonDescText_2");

	m_pResultListView = dynamic_cast<ListView*>(pRoot->getChildByName("ResultListView"));
	if (m_pResultListView == nullptr)
	{
		return false;
	}

	Node* pListNode = pRoot->getChildByName("Combo_1");
	if (pListNode == nullptr)
	{
		return false;
	}

	m_pTypeListView = dynamic_cast<ListView*>(pListNode->getChildByName("ListView"));
	if (m_pTypeListView == nullptr)
	{
		return false;
	}

	m_pListText = dynamic_cast<Text*>(pListNode->getChildByName("Text"));
	if (m_pListText == nullptr)
	{
		return false;
	}

	m_pResultListViewItem = dynamic_cast<ImageView*>(pRoot->getChildByName("Item"));
	if (m_pResultListViewItem == nullptr)
	{
		return false;
	}

	Button* pListViewButton = dynamic_cast<Button*>(pListNode->getChildByName("Button"));
	if (pListViewButton == nullptr)
	{
		return false;
	}

	pListViewButton->addClickEventListener(CC_CALLBACK_1(ExchangeInfoListLayer::menuCallback_ComboBox, this));

	Node* pItem = CSLoader::createNode("ListViewItem.csb");
	if (pItem == nullptr)
	{
		return false;
	}

	m_pTypeListViewItem = dynamic_cast<Button*>(pItem->getChildByName("Button"));
	if (m_pTypeListViewItem == nullptr)
	{
		return false;
	}

	AddTypeListViewItem(m_pTypeListView, OTP_TEXT(20011), eEXCHANGE_TYPE::ALL);
	AddTypeListViewItem(m_pTypeListView, OTP_TEXT(20012), eEXCHANGE_TYPE::BTC);
	AddTypeListViewItem(m_pTypeListView, OTP_TEXT(20013), eEXCHANGE_TYPE::EOS);

    DataManager* pDataManager = DataManager::GetInstance();
    if( pDataManager == nullptr )
    {
        return false;
    }
    
    const std::vector< sEXCHANGE_HISTORY_DATA >* pHistoryList = pDataManager->GetExchangeHistoryDataList();
    if( pHistoryList == nullptr )
    {
        return false;
    }
    
    copy(pHistoryList->begin(), pHistoryList->end(), back_inserter(m_vecExchangeHistoryList));
    pDataManager->ExchangeHistoryClear();
    
	return true;
}

void ExchangeInfoListLayer::update( float dt )
{
    for( int i_1 = 0; i_1 < 3; ++i_1)
    {
        if(m_vecExchangeHistoryList.empty())
        {
            unscheduleUpdate();
            return;
        }
        
        std::vector< sEXCHANGE_HISTORY_DATA >::iterator it = m_vecExchangeHistoryList.begin();
        sEXCHANGE_HISTORY_DATA ExchangeData = (*it);
        it = m_vecExchangeHistoryList.erase( it );
        
        char szTime[128] = { 0,};
        char szPay[128] = { 0,};
		char szBid[128] = { 0, };
        std::string strYY = ExchangeData.tPurchase.substr(0, 4);
        std::string strMM = ExchangeData.tPurchase.substr(4, 2);
        std::string strDD = ExchangeData.tPurchase.substr(6, 2);

        sprintf(szTime, "%s-%s-%s", strYY.c_str(), strMM.c_str(), strDD.c_str());
		sprintf(szPay, "%d %s", (int)ExchangeData.fPayAmount, ExchangeData.strPayCurrency.c_str());
		sprintf(szBid, "%.1f %s", ExchangeData.fBidAmt, ExchangeData.strBidCurrency.c_str());
        
		AddResultListViewItem(m_pResultListView, szTime, szPay, szBid, ExchangeData.strStatus, ExchangeData.strStatusDesc);
    }
}

void ExchangeInfoListLayer::AddTypeListViewItem(cocos2d::ui::ListView* pView, std::string strName, int nTag)
{
	if (pView == nullptr || m_pTypeListViewItem == nullptr)
	{
		return;
	}

	Button* pButtonClon = dynamic_cast<Button*>(m_pTypeListViewItem->clone());

	Size size = pView->getCustomSize();
	pButtonClon->setContentSize(Size(size.width, pButtonClon->getContentSize().height));

	pButtonClon->setTitleText(strName.c_str());
	pButtonClon->setTag(nTag);
	pButtonClon->addClickEventListener(CC_CALLBACK_1(ExchangeInfoListLayer::ListCallback_Item, this));

	pView->pushBackCustomItem(pButtonClon);
}

void ExchangeInfoListLayer::AddResultListViewItem(cocos2d::ui::ListView* pView, std::string strTime, std::string strPay, std::string strBid, std::string strStatus, std::string strStatusDesc)
{
	if (pView == nullptr || m_pResultListViewItem == nullptr)
	{
		return;
	}
	
	ImageView* pImageViewClon = dynamic_cast<ImageView*>(m_pResultListViewItem->clone());

	Text* pTimeText		= dynamic_cast<Text*>(pImageViewClon->getChildByName("TimeText"));
	Text* pResultText	= dynamic_cast<Text*>(pImageViewClon->getChildByName("ResultText"));
	Text* pPayValueText	= dynamic_cast<Text*>(pImageViewClon->getChildByName("ValueText"));
	Text* pBidValueText = dynamic_cast<Text*>(pImageViewClon->getChildByName("ValueText_0"));
	if (pTimeText == nullptr || pResultText == nullptr || pPayValueText == nullptr || pBidValueText == nullptr)
	{
		return;
	}

	pTimeText->setString(strTime.c_str());
	pResultText->setString(strStatusDesc.c_str());
	pPayValueText->setString(strPay.c_str());
	pBidValueText->setString(strBid.c_str());

	if (strStatus.compare("S") == 0)
	{
		pResultText->setColor(Color3B(0, 0, 0));
	}
	else
	{
		pResultText->setColor(Color3B(255, 0, 0));
	}
	

	Size size = pView->getCustomSize();
	pImageViewClon->setContentSize(Size(size.width, pImageViewClon->getContentSize().height));

	pView->pushBackCustomItem(pImageViewClon);
	SetVisibleNoneDescText(false);
}

void ExchangeInfoListLayer::ListCallback_Item(cocos2d::Ref* sender)
{
	Button* pButton = dynamic_cast<Button*>(sender);
	if (pButton == nullptr || m_pListText == nullptr || m_pTypeListView == nullptr)
	{
		return;
	}

	m_pListText->setString(pButton->getTitleText());

	int nTag = pButton->getTag();
	switch (nTag)
	{
	case eEXCHANGE_TYPE::ALL:
	{

	}
		break;
	case eEXCHANGE_TYPE::BTC:
	{

	}
		break;
	case eEXCHANGE_TYPE::EOS:
	{

	}
		break;
	default:
		break;
	}


	m_pTypeListView->setVisible(false);
	m_pTypeListView->setTouchEnabled(false);
	m_pTypeListView->setEnabled(false);
}

void ExchangeInfoListLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_HOME_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void ExchangeInfoListLayer::menuCallback_ComboBox(cocos2d::Ref* sender)
{
	if (m_pTypeListView == nullptr)
	{
		return;
	}

	m_pTypeListView->setVisible(!m_pTypeListView->isVisible());
	m_pTypeListView->setTouchEnabled(m_pTypeListView->isVisible());
	m_pTypeListView->setEnabled(m_pTypeListView->isVisible());
}

void ExchangeInfoListLayer::SetVisibleNoneDescText(bool bVisible)
{
	if (m_pNonDescText_1 == nullptr || m_pNonDescText_2 == nullptr)
	{
		return;
	}

	m_pNonDescText_1->setVisible(bVisible);
	m_pNonDescText_2->setVisible(bVisible);
}

bool ExchangeInfoListLayer::onTouchBegan(Touch *a_pTouch, Event *a_pEvent)
{
	if (m_pTypeListView != nullptr)
	{
		m_pTypeListView->setVisible(false);
		m_pTypeListView->setTouchEnabled(false);
		m_pTypeListView->setEnabled(false);
	}

	return true;
}

void ExchangeInfoListLayer::onEnter()
{
	Layer::onEnter();

	m_plistener = EventListenerTouchOneByOne::create();
	m_plistener->setSwallowTouches(true);
	m_plistener->onTouchBegan = CC_CALLBACK_2(ExchangeInfoListLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_plistener, this);
}

void ExchangeInfoListLayer::onExit()
{
	if (m_plistener != nullptr)
	{
		_eventDispatcher->removeEventListener(m_plistener);
	}

	Layer::onExit();
}
