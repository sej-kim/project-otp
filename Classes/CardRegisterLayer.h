#pragma once 

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "network/HttpClient.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class CardRegisterLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	virtual bool init();

	CREATE_FUNC(CardRegisterLayer);
public:
	void RunMainPage();
	void NextPage();
	void SetSerialNumber(std::string strSerial);
	void Send_Regist();
	void UpdatePublicKey();
	void SetOkPush(bool bOkPush)
	{
		m_bOKPush = bOkPush;
	}

private:
	virtual void onExit();

	void menuCallback_Back(cocos2d::Ref* sender);
	void menuCallback_OK(cocos2d::Ref* sender);
	void menuCallback_Regist(cocos2d::Ref* sender);

private:
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxReturn(cocos2d::ui::EditBox* pEditBox);

	void onHttpRequest_RegCiphercard(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);
	void Send_Hash(std::string strHash);

private:
	void SetUpPageUI(int nPage);
	

private:
	Node* m_pRoot = nullptr;
	Node* m_pPage = nullptr;

	cocos2d::ui::EditBox* m_pPinEdit	= nullptr;
	cocos2d::ui::EditBox* m_pRePinEdit  = nullptr;
	cocos2d::ui::Text* m_pPublicKeyText = nullptr;
	std::string m_strSerialKey;
	int m_nCurPage = 1;
	bool m_bOKPush						= false;
};