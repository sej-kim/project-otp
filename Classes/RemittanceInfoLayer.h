#pragma once 

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "network/HttpClient.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class TradeInfoLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	virtual bool init();

	CREATE_FUNC(TradeInfoLayer);
public:
    void Send_RtxHistory();
    void UpdatePublicKey();
	void SetOkPush(bool bOkPush)
	{
		m_bOKPush = bOkPush;
	}

private:
	virtual void onExit();

	void menuCallback_Back(cocos2d::Ref* sender);
	void menuCallback_Inquiry(cocos2d::Ref* sender);
	void menuCallback_BleConn(cocos2d::Ref* sender);

private:
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxReturn(cocos2d::ui::EditBox* pEditBox);

	void Send_Hash(std::string strHash);
private:
	void onHttpRequest_TxHist(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);

private:
	cocos2d::ui::EditBox* m_pPinEdit	= nullptr;
	cocos2d::ui::EditBox* m_pRePinEdit	= nullptr;
    cocos2d::ui::Text* m_pPublicKeyText = nullptr;
	bool m_bOKPush						= false;
	Node* m_pRoot						= nullptr;
};
