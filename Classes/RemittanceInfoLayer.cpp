#include "RemittanceInfoLayer.h"
#include "Define.h"
#include "HttpManager.h"
#include "EventManager.h"
#include "UIHelper.h"

USING_NS_CC;
using namespace ui;

bool TradeInfoLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	m_pRoot = CSLoader::createNode("TradeInfoLayer.csb");
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

	pBackButton->addClickEventListener(CC_CALLBACK_1(TradeInfoLayer::menuCallback_Back, this));

	Button* pInquiryButton = static_cast<Button*>(m_pRoot->getChildByName("InquiryButton"));
	if (pInquiryButton == nullptr)
	{
		return false;
	}

	pInquiryButton->addClickEventListener(CC_CALLBACK_1(TradeInfoLayer::menuCallback_Inquiry, this));

	Button* pBleConnButton = dynamic_cast<Button*>(m_pRoot->getChildByName("BleConnButton"));
	if (pBleConnButton == nullptr)
	{
		return false;
	}

	pBleConnButton->addClickEventListener(CC_CALLBACK_1(TradeInfoLayer::menuCallback_BleConn, this));
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	pBleConnButton->setVisible(false);
	pBleConnButton->setEnabled(false);
	pBleConnButton->setTouchEnabled(false);
#endif

	Node* ImageRoot = m_pRoot->getChildByName("Image_5");
	if (ImageRoot)
	{
		m_pPublicKeyText = dynamic_cast<ui::Text*>(ImageRoot->getChildByName("KeyLabel"));
		SetUILabelText(ImageRoot, "Text_11", 9006);
	}
	SetUILabelText(m_pRoot->getChildByName("Image_5_0"), "Text_8_0", 9005);
    
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
	m_pPinEdit->setPlaceHolder(OTP_TEXT(9002));
	m_pPinEdit->setMaxLength(4);
	m_pPinEdit->setDelegate(this);
	m_pRoot->addChild(m_pPinEdit, 0, "Pin");

	m_pRePinEdit = ui::EditBox::create(pRePinText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pRePinEdit->setAnchorPoint(pRePinText->getAnchorPoint());
	m_pRePinEdit->setPosition(pRePinText->getPosition());
	m_pRePinEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pRePinEdit->setFontColor(pRePinText->getTextColor());
	m_pRePinEdit->setFontSize(pRePinText->getFontSize());
	m_pRePinEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pRePinEdit->setPlaceHolder(OTP_TEXT(9004));
	m_pRePinEdit->setMaxLength(4);
	m_pRePinEdit->setDelegate(this);
	m_pRoot->addChild(m_pRePinEdit, 0, "RePin");

    UpdatePublicKey();
	return true;
}

void TradeInfoLayer::onExit()
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_DISCONN);
	Node::onExit();
}

void TradeInfoLayer::UpdatePublicKey()
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

void TradeInfoLayer::Send_Hash(std::string strHash)
{
	EventManager::GetInstance()->BleTx(strHash, 6);
}

void TradeInfoLayer::menuCallback_BleConn(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_CONN);
}

void TradeInfoLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_HOME_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void TradeInfoLayer::Send_RtxHistory()
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
    
    sBANKACCOUNT_DATA userBackData;
    DataManager::GetInstance()->CopyBankAccountsData( userBackData, 1 );
    HttpManager::GetInstance()->Send_TxHist(0, userBackData.u64AccntSeq, CC_CALLBACK_2(TradeInfoLayer::onHttpRequest_TxHist, this));
}

void TradeInfoLayer::menuCallback_Inquiry(cocos2d::Ref* sender)
{
	if (m_bOKPush == false)
		EventManager::GetInstance()->BleTx(m_pPinEdit->getText(), 4);
	else
		Send_RtxHistory();
}

void TradeInfoLayer::onHttpRequest_TxHist(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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
        sRTX_HISTORY_DATA sRtxHistoryData;
        sRtxHistoryData.nTransferSeq        = (*itr)["transferSeq"].GetUint();
        sRtxHistoryData.nSendAccnt          = (*itr)["sendAccnt"].GetUint();
        sRtxHistoryData.tSendTime           = (*itr)["dtsend"].GetString();
        sRtxHistoryData.strMemo             = (*itr)["sendMemo"].GetString();
        sRtxHistoryData.fSendAmt            = (*itr)["sendAmt"].GetDouble();
        sRtxHistoryData.strMessage          = (*itr)["statusDesc"].GetString();
        sRtxHistoryData.strSendAccntName    = (*itr)["sendAccntName"].GetString();
        sRtxHistoryData.strSendCurrency     = (*itr)["sendCurrency"].GetString();
        sRtxHistoryData.strReceiveAccntName = (*itr)["receiveAccntName"].GetString();
        DataManager::GetInstance()->AddRtxHisToryList(sRtxHistoryData);
    }

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_TRADE_INFO_LIST_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void TradeInfoLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{
}

void TradeInfoLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{
}

void TradeInfoLayer::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox)
{
}

void TradeInfoLayer::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{
}
