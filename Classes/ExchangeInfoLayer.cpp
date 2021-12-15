#include "ExchangeInfoLayer.h"
#include "Define.h"
#include "HttpManager.h"
#include "EventManager.h"
#include "UIHelper.h"

USING_NS_CC;
using namespace ui;

bool ExchangeInfoLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	m_pRoot = CSLoader::createNode("ExchangeInfoLayer.csb");
	if (m_pRoot == nullptr)
	{
		return false;
	}

	addChild(m_pRoot);

	Button* pBackButton = static_cast<Button*>(m_pRoot->getChildByName("BackButton"));
	if (pBackButton == nullptr)
	{
		return false;
	}

	pBackButton->addClickEventListener(CC_CALLBACK_1(ExchangeInfoLayer::menuCallback_Back, this));

	Button* pInquiryButton = static_cast<Button*>(m_pRoot->getChildByName("InquiryButton"));
	if (pInquiryButton == nullptr)
	{
		return false;
	}

	pInquiryButton->addClickEventListener(CC_CALLBACK_1(ExchangeInfoLayer::menuCallback_Inquiry, this));

	Button* pBleConnButton = dynamic_cast<Button*>(m_pRoot->getChildByName("BleConnButton"));
	if (pBleConnButton == nullptr)
	{
		return false;
	}

	pBleConnButton->addClickEventListener(CC_CALLBACK_1(ExchangeInfoLayer::menuCallback_BleConn, this));
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	pBleConnButton->setVisible(false);
	pBleConnButton->setEnabled(false);
	pBleConnButton->setTouchEnabled(false);
#endif

	Node* ImageRoot = m_pRoot->getChildByName("Image_5");
	if (ImageRoot)
	{
		m_pPublicKeyText = dynamic_cast<ui::Text*>(ImageRoot->getChildByName("KeyLabel"));
		SetUILabelText(ImageRoot, "Text_11", 20006);
	}
        
	SetUILabelText(m_pRoot->getChildByName("Image_5_0"), "Text_8_0", 20005);
    
	Text* pPinText		= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("PinTextField"));
	Text* pRePinText	= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("RePinTextField"));
	if (pPinText == nullptr || pRePinText == nullptr)
	{
		return false;
	}

	m_pPinEdit = ui::EditBox::create(pPinText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pPinEdit->setAnchorPoint(pPinText->getAnchorPoint());
	m_pPinEdit->setPosition(pPinText->getPosition());
	m_pPinEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pPinEdit->setFontColor(pPinText->getTextColor());
	m_pPinEdit->setFontSize(pPinText->getFontSize());
	m_pPinEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pPinEdit->setPlaceHolder(OTP_TEXT(20002));
	m_pPinEdit->setDelegate(this);
	m_pRoot->addChild(m_pPinEdit, 0, "Pin");

	m_pRePinEdit = ui::EditBox::create(pRePinText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pRePinEdit->setAnchorPoint(pRePinText->getAnchorPoint());
	m_pRePinEdit->setPosition(pRePinText->getPosition());
	m_pRePinEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pRePinEdit->setFontColor(pRePinText->getTextColor());
	m_pRePinEdit->setFontSize(pRePinText->getFontSize());
	m_pRePinEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pRePinEdit->setPlaceHolder(OTP_TEXT(20004));
	m_pRePinEdit->setDelegate(this);
	m_pRoot->addChild(m_pRePinEdit, 0, "RePin");

    UpdatePublicKey();
	return true;
}

void ExchangeInfoLayer::onExit()
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_DISCONN);
	Node::onExit();
}

void ExchangeInfoLayer::UpdatePublicKey()
{
    if( m_pPublicKeyText == nullptr )
    {
        return;
    }
    
	if (m_bOKPush)
	{
		std::string strHashKey = DataManager::GetInstance()->GetTimeHash();
		Send_Hash(strHashKey.substr(0, 5));
		m_pPublicKeyText->setString(strHashKey.substr(0, 25));
	}
	else
	{
		SetUILabelText(m_pRoot, "Image_5", "Text_11", EventManager::GetInstance()->IsBleConn() ? 50014 : 50015);
	}
}

void ExchangeInfoLayer::Send_Hash(std::string strHash)
{
	EventManager::GetInstance()->BleTx(strHash, 6);
}

void ExchangeInfoLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_HOME_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void ExchangeInfoLayer::Send_ExchangeHistory()
{
    if (m_pPinEdit == nullptr || m_pRePinEdit == nullptr)
    {
        return;
    }
    
    if (strlen(m_pPinEdit->getText()) < 4)
    {
        CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(30008), SMessageBox::eMESSAGEBOX_TYPE::OK)
        return;
    }
    
    if (strcmp(m_pPinEdit->getText(), m_pRePinEdit->getText()) != 0)
    {
        CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(30007), SMessageBox::eMESSAGEBOX_TYPE::OK)
        return;
    }
    
    sUSER_DATA userData;
    DataManager::GetInstance()->CopyUserData(userData);
    HttpManager::GetInstance()->Send_ReqExchanageHistory(userData.u64Seq, CC_CALLBACK_2(ExchangeInfoLayer::onHttpRequest_ReqExchgeHistory, this));
}

void ExchangeInfoLayer::menuCallback_BleConn(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_CONN);
}

void ExchangeInfoLayer::menuCallback_Inquiry(cocos2d::Ref* sender)
{
	if (m_bOKPush == false)
		EventManager::GetInstance()->BleTx(m_pPinEdit->getText(), 4);
	else
		Send_ExchangeHistory();
}
                                                  
void ExchangeInfoLayer::onHttpRequest_ReqExchgeHistory(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
{
	m_bOKPush = false;

    DESTROY_PROGRESSDIALOG
    if (!response)
    {
        return;
    }
    long statusCode = response->getResponseCode();
    
    cocos2d::log("response code: %ld", statusCode);
    
    if (!response->isSucceed())
    {
        CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50002), SMessageBox::eMESSAGEBOX_TYPE::OK)
        cocos2d::log("response failed");
        cocos2d::log("error buffer: %s", response->getErrorBuffer());
        return;
    }
    
    std::vector<char> *pData = response->getResponseData();
    if (pData->size() <= 0)
    {
        return;
    }
    
    std::string ret(&(pData->front()), pData->size());
    cocos2d::log("%s", ("Response message: " + ret).c_str());
    
    rapidjson::Document jsonDoc;
    jsonDoc.Parse(ret.c_str());
    for (rapidjson::Value::ConstValueIterator itr = jsonDoc.Begin(); itr != jsonDoc.End(); ++itr)
    {
        sEXCHANGE_HISTORY_DATA sExchangeHistoryData;
        sExchangeHistoryData.unPurchaseSeq        = (*itr)["purchaseSeq"].GetUint();
        sExchangeHistoryData.strBidCurrency       = (*itr)["bidCurrency"].GetString();
        sExchangeHistoryData.tPurchase            = (*itr)["dtpurchase"].GetString();
        sExchangeHistoryData.fBidAmt              = (*itr)["bidAmt"].GetDouble();
        sExchangeHistoryData.strPayCurrency       = (*itr)["payCurrency"].GetString();
        sExchangeHistoryData.fPayAmount           = (*itr)["payAmount"].GetDouble();
		if ((*itr)["statusDesc"].IsNull() == false)
		{
			sExchangeHistoryData.strStatusDesc = (*itr)["statusDesc"].GetString();
		}
		sExchangeHistoryData.strStatus			  = (*itr)["status"].GetString();
		
        DataManager::GetInstance()->AddExchangeHisToryList(sExchangeHistoryData);
    }
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_EXCHANGE_INFO_LIST_LAYER_EVNET);
    runAction(RemoveSelf::create());
}

void ExchangeInfoLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{
}

void ExchangeInfoLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{
}

void ExchangeInfoLayer::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox)
{
}

void ExchangeInfoLayer::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{
}
