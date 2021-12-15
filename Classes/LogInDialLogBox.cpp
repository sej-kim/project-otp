#include "LogInDialLogBox.h"
#include "HttpManager.h"

USING_NS_CC;

bool LoginDialLogBox::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	//setTouchEnabled(true);
	//setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	_eventDispatcher->setPriority(_touchListener, -10000);

	Sprite* pBG = Sprite::create("LoginBox.png");
	addChild(pBG);

	MenuItemFont* pMenuLogIItem = MenuItemFont::create("LOGIN", CC_CALLBACK_1(LoginDialLogBox::menuCallback, this));
	pMenuLogIItem->setColor(Color3B(0, 0, 0));
	pMenuLogIItem->setName("LOGIN");

	MenuItemFont* pMenuCancleItem = MenuItemFont::create("CANCLE", CC_CALLBACK_1(LoginDialLogBox::menuCallback, this));
	pMenuCancleItem->setColor(Color3B(0, 0, 0));
	pMenuCancleItem->setName("CANCLE");

	Menu* pMenu = Menu::create(pMenuLogIItem, pMenuCancleItem, NULL);
	pMenu->setPosition(0, -60);
	pMenu->alignItemsHorizontally();
	addChild(pMenu, 1);

	m_pIDEditBox = ui::EditBox::create(Size(200, 25), ui::Scale9Sprite::create("EditBox.png"));
	m_pIDEditBox->setAnchorPoint(Vec2(0.0, 0.5));
	m_pIDEditBox->setPlaceHolder("nanusys");
	m_pIDEditBox->setPosition(Vec2(-100, 30));
	m_pIDEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
	m_pIDEditBox->setFontColor(Color4B(100, 99, 99, 255));
	m_pIDEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pIDEditBox->setDelegate(this);
	m_pIDEditBox->setText("nanusys");
	addChild(m_pIDEditBox, 0, "ID");


	m_pPassWordEditBox = ui::EditBox::create(Size(200, 25), ui::Scale9Sprite::create("EditBox.png"));
	m_pPassWordEditBox->setAnchorPoint(Vec2(0.0, 0.5));
	m_pPassWordEditBox->setPosition(Vec2(-100, -20));
	m_pPassWordEditBox->setReturnType(ui::EditBox::KeyboardReturnType::DEFAULT);
	m_pPassWordEditBox->setFontColor(Color4B(100, 99, 99, 255));
	m_pPassWordEditBox->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pPassWordEditBox->setDelegate(this);
	addChild(m_pPassWordEditBox, 0, "PASSWORD");
	return true;
}

void LoginDialLogBox::menuCallback(cocos2d::Ref* sender)
{
	Node* pNode = static_cast<Node*>(sender);
	if (pNode == nullptr || m_pIDEditBox == nullptr || m_pPassWordEditBox == nullptr)
	{
		return;
	}

	if (pNode->getName().compare("LOGIN") == 0 )
	{
		//HttpManager::GetInstance()->Send_Login(m_pIDEditBox->getText(), m_pPassWordEditBox->getText());
	}
	
	runAction(RemoveSelf::create());
}

void LoginDialLogBox::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{
}

void LoginDialLogBox::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{
}

void LoginDialLogBox::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox)
{
}

void LoginDialLogBox::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{

}

bool LoginDialLogBox::onTouchBegan(Touch *a_pTouch, Event *a_pEvent)
{
	return true;
}