#include "HttpManager.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "Define.h"
#include "DataManager.h"

USING_NS_CC;

using namespace network;
static HttpManager* sg_pHttpManager		= NULL;
static const std::string OTPPROTOCOL	= "http";
static const std::string OTPDOMAIN		= "cyphercard.koreaframing.com";
static const std::string OTPQUE			= "/api/v1/getTotp";

static const std::string SERVER_PROTOCOL	= "http://";
static const std::string SERVER_DOMAIN		= "cyphercard.koreaframing.com";
static const std::string SERVER_OTPQUE		= "/api/v1/";


HttpManager::HttpManager()
{
	sg_pHttpManager = this;

	init();
}

HttpManager::~HttpManager()
{
}

HttpManager* HttpManager::Create()
{
	if (sg_pHttpManager != NULL)
	{
		HttpManager::Destroy();
	}

	sg_pHttpManager = new HttpManager();
	sg_pHttpManager->init();

	return sg_pHttpManager;
}

void HttpManager::Destroy()
{
	CC_SAFE_DELETE(sg_pHttpManager);
}

HttpManager* HttpManager::GetInstance()
{
	return sg_pHttpManager;
}


bool HttpManager::init()
{
	return true;
}


int HttpManager::Send_IDCheck(const std::string strUserID, const ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getIdDupCheck";

	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_IDCheck");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value IDValue;
	IDValue.SetString(strUserID.c_str(), allocator);

	document.AddMember("id", IDValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
	return 0;
}

int HttpManager::Send_GetCodeList(const std::string strCodeType, const std::string strUpcode, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getCodeList";

	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_IDCheck");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value CodeValue;
	CodeValue.SetString(strCodeType.c_str(), allocator);

	//rapidjson::Value UpcodeValue;
	//UpcodeValue.SetString(strUpcode.c_str(), allocator);

	document.AddMember("cateType", CodeValue, allocator);
	//document.AddMember("upcode", UpcodeValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
	return 0;
}

int HttpManager::Send_GetUserInfo(const std::string strUserID, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getUserInfo";
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_GetUserInfo");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value IDValue;
	IDValue.SetString(strUserID.c_str(), allocator);

	document.AddMember("id", IDValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
	return 0;
}

int HttpManager::Send_GetBackAccounts(const uint64_t u64UserSeq, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getUserBankAccounts";
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_GetBackAccounts");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value IDValue;
	IDValue.SetUint64(u64UserSeq);
	document.AddMember("userSeq", IDValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
	return 0;
}

int HttpManager::Send_GetTimeHash(const cocos2d::network::ccHttpRequestCallback& callback)
{
	timeval time;
	gettimeofday(&time, NULL);

	char szTime[128] = { 0, };
	sprintf(szTime, "?messageToHash=%ld", time.tv_sec);

	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getStringHash" + szTime;
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_GetTimeHash");

	//rapidjson::Document document;
	//document.SetObject();
	//rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	//rapidjson::Value TimeValue;


	//document.AddMember("messageToHash", TimeValue, allocator);

	//rapidjson::StringBuffer strbuf;
	//rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	//document.Accept(writer);

	//request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	//cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)

	return 0;
}



int HttpManager::Send_GetSurveyHash(const sSURVEY_DATA& data, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getSurveyHash";
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_GetSurveyHash");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value UserNameValue, P1Value, P2Value, P3Value, P4Value, P5Value, P6Value, P7Value, P8Value, P9Value, P10Value, P11Value, P12Value;
	
	UserNameValue.SetString(data.strSurvey[sSURVEY_DATA::USER_NAME].c_str(), allocator);
	P1Value.SetString(data.strSurvey[sSURVEY_DATA::q1].c_str(), allocator);
	P2Value.SetString(data.strSurvey[sSURVEY_DATA::q2].c_str(), allocator);
	P3Value.SetString(data.strSurvey[sSURVEY_DATA::q3].c_str(), allocator);
	P4Value.SetString(data.strSurvey[sSURVEY_DATA::q4].c_str(), allocator);
	P5Value.SetString(data.strSurvey[sSURVEY_DATA::q5].c_str(), allocator);
	P6Value.SetString(data.strSurvey[sSURVEY_DATA::q6].c_str(), allocator);
	P7Value.SetString(data.strSurvey[sSURVEY_DATA::q7].c_str(), allocator);
	P8Value.SetString(data.strSurvey[sSURVEY_DATA::q8].c_str(), allocator);
	P9Value.SetString(data.strSurvey[sSURVEY_DATA::q10].c_str(), allocator);
	P10Value.SetString(data.strSurvey[sSURVEY_DATA::q11].c_str(), allocator);
	P11Value.SetString(data.strSurvey[sSURVEY_DATA::q12].c_str(), allocator);
	P12Value.SetString(data.strSurvey[sSURVEY_DATA::q13].c_str(), allocator);

	document.AddMember("username", UserNameValue, allocator);
	document.AddMember("q1", P1Value, allocator);
	document.AddMember("q2", P2Value, allocator);
	document.AddMember("q3", P3Value, allocator);
	document.AddMember("q4", P4Value, allocator);
	document.AddMember("q5", P5Value, allocator);
	document.AddMember("q6", P6Value, allocator);
	document.AddMember("q7", P7Value, allocator);
	document.AddMember("q8", P8Value, allocator);
	document.AddMember("q10", P9Value, allocator);
	document.AddMember("q11", P10Value, allocator);
	document.AddMember("q12", P11Value, allocator);
	document.AddMember("q13", P12Value, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)

	return 0;
}

int HttpManager::Send_RegCiphercard(std::string strSerialno, std::string strPublicKey, std::string strSureyHash, std::string strUserID, std::string strUserName, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "regCiphercard";
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_RegCiphercard");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value CardSerialnoValue;
	CardSerialnoValue.SetString(strSerialno.c_str(), allocator);

	rapidjson::Value PublicKeyValue;
	PublicKeyValue.SetString(strPublicKey.c_str(), allocator);

	rapidjson::Value SureyHashValue;
	SureyHashValue.SetString(strSureyHash.c_str(), allocator);

	rapidjson::Value UserIDValue;
	UserIDValue.SetString(strUserID.c_str(), allocator);

	rapidjson::Value UserNameValue;
	UserNameValue.SetString(strUserName.c_str(), allocator);

	document.AddMember("cardSerial", CardSerialnoValue, allocator);
	document.AddMember("publicKey", PublicKeyValue, allocator);
	document.AddMember("surveyHash", SureyHashValue, allocator);
	document.AddMember("userId", UserIDValue, allocator);
	document.AddMember("userName", UserNameValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
	return 0;
}


int HttpManager::Send_ReqTx(unsigned int unReceiveAccnt, unsigned int unSendAccnt, float fSendAmt, std::string strMemo, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "reqTx";
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_ReqTx");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value ReceiveAccntValue;
	ReceiveAccntValue.SetUint(unReceiveAccnt);

	rapidjson::Value SendAccntValue;
	SendAccntValue.SetUint(unSendAccnt);

	rapidjson::Value SendAmtValue;
	SendAmtValue.SetDouble(fSendAmt);

	rapidjson::Value MemoValue;
	MemoValue.SetString(strMemo.c_str(), allocator);

	document.AddMember("receiveAccnt", ReceiveAccntValue, allocator);
	document.AddMember("sendAccnt", SendAccntValue, allocator);
	document.AddMember("sendAmt", SendAmtValue, allocator);
	document.AddMember("sendMemo", MemoValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
	return 0;
}

int HttpManager::Send_TxHist(unsigned int unAccnt, unsigned int unSendAccnt, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getTxHist";
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_TxHist");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value UserIDValue;
	rapidjson::Value AccntValue;
	AccntValue.SetUint(unAccnt);

	rapidjson::Value SendAccntValue;
	SendAccntValue.SetUint(unSendAccnt);

	document.AddMember("receiveAccnt", AccntValue, allocator);
	document.AddMember("sendAccnt", SendAccntValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
		return 0;
}

int HttpManager::Send_ReqExchanage(unsigned int unUserSeq, unsigned int unAccSeq, unsigned int unPayAmount, std::string strBitCurr, float fBitAmt, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "reqExchange";
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_ReqExchanage");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value UserSeqValue;
	UserSeqValue.SetUint(unUserSeq);

    rapidjson::Value AccSeqSeqValue;
    AccSeqSeqValue.SetUint(unAccSeq);
    
    rapidjson::Value PayAmountValue;
    PayAmountValue.SetUint(unPayAmount);
    
    rapidjson::Value BitCurrValue;
    BitCurrValue.SetString(strBitCurr.c_str(), allocator);
    
    rapidjson::Value BitAmtValue;
    BitAmtValue.SetDouble(fBitAmt);
    
    rapidjson::Value PayCurrencyValue;
    PayCurrencyValue.SetString("KRW", allocator);
    
	document.AddMember("userSeq", UserSeqValue, allocator);
    document.AddMember("accntSeq", AccSeqSeqValue, allocator);
    document.AddMember("payAmount", PayAmountValue, allocator);
    document.AddMember("bidCurrency", BitCurrValue, allocator);
    document.AddMember("bidAmt", BitAmtValue, allocator);
    document.AddMember("payCurrency", PayCurrencyValue, allocator);
    
	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
    return 0;
}

int HttpManager::Send_ReqExchanageHistory(unsigned int unUserSeq, const cocos2d::network::ccHttpRequestCallback& callback)
{
    std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getExchangeHist";
    std::vector<std::string> header;
    header.push_back("Accept: application/json");
    header.push_back("Content-Type: application/json");
    
    HttpRequest* request = new HttpRequest();
    request->setRequestType(HttpRequest::Type::POST);
    request->setHeaders(header);
    request->setUrl(strUrl.c_str());
    request->setResponseCallback(callback);
    request->setTag("Send_ReqExchanageHistory");
    
    rapidjson::Document document;
    document.SetObject();
    rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
    
    rapidjson::Value UserSeqValue;
    UserSeqValue.SetUint(unUserSeq);
    
    document.AddMember("userSeq", UserSeqValue, allocator);
    
    rapidjson::StringBuffer strbuf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
    document.Accept(writer);
    
    request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
    cocos2d::log("%s", strbuf.GetString());
    HttpClient::getInstance()->send(request);
    
    request->release();
    CREATE_PROGRESSDIALOG(0)
    return 0;
}


int HttpManager::Send_GetExchangeRate(std::string strCode, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getExchangeRate";
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_GetExchangeRate");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value CodeValue;
	CodeValue.SetString(strCode.c_str(), allocator);

	document.AddMember("bitCurrency", CodeValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
	return 0;
}

int HttpManager::Send_GetGetAccountInfo(unsigned int unAccSeq, const cocos2d::network::ccHttpRequestCallback& callback)
{
	std::string strUrl = SERVER_PROTOCOL + SERVER_DOMAIN + SERVER_OTPQUE + "getAccountInfo";
	std::vector<std::string> header;
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(callback);
	request->setTag("Send_GetGetAccountInfo");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value AccSeqValue;
	AccSeqValue.SetUint(unAccSeq);
	document.AddMember("accntSeq", AccSeqValue, allocator);

	rapidjson::Value AccSeqValue_1;
	AccSeqValue_1.SetUint(unAccSeq);
	document.AddMember("accnt_seq", AccSeqValue_1, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
		return 0;
}

int HttpManager::Send_CheckOtp(std::string strUserID, std::string strOtpValue)
{
	std::string strUrl = SERVER_PROTOCOL + "://" + SERVER_DOMAIN + "/" + "checkotpjson.do";

	std::vector<std::string> header;
	header.push_back("User-Agent: SmartOTP");
	header.push_back("Accept: application/json");
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(CC_CALLBACK_2(HttpManager::onHttpRequest_CheckOtp, this));
	request->setTag("Send_CheckOtp");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();

	rapidjson::Value MethodValue;
	MethodValue.SetString(std::string("checkotpjson.do").c_str(), allocator);

	rapidjson::Value IDValue;
	IDValue.SetString(strUserID.c_str(), allocator);

	rapidjson::Value OtpValueValue;
	OtpValueValue.SetString(strOtpValue.c_str(), allocator);

	document.AddMember("method", MethodValue, allocator);
	document.AddMember("userid", IDValue, allocator);
	document.AddMember("otpvalue", OtpValueValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();
	CREATE_PROGRESSDIALOG(0)
	return 0;
}

void HttpManager::onHttpRequest_CheckOtp(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
{
	if (!response)
	{
		return;
	}
	long statusCode = response->getResponseCode();

	cocos2d::log("response code: %ld", statusCode);

	if (!response->isSucceed())
	{
		cocos2d::log("response failed");
		cocos2d::log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char> *pData = response->getResponseData();
	std::string ret(&(pData->front()), pData->size());
	cocos2d::log("%s", ("Response message: " + ret).c_str());

	rapidjson::Document jsonDoc;
	jsonDoc.Parse(ret.c_str());
}


int HttpManager::Send_GetOtp(std::string strOtpValue)
{
	std::string strUrl = OTPPROTOCOL + "://" + OTPDOMAIN + OTPQUE;

	std::vector<std::string> header;
	header.push_back("Content-Type: application/json");

	HttpRequest* request = new HttpRequest();
	request->setRequestType(HttpRequest::Type::POST);
	request->setHeaders(header);
	request->setUrl(strUrl.c_str());
	request->setResponseCallback(CC_CALLBACK_2(HttpManager::onHttpRequest_GetOtp, this));
	request->setTag("Send_GetOtp");

	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();


	rapidjson::Value returnDigitsValue;
	returnDigitsValue.SetString("0", allocator);

	rapidjson::Value OtpValue;
	OtpValue.SetString(strOtpValue.c_str(), allocator);


	rapidjson::Value timestepwindowValue;
	timestepwindowValue.SetString("0", allocator);


	document.AddMember("returnDigits", returnDigitsValue, allocator);
	document.AddMember("sharedsecret", OtpValue, allocator);
	document.AddMember("timestepwindow", timestepwindowValue, allocator);

	rapidjson::StringBuffer strbuf;
	rapidjson::Writer<rapidjson::StringBuffer> writer(strbuf);
	document.Accept(writer);

	request->setRequestData(strbuf.GetString(), strbuf.GetSize() + 1);
	cocos2d::log("%s", strbuf.GetString());
	HttpClient::getInstance()->send(request);

	request->release();

	return 0;
}

void HttpManager::onHttpRequest_GetOtp(cocos2d::network::HttpClient* sender, cocos2d::network::HttpResponse *response)
{
	if (!response)
	{
		return;
	}
	long statusCode = response->getResponseCode();

	cocos2d::log("response code: %ld", statusCode);

	if (!response->isSucceed())
	{
		cocos2d::log("response failed");
		cocos2d::log("error buffer: %s", response->getErrorBuffer());
		return;
	}

	std::vector<char> *pData = response->getResponseData();
	std::string ret(&(pData->front()), pData->size());
	cocos2d::log("%s", ("Response message: " + ret).c_str());

	rapidjson::Document jsonDoc;
	jsonDoc.Parse(ret.c_str());
}
