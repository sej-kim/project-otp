#include "RemittanceLayer.h"
#include "Define.h"
#include "UIHelper.h"
#include "HttpManager.h"
#include "EventManager.h"

USING_NS_CC;
using namespace ui;

bool RemittanceLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	m_pRoot = CSLoader::createNode("RemittanceLayer.csb");
	if (m_pRoot == nullptr)
	{
		return false;
	}
	addChild(m_pRoot);

	ImageView* pTextImageView_1 = dynamic_cast<ImageView*>(m_pRoot->getChildByName("Image_5_0"));
	ImageView* pTextImageView_2 = dynamic_cast<ImageView*>(m_pRoot->getChildByName("Image_5"));
	if (pTextImageView_1 == nullptr || pTextImageView_2 == nullptr)
	{
		return false;
	}

	m_pPublicKeyText = dynamic_cast<Text*>(pTextImageView_2->getChildByName("KeyLabel"));
	if (m_pPublicKeyText == nullptr)
	{
		return false;
	}

	Button* pRemittanceButton = dynamic_cast<Button*>(m_pRoot->getChildByName("RemittanceButton"));
	if (pRemittanceButton == nullptr)
	{
		return false;
	}
	pRemittanceButton->addClickEventListener(CC_CALLBACK_1(RemittanceLayer::menuCallback_Remittance, this));

	Button* pBackButton = dynamic_cast<Button*>(m_pRoot->getChildByName("BackButton"));
	if (pBackButton == nullptr)
	{
		return false;
	}
	pBackButton->addClickEventListener(CC_CALLBACK_1(RemittanceLayer::menuCallback_Back, this));

	Button* pBleConnButton = dynamic_cast<Button*>(m_pRoot->getChildByName("BleConnButton"));
	if (pBleConnButton == nullptr)
	{
		return false;
	}
	pBleConnButton->addClickEventListener(CC_CALLBACK_1(RemittanceLayer::menuCallback_BleConn, this));

	Button* pCancleButton = dynamic_cast<Button*>(m_pRoot->getChildByName("CnacleButton"));
	if (pCancleButton == nullptr)
	{
		return false;
	}
	pCancleButton->addClickEventListener(CC_CALLBACK_1(RemittanceLayer::menuCallback_Back, this));

	
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	pBleConnButton->setVisible(false);
	pBleConnButton->setEnabled(false);
	pBleConnButton->setTouchEnabled(false);
#endif

	Button* pAccountCheck = dynamic_cast<Button*>(m_pRoot->getChildByName("Button_4"));
	if (pAccountCheck == nullptr)
	{
		return false;
	}
	
	pAccountCheck->addClickEventListener(CC_CALLBACK_1(RemittanceLayer::menuCallback_GetAccountInfo, this));

	Text* pIDText		= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("IDTextField"));
	Text* pBTCTextText	= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("BTTextField"));
	Text* pPinText		= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("PinTextField"));
	Text* pRePinText	= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("RePinTextField"));
    Text* pMemoText     = dynamic_cast<ui::Text*>(m_pRoot->getChildByName("MemoTextField"));
	if (pIDText == nullptr || pBTCTextText == nullptr || pPinText == nullptr || pRePinText == nullptr || pMemoText == nullptr)
	{
		return false;
	}

	m_pIDEdit = ui::EditBox::create(pIDText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pIDEdit->setAnchorPoint(pIDText->getAnchorPoint());
	m_pIDEdit->setPosition(pIDText->getPosition());
	m_pIDEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pIDEdit->setFontColor(pIDText->getTextColor());
	m_pIDEdit->setFontSize(pIDText->getFontSize());
	m_pIDEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pIDEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pIDEdit->setPlaceHolder(OTP_TEXT(7002));
	m_pIDEdit->setDelegate(this);
	m_pRoot->addChild(m_pIDEdit, 0, "ID");

	m_pBTCEdit = ui::EditBox::create(pBTCTextText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pBTCEdit->setAnchorPoint(pBTCTextText->getAnchorPoint());
	m_pBTCEdit->setPosition(pBTCTextText->getPosition());
	m_pBTCEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pBTCEdit->setFontColor(pBTCTextText->getTextColor());
	m_pBTCEdit->setFontSize(pBTCTextText->getFontSize());
	m_pBTCEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pBTCEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pBTCEdit->setPlaceHolder(OTP_TEXT(7004));
	m_pBTCEdit->setDelegate(this);
	m_pRoot->addChild(m_pBTCEdit, 0, "BTC");

    m_pMemodit = ui::EditBox::create(pMemoText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
    m_pMemodit->setAnchorPoint(pMemoText->getAnchorPoint());
    m_pMemodit->setPosition(pMemoText->getPosition());
    m_pMemodit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
    m_pMemodit->setFontColor(pMemoText->getTextColor());
    m_pMemodit->setFontSize(pMemoText->getFontSize());
    m_pMemodit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
    m_pMemodit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
    m_pMemodit->setPlaceHolder(OTP_TEXT(7016));
    m_pMemodit->setDelegate(this);
	m_pRoot->addChild(m_pMemodit, 0, "BTC");
    
	m_pPinEdit = ui::EditBox::create(pPinText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pPinEdit->setAnchorPoint(pPinText->getAnchorPoint());
	m_pPinEdit->setPosition(pPinText->getPosition());
	m_pPinEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pPinEdit->setFontColor(pPinText->getTextColor());
	m_pPinEdit->setFontSize(pPinText->getFontSize());
	m_pPinEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pPinEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pPinEdit->setPlaceHolder(OTP_TEXT(7006));
	m_pPinEdit->setMaxLength(4);
	m_pPinEdit->setDelegate(this);
	m_pRoot->addChild(m_pPinEdit, 0, "Pin");

	m_pRePinEdit = ui::EditBox::create(pRePinText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pRePinEdit->setAnchorPoint(pRePinText->getAnchorPoint());
	m_pRePinEdit->setPosition(pRePinText->getPosition());
	m_pRePinEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pRePinEdit->setFontColor(pRePinText->getTextColor());
	m_pRePinEdit->setFontSize(pRePinText->getFontSize());
	m_pRePinEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pRePinEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pRePinEdit->setPlaceHolder(OTP_TEXT(7008));
	m_pRePinEdit->setMaxLength(4);
	m_pRePinEdit->setDelegate(this);
	m_pRoot->addChild(m_pRePinEdit, 0, "RePin");

	SetUILabelText(m_pRoot, "Text_1", 7000);
	SetUILabelText(m_pRoot, "Text_5", 7001);
	SetUILabelText(m_pRoot, "Text_5_0", 7003);
	SetUILabelText(m_pRoot, "Text_5_0_0", 7005);
	SetUILabelText(m_pRoot, "Text_5_0_0_0", 7007);
	SetUILabelText(m_pRoot, "Text_5_0_1", 7015);
    SetUILabelText(pTextImageView_1, "Text_8_0", 7009);
    SetUILabelText(pTextImageView_2, "Text_11", 7010);

	SetUIButtonText(m_pRoot, "RemittanceButton", 7011);
    UpdatePublicKey();
	return true;
}

void RemittanceLayer::onExit()
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_DISCONN);
	Node::onExit();
}

void RemittanceLayer::UpdatePublicKey()
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

void RemittanceLayer::Send_Ble(int nIndex)
{
	if (nIndex == 1)
	{
		Send_UserName();
	}
	else if (nIndex == 2)
	{
		runAction(Sequence::create(DelayTime::create(1), CallFunc::create(CC_CALLBACK_0(RemittanceLayer::Send_SendAccNo, this)), NULL));
	}
	else if (nIndex == 3)
	{
		runAction(Sequence::create(DelayTime::create(1), CallFunc::create(CC_CALLBACK_0(RemittanceLayer::Send_Amount, this)), NULL));
	}
}

void RemittanceLayer::Send_UserName()
{
	EventManager::GetInstance()->BleTx(m_AccntName, 5);
}

void RemittanceLayer::Send_SendAccNo()
{
    EventManager::GetInstance()->BleTx(m_pIDEdit->getText(), 7);
}

void RemittanceLayer::Send_Hash( std::string strHash )
{
	EventManager::GetInstance()->BleTx(strHash, 6);
}

void RemittanceLayer::Send_Amount()
{
	EventManager::GetInstance()->BleTx(m_pBTCEdit->getText(), 6);
}

void RemittanceLayer::Send_Rtx()
{
    sBANKACCOUNT_DATA sBakData;
    DataManager::GetInstance()->CopyBankAccountsData( sBakData, 1);
    
    char* ptr;
    unsigned int unReceiveAccnt = (unsigned int)strtoul(m_pIDEdit->getText(), &ptr, 10);
    float fSendAmt              = strtof(m_pBTCEdit->getText(), &ptr);
    HttpManager::GetInstance()->Send_ReqTx(unReceiveAccnt, sBakData.u64AccntSeq, fSendAmt, m_pMemodit->getText(), CC_CALLBACK_2(RemittanceLayer::onHttpRequest_ReqTx, this));

}

void RemittanceLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_HOME_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void RemittanceLayer::menuCallback_BleConn(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_CONN);
}

void RemittanceLayer::Ok_CallBack()
{
	EventManager::GetInstance()->BleTx(m_pPinEdit->getText(), 4);
}

void RemittanceLayer::menuCallback_Remittance(cocos2d::Ref* sender)
{
	if (m_pPinEdit == nullptr || m_pRePinEdit == nullptr || m_pIDEdit == nullptr || m_pBTCEdit == nullptr || m_pMemodit == nullptr)
	{
		return;
	}

	if (m_strAccount.length() <= 0)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50010), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	if (m_unSenduserSeq <= 0)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50012), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}
	
	if (m_strPin.length() < 4)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(30008), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	if (strcmp(m_strPin.c_str(), m_pRePinEdit->getText()) != 0)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(30007), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	if (strlen(m_pBTCEdit->getText()) <= 0)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(30007), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	if (m_bOKPush)
	{
		CREATE_MESSAGE_CALLBACK(OTP_TEXT(50008), OTP_TEXT(7017), SMessageBox::eMESSAGEBOX_TYPE::YES_NO, CC_CALLBACK_0(RemittanceLayer::Send_Rtx, this))
	}
	else
	{
		CREATE_MESSAGE_CALLBACK(OTP_TEXT(50008), OTP_TEXT(50020), SMessageBox::eMESSAGEBOX_TYPE::OK, CC_CALLBACK_0(RemittanceLayer::Ok_CallBack, this))
	}
}

void RemittanceLayer::onHttpRequest_ReqTx(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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

    std::string strResult = jsonDoc["result"].GetString();
    if (strResult.compare("SUCCESS") != 0)
    {
        CREATE_MESSAGE(OTP_TEXT(50001), jsonDoc["msg"].GetString(), SMessageBox::eMESSAGEBOX_TYPE::OK)
        return;
    }
    
    rapidjson::Value& DataValue = jsonDoc["data"];
    
    sRTX_RESULT_DATA sRtxResultData;
    sRtxResultData.strReceiveName    = DataValue["receiverAccntName"].GetString();
    sRtxResultData.tSendTime         = DataValue["dtsend"].GetString();
    sRtxResultData.fAmtSent          = DataValue["amtSent"].GetDouble();
    sRtxResultData.fSenderBalance    = DataValue["senderBalance"].GetDouble();

	sBANKACCOUNT_DATA BankData;
	DataManager::GetInstance()->CopyBankAccountsData(BankData, 1);
	BankData.dAmt = sRtxResultData.fSenderBalance;
	DataManager::GetInstance()->AddankAccountsData(BankData);

    DataManager::GetInstance()->SetRtxResultData(sRtxResultData);
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_REMITTANCE_HOME_LAYER_EVNET);
    runAction(RemoveSelf::create());
}

void RemittanceLayer::menuCallback_GetAccountInfo(cocos2d::Ref* sender)
{
	if (m_strAccount.length() <= 0)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50010), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	char* pPtr;
	unsigned int unAccount = (unsigned int)std::strtoul(m_strAccount.c_str(), &pPtr, 10);
	HttpManager::GetInstance()->Send_GetGetAccountInfo(unAccount, CC_CALLBACK_2(RemittanceLayer::onHttpRequest_GetAccountInfo, this));
}

void RemittanceLayer::onHttpRequest_GetAccountInfo(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
{
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
		return;
	}

	std::vector<char> *pData = response->getResponseData();
	if (pData->size() <= 0)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50010), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	std::string ret(&(pData->front()), pData->size());
	cocos2d::log("%s", ("Response message: " + ret).c_str());

	rapidjson::Document jsonDoc;
	jsonDoc.Parse(ret.c_str());

	std::string strResult = jsonDoc["result"].GetString();
	if (strResult.compare("SUCCESS") != 0)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), jsonDoc["msg"].GetString(), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	m_AccntName		= jsonDoc["accntName"].GetString();
	m_unSenduserSeq = jsonDoc["userSeq"].GetUint();

	CREATE_MESSAGE(OTP_TEXT(50008), OTP_TEXT(50013), SMessageBox::eMESSAGEBOX_TYPE::OK)
}

void RemittanceLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{
}

void RemittanceLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{
}

void RemittanceLayer::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox)
{
}
void RemittanceLayer::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{
	if (pEditBox == nullptr)
	{
		return;
	}
	
	if (pEditBox->getName().compare("Pin") == 0)
	{
		m_strPin = pEditBox->getText();
	}
	else if (pEditBox->getName().compare("ID") == 0)
	{
		if (m_strAccount.compare(pEditBox->getText()) != 0)
		{
			m_AccntName = "";
			m_unSenduserSeq = 0;
		}

		m_strAccount = pEditBox->getText();
	}
}
