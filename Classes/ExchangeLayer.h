#pragma once 

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"

class ExchangeLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	virtual bool init();

	CREATE_FUNC(ExchangeLayer);
public:
    void Send_Exchange();
    void UpdatePublicKey();
	void SetOkPush(bool bOkPush)
	{
		m_bOKPush = bOkPush;
	}
    
private:
	virtual void onExit();

	void menuCallback_Back(cocos2d::Ref* sender);
	void menuCallback_Exchange(cocos2d::Ref* sender);
	void menuCallback_BleConn(cocos2d::Ref* sender);
	void menuCallback_ComboBox(cocos2d::Ref* sender);

private:
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxReturn(cocos2d::ui::EditBox* pEditBox);

	void Send_Hash(std::string strHash);
private:
    void onHttpRequest_ReqExchanage(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);
    
	void AddListViewItem(cocos2d::ui::ListView* pView, std::string strName, int nTag, const cocos2d::ui::Widget::ccWidgetClickCallback &callback);
	
	void ListAccountCallback_Item(cocos2d::Ref* sender);
	void ListPayCallback_Item(cocos2d::Ref* sender);

	bool InitAccuntList(cocos2d::Node* pRoot);
	bool InitPayTypeList(cocos2d::Node* pRoot);

	void Ok_CallBack();
private:
	cocos2d::ui::EditBox* m_pExChangeBTCEdit	= nullptr;
	cocos2d::ui::EditBox* m_pExChangeUSDEdit	= nullptr;
	cocos2d::ui::EditBox* m_pPinEdit			= nullptr;
	cocos2d::ui::EditBox* m_pRePinEdit			= nullptr;

	cocos2d::ui::Button* m_pListViewItem		= nullptr;

	cocos2d::ui::ListView* m_pAccountListView	= nullptr;
	cocos2d::ui::Text* m_pAccountListText		= nullptr;
	cocos2d::ui::Button* m_pAccountListViewItem = nullptr;

	cocos2d::ui::ListView* m_pPayTypeListView	= nullptr;
	cocos2d::ui::Text* m_pPayTypeListText		= nullptr;
	cocos2d::ui::Button* m_pPayTypeListViewItem = nullptr;

    cocos2d::ui::Text* m_pPublicKeyText         = nullptr;

	std::string m_strPin;

	bool m_bOKPush								= false;
	Node* m_pRoot						= nullptr;
};
