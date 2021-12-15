#pragma once 

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "network/HttpClient.h"

class LoginLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	virtual bool init();

	CREATE_FUNC(LoginLayer);
private:
	void menuCallback_Login(cocos2d::Ref* sender);
	void menuCallback_Back(cocos2d::Ref* sender);

private:
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxReturn(cocos2d::ui::EditBox* pEditBox);

private:
	void onHttpRequest_GetUserInfo(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);
	void onHttpRequest_GetBankAccounts(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);

private:
	cocos2d::ui::EditBox* m_pUserIDEdit = nullptr;
};