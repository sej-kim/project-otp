#include "RemittanceHomeLayer.h"
#include "Define.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "HttpManager.h"
#include "DataManager.h"
#include "UIHelper.h"

USING_NS_CC;
using namespace ui;


bool RemittanceHomeLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	Node* pRoot = CSLoader::createNode("RemittanceHomeLayer.csb");
	if (pRoot == nullptr)
	{
		return false;
	}
    addChild(pRoot);

	Button* pBackButton = static_cast<Button*>(pRoot->getChildByName("BackButton"));
	if (pBackButton == nullptr)
	{
		return false;
	}

	pBackButton->addClickEventListener(CC_CALLBACK_1(RemittanceHomeLayer::menuCallback_Back, this));

	Button* pOKButton = static_cast<Button*>(pRoot->getChildByName("OK_Button"));
	if (pOKButton == nullptr)
	{
		return false;
	}

	pOKButton->addClickEventListener(CC_CALLBACK_1(RemittanceHomeLayer::menuCallback_OK, this));

    char szTemp[512] = { 0,};
    char szDate[128] = { 0,};
    char szTime[512] = { 0,};
    sRTX_RESULT_DATA sResultData = DataManager::GetInstance()->GetRtxResultData();

    std::string strYY  = sResultData.tSendTime.substr(0, 4);
    std::string strMM  = sResultData.tSendTime.substr(4, 2);
    std::string strDD  = sResultData.tSendTime.substr(6, 2);
    std::string strHH  = sResultData.tSendTime.substr(8, 2);
    std::string strMin = sResultData.tSendTime.substr(10, 2);
    std::string strMil = sResultData.tSendTime.substr(12, 2);
    
    sprintf(szDate, "%s-%s-%s", strYY.c_str(), strMM.c_str(), strDD.c_str());
    sprintf(szTime, "%s:%s:%s", strHH.c_str(), strMin.c_str(), strMil.c_str());
    SetUILabelText(pRoot, "Text_2", szDate);
    SetUILabelText(pRoot, "Text_4", szTime);
    
	sprintf(szTemp, OTP_TEXT(8002), sResultData.strReceiveName.c_str(), sResultData.fAmtSent);
    SetUILabelText(pRoot, "UserInfoLabel", szTemp);
	SetUILabelText(pRoot, "Text_3", OTP_TEXT(8003));
    
	sprintf(szTemp, OTP_TEXT(8004), sResultData.fSenderBalance);
    SetUILabelText(pRoot, "BTCLabel", szTemp);

	return true;
}

void RemittanceHomeLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_REMITTANCE_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void RemittanceHomeLayer::menuCallback_OK(cocos2d::Ref* sender)
{
    sUSER_DATA sUserData;
    DataManager::GetInstance()->CopyUserData(sUserData);
    HttpManager::GetInstance()->Send_GetBackAccounts(sUserData.u64Seq, CC_CALLBACK_2(RemittanceHomeLayer::onHttpRequest_GetBankAccounts, this));
}

void RemittanceHomeLayer::onHttpRequest_GetBankAccounts(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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
        sBankAccountData.u64AccntSeq    = (*itr)["accntSeq"].GetUint64();
        sBankAccountData.strCurrency    = (*itr)["currency"].GetString();
        sBankAccountData.strAccntName   = (*itr)["accntName"].GetString();
		sBankAccountData.dAmt			= (*itr)["amt"].GetDouble();
        sBankAccountData.u64UserSeq     = (*itr)["userSeq"].GetUint64();
        DataManager::GetInstance()->AddankAccountsData(sBankAccountData);
    }
    
    Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_HOME_LAYER_EVNET);
    runAction(RemoveSelf::create());
}
