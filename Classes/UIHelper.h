#pragma once
#include "cocos2d.h"

bool SetUILabelText(cocos2d::Node* pRoot, std::string strName, int nIndex);
bool SetUILabelText(cocos2d::Node* pRoot, std::string strName, std::string strText);
bool SetUILabelText(cocos2d::Node* pBase, std::string strRoot, std::string strName, int nIndex);

bool SetUIButtonText(cocos2d::Node* pRoot, std::string strName, int nIndex);
bool SetUIButtonText(cocos2d::Node* pRoot, std::string strName, std::string strText);