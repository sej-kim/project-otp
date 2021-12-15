#pragma once 

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

class HomeLayer : public cocos2d::Layer
{
	enum eMENU_LIST
	{
		LOGOUT, 
		EXCHANGE,
		EXCHANGE_LIST,
	};

public:
	virtual bool init();

	CREATE_FUNC(HomeLayer);
private:
	void AddListViewItem(cocos2d::ui::ListView* pView, std::string strName, int nTag = -1);

private:
	void menuCallback_Menu(cocos2d::Ref* sender);
	void menuCallback_Remittance(cocos2d::Ref* sender);
	void menuCallback_Trade(cocos2d::Ref* sender);
	
	void ListCallback_Item(cocos2d::Ref* sender);

protected:
	virtual bool onTouchBegan(cocos2d::Touch *a_pTouch, cocos2d::Event *a_pEvent);

	virtual void onEnter();
	virtual void onExit();

private:
	cocos2d::EventListenerTouchOneByOne* m_plistener = nullptr;

	cocos2d::ui::ListView* m_pListView		= nullptr;
	cocos2d::ui::Button* m_pListViewItem	= nullptr;

	Node* m_pRoot							= nullptr;
};