#pragma once 
#include "cocos2d.h"

struct sUSER_DATA
{
	sUSER_DATA()
	{
		u64Seq = 0;
	}

	uint64_t u64Seq;
	std::string strUserID		= "";
	std::string strDtreg		= "";
	std::string strUserName		= "";
	std::string strSurveyHash	= "";
	std::string strPublicKey	= "";
};

struct sBANKACCOUNT_DATA
{
	sBANKACCOUNT_DATA()
	{
		u64AccntSeq = 0;
		dAmt		= 0;
		u64UserSeq	= 0;
	}

	uint64_t u64AccntSeq;
	uint64_t u64UserSeq;
	double dAmt;
	std::string strDtreg		= "";
	std::string strCurrency		= "";
	std::string strAccntName	= "";
};

struct sCODE_DATA
{
	int nCodeSeq = 0;
	std::string strType;
	std::string strCode;
	std::string strCodeKr;
	std::string strVal;
};

struct sRTX_HISTORY_DATA
{
    unsigned int nTransferSeq;
    unsigned int nSendAccnt;
    std::string tSendTime;
    std::string strMemo;
    float fSendAmt;
    std::string strMessage;
    std::string strSendAccntName;
    std::string strSendCurrency;
    std::string strReceiveAccntName;
};

struct sEXCHANGE_HISTORY_DATA
{
    unsigned int unPurchaseSeq;
    std::string strBidCurrency;
    std::string tPurchase;
    float       fBidAmt;
    std::string strPayCurrency;
    float       fPayAmount;
	std::string strStatusDesc;
	std::string strStatus;
};

struct sRTX_RESULT_DATA
{
    std::string strReceiveName;
    std::string tSendTime;
    float fAmtSent;
    float fSenderBalance;
    float fAmt;
};

struct sEXCHANGE_RESULT_DATA
{
    std::string tSendTime;
    float fBitAmt;
    float fAmt;
};

class DataManager
{
private:
	DataManager(){}

public:
	static DataManager* Create();
	static DataManager* GetInstance();
	static void Destroy();

public:
	void InitData();
	void CopyUserData(sUSER_DATA& data)
	{
		data = m_sUserData;
	}

	void SetUserData(const sUSER_DATA& data)
	{
		m_sUserData = data;
	}
    
    void ClearUserData()
    {
        m_sUserData = sUSER_DATA();
    }

	bool CopyBankAccountsData(sBANKACCOUNT_DATA& data, unsigned int nIndex)
	{
		if (m_vecBankAccountData.empty() || m_vecBankAccountData.size() < nIndex)
		{
			return false;
		}

		data = m_vecBankAccountData[nIndex - 1];

		return true;
	}

	const std::vector< sBANKACCOUNT_DATA >* GetBackAccountList() const
	{
		return &m_vecBankAccountData;
	}

    void AddankAccountsData(const sBANKACCOUNT_DATA& data);
    
    void ClearBankAccountData()
    {
        m_vecBankAccountData.clear();
    }

	void AddCodeData(const sCODE_DATA& dat);
	const std::vector< sCODE_DATA >* GetNationList()
	{
		return &m_vecNationList;
	}

	const std::vector< sCODE_DATA >* GetBloodList()
	{
		return &m_vecBloodList;
	}
	
	const std::vector< sCODE_DATA >* GetCityList()
	{
		return &m_vecCityList;
	}

	const std::vector< sCODE_DATA >* GetReligionList()
	{
		return &m_vecReligion;
	}

	void SetSurVeryHash(const std::string& strHash)
	{
		m_strSurVeryHash = strHash;
	}

	std::string GetSurVeryHash() const
	{
		return m_strSurVeryHash;
	}

	void SetTimeHash(const std::string& strHash)
	{
		m_strTimeHash = strHash;
	}

	std::string GetTimeHash()
	{
		return m_strTimeHash;
	}

	void SetCardSerial(const std::string& strCardSerial)
	{
		m_strCardSerial = strCardSerial;
	}

	std::string GetCardSerial() const
	{
		return m_strCardSerial;
	}

    void AddRtxHisToryList( const sRTX_HISTORY_DATA dat )
    {
        m_vecRtxHistoryList.push_back(dat);
    }
    
    const std::vector< sRTX_HISTORY_DATA >* GetRtxHistoryDataList() const
    {
        return &m_vecRtxHistoryList;
    }
    
    void RtxHistoryClear()
    {
        m_vecRtxHistoryList.clear();
    }
    
    void AddExchangeHisToryList( const sEXCHANGE_HISTORY_DATA dat )
    {
        m_vecExchangeHistoryList.push_back(dat);
    }
    
    const std::vector< sEXCHANGE_HISTORY_DATA >* GetExchangeHistoryDataList() const
    {
        return &m_vecExchangeHistoryList;
    }
    
    void ExchangeHistoryClear()
    {
        m_vecExchangeHistoryList.clear();
    }
    
    void SetRtxResultData( const sRTX_RESULT_DATA& dat )
    {
        m_sRxtResultData = dat;
    }
    
    const sRTX_RESULT_DATA& GetRtxResultData() const
    {
        return m_sRxtResultData;
    }
    
    void SetExchangeResultData( const sEXCHANGE_RESULT_DATA& dat )
    {
        m_sExchangeResultData = dat;
    }
    
    const sEXCHANGE_RESULT_DATA& GetExchangeResultData() const
    {
        return m_sExchangeResultData;
    }
    
	std::string GetOtp();
	std::string WCHARConvertToUTF8(const wchar_t* str1, const char* str2);
	std::wstring UTF8toUnicode(const std::string& s);
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	bool UTF8toEuc_kr(const std::string& s, char* pBuffer);
#endif
private:
	sUSER_DATA m_sUserData;
	std::string m_strSurVeryHash;
	std::string m_strCardSerial;
	std::string m_strTimeHash;
	std::vector< sBANKACCOUNT_DATA > m_vecBankAccountData;
	std::vector< sCODE_DATA > m_vecNationList;
	std::vector< sCODE_DATA > m_vecBloodList;
	std::vector< sCODE_DATA > m_vecCityList;
	std::vector< sCODE_DATA > m_vecReligion;
    std::vector< sRTX_HISTORY_DATA > m_vecRtxHistoryList;
    std::vector< sEXCHANGE_HISTORY_DATA > m_vecExchangeHistoryList;
    sRTX_RESULT_DATA m_sRxtResultData;
    sEXCHANGE_RESULT_DATA m_sExchangeResultData;
    
};
