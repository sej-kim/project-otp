#include "MainScene.h"
#include "hmac_sha1.h"
#include "HttpManager.h"
#include "Define.h"
#include "DataManager.h"
#include "math/CCMath.h"
#include "MyBase64.h"
#include "EventManager.h"

#include "StartLayer.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "WinBluetooth.h"
#endif


USING_NS_CC;
static MainScene* gs_pMainScene = nullptr;

Scene* MainScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	gs_pMainScene = MainScene::create();
	// add layer as a child to scene
	scene->addChild(gs_pMainScene);

	// return the scene
	return scene;
}

MainScene* MainScene::GetInstance()
{
	return gs_pMainScene;
}

MainScene::~MainScene()
{
	DataManager::Destroy();
	EventManager::Destroy();
	HttpManager::Destroy();
	XmlParser::Destroy();
	if (m_pEventManager != nullptr)
	{
		removeChild(m_pEventManager);
		CC_SAFE_DELETE(m_pEventManager);
	}

	gs_pMainScene = nullptr;
}

bool MainScene::init()
{
	if (Layer::init() == false)
	{
		return false;
	}
   
	
	m_pEventManager = EventManager::Create();
	addChild(m_pEventManager);
	m_pEventManager->InitEvent(this);

	SetupUI();

	auto listener = EventListenerKeyboard::create();
	listener->onKeyReleased = CC_CALLBACK_2(MainScene::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void MainScene::SetupUI()
{
	//m_pDebugpLabel = Label::create("test code", "", 100);
	//m_pDebugpLabel->setPosition(500, 600);
	//m_pDebugpLabel->setColor(Color3B(255, 0, 0));
	//addChild(m_pDebugpLabel, 100000);


	StartLayer* pLayer = StartLayer::create();
	addChild(pLayer, 1);
}

void MainScene::SetDebugText(std::string str)
{
	//if (m_pDebugpLabel)
	//	m_pDebugpLabel->setString(str);
}
void MainScene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	if (keyCode == EventKeyboard::KeyCode::KEY_BACK)
	{
		JniMethodInfo t;
		bool isHave = JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "GetAppActivity", "()Ljava/lang/Object;");

		jobject activityObj;
		if (isHave)
		{
			activityObj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
		}

		isHave = JniHelper::getMethodInfo(t
			, "org/cocos2dx/cpp/AppActivity"
			, "onBackPressed"
			, "()V");

		if (isHave)
		{
			t.env->CallVoidMethod(activityObj, t.methodID);
		}
	}
#endif
}
