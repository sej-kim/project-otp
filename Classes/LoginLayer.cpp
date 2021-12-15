#include "LoginLayer.h"
#include "Define.h"
#include "DataManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

#include "HttpManager.h"
#include "UIHelper.h"

USING_NS_CC;
using namespace ui;

bool LoginLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	Node* pRoot = CSLoader::createNode("LoginLayer.csb");
	if (pRoot == nullptr)
	{
		return false;
	}
	addChild(pRoot);

	Button* pLoginButton = static_cast<Button*>(pRoot->getChildByName("LoginButton"));
	if (pLoginButton == nullptr)
	{
		return false;
	}

	pLoginButton->addClickEventListener(CC_CALLBACK_1(LoginLayer::menuCallback_Login, this));

	Button* pBackButton = static_cast<Button*>(pRoot->getChildByName("BackButton"));
	if (pBackButton == nullptr)
	{
		return false;
	}

	pBackButton->addClickEventListener(CC_CALLBACK_1(LoginLayer::menuCallback_Back, this));

	Text* pIDTextField = dynamic_cast<ui::Text*>(pRoot->getChildByName("UserNameTextField"));
	if (pIDTextField == nullptr)
	{
		return false;
	}

	m_pUserIDEdit = ui::EditBox::create(pIDTextField->getContentSize(), ui::Scale9Sprite::create("img/LineImage.png"));
	m_pUserIDEdit->setAnchorPoint(pIDTextField->getAnchorPoint());
	m_pUserIDEdit->setPosition(pIDTextField->getPosition());
	m_pUserIDEdit->setFontColor(pIDTextField->getTextColor());
	m_pUserIDEdit->setFontSize(pIDTextField->getFontSize());
	m_pUserIDEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pUserIDEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pUserIDEdit->setMaxLength(ID_MAX_LEN);
	m_pUserIDEdit->setPlaceHolder(OTP_TEXT(2002));
	m_pUserIDEdit->setDelegate(this);
	pRoot->addChild(m_pUserIDEdit, 0, "UserID");

	SetUILabelText(pRoot, "Text_1", 2000);
	SetUILabelText(pRoot, "Text_2", 2001);
	SetUILabelText(pRoot, "Text_3", 2006);
	
	return true;
}

void LoginLayer::menuCallback_Login(cocos2d::Ref* sender)
{
	if (m_pUserIDEdit == nullptr)
	{
		return;
	}

	if (strlen(m_pUserIDEdit->getText()) < 4)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50005), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	HttpManager::GetInstance()->Send_GetUserInfo(m_pUserIDEdit->getText(), CC_CALLBACK_2(LoginLayer::onHttpRequest_GetUserInfo, this));
}

void LoginLayer::onHttpRequest_GetUserInfo(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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

	sUSER_DATA sUserData;
	DataManager::GetInstance()->CopyUserData(sUserData);
	sUserData.u64Seq		= jsonDoc["userSeq"].GetUint64(); 
	sUserData.strUserID		= jsonDoc["id"].GetString();
	sUserData.strDtreg		= jsonDoc["dtreg"].GetString();
	sUserData.strUserName	= jsonDoc["userName"].GetString();
	sUserData.strPublicKey  = jsonDoc["publicKey"].GetString();
    sUserData.strSurveyHash  = jsonDoc["surveyHash"].GetString();
    
	
	DataManager::GetInstance()->SetUserData(sUserData);

	HttpManager::GetInstance()->Send_GetBackAccounts(sUserData.u64Seq, CC_CALLBACK_2(LoginLayer::onHttpRequest_GetBankAccounts, this));
}

void LoginLayer::onHttpRequest_GetBankAccounts(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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
	for (rapidjson::Value::ConstValueIterator itr = jsonDoc.Begin(); itr != jsonDoc.End(); ++itr)
	{
		sBANKACCOUNT_DATA sBankAccountData;
		sBankAccountData.u64AccntSeq	= (*itr)["accntSeq"].GetUint64();
		sBankAccountData.strCurrency	= (*itr)["currency"].GetString();
		sBankAccountData.strAccntName	= (*itr)["accntName"].GetString();
		sBankAccountData.dAmt			= (*itr)["amt"].GetDouble();
		sBankAccountData.u64UserSeq		= (*itr)["userSeq"].GetUint64();
		DataManager::GetInstance()->AddankAccountsData(sBankAccountData);
	}

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_HOME_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void LoginLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_START_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void LoginLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{
}

void LoginLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{
}

void LoginLayer::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox)
{
}

void LoginLayer::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{
}
