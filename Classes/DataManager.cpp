#include "DataManager.h"
#include "hmac_sha1.h"
#include "math/CCMath.h"
#include "MyBase64.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
#include "iconv.h"
#endif

static DataManager* gs_pDataManager = NULL;

DataManager* DataManager::Create()
{
	if (gs_pDataManager)
	{
		DataManager::Destroy();
	}

	gs_pDataManager = new DataManager();

	return gs_pDataManager;
}

void DataManager::Destroy()
{
	CC_SAFE_DELETE(gs_pDataManager);
}

void DataManager::InitData()
{
	m_sUserData = sUSER_DATA();
}

DataManager* DataManager::GetInstance()
{
	return gs_pDataManager;
}

std::string DataManager::WCHARConvertToUTF8(const wchar_t* str1, const char* str2)
{
	char msg[128] = { 0, };
#if CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	return str2;
#else
	WideCharToMultiByte(CP_UTF8, 0, str1, -1, msg, 128, 0, 0);
	return msg;
#endif

	
}

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
bool DataManager::UTF8toEuc_kr(const std::string& s, char* pBuffer)
{
	size_t in_size, out_size;

	// 어떤 시스템에서는 char** 가 아니라 const char** 인 경우도 있음
	char *input_buf_ptr = (char*)s.c_str();
	char *output_buf_ptr = pBuffer;

	in_size = s.length();
	out_size = 1024;

	iconv_t it = iconv_open("EUC-KR", "UTF-8"); // UTF-8을 EUC-KR로
	int ret = iconv(it, (const char**)&input_buf_ptr, &in_size, &output_buf_ptr, &out_size);

	iconv_close(it);
	return ret > 0;
}
#endif

void DataManager::AddCodeData(const sCODE_DATA& dat)
{
	if (dat.strType.compare("regA") == 0)
	{
		m_vecNationList.push_back(dat);
	}
	else if (dat.strType.compare("regB") == 0)
	{
		m_vecBloodList.push_back(dat);
	}
	else if (dat.strType.compare("regC") == 0)
	{
		m_vecReligion.push_back(dat);
	}
	else if (dat.strType.compare("currency") == 0)
	{
		m_vecCityList.push_back(dat);
	}
}

void DataManager::AddankAccountsData(const sBANKACCOUNT_DATA& data)
{
    for( int i_1 = 0; m_vecBankAccountData.size(); ++i_1 )
    {
        sBANKACCOUNT_DATA* temp = &m_vecBankAccountData[i_1];
        if(temp && temp->u64AccntSeq == data.u64AccntSeq)
        {
            *temp = data;
            return;
        }
    }
    
    m_vecBankAccountData.push_back(data);
}

std::wstring DataManager::UTF8toUnicode(const std::string& s)
{
	std::wstring ws;
	wchar_t wc;
	for (unsigned int i = 0; i < s.length();)
	{
		char c = s[i];
		if ((c & 0x80) == 0)
		{
			wc = c;
			++i;
		}
		else if ((c & 0xE0) == 0xC0)
		{
			wc = (s[i] & 0x1F) << 6;
			wc |= (s[i + 1] & 0x3F);
			i += 2;
		}
		else if ((c & 0xF0) == 0xE0)
		{
			wc = (s[i] & 0xF) << 12;
			wc |= (s[i + 1] & 0x3F) << 6;
			wc |= (s[i + 2] & 0x3F);
			i += 3;
		}
		else if ((c & 0xF8) == 0xF0)
		{
			wc = (s[i] & 0x7) << 18;
			wc |= (s[i + 1] & 0x3F) << 12;
			wc |= (s[i + 2] & 0x3F) << 6;
			wc |= (s[i + 3] & 0x3F);
			i += 4;
		}
		else if ((c & 0xFC) == 0xF8)
		{
			wc = (s[i] & 0x3) << 24;
			wc |= (s[i] & 0x3F) << 18;
			wc |= (s[i] & 0x3F) << 12;
			wc |= (s[i] & 0x3F) << 6;
			wc |= (s[i] & 0x3F);
			i += 5;
		}
		else if ((c & 0xFE) == 0xFC)
		{
			wc = (s[i] & 0x1) << 30;
			wc |= (s[i] & 0x3F) << 24;
			wc |= (s[i] & 0x3F) << 18;
			wc |= (s[i] & 0x3F) << 12;
			wc |= (s[i] & 0x3F) << 6;
			wc |= (s[i] & 0x3F);
			i += 6;
		}
		ws += wc;
	}
	return ws;
}

std::string DataManager::GetOtp()
{
	std::string strKey = base64_decode("zI1PVM3d7hR+gMSNbDdUFIdUdaMj+kBxceKhWWqCqR0=");

	
	time_t now;
	time(&now);
	char szTime[64];

	sprintf(szTime, "%d", (now / 30));

	char out[256] = { 0 };
	size_t len = sizeof(out);

	hmac_sha1((uint8_t*)strKey.c_str(), strKey.length(), (uint8_t*)szTime, strlen(szTime), (uint8_t*)out, &len);

	int offset = out[19] & 0xf;

	int binay = ((out[offset] & 0x7f) << 24) | ((out[offset + 1] & 0xff) << 16) |
	((out[offset + 2] & 0xff) << 8) | (out[offset + 3] & 0xff);

	int otp1 = binay % ((int)pow(10, 6));
	cocos2d::log("%d", otp1);

	char szTemp[16] = {0, };
	sprintf(szTemp, "%d", otp1);
	std::string strOtp = szTemp;
	while (strOtp.length() < 6)
	{
		strOtp = "0" + strOtp;
	}

	return strOtp;
}
