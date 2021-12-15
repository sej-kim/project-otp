#pragma once 

#include "cocos2d.h"
#include "network/HttpClient.h"

class RemittanceHomeLayer : public cocos2d::Layer
{
public:
	virtual bool init();

	CREATE_FUNC(RemittanceHomeLayer);

private:
	void menuCallback_Back(cocos2d::Ref* sender);
	void menuCallback_OK(cocos2d::Ref* sender);

private:
    void onHttpRequest_GetBankAccounts(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);
};
