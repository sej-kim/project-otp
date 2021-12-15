#include "ExchangeLayer.h"
#include "Define.h"
#include "EventManager.h"
#include "HttpManager.h"
#include "DataManager.h"
#include "UIHelper.h"

USING_NS_CC;
using namespace ui;

bool ExchangeLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	m_pRoot = CSLoader::createNode("ExchangeLayer.csb");
	if (m_pRoot == nullptr)
	{
		return false;
	}

	addChild(m_pRoot);

	Button* pExchangeButton		= dynamic_cast<Button*>(m_pRoot->getChildByName("ExChangeButton"));
	Button* pBackButton			= dynamic_cast<Button*>(m_pRoot->getChildByName("BackButton"));
	if (pExchangeButton == nullptr || pBackButton == nullptr)
	{
		return false;
	}

	pExchangeButton->addClickEventListener(CC_CALLBACK_1(ExchangeLayer::menuCallback_Exchange, this));
	pBackButton->addClickEventListener(CC_CALLBACK_1(ExchangeLayer::menuCallback_Back, this));

	Button* pBleConnButton = dynamic_cast<Button*>(m_pRoot->getChildByName("BleConnButton"));
	if (pBleConnButton == nullptr)
	{
		return false;
	}

	pBleConnButton->addClickEventListener(CC_CALLBACK_1(ExchangeLayer::menuCallback_BleConn, this));
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
	pBleConnButton->setVisible(false);
	pBleConnButton->setEnabled(false);
	pBleConnButton->setTouchEnabled(false);
#endif

	Node* ImageRoot = m_pRoot->getChildByName("Image_5");
	if (ImageRoot)
	{
		m_pPublicKeyText = dynamic_cast<ui::Text*>(ImageRoot->getChildByName("KeyLabel"));
		SetUILabelText(ImageRoot, "Text_11", 10011);
	}
	SetUILabelText(m_pRoot->getChildByName("Image_5_0"), "Text_8_0", 10010);
    
	Text* pIDTextFieldText		= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("IDTextField"));
	Text* pBTTextFieldText		= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("BTTextField"));
	Text* pPinTextFieldText		= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("PinTextField"));
	Text* pRePinTextFieldText	= dynamic_cast<ui::Text*>(m_pRoot->getChildByName("RePinTextField"));
	if (pIDTextFieldText == nullptr || pBTTextFieldText == nullptr || pPinTextFieldText == nullptr || pRePinTextFieldText == nullptr)
	{
		return false;
	}

	m_pExChangeBTCEdit = ui::EditBox::create(pIDTextFieldText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pExChangeBTCEdit->setAnchorPoint(pIDTextFieldText->getAnchorPoint());
	m_pExChangeBTCEdit->setPosition(pIDTextFieldText->getPosition());
	m_pExChangeBTCEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pExChangeBTCEdit->setFontColor(pIDTextFieldText->getTextColor());
	m_pExChangeBTCEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pExChangeBTCEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pExChangeBTCEdit->setPlaceHolder(OTP_TEXT(10002));
	m_pExChangeBTCEdit->setDelegate(this);
	m_pRoot->addChild(m_pExChangeBTCEdit, 0, "ExChangeBTC");

	m_pExChangeUSDEdit = ui::EditBox::create(pBTTextFieldText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pExChangeUSDEdit->setAnchorPoint(pBTTextFieldText->getAnchorPoint());
	m_pExChangeUSDEdit->setPosition(pBTTextFieldText->getPosition());
	m_pExChangeUSDEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pExChangeUSDEdit->setFontColor(pBTTextFieldText->getTextColor());
	m_pExChangeUSDEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pExChangeUSDEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pExChangeUSDEdit->setPlaceHolder(OTP_TEXT(10005));
	m_pExChangeUSDEdit->setDelegate(this);
	m_pRoot->addChild(m_pExChangeUSDEdit, 0, "ExChangeUSD");

	m_pPinEdit = ui::EditBox::create(pPinTextFieldText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pPinEdit->setAnchorPoint(pPinTextFieldText->getAnchorPoint());
	m_pPinEdit->setPosition(pPinTextFieldText->getPosition());
	m_pPinEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pPinEdit->setFontColor(pPinTextFieldText->getTextColor());
	m_pPinEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pPinEdit->setPlaceHolder(OTP_TEXT(10007));
	m_pPinEdit->setDelegate(this);
	m_pRoot->addChild(m_pPinEdit, 0, "Pin");

	m_pRePinEdit = ui::EditBox::create(pRePinTextFieldText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pRePinEdit->setAnchorPoint(pRePinTextFieldText->getAnchorPoint());
	m_pRePinEdit->setPosition(pRePinTextFieldText->getPosition());
	m_pRePinEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pRePinEdit->setFontColor(pRePinTextFieldText->getTextColor());
	m_pRePinEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pRePinEdit->setPlaceHolder(OTP_TEXT(10009));
	m_pRePinEdit->setDelegate(this);
	m_pRoot->addChild(m_pRePinEdit, 0, "RePin");

	SetUILabelText(m_pRoot, "Text_5", 10001);
	SetUILabelText(m_pRoot, "Text_5_0", 10004);
	SetUILabelText(m_pRoot, "Text_5_0_0", 10006);
	SetUILabelText(m_pRoot, "Text_5_0_0_0", 10008);

	Node* pListItem = CSLoader::createNode("ListViewItem.csb");
	if (pListItem == nullptr)
	{
		return false;
	}

	m_pListViewItem = dynamic_cast<Button*>(pListItem->getChildByName("Button"));
	if (m_pListViewItem == nullptr)
	{
		return false;
	}

	InitAccuntList(m_pRoot);
	InitPayTypeList(m_pRoot);
    UpdatePublicKey();
	return true;
}

bool ExchangeLayer::InitAccuntList(Node* pRoot)
{
	Node* pComboNode = pRoot->getChildByName("Combo_1");
	if (pComboNode == nullptr)
	{
		return false;
	}

	Button* pListViewButton = dynamic_cast<Button*>(pComboNode->getChildByName("Button"));
	if (pListViewButton == nullptr)
	{
		return false;
	}

	pListViewButton->addClickEventListener(CC_CALLBACK_1(ExchangeLayer::menuCallback_ComboBox, this));
	pListViewButton->setTag(1);


	m_pAccountListText = dynamic_cast<Text*>(pComboNode->getChildByName("Text"));
	if (m_pAccountListText == nullptr)
	{
		return false;
	}

	m_pAccountListView = dynamic_cast<ListView*>(pComboNode->getChildByName("ListView"));
	if (m_pAccountListView == nullptr)
	{
		return false;
	}

	const std::vector< sBANKACCOUNT_DATA >* pBackAccountList = DataManager::GetInstance()->GetBackAccountList();
	if (pBackAccountList == nullptr)
	{
		return false;
	}

	char szTemp[64] = { 0, };
	for (unsigned int i_1 = 0; i_1 < pBackAccountList->size(); ++i_1)
	{
		sBANKACCOUNT_DATA BankData = ((*pBackAccountList)[i_1]);

		sprintf(szTemp, "%d", (int)BankData.u64AccntSeq);
		AddListViewItem(m_pAccountListView, szTemp, i_1 + 1, CC_CALLBACK_1(ExchangeLayer::ListAccountCallback_Item, this));
		if (i_1 == 0)
		{
			m_pAccountListText->setString(szTemp);
		}
	}

	return true;
 
}

bool ExchangeLayer::InitPayTypeList(Node* pRoot)
{
	Node* pComboNode = pRoot->getChildByName("Combo_1_0");
	if (pComboNode == nullptr)
	{
		return false;
	}

	Button* pListViewButton = dynamic_cast<Button*>(pComboNode->getChildByName("Button"));
	if (pListViewButton == nullptr)
	{
		return false;
	}

	pListViewButton->addClickEventListener(CC_CALLBACK_1(ExchangeLayer::menuCallback_ComboBox, this));
	pListViewButton->setTag(2);

	m_pPayTypeListText = dynamic_cast<Text*>(pComboNode->getChildByName("Text"));
	if (m_pPayTypeListText == nullptr)
	{
		return false;
	}

	m_pPayTypeListView = dynamic_cast<ListView*>(pComboNode->getChildByName("ListView"));
	if (m_pPayTypeListView == nullptr)
	{
		return false;
	}

	AddListViewItem(m_pPayTypeListView, "GIB", 1, CC_CALLBACK_1(ExchangeLayer::ListPayCallback_Item, this));
	AddListViewItem(m_pPayTypeListView, "KRW", 2, CC_CALLBACK_1(ExchangeLayer::ListPayCallback_Item, this));
	m_pPayTypeListText->setString("GIB");
	return true;
}

void ExchangeLayer::AddListViewItem(cocos2d::ui::ListView* pView, std::string strName, int nTag, const cocos2d::ui::Widget::ccWidgetClickCallback &callback)
{
	if (pView == nullptr || m_pListViewItem == nullptr)
	{
		return;
	}

	Button* pButtonClon = dynamic_cast<Button*>(m_pListViewItem->clone());

	Size size = pView->getCustomSize();
	pButtonClon->setContentSize(Size(size.width, pButtonClon->getContentSize().height));

	pButtonClon->setTitleText(strName.c_str());
	pButtonClon->setTag(nTag);
	pButtonClon->addClickEventListener(callback);
	pView->pushBackCustomItem(pButtonClon);
}

void ExchangeLayer::ListAccountCallback_Item(cocos2d::Ref* sender)
{
	Button* pButton = dynamic_cast<Button*>(sender);
	if (pButton == nullptr || m_pAccountListView == nullptr || m_pAccountListText == nullptr)
	{
		return;
	}

	m_pAccountListText->setString(pButton->getTitleText());

	m_pAccountListView->setVisible(false);
	m_pAccountListView->setTouchEnabled(false);
	m_pAccountListView->setEnabled(false);
}

void ExchangeLayer::ListPayCallback_Item(cocos2d::Ref* sender)
{
	Button* pButton = dynamic_cast<Button*>(sender);
	if (pButton == nullptr || m_pPayTypeListView == nullptr || m_pPayTypeListText == nullptr)
	{
		return;
	}

	m_pPayTypeListText->setString(pButton->getTitleText());

	m_pPayTypeListView->setVisible(false);
	m_pPayTypeListView->setTouchEnabled(false);
	m_pPayTypeListView->setEnabled(false);
}

void ExchangeLayer::menuCallback_ComboBox(cocos2d::Ref* sender)
{
	if (m_pAccountListView == nullptr || m_pPayTypeListView == nullptr)
	{
		return;
	}

	Button* pButton = dynamic_cast<Button*>(sender);
	if (pButton == nullptr)
	{
		return;
	}

	int nTag = pButton->getTag();
	if (nTag == 1)
	{
		m_pAccountListView->setVisible(!m_pAccountListView->isVisible());
		m_pAccountListView->setTouchEnabled(m_pAccountListView->isVisible());
		m_pAccountListView->setEnabled(m_pAccountListView->isVisible());
	}
	else
	{
		m_pPayTypeListView->setVisible(!m_pPayTypeListView->isVisible());
		m_pPayTypeListView->setTouchEnabled(m_pPayTypeListView->isVisible());
		m_pPayTypeListView->setEnabled(m_pPayTypeListView->isVisible());
	}
}

void ExchangeLayer::onExit()
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_DISCONN);
	Node::onExit();
}

void ExchangeLayer::UpdatePublicKey()
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

void ExchangeLayer::Send_Hash(std::string strHash)
{
	EventManager::GetInstance()->BleTx(strHash, 6);
}

void ExchangeLayer::menuCallback_BleConn(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(WIN_BLE_CONN);
}

void ExchangeLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_HOME_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void ExchangeLayer::Send_Exchange()
{
	if (m_pAccountListText == nullptr || m_pPayTypeListText == nullptr)
	{
		return;
	}

    sUSER_DATA userData;
    sBANKACCOUNT_DATA BackData;
    DataManager::GetInstance()->CopyUserData( userData);
    DataManager::GetInstance()->CopyBankAccountsData(BackData, 1);
    
    char* pPtr;
    unsigned int nPay		= (unsigned int)strtol(m_pExChangeUSDEdit->getText(), &pPtr, 10);
	unsigned int nAccount = (unsigned int)strtol(m_pAccountListText->getString().c_str(), &pPtr, 10);
    float fBitAmt       = strtof(m_pExChangeBTCEdit->getText(), &pPtr);
	HttpManager::GetInstance()->Send_ReqExchanage(BackData.u64UserSeq, nAccount, nPay, m_pPayTypeListText->getString(), fBitAmt, CC_CALLBACK_2(ExchangeLayer::onHttpRequest_ReqExchanage, this));
}

void ExchangeLayer::Ok_CallBack()
{
	EventManager::GetInstance()->BleTx(m_pPinEdit->getText(), 4);
}

void ExchangeLayer::menuCallback_Exchange(cocos2d::Ref* sender)
{
	if (m_pAccountListText == nullptr || m_pPayTypeListText == nullptr)
	{
		return;
	}

	if (m_pAccountListText->getString().length() <= 0)
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50011), SMessageBox::eMESSAGEBOX_TYPE::OK)
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


	if (m_bOKPush)
	{
		CREATE_MESSAGE_CALLBACK(OTP_TEXT(50008), OTP_TEXT(10022), SMessageBox::eMESSAGEBOX_TYPE::YES_NO, CC_CALLBACK_0(ExchangeLayer::Send_Exchange, this))
	}
	else
	{
		Ok_CallBack();
	}
}

void ExchangeLayer::onHttpRequest_ReqExchanage(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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

    rapidjson::Value& DataValue = jsonDoc["exdata"];
    sEXCHANGE_RESULT_DATA sExChangeResultData;
    sExChangeResultData.tSendTime       = DataValue["dtpurchase"].GetString();
    sExChangeResultData.fBitAmt         = DataValue["bitAmt"].GetDouble();
    sExChangeResultData.fAmt            = DataValue["amt"].GetDouble();
    
	sBANKACCOUNT_DATA BankData;
	DataManager::GetInstance()->CopyBankAccountsData(BankData, 1);
	BankData.dAmt = sExChangeResultData.fAmt;
	DataManager::GetInstance()->AddankAccountsData(BankData);

    DataManager::GetInstance()->SetExchangeResultData(sExChangeResultData);
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_EXCHANGE_HOME_LAYER_EVNET);
    runAction(RemoveSelf::create());
}

void ExchangeLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{

}

void ExchangeLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{

}

void ExchangeLayer::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox)
{

}

void ExchangeLayer::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{
	if (pEditBox == nullptr)
	{
		return;
	}

	if (pEditBox->getName().compare("Pin") == 0)
	{
		m_strPin = pEditBox->getText();
	}
}
