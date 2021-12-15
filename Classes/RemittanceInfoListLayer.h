#pragma once 

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "DataManager.h"

class TradeInfoListLayer : public cocos2d::Layer
{
	enum eTRAND_TYPE
	{
		ALL,
		REMITTANCE,
		DEPOSIT,
	};
public:
	virtual bool init();

	CREATE_FUNC(TradeInfoListLayer);

public:
	void SetVisibleNoneDescText(bool bVisible);

    virtual void update( float dt );

private:
	void AddTypeListViewItem(cocos2d::ui::ListView* pView, std::string strName, int nTag = -1);
	void AddResultListViewItem(cocos2d::ui::ListView* pView, std::string strTime, std::string strResult, std::string strTargetName, std::string strValue);

private:
	void ListCallback_Item(cocos2d::Ref* sender);

	void menuCallback_Back(cocos2d::Ref* sender);
	void menuCallback_ComboBox(cocos2d::Ref* sender);

protected:
	virtual bool onTouchBegan(cocos2d::Touch *a_pTouch, cocos2d::Event *a_pEvent);

	virtual void onEnter();
	virtual void onExit();

private:
	cocos2d::EventListenerTouchOneByOne* m_plistener = nullptr;

	cocos2d::ui::ListView* m_pTypeListView		= nullptr;
	cocos2d::ui::ListView* m_pResultListView	= nullptr;
	
	Node* m_pNonDescText_1						= nullptr;
	Node* m_pNonDescText_2						= nullptr;

	cocos2d::ui::Button* m_pTypeListViewItem		= nullptr;
	cocos2d::ui::ImageView* m_pResultListViewItem	= nullptr;
	cocos2d::ui::Text* m_pListText					= nullptr;
    
    std::vector< sRTX_HISTORY_DATA > m_vecRtxHistoryList;
};
