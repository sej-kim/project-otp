#pragma once
#pragma warning(default:949)

#include "DataManager.h"
#include "XmlParser.h"
#include "ProgressDialog.h"
#include "SMessageBox.h"
#include "MainScene.h"


#define LOGIN_STATUS_EVENT						"LoginStatusEvent"
#define CREATE_START_LAYER_EVNET				"CreateStartLayerEvent"
#define CREATE_LOGIN_LAYER_EVNET				"CreateLoginLayerEvent"
#define CREATE_ID_REGISTER_LAYER_EVNET			"CreateIDRegisterLayerEvent"
#define CREATE_CARD_REGISTER_LAYER_EVNET		"CreateCardRegisterLayerEvent"
#define CREATE_SURVERY_LAYER_EVNET				"CreateSurveryLayerEvent"
#define CREATE_REMITTANCE_LAYER_EVNET			"CreateRemittanceLayerEvent"
#define CREATE_REMITTANCE_HOME_LAYER_EVNET		"CreateRemittanceHomeLayerEvent"
#define CREATE_HOME_LAYER_EVNET					"CreateHomeLayerEvent"
#define CREATE_TRADE_INO_LAYER_EVNET			"CreateTradeInfoLayerEvent"
#define CREATE_TRADE_INFO_LIST_LAYER_EVNET		"CreateTradeInfoListLayerEvent"

#define CREATE_EXCHANGE_LAYER_EVNET				"CreateExChangeLayerEvent"
#define CREATE_EXCHANGE_INFO_LAYER_EVNET		"CreateExChangeInfoLayerEvent"
#define CREATE_EXCHANGE_INFO_LIST_LAYER_EVNET	"CreateExChangeInfoListLayerEvent"
#define CREATE_EXCHANGE_HOME_LAYER_EVNET		"CreateExChangeHomeListLayerEvent"

#define SEND_CARD_REGISTER_EVNET                "SendCardRegisterEvent"
#define SEND_RTX_EVNET                          "SendRtxEvent"
#define SEND_EXCHANGE_EVENT                     "SendExchangeEvent"
#define SEND_RTX_HISTORY_EVENT                  "SendRtxHistoryEvent"
#define SEND_EXCHANGE_HISTORY_EVENT             "SEndExchangeHistoryEvent"

#define UPDATE_PUBLIC_KEY                       "UpdatePublicKey"
#define CARD_OK_PUSH							"CardOkPush"


#define WIN_BLE_CONN							"WinBleConn"
#define WIN_BLE_DISCONN							"WinBleDisConn"


#define TEXT_CARD_SERIAL_1 "182200001111"
#define TEXT_CARD_SERIAL_2 "182200001112"
#define TEXT_CARD_SERIAL_3 "182200001113"
#define TEXT_CARD_SERIAL_4 "182200001114"
#define TEXT_CARD_SERIAL_5 "182200001115"

#define TEXT_PUBLIC_KEY_1 "asdfghjklqwe"
#define TEXT_PUBLIC_KEY_2 "asdfghjklqwe"
#define TEXT_PUBLIC_KEY_3 "asdfghjklqwu"
#define TEXT_PUBLIC_KEY_4 "asdfghjklqwi"
#define TEXT_PUBLIC_KEY_5 "asdfghjklqwo"

#define START_LAYER_TAG				1010 
#define LOGIN_LAYER_TAG				1011 
#define ID_REGISTER_TAG				1012 
#define CARD_REGISTER_LAYER_TAG		1013 
#define SURVERY_LAYER_TAG			1014 
#define REMITTANCET_LAYER_TAG		1015 
#define HOME_LAYER_TAG				1016 
#define TRADE_INO_LAYER_TAG			1017 
#define TRADE_INFO_LIST_LAYER_TAG	1018 
#define EXCHANGE_LAYER_TAG			1019 
#define EXCHANGE_INFO_LIST_LAYER_TAG 1020 
#define EXCHANGE_HOME_LAYER_TAG		1021 
#define EXCHANGE_INFO_LAYER_TAG		1022 
#define REMITTANCE_HOME_LAYER_TAG	1023 

enum eEVENT_TYPE
{
	EVENT_NONE,
	CARD_REGISTER_STEP_1,
	CARD_REGISTER_STEP_2,
	CARD_REGISTER_STEP_3,
	CARD_REGISTER_STEP_4,
	CARD_REGISTER_STEP_5,
	CARD_REGISTER_STEP_6,
};

#define WTOM(str, str2) DataManager::GetInstance()->WCHARConvertToUTF8(str, str2)
#define OTP_TEXT(index) XmlParser::GetInstance()->GetText(index).c_str()

#define CREATE_PROGRESSDIALOG( time )	ProgressDialog* pProgressDialog = ProgressDialog::GetInstance();		\
										if (pProgressDialog == nullptr)										\
										{														\
											pProgressDialog = ProgressDialog::create();						\
											MainScene* pMain = MainScene::GetInstance();		\
											if (pMain)											\
											{													\
												pMain->addChild(pProgressDialog, 1000);						\
											}													\
										}														\
										pProgressDialog->SetTime(time);										\
		

#define DESTROY_PROGRESSDIALOG			ProgressDialog* pProgressDi = ProgressDialog::GetInstance();		\
										if (pProgressDi)													\
										{														\
											pProgressDi->Destroy();										\
										}														\








#define CREATE_MESSAGE( title, desc, type )	SMessageBox* pMessageBox = SMessageBox::create();		\
											if (pMessageBox != nullptr)								\
											{														\
												MainScene* pMain = MainScene::GetInstance();		\
												if (pMain)											\
												{													\
													pMain->addChild(pMessageBox, 1000);						\
												}													\
												pMessageBox->SetMessge(title, desc, type);			\
											}														\







#define CREATE_MESSAGE_CALLBACK( title, desc, type, call )	SMessageBox* pMessageBoxCall = SMessageBox::create();	\
											if (pMessageBoxCall != nullptr)										    \
											{														\
												MainScene* pMain = MainScene::GetInstance();		\
												if (pMain)											\
												{													\
													pMain->addChild(pMessageBoxCall, 1000);						\
												}													\
												pMessageBoxCall->SetMessge(title, desc, type, call);	\
											}														\

#define ID_MAX_LEN 11


