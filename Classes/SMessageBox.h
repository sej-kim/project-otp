#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"



class SMessageBox : public cocos2d::Layer
{
public:
	enum eMESSAGEBOX_TYPE
	{
		NONE,
		OK,
		YES_NO,
	};

public:
	static SMessageBox* GetInstance();

	SMessageBox();
	virtual ~SMessageBox();

	CREATE_FUNC(SMessageBox);
public:
	virtual bool init();

	void SetMessge(std::string strTitle, std::string strDesc, eMESSAGEBOX_TYPE type);
	void SetMessge(std::string strTitle, std::string strDesc, eMESSAGEBOX_TYPE type, const std::function<void()>& func);
    void Destroy();

private:
	virtual bool onTouchBegan(cocos2d::Touch *a_pTouch, cocos2d::Event *a_pEvent);

	void menuCallback_OK(cocos2d::Ref* sender);
	void menuCallback_YES(cocos2d::Ref* sender);
	void menuCallback_NO(cocos2d::Ref* sender);
private:
	cocos2d::ui::Text* m_pTitleText = nullptr;
	cocos2d::ui::Text* m_pDescText	= nullptr;

	cocos2d::ui::Button* m_pOKButton = nullptr;
	cocos2d::ui::Button* m_pYESButton = nullptr;
	cocos2d::ui::Button* m_pNOButton = nullptr;

	std::function<void()> m_callback = nullptr;
};
