#pragma once
#include "cocos2d.h"
#include "Define.h"
#include "network/HttpClient.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "BleComm.h"
#endif

class WinBluetooth;

class EventManager : public cocos2d::Node
{
	struct sBLE_TX_DATA
	{
		int nStep;
		std::string strData;
	};
private:
	EventManager() {}

public:
    
	static EventManager* Create();
	static EventManager* GetInstance();
	static void Destroy();

public:
	void InitEvent(cocos2d::Node* pMainScene);
	void BleTx(std::string strData, int nStep);
	void BleRecvData(std::string strData, int nStep);
	void Card_OK_Click();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    void StatusChanged(BLEComm::STATUS in_status);
#else
    void StatusChanged(int nStatus);
#endif
	void DataIn(std::string in_data);
	void ConnectWinBLEDevice();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    bool IsBleConn()
    {
        return m_eBleStatus == BLEComm::STATUS::CONNECTED;
    }
#else
	bool IsBleConn()
	{
		return m_eBleStatus == 2;
	}
#endif
	
	virtual void update(float dt);
private:
	void RunEvent(sBLE_TX_DATA sData);
    

	void Send_GetTimeHash();
	void onHttpRequest_GetTimeHash(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);
private:
	int m_nCurStep = 0;
	std::list< sBLE_TX_DATA > m_listEven;
	eEVENT_TYPE m_eCurrEvent = EVENT_NONE;
    std::string m_strEvent;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	BLEComm m_ble;
    BLEComm::STATUS m_eBleStatus;
#else
	int m_eBleStatus;
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	WinBluetooth* m_pWinBluetooth = nullptr;
#endif
};
