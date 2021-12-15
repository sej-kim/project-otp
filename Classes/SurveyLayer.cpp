#include "SurveyLayer.h"
#include "Define.h"
#include "DataManager.h"
#include "UIHelper.h"
#include "HttpManager.h"



#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif
	void Java_org_cocos2dx_cpp_AppActivity_SetDate(JNIEnv *env, jobject thiz)
	{
		//it corresponds to java native function
	}

#ifdef __cplusplus
}
#endif
#endif


USING_NS_CC;
using namespace ui;

bool SurveyLayer::init()
{
	if (Layer::init() == false)
	{
		return false;
	}

	if (Layer::init() == false)
	{
		return false;
	}

	Node* pRoot = CSLoader::createNode("SurveyLayer.csb");
	if (pRoot == nullptr)
	{
		return false;
	}
	addChild(pRoot);

	m_pPageView = dynamic_cast<PageView*>(pRoot->getChildByName("PageView_1"));
	if (m_pPageView == nullptr)
	{
		return false;
	}

	m_pPageView->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type)
	{
		onTouch(sender, type);
	});

	Node* pPage1 = CSLoader::createNode("SurveyPageLayer_1.csb");
	Node* pPage2 = CSLoader::createNode("SurveyPageLayer_2.csb");
	Node* pPage3 = CSLoader::createNode("SurveyPageLayer_3.csb");

	if (pPage1 == nullptr || pPage2 == nullptr || pPage3 == nullptr)
	{
		return false;
	}

	Widget* pWidget1 = Widget::create();
	Widget* pWidget2 = Widget::create();
	Widget* pWidget3 = Widget::create();

	pWidget1->addChild(pPage1);
	pWidget2->addChild(pPage2);
	pWidget3->addChild(pPage3);

	m_pPageView->addPage(pWidget1);
	m_pPageView->addPage(pWidget2);
	m_pPageView->addPage(pWidget3);

	Button* pNextButton_1 = dynamic_cast<Button*>(pPage1->getChildByName("NextButton"));
	Button* pNextButton_2 = dynamic_cast<Button*>(pPage2->getChildByName("NextButton"));

	Button* pPrevButton_1 = dynamic_cast<Button*>(pPage2->getChildByName("PrevButton"));
	Button* pPrevButton_2 = dynamic_cast<Button*>(pPage3->getChildByName("PrevButton"));

	Button* pResigerButton = dynamic_cast<Button*>(pPage3->getChildByName("CompletButton"));
	Button* pBackButton = dynamic_cast<Button*>(pRoot->getChildByName("BackButton"));
	if (pNextButton_1 == nullptr || pNextButton_2 == nullptr || pPrevButton_1 == nullptr || pPrevButton_2 == nullptr || pResigerButton == nullptr || pNextButton_1 == nullptr)
	{
		return false;
	}

	pNextButton_1->addClickEventListener(CC_CALLBACK_1(SurveyLayer::menuCallback_Next, this));
	pNextButton_2->addClickEventListener(CC_CALLBACK_1(SurveyLayer::menuCallback_Next, this));
	pPrevButton_1->addClickEventListener(CC_CALLBACK_1(SurveyLayer::menuCallback_Prev, this));
	pPrevButton_2->addClickEventListener(CC_CALLBACK_1(SurveyLayer::menuCallback_Prev, this));
	pResigerButton->addClickEventListener(CC_CALLBACK_1(SurveyLayer::menuCallback_Register, this));
	pBackButton->addClickEventListener(CC_CALLBACK_1(SurveyLayer::menuCallback_Back, this));

	Text* pUserNameText			= dynamic_cast<ui::Text*>(pPage1->getChildByName("NameTextField_1"));
	Text* pFamilyNameText		= dynamic_cast<ui::Text*>(pPage1->getChildByName("NameTextField_2"));
	Text* pFirstBabyNameText	= dynamic_cast<ui::Text*>(pPage1->getChildByName("NameTextField_3"));
	Text* pBirthDayYYYYText		= dynamic_cast<ui::Text*>(pPage1->getChildByName("NameTextField_4"));
	Text* pBirthDayMMText		= dynamic_cast<ui::Text*>(pPage1->getChildByName("NameTextField_5"));
	Text* pBirthDayDDText		= dynamic_cast<ui::Text*>(pPage1->getChildByName("NameTextField_6"));
	if (pUserNameText == nullptr || pFamilyNameText == nullptr || pFirstBabyNameText == nullptr || pBirthDayYYYYText == nullptr || pBirthDayMMText == nullptr || pBirthDayDDText == nullptr)
	{
		return false;
	}

	m_pUserNameEdit = ui::EditBox::create(pUserNameText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pUserNameEdit->setAnchorPoint(pUserNameText->getAnchorPoint());
	m_pUserNameEdit->setPosition(pUserNameText->getPosition());
	m_pUserNameEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pUserNameEdit->setFontColor(pUserNameText->getTextColor());
	m_pUserNameEdit->setFontSize(pUserNameText->getFontSize());
	m_pUserNameEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pUserNameEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pUserNameEdit->setPlaceHolder(OTP_TEXT(5003));
	m_pUserNameEdit->setDelegate(this);
	pPage1->addChild(m_pUserNameEdit, 0, "UserName");

	m_pFamilyNameEdit = ui::EditBox::create(pFamilyNameText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pFamilyNameEdit->setAnchorPoint(pFamilyNameText->getAnchorPoint());
	m_pFamilyNameEdit->setPosition(pFamilyNameText->getPosition());
	m_pFamilyNameEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pFamilyNameEdit->setFontColor(pFamilyNameText->getTextColor());
	m_pFamilyNameEdit->setFontSize(pFamilyNameText->getFontSize());
	m_pFamilyNameEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pFamilyNameEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pFamilyNameEdit->setPlaceHolder(OTP_TEXT(5008));
	m_pFamilyNameEdit->setDelegate(this);
	pPage1->addChild(m_pFamilyNameEdit, 0, "FamilyName");

	m_pFirstBabyNameEdit = ui::EditBox::create(pFirstBabyNameText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pFirstBabyNameEdit->setAnchorPoint(pFirstBabyNameText->getAnchorPoint());
	m_pFirstBabyNameEdit->setPosition(pFirstBabyNameText->getPosition());
	m_pFirstBabyNameEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pFirstBabyNameEdit->setFontColor(pFirstBabyNameText->getTextColor());
	m_pFirstBabyNameEdit->setFontSize(pFirstBabyNameText->getFontSize());
	m_pFirstBabyNameEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pFirstBabyNameEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pFirstBabyNameEdit->setPlaceHolder(OTP_TEXT(5013));
	m_pFirstBabyNameEdit->setDelegate(this);
	pPage1->addChild(m_pFirstBabyNameEdit, 0, "FirstBabyName");


	m_pBirthDayYYYYEdit = ui::EditBox::create(pBirthDayYYYYText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pBirthDayYYYYEdit->setAnchorPoint(pBirthDayYYYYText->getAnchorPoint());
	m_pBirthDayYYYYEdit->setPosition(pBirthDayYYYYText->getPosition());
	m_pBirthDayYYYYEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pBirthDayYYYYEdit->setFontColor(pBirthDayYYYYText->getTextColor());
	m_pBirthDayYYYYEdit->setFontSize(pBirthDayYYYYText->getFontSize());
	m_pBirthDayYYYYEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pBirthDayYYYYEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pBirthDayYYYYEdit->setPlaceHolder(OTP_TEXT(5040));
	m_pBirthDayYYYYEdit->setMaxLength(4);
	m_pBirthDayYYYYEdit->setDelegate(this);
	pPage1->addChild(m_pBirthDayYYYYEdit, 0, "BirthDayYYYY");

	m_pBirthDayMMEdit = ui::EditBox::create(pBirthDayMMText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pBirthDayMMEdit->setAnchorPoint(pBirthDayMMText->getAnchorPoint());
	m_pBirthDayMMEdit->setPosition(pBirthDayMMText->getPosition());
	m_pBirthDayMMEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pBirthDayMMEdit->setFontColor(pBirthDayMMText->getTextColor());
	m_pBirthDayMMEdit->setFontSize(pBirthDayMMText->getFontSize());
	m_pBirthDayMMEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pBirthDayMMEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pBirthDayMMEdit->setMaxLength(2);
	m_pBirthDayMMEdit->setPlaceHolder(OTP_TEXT(5041));
	m_pBirthDayMMEdit->setDelegate(this);
	pPage1->addChild(m_pBirthDayMMEdit, 0, "BirthDayMM");

	m_pBirthDayDDEdit = ui::EditBox::create(pBirthDayDDText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pBirthDayDDEdit->setAnchorPoint(pBirthDayDDText->getAnchorPoint());
	m_pBirthDayDDEdit->setPosition(pBirthDayDDText->getPosition());
	m_pBirthDayDDEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pBirthDayDDEdit->setFontColor(pBirthDayDDText->getTextColor());
	m_pBirthDayDDEdit->setFontSize(pBirthDayDDText->getFontSize());
	m_pBirthDayDDEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pBirthDayDDEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pBirthDayDDEdit->setMaxLength(2);
	m_pBirthDayDDEdit->setPlaceHolder(OTP_TEXT(5042));
	m_pBirthDayDDEdit->setDelegate(this);
	pPage1->addChild(m_pBirthDayDDEdit, 0, "BirthDayDD");


	Text* pLastSchoolText		= dynamic_cast<ui::Text*>(pPage2->getChildByName("SchoolTextField_1"));
	Text* pFirstSchoolText		= dynamic_cast<ui::Text*>(pPage2->getChildByName("SchoolTextField_2"));
	Text* pRegistPlaceText		= dynamic_cast<ui::Text*>(pPage2->getChildByName("NameTextField_3"));
	Text* pRegistYYYYText		= dynamic_cast<ui::Text*>(pPage2->getChildByName("NameTextField_4"));
	Text* pRegistMMText			= dynamic_cast<ui::Text*>(pPage2->getChildByName("NameTextField_5"));
	Text* pRegistDDext			= dynamic_cast<ui::Text*>(pPage2->getChildByName("NameTextField_6"));
	if (pLastSchoolText == nullptr || pFirstSchoolText == nullptr || pRegistPlaceText == nullptr || pRegistYYYYText == nullptr || pRegistMMText == nullptr || pRegistDDext == nullptr)
	{
		return false;
	}

	m_pLastSchoolEdit = ui::EditBox::create(pLastSchoolText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pLastSchoolEdit->setAnchorPoint(pLastSchoolText->getAnchorPoint());
	m_pLastSchoolEdit->setPosition(pLastSchoolText->getPosition());
	m_pLastSchoolEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pLastSchoolEdit->setFontColor(pLastSchoolText->getTextColor());
	m_pLastSchoolEdit->setFontSize(pLastSchoolText->getFontSize());
	m_pLastSchoolEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pLastSchoolEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pLastSchoolEdit->setPlaceHolder(OTP_TEXT(5021));
	m_pLastSchoolEdit->setDelegate(this);
	pPage2->addChild(m_pLastSchoolEdit, 0, "LastSchool");

	m_pFirstSchoolEdit = ui::EditBox::create(pFirstSchoolText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pFirstSchoolEdit->setAnchorPoint(pFirstSchoolText->getAnchorPoint());
	m_pFirstSchoolEdit->setPosition(pFirstSchoolText->getPosition());
	m_pFirstSchoolEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pFirstSchoolEdit->setFontColor(pFirstSchoolText->getTextColor());
	m_pFirstSchoolEdit->setFontSize(pFirstSchoolText->getFontSize());
	m_pFirstSchoolEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pFirstSchoolEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pFirstSchoolEdit->setPlaceHolder(OTP_TEXT(5023));
	m_pFirstSchoolEdit->setDelegate(this);
	pPage2->addChild(m_pFirstSchoolEdit, 0, "FirstSchool");


	m_pRegistPlaceEdit = ui::EditBox::create(pRegistPlaceText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pRegistPlaceEdit->setAnchorPoint(pRegistPlaceText->getAnchorPoint());
	m_pRegistPlaceEdit->setPosition(pRegistPlaceText->getPosition());
	m_pRegistPlaceEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pRegistPlaceEdit->setFontColor(pRegistPlaceText->getTextColor());
	m_pRegistPlaceEdit->setFontSize(pRegistPlaceText->getFontSize());
	m_pRegistPlaceEdit->setInputMode(ui::EditBox::InputMode::SINGLE_LINE);
	m_pRegistPlaceEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pRegistPlaceEdit->setPlaceHolder(OTP_TEXT(5043));
	m_pRegistPlaceEdit->setDelegate(this);
	pPage2->addChild(m_pRegistPlaceEdit, 0, "RegistPlace");

	m_pRegistYYYYEdit = ui::EditBox::create(pRegistYYYYText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pRegistYYYYEdit->setAnchorPoint(pRegistYYYYText->getAnchorPoint());
	m_pRegistYYYYEdit->setPosition(pRegistYYYYText->getPosition());
	m_pRegistYYYYEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pRegistYYYYEdit->setFontColor(pRegistYYYYText->getTextColor());
	m_pRegistYYYYEdit->setFontSize(pRegistYYYYText->getFontSize());
	m_pRegistYYYYEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pRegistYYYYEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pRegistYYYYEdit->setMaxLength(4);
	m_pRegistYYYYEdit->setPlaceHolder(OTP_TEXT(5040));
	m_pRegistYYYYEdit->setDelegate(this);
	pPage2->addChild(m_pRegistYYYYEdit, 0, "RegistYYYY");

	m_pRegistMMEdit = ui::EditBox::create(pRegistMMText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pRegistMMEdit->setAnchorPoint(pRegistMMText->getAnchorPoint());
	m_pRegistMMEdit->setPosition(pRegistMMText->getPosition());
	m_pRegistMMEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pRegistMMEdit->setFontColor(pRegistMMText->getTextColor());
	m_pRegistMMEdit->setFontSize(pRegistMMText->getFontSize());
	m_pRegistMMEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pRegistMMEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pRegistMMEdit->setMaxLength(2);
	m_pRegistMMEdit->setPlaceHolder(OTP_TEXT(5041));
	m_pRegistMMEdit->setDelegate(this);
	pPage2->addChild(m_pRegistMMEdit, 0, "RegistMM");

	m_pRegistDDEdit = ui::EditBox::create(pRegistDDext->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pRegistDDEdit->setAnchorPoint(pRegistDDext->getAnchorPoint());
	m_pRegistDDEdit->setPosition(pRegistDDext->getPosition());
	m_pRegistDDEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pRegistDDEdit->setFontColor(pRegistDDext->getTextColor());
	m_pRegistDDEdit->setFontSize(pRegistDDext->getFontSize());
	m_pRegistDDEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pRegistDDEdit->setInputFlag(ui::EditBox::InputFlag::INITIAL_CAPS_WORD);
	m_pRegistDDEdit->setMaxLength(2);
	m_pRegistDDEdit->setPlaceHolder(OTP_TEXT(5042));
	m_pRegistDDEdit->setDelegate(this);
	pPage2->addChild(m_pRegistDDEdit, 0, "RegistDD");

	Text* pSecretNumberText		= dynamic_cast<ui::Text*>(pPage3->getChildByName("NumberTextField_1"));
	Text* pReSecretNumberText	= dynamic_cast<ui::Text*>(pPage3->getChildByName("NumberTextField_2"));
	Text* pFamilyCountText		= dynamic_cast<ui::Text*>(pPage3->getChildByName("NumberTextField_3"));
	if (pSecretNumberText == nullptr || pReSecretNumberText == nullptr || pFamilyCountText == nullptr)
	{
		return false;
	}

	m_pSecretNumberEdit = ui::EditBox::create(pSecretNumberText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pSecretNumberEdit->setAnchorPoint(pSecretNumberText->getAnchorPoint());
	m_pSecretNumberEdit->setPosition(pSecretNumberText->getPosition());
	m_pSecretNumberEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pSecretNumberEdit->setFontColor(pSecretNumberText->getTextColor());
	m_pSecretNumberEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pSecretNumberEdit->setPlaceHolder(OTP_TEXT(5030));
	m_pSecretNumberEdit->setDelegate(this);
	pPage3->addChild(m_pSecretNumberEdit, 0, "SecretNumber");

	m_pReSecretNumberEdit = ui::EditBox::create(pReSecretNumberText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pReSecretNumberEdit->setAnchorPoint(pReSecretNumberText->getAnchorPoint());
	m_pReSecretNumberEdit->setPosition(pReSecretNumberText->getPosition());
	m_pReSecretNumberEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pReSecretNumberEdit->setFontColor(pReSecretNumberText->getTextColor());
	m_pReSecretNumberEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pReSecretNumberEdit->setPlaceHolder(OTP_TEXT(5032));
	m_pReSecretNumberEdit->setDelegate(this);
	pPage3->addChild(m_pReSecretNumberEdit, 0, "ReSecretNumber");

	m_pFamilyCountEdit = ui::EditBox::create(pFamilyCountText->getContentSize(), ui::Scale9Sprite::create("LineImage.png"));
	m_pFamilyCountEdit->setAnchorPoint(pFamilyCountText->getAnchorPoint());
	m_pFamilyCountEdit->setPosition(pFamilyCountText->getPosition());
	m_pFamilyCountEdit->setReturnType(ui::EditBox::KeyboardReturnType::DONE);
	m_pFamilyCountEdit->setFontColor(pFamilyCountText->getTextColor());
	m_pFamilyCountEdit->setInputMode(ui::EditBox::InputMode::NUMERIC);
	m_pFamilyCountEdit->setPlaceHolder(OTP_TEXT(5034));
	m_pFamilyCountEdit->setDelegate(this);
	pPage3->addChild(m_pFamilyCountEdit, 0, "ReSecretNumber");

	Node* pListNode			= nullptr;
	Button* pListViewButton = nullptr;
	Text* pListText			= nullptr;
	char szTemp[128]		= { 0, };
	int szButtonText[eLIST_VIEW_TYPE::MAX] = { 5004, 5009, 5015, 5017, 5019 };
	Node* zNode[eLIST_VIEW_TYPE::MAX] = { pPage1, pPage1, pPage1, pPage2, pPage3 };
	for (int i_1 = SEX; i_1 < eLIST_VIEW_TYPE::MAX; ++i_1)
	{
		Node* pPageNode = zNode[i_1];
		if (pPageNode == nullptr)
			continue;

		sprintf(szTemp, "Combo_%d", i_1 + 1);
		pListNode = pPageNode->getChildByName(szTemp);
		if (pListNode == nullptr)
		{
			return false;
		}

		m_pListView[i_1] = dynamic_cast<ListView*>(pListNode->getChildByName("ListView"));

		pListViewButton = dynamic_cast<Button*>(pListNode->getChildByName("Button"));
		if (pListViewButton != nullptr)
		{
			pListViewButton->addClickEventListener(CC_CALLBACK_1(SurveyLayer::menuCallback_ComboBox, this));
			pListViewButton->setTag(eLIST_VIEW_TYPE::SEX + i_1);
		}

		m_pListText[i_1] = dynamic_cast<Text*>(pListNode->getChildByName("Text"));
		SetUILabelText(pListNode, "Text", szButtonText[i_1]);
	}

	
	Node* pItem = CSLoader::createNode("ListViewItem.csb");
	if (pItem == nullptr)
	{
		return false;
	}
	
	m_pListViewItem = dynamic_cast<Button*>(pItem->getChildByName("Button"));
	if (m_pListViewItem == nullptr)
	{
		return false;
	}

	AddListViewItem(m_pListView[eLIST_VIEW_TYPE::SEX], OTP_TEXT(5005), eLIST_VIEW_TYPE::SEX);
	AddListViewItem(m_pListView[eLIST_VIEW_TYPE::SEX], OTP_TEXT(5006), eLIST_VIEW_TYPE::SEX);

	AddListViewItem(m_pListView[eLIST_VIEW_TYPE::FAMILY_SEX], OTP_TEXT(5010), eLIST_VIEW_TYPE::FAMILY_SEX);
	AddListViewItem(m_pListView[eLIST_VIEW_TYPE::FAMILY_SEX], OTP_TEXT(5011), eLIST_VIEW_TYPE::FAMILY_SEX);
	
	
	const std::vector< sCODE_DATA >* pNationList = DataManager::GetInstance()->GetNationList();
	for (unsigned int i_1 = 0; i_1 < pNationList->size(); ++i_1)
	{
		const sCODE_DATA& dat = (*pNationList)[i_1];
		AddListViewItem(m_pListView[eLIST_VIEW_TYPE::NATION], dat.strVal, eLIST_VIEW_TYPE::NATION);
	}

	const std::vector< sCODE_DATA >* pBloodList = DataManager::GetInstance()->GetBloodList();
	for (unsigned int i_1 = 0; i_1 < pBloodList->size(); ++i_1)
	{
		const sCODE_DATA& dat = (*pBloodList)[i_1];
		AddListViewItem(m_pListView[eLIST_VIEW_TYPE::BLOOD], dat.strVal, eLIST_VIEW_TYPE::BLOOD);
	}


	const std::vector< sCODE_DATA >* pReligionList = DataManager::GetInstance()->GetReligionList();
	for (unsigned int i_1 = 0; i_1 < pReligionList->size(); ++i_1)
	{
		const sCODE_DATA& dat = (*pReligionList)[i_1];
		AddListViewItem(m_pListView[eLIST_VIEW_TYPE::RELIGION], dat.strVal, eLIST_VIEW_TYPE::RELIGION);
	}

	SetUILabelText(pRoot, "Text_1", 5000);
	SetUILabelText(pRoot, "Text_2", 5001);

	SetUILabelText(pPage1, "Text_3", 5002);
	SetUILabelText(pPage1, "Text1", 5007);
	SetUILabelText(pPage1, "Text_3_0_0", 5012);
	SetUILabelText(pPage1, "Text_3_0_0_0", 5014);
	SetUILabelText(pPage1, "Text_3_0_0_0_0", 5016);
	
	SetUILabelText(pPage2, "Text_13", 5018);
	SetUILabelText(pPage2, "Text_3_0_1", 5020);
	SetUILabelText(pPage2, "Text_3_0_1_0", 5022);
	SetUILabelText(pPage2, "Text_3_0_1_0_0", 5024);
	SetUILabelText(pPage2, "Text_3_0_1_0_0_0", 5027);

	SetUILabelText(pPage3, "Text_3_0_0_1", 5029);
	SetUILabelText(pPage3, "Text_3_0_0_1_0", 5031);
	SetUILabelText(pPage3, "Text_3_0_0_1_1", 5033);
	SetUILabelText(pPage3, "Text_3_0_0_1_1_0", 5035);


	SetUIButtonText(pPage3, "CompletButton", 5037);

	return true;
}

void SurveyLayer::AddListViewItem(cocos2d::ui::ListView* pView, std::string strName, int nTag)
{
	if (pView == nullptr || m_pListViewItem == nullptr)
	{
		return;
	}

	Button* pButtonClon = dynamic_cast<Button*>(m_pListViewItem->clone());

	Size size = pView->getCustomSize();
	pButtonClon->setContentSize(Size(size.width, pButtonClon->getContentSize().height));

	pButtonClon->setTitleText(strName.c_str());
	pButtonClon->addClickEventListener(CC_CALLBACK_1(SurveyLayer::ListCallback_Item, this));
	pButtonClon->setTag(nTag);

	pView->pushBackCustomItem(pButtonClon);
}

void SurveyLayer::ListCallback_Item(cocos2d::Ref* sender)
{
	Button* pButton = dynamic_cast<Button*>(sender);
	if (pButton == nullptr)
	{
		return;
	}

	int nTag = pButton->getTag();

	Text* pText = m_pListText[nTag];
	if (pText == nullptr)
	{
		return;
	}

	pText->setString(pButton->getTitleText());

	ListView* pView = m_pListView[nTag];
	if (pView == nullptr)
	{
		return;
	}

	pView->setVisible(false);
	pView->setTouchEnabled(false);
	pView->setEnabled(false);
}

void SurveyLayer::menuCallback_ComboBox(cocos2d::Ref* sender)
{
	Node* pButton = dynamic_cast<Node*>(sender);
	if (pButton == nullptr)
	{
		return;
	}

	int nTag = pButton->getTag();
	
	ListView* pView = m_pListView[nTag];
	if (pView == nullptr)
	{
		return;
	}

	pView->setVisible(!pView->isVisible());
	pView->setTouchEnabled(pView->isVisible());
	pView->setEnabled(pView->isVisible());
}

void SurveyLayer::menuCallback_Prev(cocos2d::Ref* sender)
{
	if (m_pPageView == nullptr)
	{
		return;
	}

	m_nCurPageIndex = m_pPageView->getCurrentPageIndex();
	if (m_nCurPageIndex <= 0)
	{
		return;
	}

	--m_nCurPageIndex;

	m_pPageView->setCurrentPageIndex(m_nCurPageIndex);
}

void SurveyLayer::menuCallback_Next(cocos2d::Ref* sender)
{
	if (m_pPageView == nullptr)
	{
		return;
	}

	m_nCurPageIndex = m_pPageView->getCurrentPageIndex();
	if (m_nCurPageIndex >= 2)
	{
		return;
	}

	++m_nCurPageIndex;

	m_pPageView->setCurrentPageIndex(m_nCurPageIndex);
}


void SurveyLayer::menuCallback_Register(cocos2d::Ref* sender)
{
	char szBirthDayTime[128] = { 0, };
	char szRegisterTime[128] = { 0, };
	sprintf(szBirthDayTime, "%s%s%s", m_pBirthDayYYYYEdit->getText(), m_pBirthDayMMEdit->getText(), m_pBirthDayDDEdit->getText());
	sprintf(szRegisterTime, "%s%s%s", m_pRegistYYYYEdit->getText(), m_pRegistMMEdit->getText(), m_pRegistDDEdit->getText());

	sSURVEY_DATA data;
	data.strSurvey[sSURVEY_DATA::USER_NAME] = m_pUserNameEdit->getText();
	data.strSurvey[sSURVEY_DATA::q1] = m_pFamilyNameEdit->getText();
	data.strSurvey[sSURVEY_DATA::q2] = m_pFirstBabyNameEdit->getText();
	data.strSurvey[sSURVEY_DATA::q3] = szBirthDayTime;
	data.strSurvey[sSURVEY_DATA::q4] = m_pListView[NATION]->getCurSelectedIndex() == 0 ? "KR" : "US";
	data.strSurvey[sSURVEY_DATA::q5] = m_pListText[BLOOD]->getString();
	data.strSurvey[sSURVEY_DATA::q6] = m_pFirstSchoolEdit->getText();
	data.strSurvey[sSURVEY_DATA::q7] = m_pLastSchoolEdit->getText();
	data.strSurvey[sSURVEY_DATA::q8] = szRegisterTime;
	data.strSurvey[sSURVEY_DATA::q10] = m_pRegistPlaceEdit->getText();
	data.strSurvey[sSURVEY_DATA::q11] = m_pSecretNumberEdit->getText();
	data.strSurvey[sSURVEY_DATA::q12] = m_pFamilyCountEdit->getText();
	data.strSurvey[sSURVEY_DATA::q13] = m_pListText[RELIGION]->getString();;

    sUSER_DATA UserData;
    DataManager::GetInstance()->CopyUserData(UserData);
    UserData.strUserName = m_pUserNameEdit->getText();
    DataManager::GetInstance()->SetUserData(UserData);
    
	HttpManager::GetInstance()->Send_GetSurveyHash(data, CC_CALLBACK_2(SurveyLayer::onHttpRequest_GetSurveyHash, this));
}

void SurveyLayer::onHttpRequest_GetSurveyHash(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
{
	DESTROY_PROGRESSDIALOG
	if (!response)
	{
		return;
	}
	long statusCode = response->getResponseCode();

	cocos2d::log("response code: %ld", statusCode);

	if (!response->isSucceed())
	{
		CREATE_MESSAGE(OTP_TEXT(50001), OTP_TEXT(50002), SMessageBox::eMESSAGEBOX_TYPE::OK)
			cocos2d::log("response failed");
		cocos2d::log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char> *pData = response->getResponseData();
	if (pData->size() <= 0)
	{
		return;
	}

	std::string ret(&(pData->front()), pData->size());
	cocos2d::log("%s", ("Response message: " + ret).c_str());

	rapidjson::Document jsonDoc;
	jsonDoc.Parse(ret.c_str());

	DataManager::GetInstance()->SetSurVeryHash(ret);

	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_CARD_REGISTER_LAYER_EVNET);
	runAction(RemoveSelf::create());
}


void SurveyLayer::menuCallback_Back(cocos2d::Ref* sender)
{
	Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(CREATE_START_LAYER_EVNET);
	runAction(RemoveSelf::create());
}

void SurveyLayer::editBoxEditingDidBegin(cocos2d::ui::EditBox* pEditBox)
{
}

void SurveyLayer::editBoxEditingDidEnd(cocos2d::ui::EditBox* pEditBox)
{
}

void SurveyLayer::editBoxTextChanged(cocos2d::ui::EditBox* pEditBox)
{
}

void SurveyLayer::editBoxReturn(cocos2d::ui::EditBox* pEditBox)
{

}

void SurveyLayer::onTouch(Ref* sender, Widget::TouchEventType type)
{
	if (type != Widget::TouchEventType::BEGAN)
	{
		return;
	}
	for (int i_1 = SEX; i_1 < eLIST_VIEW_TYPE::MAX; ++i_1)
	{
		if (m_pListView[i_1] != NULL)
		{
			m_pListView[i_1]->setVisible(false);
			m_pListView[i_1]->setEnabled(false);
			m_pListView[i_1]->setTouchEnabled(false);
		}
	}

	//CreateDatePicker();
}


void SurveyLayer::CreateDatePicker()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

	JniMethodInfo t;
	bool isHave = JniHelper::getStaticMethodInfo(t, "org/cocos2dx/cpp/AppActivity", "GetAppActivity" ,"()Ljava/lang/Object;");

	jobject activityObj;
	if(isHave)
	{
		activityObj = t.env->CallStaticObjectMethod(t.classID, t.methodID);
	}

	isHave = JniHelper::getMethodInfo(t
		, "org/cocos2dx/cpp/AppActivity"
		, "CreateDatePicker"
		, "()V");

	if (isHave)
	{
		jstring jmsg = t.env->NewStringUTF(NULL);
		t.env->CallVoidMethod(activityObj, t.methodID, jmsg);
	}

#endif

}
