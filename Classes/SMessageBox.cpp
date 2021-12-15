#include "SMessageBox.h"

USING_NS_CC;
using namespace ui;

static SMessageBox* gs_pMessageBox	= nullptr;
static int gs_nIndex				= 0;

SMessageBox* SMessageBox::GetInstance()
{
	return gs_pMessageBox;
}

SMessageBox::SMessageBox()
{
	gs_pMessageBox = this;
	++gs_nIndex;
}

SMessageBox::~SMessageBox()
{
	if (gs_pMessageBox == this)
	{
		gs_pMessageBox = nullptr;
	}

	--gs_nIndex;
}

bool SMessageBox::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	_eventDispatcher->setPriority(_touchListener, -(10000 + gs_nIndex));

	Node* pRoot = CSLoader::createNode("MessageBox.csb");
	if (pRoot == nullptr)
	{
		return false;
	}

	addChild(pRoot);

	ImageView* pImage = dynamic_cast<ImageView*>(pRoot->getChildByName("Image"));
	if (pImage == nullptr)
	{
		return false;
	}

	m_pTitleText	= dynamic_cast<Text*>(pImage->getChildByName("Title_Text"));
	m_pDescText		= dynamic_cast<Text*>(pImage->getChildByName("Desc_Text"));

	m_pOKButton		= dynamic_cast<Button*>(pImage->getChildByName("OK_Button"));
	m_pYESButton	= dynamic_cast<Button*>(pImage->getChildByName("YES_Button"));
	m_pNOButton		= dynamic_cast<Button*>(pImage->getChildByName("No_Button"));

	if (m_pOKButton == nullptr || m_pYESButton == nullptr || m_pNOButton == nullptr)
	{
		return false;
	}

	m_pOKButton->addClickEventListener(CC_CALLBACK_1(SMessageBox::menuCallback_OK, this));
	m_pYESButton->addClickEventListener(CC_CALLBACK_1(SMessageBox::menuCallback_YES, this));
	m_pNOButton->addClickEventListener(CC_CALLBACK_1(SMessageBox::menuCallback_NO, this));

	return true;
}

void SMessageBox::SetMessge(std::string strTitle, std::string strDesc, eMESSAGEBOX_TYPE type, const std::function<void()>& func)
{
	m_callback = func;
	SetMessge(strTitle, strDesc, type);
}

void SMessageBox::SetMessge(std::string strTitle, std::string strDesc, eMESSAGEBOX_TYPE type)
{
	if (m_pTitleText == nullptr || m_pDescText == nullptr || m_pOKButton == nullptr || m_pYESButton == nullptr || m_pNOButton == nullptr)
	{
		return;
	}

	m_pTitleText->setString(strTitle);
	m_pDescText->setString(strDesc);

	m_pOKButton->setVisible(false);
	m_pOKButton->setEnabled(false);
	m_pOKButton->setTouchEnabled(false);

	m_pYESButton->setVisible(false);
	m_pYESButton->setEnabled(false);
	m_pYESButton->setTouchEnabled(false);

	m_pNOButton->setVisible(false);
	m_pNOButton->setEnabled(false);
	m_pNOButton->setTouchEnabled(false);

	switch (type)
	{
	case eMESSAGEBOX_TYPE::OK:
	{
		m_pOKButton->setVisible(true);
		m_pOKButton->setEnabled(true);
		m_pOKButton->setTouchEnabled(true);

		m_pYESButton->setVisible(false);
		m_pYESButton->setEnabled(false);
		m_pYESButton->setTouchEnabled(false);

		m_pNOButton->setVisible(false);
		m_pNOButton->setEnabled(false);
		m_pNOButton->setTouchEnabled(false);
	}
		break;
	case eMESSAGEBOX_TYPE::YES_NO:
	{
		m_pOKButton->setVisible(false);
		m_pOKButton->setEnabled(false);
		m_pOKButton->setTouchEnabled(false);

		m_pYESButton->setVisible(true);
		m_pYESButton->setEnabled(true);
		m_pYESButton->setTouchEnabled(true);

		m_pNOButton->setVisible(true);
		m_pNOButton->setEnabled(true);
		m_pNOButton->setTouchEnabled(true);
	}
		break;
	default:
		break;
	}
}

void SMessageBox::menuCallback_OK(cocos2d::Ref* sender)
{
	if (m_callback != nullptr)
	{
		m_callback();
	}

	runAction(RemoveSelf::create());
}

void SMessageBox::menuCallback_YES(cocos2d::Ref* sender)
{
	if (m_callback != nullptr)
	{
		m_callback();
	}

	runAction(RemoveSelf::create());
}

void SMessageBox::menuCallback_NO(cocos2d::Ref* sender)
{
	runAction(RemoveSelf::create());
}

bool SMessageBox::onTouchBegan(Touch *a_pTouch, Event *a_pEvent)
{
	return true;
}

void SMessageBox::Destroy()
{
    runAction(RemoveSelf::create());
}
