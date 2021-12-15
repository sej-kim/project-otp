#pragma once 

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "network/HttpClient.h"

class SurveyLayer : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
	enum eLIST_VIEW_TYPE
	{
		SEX,
		FAMILY_SEX,
		NATION,
		BLOOD,
		RELIGION,
		MAX,
	};
public:
	virtual bool init();

	CREATE_FUNC(SurveyLayer);

private:
	void menuCallback_Prev(cocos2d::Ref* sender);
	void menuCallback_Next(cocos2d::Ref* sender);
	void menuCallback_Register(cocos2d::Ref* sender);
	void menuCallback_Back(cocos2d::Ref* sender);
	void menuCallback_ComboBox(cocos2d::Ref* sender);

	void ListCallback_Item(cocos2d::Ref* sender);

protected:
	void onTouch(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType);

private:
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxReturn(cocos2d::ui::EditBox* pEditBox);

private:
	void AddListViewItem(cocos2d::ui::ListView* pView, std::string strName, int nTag = -1);

private:
	void onHttpRequest_GetSurveyHash(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);


private:
	cocos2d::ui::PageView* m_pPageView = nullptr;
	ssize_t m_nCurPageIndex = 0;

	cocos2d::ui::EditBox* m_pUserNameEdit		= nullptr;
	cocos2d::ui::EditBox* m_pFamilyNameEdit		= nullptr;
	cocos2d::ui::EditBox* m_pFirstBabyNameEdit	= nullptr;

	cocos2d::ui::EditBox* m_pFirstSchoolEdit	= nullptr;
	cocos2d::ui::EditBox* m_pLastSchoolEdit		= nullptr;

	cocos2d::ui::EditBox* m_pSecretNumberEdit	= nullptr;
	cocos2d::ui::EditBox* m_pReSecretNumberEdit = nullptr;
	cocos2d::ui::EditBox* m_pFamilyCountEdit	= nullptr;

	cocos2d::ui::EditBox* m_pBirthDayYYYYEdit	= nullptr;
	cocos2d::ui::EditBox* m_pBirthDayMMEdit		= nullptr;
	cocos2d::ui::EditBox* m_pBirthDayDDEdit		= nullptr;

	cocos2d::ui::EditBox* m_pRegistPlaceEdit	= nullptr;
	cocos2d::ui::EditBox* m_pRegistYYYYEdit		= nullptr;
	cocos2d::ui::EditBox* m_pRegistMMEdit		= nullptr;
	cocos2d::ui::EditBox* m_pRegistDDEdit		= nullptr;

	cocos2d::ui::Button* m_pListViewItem		= nullptr;
	cocos2d::ui::ListView* m_pListView[eLIST_VIEW_TYPE::MAX];
	cocos2d::ui::Text* m_pListText[eLIST_VIEW_TYPE::MAX];

	void CreateDatePicker();
};