#include "EventManager.h"
#include "MainScene.h"

#include "CardRegisterLayer.h"
#include "ExchangeHomeLayer.h"
#include "ExchangeInfoLayer.h"
#include "ExchangeInfoListLayer.h"
#include "ExchangeLayer.h"
#include "HomeLayer.h"
#include "LoginLayer.h"
#include "RemittanceHomeLayer.h"
#include "RemittanceLayer.h"
#include "StartLayer.h"
#include "SurveyLayer.h"
#include "RemittanceInfoLayer.h"
#include "RemittanceInfoListLayer.h"
#include "UserIDRegisterlayer.h"
#include "DataManager.h"
#include "HttpManager.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "WinBluetooth.h"
#endif

USING_NS_CC;

static EventManager* gs_pEventManager = NULL;


EventManager* EventManager::Create()
{
	if (gs_pEventManager)
	{
		EventManager::Destroy();
	}

	gs_pEventManager = new EventManager();

	return gs_pEventManager;
}

EventManager* EventManager::GetInstance()
{
	return gs_pEventManager;
}

void EventManager::Destroy()
{
	CC_SAFE_DELETE(gs_pEventManager);
}

void EventManager::ConnectWinBLEDevice()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	m_pWinBluetooth->CloseBLE();
	int result = m_pWinBluetooth->ConnectBLEDevice();
#endif
}

void EventManager::InitEvent(Node* pMainScene)
{
	if (pMainScene == nullptr)
	{
		return;
	}
	scheduleUpdate();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	CC_SAFE_DELETE(m_pWinBluetooth);
	m_pWinBluetooth = new WinBluetooth;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    using namespace std::placeholders; //Some esoteric c++ to enable the _1
    m_ble.setStatusCallback(std::bind(&EventManager::StatusChanged, this, _1));
    m_ble.setDataCallback(std::bind(&EventManager::DataIn, this, _1));
    m_ble.Connect("SMART-OTP_B1D6");
#endif
//	ConnectWinBLEDevice();

	_eventDispatcher->addCustomEventListener(LOGIN_STATUS_EVENT, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{

		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_START_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			StartLayer* pLayer = StartLayer::create();
			pMainScene->addChild(pLayer, 1, START_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_LOGIN_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			LoginLayer* pLayer = LoginLayer::create();
			pMainScene->addChild(pLayer, 1, LOGIN_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_ID_REGISTER_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			UserIDRegisterLayer* pLayer = UserIDRegisterLayer::create();
			pMainScene->addChild(pLayer, 1, ID_REGISTER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_CARD_REGISTER_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			CardRegisterLayer* pLayer = CardRegisterLayer::create();
			pMainScene->addChild(pLayer, 1, CARD_REGISTER_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_SURVERY_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			SurveyLayer* pLayer = SurveyLayer::create();
			pMainScene->addChild(pLayer, 1, SURVERY_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_REMITTANCE_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			RemittanceLayer* pLayer = RemittanceLayer::create();
			pMainScene->addChild(pLayer, 1, REMITTANCET_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_HOME_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			HomeLayer* pLayer = HomeLayer::create();
			pMainScene->addChild(pLayer, 1, HOME_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_TRADE_INO_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			TradeInfoLayer* pLayer = TradeInfoLayer::create();
			pMainScene->addChild(pLayer, 1, TRADE_INO_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_TRADE_INFO_LIST_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			TradeInfoListLayer* pLayer = TradeInfoListLayer::create();
			pMainScene->addChild(pLayer, 1, TRADE_INFO_LIST_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_EXCHANGE_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			ExchangeLayer* pLayer = ExchangeLayer::create();
			pMainScene->addChild(pLayer, 1, EXCHANGE_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_EXCHANGE_INFO_LIST_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			ExchangeInfoListLayer* pLayer = ExchangeInfoListLayer::create();
			pMainScene->addChild(pLayer, 1, EXCHANGE_INFO_LIST_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_EXCHANGE_HOME_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			ExchangeHomeLayer* pLayer = ExchangeHomeLayer::create();
			pMainScene->addChild(pLayer, 1, EXCHANGE_HOME_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_EXCHANGE_INFO_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			ExchangeInfoLayer* pLayer = ExchangeInfoLayer::create();
			pMainScene->addChild(pLayer, 1, EXCHANGE_INFO_LAYER_TAG);
		});
	});

	_eventDispatcher->addCustomEventListener(CREATE_REMITTANCE_HOME_LAYER_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			RemittanceHomeLayer* pLayer = RemittanceHomeLayer::create();
			pMainScene->addChild(pLayer, 1, REMITTANCE_HOME_LAYER_TAG);
		});
	});
    
    _eventDispatcher->addCustomEventListener(SEND_CARD_REGISTER_EVNET, [=](EventCustom *a_pEventCustom) -> void
    {
         Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
       {
            CardRegisterLayer* pLayer = dynamic_cast<CardRegisterLayer*>(pMainScene->getChildByTag(CARD_REGISTER_LAYER_TAG));
            if (pLayer)
            {
				pLayer->SetOkPush(true);
				pLayer->UpdatePublicKey();
                //pLayer->Send_Regist();
            }
       });
    });
    
   
	_eventDispatcher->addCustomEventListener(SEND_RTX_EVNET, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			m_strEvent = "";
			RemittanceLayer* pLayer = dynamic_cast<RemittanceLayer*>(pMainScene->getChildByTag(REMITTANCET_LAYER_TAG));
			if (pLayer)
			{
				pLayer->SetOkPush(true);
				pLayer->UpdatePublicKey();
			}
		});
	});

	_eventDispatcher->addCustomEventListener(SEND_EXCHANGE_EVENT, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			m_strEvent = "";
			ExchangeLayer* pLayer = dynamic_cast<ExchangeLayer*>(pMainScene->getChildByTag(EXCHANGE_LAYER_TAG));
			if (pLayer)
			{
				pLayer->SetOkPush(true);
				pLayer->UpdatePublicKey();
			}
		});
	});

	_eventDispatcher->addCustomEventListener(SEND_RTX_HISTORY_EVENT, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			m_strEvent = "";
			TradeInfoLayer* pLayer = dynamic_cast<TradeInfoLayer*>(pMainScene->getChildByTag(TRADE_INO_LAYER_TAG));
			if (pLayer)
			{
				pLayer->SetOkPush(true);
				pLayer->UpdatePublicKey();
			}
		});
	});


	_eventDispatcher->addCustomEventListener(SEND_EXCHANGE_HISTORY_EVENT, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			m_strEvent = "";
			ExchangeInfoLayer* pLayer = dynamic_cast<ExchangeInfoLayer*>(pMainScene->getChildByTag(EXCHANGE_INFO_LAYER_TAG));
			if (pLayer)
			{
				pLayer->SetOkPush(true);
				pLayer->UpdatePublicKey();
			}
		});
	});

    
    _eventDispatcher->addCustomEventListener(UPDATE_PUBLIC_KEY, [=](EventCustom *a_pEventCustom) -> void
    {
        Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
        {
            RemittanceLayer* pRemittanceLayer = dynamic_cast<RemittanceLayer*>(pMainScene->getChildByTag(REMITTANCET_LAYER_TAG));
            if (pRemittanceLayer)
            {
                pRemittanceLayer->UpdatePublicKey();
                return;
            }
            
            ExchangeLayer* pExchangeLayer = dynamic_cast<ExchangeLayer*>(pMainScene->getChildByTag(EXCHANGE_LAYER_TAG));
            if (pExchangeLayer)
            {
                pExchangeLayer->UpdatePublicKey();
                return;
            }
            
            TradeInfoLayer* pTradeInfoLayer = dynamic_cast<TradeInfoLayer*>(pMainScene->getChildByTag(TRADE_INO_LAYER_TAG));
            if (pTradeInfoLayer)
            {
                pTradeInfoLayer->UpdatePublicKey();
                return;
            }
            
            ExchangeInfoLayer* pExchangeInfoLayer = dynamic_cast<ExchangeInfoLayer*>(pMainScene->getChildByTag(EXCHANGE_INFO_LAYER_TAG));
            if (pExchangeInfoLayer)
            {
                pExchangeInfoLayer->UpdatePublicKey();
                return;
            }
        });
    });

	
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)	
	_eventDispatcher->addCustomEventListener(WIN_BLE_CONN, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			ConnectWinBLEDevice();
		});
	});

	_eventDispatcher->addCustomEventListener(WIN_BLE_DISCONN, [=](EventCustom *a_pEventCustom) -> void
	{
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([=](void) -> void
		{
			m_pWinBluetooth->CloseBLE();
		});
	});
#endif
	
}

void EventManager::update(float dt)
{
	if (m_listEven.empty())
	{
		return;
	}
	
	std::list< sBLE_TX_DATA >::iterator it = m_listEven.begin();
	sBLE_TX_DATA sData = (*it);
	m_listEven.pop_front();

	RunEvent(sData);
}

unsigned char Calc_Bcc(unsigned char value[])
{
    unsigned char cnret = 0;
    for( int i_1 = 1; i_1 < 16; ++i_1)
    {
        cnret ^= value[i_1];
    }
    
    return cnret;
}

void EventManager::BleTx(std::string strData, int nStep)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "GetAppActivity", "()Ljava/lang/Object;");

	jobject activityObj;
	if (isHave)
	{
		activityObj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
	}

	isHave = JniHelper::getMethodInfo(t
		, "org/cocos2dx/cpp/AppActivity"
		, "Send"
		, "(Ljava/lang/String;I)V");

	if (isHave)
	{
		jstring jmsg = t.env->NewStringUTF(strData.c_str());
		t.env->CallVoidMethod(activityObj, t.methodID, jmsg, nStep);
		t.env->DeleteLocalRef(jmsg);
	}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    unsigned char value[19] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    value[0] = 0xaa;
    if( nStep == 1)
    {
        value[1] = 0xa1;
        value[2] = 0x0e;
    }
    else if(nStep == 2)
    {
        value[1] = 0xa2;
        value[2] = 0x0e;
    }
    else if(nStep == 3)
    {
        value[1] = 0xa3;
        value[2] = 0x0e;
    }
    else if(nStep == 4)
    {
        value[1] = 0xc1;
        value[2] = 0x0e;
    }
    else if(nStep == 5)
    {
        value[1] = 0xc2;
        value[2] = 0x0e;
    }
    else if(nStep == 6)
    {
        value[1] = 0xc3;
        value[2] = 0x0e;
    }
    else if(nStep == 7)
    {
        value[1] = 0xc4;
        value[2] = 0x0e;
    }

    if(nStep == 5)
    {
        value[3] = 0xc8;
        value[4] = 0xab;
        value[5] = 0xb1;
        value[6] = 0xe6;
        value[7] = 0xb5;
        value[8] = 0xbf;
    }
    else
    {
        for( int i_1 = 0; i_1 < strData.length(); ++i_1)
        {
            value[i_1 + 3] = (unsigned char)strData[i_1];
        }
    }

    value[17] = Calc_Bcc(value);
    value[18] = 0x55;
    
    m_ble.Send_1( (unsigned char*)value );
#else
	unsigned char value[19] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	value[0] = 0xaa;
	if (nStep == 1)
	{
		value[1] = 0xa1;
		value[2] = 0x0e;
	}
	else if (nStep == 2)
	{
		value[1] = 0xa2;
		value[2] = 0x0e;
	}
	else if (nStep == 3)
	{
		value[1] = 0xa3;
		value[2] = 0x0e;
	}
	else if (nStep == 4)
	{
		value[1] = 0xc1;
		value[2] = 0x0e;
	}
	else if (nStep == 5)
	{
		value[1] = 0xc2;
		value[2] = 0x0e;
	}
	else if (nStep == 6)
	{
		value[1] = 0xc3;
		value[2] = 0x0e;
	}
	else if (nStep == 7)
	{
		value[1] = 0xc4;
		value[2] = 0x0e;
	}

	if (nStep == 5)
	{
		char szEuc_kr[1024] = { 0, };
		DataManager::GetInstance()->UTF8toEuc_kr(strData, szEuc_kr);
		unsigned int nLe = strlen(szEuc_kr);
		for (unsigned int i_1 = 0; i_1 < nLe; ++i_1)
		{
			value[i_1 + 3] = szEuc_kr[i_1] & 0xff;
			cocos2d::log("%02x", value[i_1 + 3]);
		}
	}
	else
	{
		for (unsigned int i_1 = 0; i_1 < strData.length(); ++i_1)
		{
			value[i_1 + 3] = (unsigned char)strData[i_1];
		}
	}

	value[17] = Calc_Bcc(value);
	value[18] = 0x55;
	m_pWinBluetooth->SendBLEData((unsigned char*)value);
#endif
    
}

void EventManager::BleRecvData(std::string strData, int nStep)
{
	sBLE_TX_DATA dat = { nStep, strData };
	m_listEven.push_back(dat);
}


void EventManager::RunEvent(sBLE_TX_DATA sData)
{
    cocos2d::log("BleRecvData %s   %d", sData.strData.c_str(), sData.nStep);

	//if (MainScene::GetInstance())
	//{
	//	MainScene::GetInstance()->SetDebugText(sData.strData);
	//}

	MainScene* pMainScene = MainScene::GetInstance();
	if (pMainScene == nullptr || m_nCurStep == sData.nStep)
	{
		return;
	}

	
	m_nCurStep = sData.nStep;
	switch (sData.nStep)
	{
	case 1:
	{
		if (sData.strData.compare("OK") != 0)
		{
			CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50007), SMessageBox::eMESSAGEBOX_TYPE::OK);
		}
	}
		break;
	case 2:
	{
		CardRegisterLayer* pLayer = dynamic_cast<CardRegisterLayer*>(pMainScene->getChildByTag(CARD_REGISTER_LAYER_TAG));
		if (pLayer)
		{
			pLayer->SetSerialNumber(sData.strData);
			pLayer->NextPage();
		}
	}
		break;
	case 3:
	{
		if (sData.strData.compare("OK") != 0)
		{
			CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50007), SMessageBox::eMESSAGEBOX_TYPE::OK);
		}
		else
		{
            m_strEvent = SEND_CARD_REGISTER_EVNET;
			CREATE_MESSAGE(OTP_TEXT(50008), OTP_TEXT(6014), SMessageBox::NONE);
		}
	}
		break;
	case 4:
	{
		if (sData.strData.compare("OK") != 0)
		{
			CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(30007), SMessageBox::eMESSAGEBOX_TYPE::OK);
		}
		else
		{
            RemittanceLayer* pLayer = dynamic_cast<RemittanceLayer*>(pMainScene->getChildByTag(REMITTANCET_LAYER_TAG));
            if (pLayer)
            {
                pLayer->Send_Ble(1);
				CREATE_MESSAGE(OTP_TEXT(50008), OTP_TEXT(50016), SMessageBox::NONE);
                return;
            }
            else
            {
				int nTexIndex = 0;
                if( pMainScene->getChildByTag(TRADE_INO_LAYER_TAG) )
                {
					nTexIndex = 50017;
                    m_strEvent = SEND_RTX_HISTORY_EVENT;
                }
                else if(pMainScene->getChildByTag(EXCHANGE_LAYER_TAG))
                {
					nTexIndex = 50018;
                    m_strEvent = SEND_EXCHANGE_EVENT;
                }
                else if(pMainScene->getChildByTag(EXCHANGE_INFO_LAYER_TAG))
                {
					nTexIndex = 50019;
                    m_strEvent = SEND_EXCHANGE_HISTORY_EVENT;
                }

				CREATE_MESSAGE(OTP_TEXT(50008), OTP_TEXT(nTexIndex), SMessageBox::NONE);
            }
		}
		
	}
		break;
	case 5:
	{
        if (sData.strData.compare("OK") != 0)
        {
			CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50009), SMessageBox::eMESSAGEBOX_TYPE::OK);
            return;
        }

        RemittanceLayer* pLayer = dynamic_cast<RemittanceLayer*>(pMainScene->getChildByTag(REMITTANCET_LAYER_TAG));
        if (pLayer)
        {
            pLayer->Send_Ble(2);
            return;
        }
	}
		break;
	case 6:
	{
        if (sData.strData.compare("ERROR") == 0 )
        {
			CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50010), SMessageBox::eMESSAGEBOX_TYPE::OK)
            return;
        }
        
        RemittanceLayer* pLayer = dynamic_cast<RemittanceLayer*>(pMainScene->getChildByTag(REMITTANCET_LAYER_TAG));
        if (pLayer)
        {
            m_strEvent = SEND_RTX_EVNET;
            return;
        }
	}
		break;
    case 7:
    {
        if (sData.strData.compare("ERROR") == 0 )
        {
			CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50011), SMessageBox::eMESSAGEBOX_TYPE::OK);
        }
        else
        {
            RemittanceLayer* pLayer = dynamic_cast<RemittanceLayer*>(pMainScene->getChildByTag(REMITTANCET_LAYER_TAG));
            if (pLayer)
            {
                pLayer->Send_Ble(3);
                return;
            }
        }
    }
            break;
	default:
    {
        if (sData.strData.compare("PIN OK") == 0 && m_strEvent.empty() == false)
        {
			Send_GetTimeHash();
            SMessageBox* pBox = SMessageBox::GetInstance();
            if(pBox)
            {
                pBox->Destroy();
            }
        }
    }
		break;
	}
}

void EventManager::Send_GetTimeHash()
{
	HttpManager::GetInstance()->Send_GetTimeHash(CC_CALLBACK_2(EventManager::onHttpRequest_GetTimeHash, this));
}

void EventManager::onHttpRequest_GetTimeHash(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
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

	DataManager::GetInstance()->SetTimeHash(ret);
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(m_strEvent);
}

void EventManager::Card_OK_Click()
{
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
void EventManager::StatusChanged(BLEComm::STATUS in_status)
{
    switch(in_status)
    {
        case BLEComm::STATUS::CONNECTED:
        {
            m_eBleStatus = in_status;
            m_nCurStep = 0;
            m_strEvent = "";
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(UPDATE_PUBLIC_KEY);
        }
            break;
        case BLEComm::STATUS::DISCONNECTED:
        {
            SMessageBox* pBox = SMessageBox::GetInstance();
            if(pBox)
            {
                pBox->Destroy();
            }
            
            m_eBleStatus = in_status;
            m_nCurStep = 0;
            m_strEvent = "";
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(UPDATE_PUBLIC_KEY);
        }
            break;
        case BLEComm::STATUS::ERROR:
            break;
        case BLEComm::STATUS::FOUND_NAMED_MODULE:
            break;
        case BLEComm::STATUS::NOTHING:
            break;
    }
}
#else
void EventManager::StatusChanged(int in_status)
{
    switch(in_status)
    {
        case 2:
        {
            m_eBleStatus = in_status;
            m_nCurStep = 0;
            m_strEvent = "";
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(UPDATE_PUBLIC_KEY);
        }
            break;
        case 3:
        {
            SMessageBox* pBox = SMessageBox::GetInstance();
            if(pBox)
            {
                pBox->Destroy();
            }
            
            m_eBleStatus = in_status;
            m_nCurStep = 0;
            m_strEvent = "";
            Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(UPDATE_PUBLIC_KEY);
        }
            break;
        case 4:
            break;
        case 1:
            break;
        case 0:
            break;
    }
}
#endif



void EventManager::DataIn(std::string in_data)
{
    char szStep[8];
    char nStep = 0;
    sprintf(szStep, "%02X", in_data[1]&0xff);
    
    if( strcmp( szStep, "B1") == 0 )
        nStep = 1;
    else if( strcmp( szStep, "A2") == 0 )
        nStep = 2;
    else if( strcmp( szStep, "B3") == 0 )
        nStep = 3;
    else if( strcmp( szStep, "D1") == 0 )
        nStep = 4;
    else if( strcmp( szStep, "D2") == 0 )
        nStep = 5;
    else if( strcmp( szStep, "D3") == 0 )
        nStep = 6;
    else if( strcmp( szStep, "D4") == 0 )
        nStep = 7;
    
    char szInData[19];
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	strlcpy(szInData, &in_data[3], 15);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	strcpy_s(szInData, 15, &in_data[3]);
#endif
    BleRecvData(szInData, nStep);
}


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

extern "C"
{
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_BleRecvData(JNIEnv* env, jobject thiz,jstring jStr, int nStep)
	{
		EventManager* pEventManager = EventManager::GetInstance();
		if(pEventManager == NULL)
		{
			return;
		}

		const char *nativeString = env->GetStringUTFChars(jStr, 0);
		std::string strData = nativeString;
		env->ReleaseStringUTFChars(jStr, nativeString);

		pEventManager->BleRecvData(strData, nStep);
	}

	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_SetBleStatus(JNIEnv* env, jobject thiz, int nStep)
	{
		EventManager* pEventManager = EventManager::GetInstance();
		if (pEventManager == NULL)
		{
			return;
		}

		pEventManager->StatusChanged(nStep);
	}
}
#endif	
