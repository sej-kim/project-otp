#pragma once


#include "cocos2d.h"

class ProgressDialog : public cocos2d::Layer
{
public:
	ProgressDialog();
	virtual ~ProgressDialog();

	static ProgressDialog* GetInstance();

public:
	virtual bool init();

	CREATE_FUNC(ProgressDialog);

public:
	void SetTime(unsigned int nTime);
	void Destroy();

private:
	virtual bool onTouchBegan(cocos2d::Touch *a_pTouch, cocos2d::Event *a_pEvent);

};