#pragma once

#include "cocos2d.h"
#include "ui/UIEditBox/UIEditBox.h"


class EventManager;

class MainScene : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	static MainScene* GetInstance();

	virtual ~MainScene();

	virtual bool init();

	void SetupUI();

	void SetDebugText( std::string str );
	// implement the "static create()" method manually
	CREATE_FUNC(MainScene);
    
private:
	void onKeyReleased( cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

private:
	EventManager* m_pEventManager = nullptr;
	//cocos2d::Label* m_pDebugpLabel = nullptr;
};
