#include "CardRegisterLayer.h"
#include "Define.h"
#include "UIHelper.h"
#include "HttpManager.h"
#include "EventManager.h"
#define RESIST_PAGE 4

USING_NS_CC;
using namespace ui;

bool CardRegisterLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	m_pRoot = CSLoader::createNode("CardRegisterLayer.csb");
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

	pBackButton->addClickEventListener(CC_CALLBACK_1(CardRegisterLayer::menuCallback_Back, this));

	SetUpPageUI(1);

	SetUILabelText(m_pRoot, "Text_1", 6000);
	SetUILabelText(m_pRoot, "Text_2", 6001);
	return true;
}

void CardRegisterLayer::onExit()
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_DISCONN);
	Node::onExit();
}

void CardRegisterLayer::UpdatePublicKey()
{
	if (m_pPublicKeyText == nullptr)
	{
		return;
	}
	std::string strHashKey = DataManager::GetInstance()->GetSurVeryHash();
	Send_Hash(strHashKey.substr(0, 5));
	m_pPublicKeyText->setString(strHashKey.substr(0, 25));
}

void CardRegisterLayer::Send_Hash(std::string strHash)
{
	EventManager::GetInstance()->BleTx(strHash, 6);
}

void CardRegisterLayer::SetUpPageUI(int nPage)
{
	if (m_pRoot == nullptr || nPage > RESIST_PAGE)
	{
		return;
	}

	m_bOKPush = false;

	if (m_pPage != nullptr)
	{
		m_pRoot->removeChild(m_pPage);
		m_pPage = nullptr;
		m_pPublicKeyText = nullptr;
	}
	
	m_nCurPage = nPage;


	char szTemp[128] = { 0, };
	sprintf(szTemp, "CardRegisterPageLayer_%d.csb", nPage);

	m_pPage = CSLoader::createNode(szTemp);
	if (m_pPage == nullptr)
	{
		return;
	}

	if (nPage == RESIST_PAGE)
	{
		Text* pPinText		= dynamic_cast<ui::Text*>(m_pPage->getChildByName("PinTextField"));
		Text* pRePinText	= dynamic_cast<ui::Text*>(m_pPage->getChildByName("RePinTextField"));
		Text* pPublicKey	= dynamic_cast<ui::Text*>(m_pPage->getChildByName("Text_5"));
		if (pPinText == nullptr || pRePinText == nullptr || pPublicKey == nullptr)
		{
			return;
		}

		m_pPublicKeyText = pPublicKey;
		m_pPublicKeyText->setVisible(true);

		m_pPinEdit = ui::EditBox::create(pPinText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
		m_pPinEdit->setAnchorPoint(pPinText->getAnchorPoint());
		m_pPinEdit->setPosition(pPinText->getPosition());
		m_pPinEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		m_pPinEdit->setFontColor(pPinText->getTextColor());
		m_pPinEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
		m_pPinEdit->setPlaceHolder(OTP_TEXT(6010));
		m_pPinEdit->setMaxLength(4);
		m_pPinEdit->setDelegate(this);
		m_pPage->addChild(m_pPinEdit, 0, "Pin");

		m_pRePinEdit = ui::EditBox::create(pRePinText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
		m_pRePinEdit->setAnchorPoint(pRePinText->getAnchorPoint());
		m_pRePinEdit->setPosition(pRePinText->getPosition());
		m_pRePinEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
		m_pRePinEdit->setFontColor(pRePinText->getTextColor());
		m_pRePinEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
		m_pRePinEdit->setPlaceHolder(OTP_TEXT(6012));
		m_pRePinEdit->setMaxLength(4);
		m_pRePinEdit->setDelegate(this);
		m_pPage->addChild(m_pRePinEdit, 0, "RePin");

		Button* pRegistButton = static_cast<Button*>(m_pPage->getChildByName("RegisterButton"));
		if (pRegistButton != nullptr)
		{
			pRegistButton->addClickEventListener(CC_CALLBACK_1(CardRegisterLayer::menuCallback_Regist, this));
			SetUIButtonText(m_pPage, "RegisterButton", 6013);
		}

		SetUILabelText(m_pPage, "Text_3", 6007);
		SetUILabelText(m_pPage, "Text_4", 6008);
		SetUILabelText(m_pPage, "Text_6", 6009);
		SetUILabelText(m_pPage, "Text_6_0", 6011);

		EventManager::GetInstance()->BleTx("OK", 2);
	}
	else
	{
		Button* pOK_Button = static_cast<Button*>(m_pPage->getChildByName("OK_Button"));
		if (pOK_Button != nullptr)
		{
			pOK_Button->addClickEventListener(CC_CALLBACK_1(CardRegisterLayer::menuCallback_OK, this));
			SetUIButtonText(m_pPage, "OK_Button", 6004);
		}

		switch (nPage)
		{
		case 1:
		{
			SetUILabelText(m_pPage, "Text_5", 6002);
		}
			break;
		case 2:
		{
			SetUILabelText(m_pPage, "Text_5", 6003);
		}
			break;
		case 3:
		{
			SetUILabelText(m_pPage, "Text_3", 6005);
			SetUILabelText(m_pPage, "Text_4", 6006);
			sUSER_DATA userData;
			DataManager::GetInstance()->CopyUserData(userData);
			
			EventManager::GetInstance()->BleTx(userData.strUserID.c_str(), 1);
		}
			break;
		default:
			break;
		}
		
	}

	m_pRoot->addChild(m_pPage);
}

void CardRegisterLayer::SetSerialNumber(std::string strSerial)
{
	m_strSerialKey = strSerial;
	DataManager::GetInstance()->SetCardSerial(strSerial);
}

void CardRegisterLayer::NextPage()
{
	SetUpPageUI(m_nCurPage + 1);
}

void CardRegisterLayer::RunMainPage()
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_START_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void CardRegisterLayer::Send_Regist()
{
	sUSER_DATA data;
	DataManager::GetInstance()->CopyUserData(data);
	std::string strSurveyHash = DataManager::GetInstance()->GetSurVeryHash();
	HttpManager::GetInstance()->Send_RegCiphercard(m_strSerialKey, TEXT_PUBLIC_KEY_1, strSurveyHash, data.strUserID, data.strUserName, CC_CALLBACK_2(CardRegisterLayer::onHttpRequest_RegCiphercard, this));
}

void CardRegisterLayer::onHttpRequest_RegCiphercard(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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
    std::string strResult = jsonDoc["actionReesultCode"].GetString();
    if (strResult.compare("SUCCESS") != 0)
    {
        CREATE_MESSAGE(OTP_TEXT(50001), jsonDoc["actionResultMsg"].GetString(), SMessageBox::eMESSAGEBOX_TYPE::OK)
        return;
    }
    
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_LOGIN_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void CardRegisterLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_START_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void CardRegisterLayer::menuCallback_OK(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_CONN);
	NextPage();
}

void CardRegisterLayer::menuCallback_Regist(cocos2d::Ref* sender)
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

	if (m_bOKPush)
	{
		CREATE_MESSAGE_CALLBACK(OTP_TEXT(50008), OTP_TEXT(6015), SMessageBox::eMESSAGEBOX_TYPE::YES_NO, CC_CALLBACK_0(CardRegisterLayer::Send_Regist, this))
	}
	else
	{
		EventManager::GetInstance()->BleTx(m_pPinEdit->getText(), 3);
	}
	
}

void CardRegisterLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{
}

void CardRegisterLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{
}

void CardRegisterLayer::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox)
{
}

void CardRegisterLayer::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{
}
