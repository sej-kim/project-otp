#include "RemittanceInfoListLayer.h"
#include "Define.h"


USING_NS_CC;
using namespace ui;

bool TradeInfoListLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

    scheduleUpdate();
    
	Node* pRoot = CSLoader::createNode("TradInfoListLayer.csb");
	if (pRoot == nullptr)
	{
		return false;
	}

	addChild(pRoot);

	Button* pBackButton = dynamic_cast<Button*>(pRoot->getChildByName("BackButton"));
	if (pBackButton == nullptr)
	{
		return false;
	}

	pBackButton->addClickEventListener(CC_CALLBACK_1(TradeInfoListLayer::menuCallback_Back, this));

	m_pResultListView = dynamic_cast<ListView*>(pRoot->getChildByName("ResultListView"));
	if (m_pResultListView == nullptr)
	{
		return false;
	}

	m_pNonDescText_1 = pRoot->getChildByName("NonDescText_1");
	m_pNonDescText_2 = pRoot->getChildByName("NonDescText_2");

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

	pListViewButton->addClickEventListener(CC_CALLBACK_1(TradeInfoListLayer::menuCallback_ComboBox, this));

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

	AddTypeListViewItem(m_pTypeListView, OTP_TEXT(9011), eTRAND_TYPE::ALL);
	AddTypeListViewItem(m_pTypeListView, OTP_TEXT(9012), eTRAND_TYPE::REMITTANCE);
	AddTypeListViewItem(m_pTypeListView, OTP_TEXT(9013), eTRAND_TYPE::DEPOSIT);
    
    DataManager* pDataManager = DataManager::GetInstance();
    if( pDataManager == nullptr )
    {
        return false;
    }
    
    const std::vector< sRTX_HISTORY_DATA >* pHistoryList = pDataManager->GetRtxHistoryDataList();
    if( pHistoryList == nullptr )
    {
        return false;
    }
    
    copy(pHistoryList->begin(), pHistoryList->end(), back_inserter(m_vecRtxHistoryList));
    pDataManager->RtxHistoryClear();
    
	return true;
}

void TradeInfoListLayer::update( float dt )
{
    for( int i_1 = 0; i_1 < 3; ++i_1)
    {
        if(m_vecRtxHistoryList.empty())
        {
            unscheduleUpdate();
            return;
        }
        
        std::vector< sRTX_HISTORY_DATA >::iterator it = m_vecRtxHistoryList.begin();
        sRTX_HISTORY_DATA rtxData = (*it);
        it = m_vecRtxHistoryList.erase( it );
        
       
        char szDate[128] = { 0,};
        char szAmt[128]  = { 0,};
        std::string strYY = rtxData.tSendTime.substr(0, 3);
        std::string strMM = rtxData.tSendTime.substr(4, 2);
        std::string strDD = rtxData.tSendTime.substr(6, 2);
        
        sprintf(szDate, "%s.%s.%s", strYY.c_str() , strMM.c_str(), strDD.c_str() );
        sprintf(szAmt, "%.1f %s", rtxData.fSendAmt, rtxData.strSendCurrency.c_str());
        AddResultListViewItem(m_pResultListView, szDate, rtxData.strMessage, rtxData.strReceiveAccntName, szAmt);
    }
}

void TradeInfoListLayer::AddTypeListViewItem(cocos2d::ui::ListView* pView, std::string strName, int nTag)
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
	pButtonClon->addClickEventListener(CC_CALLBACK_1(TradeInfoListLayer::ListCallback_Item, this));

	pView->pushBackCustomItem(pButtonClon);
}

void TradeInfoListLayer::AddResultListViewItem(cocos2d::ui::ListView* pView, std::string strTime, std::string strResult, std::string strTargetName,  std::string strValue)
{
	if (pView == nullptr || m_pResultListViewItem == nullptr)
	{
		return;
	}

	ImageView* pImageViewClon = dynamic_cast<ImageView*>(m_pResultListViewItem->clone());

	Text* pTimeText			= dynamic_cast<Text*>(pImageViewClon->getChildByName("TimeText"));
	Text* pResultText		= dynamic_cast<Text*>(pImageViewClon->getChildByName("ResultTypeText"));
	Text* pTargetNameText	= dynamic_cast<Text*>(pImageViewClon->getChildByName("TargetNameText"));
	Text* pValueText		= dynamic_cast<Text*>(pImageViewClon->getChildByName("ValueText"));
	if (pTimeText == nullptr || pResultText == nullptr || pTargetNameText == nullptr || pValueText == nullptr)
	{
		return;
	}

	pTimeText->setString(strTime.c_str());
	pResultText->setString(strResult.c_str());
	pTargetNameText->setString(strTargetName.c_str());
	pValueText->setString(strValue.c_str());

	Size size = pView->getCustomSize();
	pImageViewClon->setContentSize(Size(size.width, pImageViewClon->getContentSize().height));

	pView->pushBackCustomItem(pImageViewClon);

	SetVisibleNoneDescText(false);
}

void TradeInfoListLayer::ListCallback_Item(cocos2d::Ref* sender)
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
	case eTRAND_TYPE::ALL:
	{
							 
	}
		break;
	case eTRAND_TYPE::REMITTANCE:
	{

	}
		break;
	case eTRAND_TYPE::DEPOSIT:
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

void TradeInfoListLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_HOME_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void TradeInfoListLayer::menuCallback_ComboBox(cocos2d::Ref* sender)
{
	if (m_pTypeListView == nullptr)
	{
		return;
	}

	m_pTypeListView->setVisible(!m_pTypeListView->isVisible());
	m_pTypeListView->setTouchEnabled(m_pTypeListView->isVisible());
	m_pTypeListView->setEnabled(m_pTypeListView->isVisible());
}

void TradeInfoListLayer::SetVisibleNoneDescText(bool bVisible)
{
	if (m_pNonDescText_1 == nullptr || m_pNonDescText_2 == nullptr)
	{
		return;
	}

	m_pNonDescText_1->setVisible(bVisible);
	m_pNonDescText_2->setVisible(bVisible);
}

bool TradeInfoListLayer::onTouchBegan(Touch *a_pTouch, Event *a_pEvent)
{
	if (m_pTypeListView != nullptr)
	{
		m_pTypeListView->setVisible(false);
		m_pTypeListView->setTouchEnabled(false);
		m_pTypeListView->setEnabled(false);
	}

	return true;
}

void TradeInfoListLayer::onEnter()
{
	Layer::onEnter();

	m_plistener = EventListenerTouchOneByOne::create();
	m_plistener->setSwallowTouches(true);
	m_plistener->onTouchBegan = CC_CALLBACK_2(TradeInfoListLayer::onTouchBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(m_plistener, this);
}

void TradeInfoListLayer::onExit()
{
	if (m_plistener != nullptr)
	{
		_eventDispatcher->removeEventListener(m_plistener);
	}

	Layer::onExit();
}
