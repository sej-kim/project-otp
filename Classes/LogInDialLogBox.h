#pragma once
#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"

class LoginDialLogBox : public cocos2d::Layer, public cocos2d::ui::EditBoxDelegate
{
public:
	CREATE_FUNC(LoginDialLogBox);

public:
	virtual bool init();

private:
	void menuCallback(cocos2d::Ref* sender);

private:
	virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxTextChanged(cocos2d::ui::EditBox* pEditBox);
	virtual void editBoxReturn(cocos2d::ui::EditBox* pEditBox);


	virtual bool onTouchBegan(cocos2d::Touch *a_pTouch, cocos2d::Event *a_pEvent);
private:
	cocos2d::ui::EditBox* m_pIDEditBox			= nullptr;
	cocos2d::ui::EditBox* m_pPassWordEditBox	= nullptr;
};