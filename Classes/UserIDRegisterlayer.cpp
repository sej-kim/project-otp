#include "UserIDRegisterlayer.h"
#include "Define.h"
#include "HttpManager.h"
#include "DataManager.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "UIHelper.h"

USING_NS_CC;
using namespace ui;

bool UserIDRegisterLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	Node* pRoot = CSLoader::createNode("UserIDRegisterLayer.csb");
	if (pRoot == nullptr)
	{
		return false;
	}
	addChild(pRoot);

	Button* pOK_Button = dynamic_cast<Button*>(pRoot->getChildByName("OK_Button"));
	if (pOK_Button == nullptr)
	{
		return false;
	}

	pOK_Button->addClickEventListener(CC_CALLBACK_1(UserIDRegisterLayer::menuCallback_ID_OK, this));

	Button* pBackButton = dynamic_cast<Button*>(pRoot->getChildByName("BackButton"));
	if (pBackButton == nullptr)
	{
		return false;
	}

	pBackButton->addClickEventListener(CC_CALLBACK_1(UserIDRegisterLayer::menuCallback_Back, this));

	Text* pIDTextField = dynamic_cast<ui::Text*>(pRoot->getChildByName("IDTextField"));
	Text* pReIDTextField = dynamic_cast<ui::Text*>(pRoot->getChildByName("ReIDTextField"));
	if (pIDTextField == nullptr || pReIDTextField == nullptr)
	{
		return false;
	}

	m_pUserIDEdit = ui::EditBox::create(pIDTextField->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pUserIDEdit->setAnchorPoint(pIDTextField->getAnchorPoint());
	m_pUserIDEdit->setPosition(pIDTextField->getPosition());
	m_pUserIDEdit->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
	m_pUserIDEdit->setFontColor(pIDTextField->getTextColor());
	m_pUserIDEdit->setFontSize(pIDTextField->getFontSize());
	m_pUserIDEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pUserIDEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pUserIDEdit->setPlaceHolder(OTP_TEXT(4003));
	m_pUserIDEdit->setDelegate(this);
	pRoot->addChild(m_pUserIDEdit, 0, "UserID");

	m_pReUserIDEdit = ui::EditBox::create(pReIDTextField->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pReUserIDEdit->setAnchorPoint(pReIDTextField->getAnchorPoint());
	m_pReUserIDEdit->setPosition(pReIDTextField->getPosition());
	m_pReUserIDEdit->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
	m_pReUserIDEdit->setFontColor(pReIDTextField->getTextColor());
	m_pReUserIDEdit->setFontSize(pReIDTextField->getFontSize());
	m_pReUserIDEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pReUserIDEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pReUserIDEdit->setPlaceHolder(OTP_TEXT(4005));
	m_pReUserIDEdit->setDelegate(this);
	pRoot->addChild(m_pReUserIDEdit, 0, "ReUserID");

	SetUILabelText(pRoot, "Text_3", 4000);
	SetUILabelText(pRoot, "Text_4", 4001);
	SetUILabelText(pRoot, "Text_1", 4002);
	SetUILabelText(pRoot, "Text_2", 4004);

	SetUIButtonText(pRoot, "OK_Button", 4006);
	return true;
}

void UserIDRegisterLayer::menuCallback_ID_OK(cocos2d::Ref* sender)
{
	if (m_pUserIDEdit == nullptr || m_pReUserIDEdit == nullptr)
	{
		return;
	}

	if (strlen(m_pUserIDEdit->getText()) <= 1)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50000), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	if (strcmp(m_pUserIDEdit->getText(), m_pReUserIDEdit->getText()) != 0)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50003), SMessageBox::eMESSAGEBOX_TYPE::OK)
		return;
	}

	HttpManager::GetInstance()->Send_IDCheck(m_pUserIDEdit->getText(), CC_CALLBACK_2(UserIDRegisterLayer::onHttpRequest_IDCheck, this));
}

void UserIDRegisterLayer::onHttpRequest_IDCheck(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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

	sUSER_DATA userData;
	DataManager::GetInstance()->CopyUserData(userData);
	userData.strUserID = m_pUserIDEdit->getText();
	DataManager::GetInstance()->SetUserData(userData);

	if (DataManager::GetInstance()->GetNationList()->empty())
	{
		HttpManager::GetInstance()->Send_GetCodeList("regA", "", CC_CALLBACK_2(UserIDRegisterLayer::onHttpRequest_GetCodeList, this));
		return;
	}

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_SURVERY_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void UserIDRegisterLayer::onHttpRequest_GetCodeList(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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
		sCODE_DATA dat;

		dat.nCodeSeq	= (*itr)["codeSeq"].GetInt();
		dat.strType		= (*itr)["cateType"].GetString();
		dat.strCode		= (*itr)["code"].GetString();
		dat.strCodeKr	= (*itr)["codeKr"].GetString();
		dat.strVal		= (*itr)["val"].GetString();

		DataManager::GetInstance()->AddCodeData(dat);
	}

	if (DataManager::GetInstance()->GetBloodList()->empty())
	{
		HttpManager::GetInstance()->Send_GetCodeList("regB", "", CC_CALLBACK_2(UserIDRegisterLayer::onHttpRequest_GetCodeList, this));
	}
	else if (DataManager::GetInstance()->GetReligionList()->empty())
	{
		HttpManager::GetInstance()->Send_GetCodeList("regC", "", CC_CALLBACK_2(UserIDRegisterLayer::onHttpRequest_GetCodeList, this));
	}
	else
	{
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_SURVERY_LAYER_EVNET);
		runAction(RemoveSelf::create());
	}
}


void UserIDRegisterLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_START_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void UserIDRegisterLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{
}

void UserIDRegisterLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{
}

void UserIDRegisterLayer::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox)
{
}

void UserIDRegisterLayer::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{
}

