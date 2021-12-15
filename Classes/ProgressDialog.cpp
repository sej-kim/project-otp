#include "ProgressDialog.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"

USING_NS_CC;
using namespace ui;

static ProgressDialog* gs_pProgressDialog = nullptr;

ProgressDialog* ProgressDialog::GetInstance()
{
	return gs_pProgressDialog;
}

ProgressDialog::ProgressDialog()
{
	gs_pProgressDialog = this;
}

ProgressDialog::~ProgressDialog()
{
	gs_pProgressDialog = nullptr;
}

bool ProgressDialog::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	setTouchEnabled(true);
	setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
	_eventDispatcher->setPriority(_touchListener, -10000);

	Node* pRoot = CSLoader::createNode("ProgressDialog.csb");
	if (pRoot == nullptr)
	{
		return false;
	}

	addChild(pRoot);

	auto action2 = CSLoader::createTimeline("ProgressDialog.csb");


	ImageView* pImage = dynamic_cast<ImageView*>(pRoot->getChildByName("Image_3"));
	pImage->runAction(action2);

	action2->play("animation0", true);
	action2->retain(); //released later on
	return true;
}

void ProgressDialog::SetTime(unsigned int nTime)
{
	if (nTime > 0)
	{
		runAction(Sequence::create(DelayTime::create(nTime), RemoveSelf::create(), NULL));
	}
}

void ProgressDialog::Destroy()
{
	runAction(RemoveSelf::create());
}

bool ProgressDialog::onTouchBegan(Touch *a_pTouch, Event *a_pEvent)
{
	return true;
}