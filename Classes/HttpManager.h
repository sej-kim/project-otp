
#pragma once

#include "cocos2d.h"
#include "network/HttpClient.h"

struct sSURVEY_DATA
{
	enum eSURVEY_TYPE
	{
		USER_NAME,
		q1, q2, q3, q4, q5, q6,
		q7, q8, q9, q10, q11, q12, q13,
		MAX,
	};
	std::string strSurvey[eSURVEY_TYPE::MAX];
};

class HttpManager
{
private:
	HttpManager();
	virtual ~HttpManager();

public:
	static HttpManager* Create();
	static HttpManager* GetInstance();
	static void Destroy();

	bool init();

public:
	/**
	* 아아디 체크
	*
	* @param listener
	*/
	int Send_IDCheck(const std::string strUserID, const cocos2d::network::ccHttpRequestCallback& callback);

	/**
	* 국가 혈액형 도시 리스트
	*
	* @param listener
	*/
	int Send_GetCodeList(const std::string strCodeType, const std::string strUpcode, const cocos2d::network::ccHttpRequestCallback& callback);

	/**
	* 로그인 정보 얻기
	*
	* @param listener
	*/
	int Send_GetUserInfo(const std::string strUserID, const cocos2d::network::ccHttpRequestCallback& callback);

	/**
	* 통장 잔고 조회
	*
	* @param listener
	*/
	int Send_GetBackAccounts(const uint64_t u64UserSeq, const cocos2d::network::ccHttpRequestCallback& callback);

	int Send_GetSurveyHash(const sSURVEY_DATA& data, const cocos2d::network::ccHttpRequestCallback& callback);

	int Send_GetTimeHash(const cocos2d::network::ccHttpRequestCallback& callback);
	/**
	* 사이퍼카드 등록
	*
	* @param listener
	*/
	int Send_RegCiphercard(std::string strSerialno, std::string strPublicKey, std::string strSureyHash, std::string UserID, std::string strUserName, const cocos2d::network::ccHttpRequestCallback& callback);
	


	/**
	* 송금요청
	*
	* @param listener
	*/
	int Send_ReqTx(unsigned int unReceiveAccnt, unsigned int unSendAccnt, float fSendAmt, std::string strMemo, const cocos2d::network::ccHttpRequestCallback& callback);

	/**
	* 거래내역조회 상태코드
	*
	* @param listener
	*/
	int Send_TxHist(unsigned int unAccnt, unsigned int nSendAccnt, const cocos2d::network::ccHttpRequestCallback& callback);
	
	int Send_ReqExchanage(unsigned int unUserSeq, unsigned int unAccSeq, unsigned int unPayAmount, std::string strBitCurr, float unBitAmt, const cocos2d::network::ccHttpRequestCallback& callback);
    int Send_ReqExchanageHistory(unsigned int unUserSeq, const cocos2d::network::ccHttpRequestCallback& callback);
    
	int Send_GetExchangeRate(std::string strCode, const cocos2d::network::ccHttpRequestCallback& callback);
    
	int Send_GetGetAccountInfo(unsigned int unAccSeq, const cocos2d::network::ccHttpRequestCallback& callback);

	/**
	* OTP Compare request 함수
	*
	* @param listener
	*/
	int Send_CheckOtp(std::string strUserID, std::string strOtpValue);
	void onHttpRequest_CheckOtp(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);


	int Send_GetOtp(std::string strOtpValue);
	void onHttpRequest_GetOtp(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response);
private:
	std::string m_strToken;

};
