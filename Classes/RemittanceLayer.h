#pragma once

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"

class RemittanceLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	virtual bool init();

	CREATE_FUNC(RemittanceLayer);
public:
    void Send_Rtx();
    void Send_Ble(int nIndex);
    void UpdatePublicKey();
	void SetOkPush(bool bOkPush)
	{
		m_bOKPush = bOkPush;
	}

private:
	virtual void onExit();

	void menuCallback_Remittance(cocos2d::Ref* sender);
	void menuCallback_Back(cocos2d::Ref* sender);
	void menuCallback_BleConn(cocos2d::Ref* sender);
	void menuCallback_GetAccountInfo(cocos2d::Ref* sender);
private:
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxReturn(cocos2d::ui::EditBox* pEditBox);
    
private:
    void Send_UserName();
	void Send_Hash(std::string strHash);
	void Send_SendAccNo();
	void Send_Amount();
	void Ok_CallBack();
private:
	void onHttpRequest_ReqTx(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);
	void onHttpRequest_GetAccountInfo(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);

private:
	cocos2d::ui::EditBox* m_pIDEdit		= nullptr;
	cocos2d::ui::EditBox* m_pBTCEdit	= nullptr;
	cocos2d::ui::EditBox* m_pPinEdit	= nullptr;
	cocos2d::ui::EditBox* m_pRePinEdit	= nullptr;
    cocos2d::ui::EditBox* m_pMemodit    = nullptr;
	cocos2d::ui::Text* m_pPublicKeyText = nullptr;
	std::string m_strPin;
	std::string m_strAccount;
	std::string m_AccntName;
	unsigned int m_unSenduserSeq		= 0;
	bool m_bOKPush						= false;
	Node* m_pRoot						= nullptr;
};
