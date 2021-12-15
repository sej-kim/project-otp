/**
@file KISA_SHA_256.h
@brief SHA256 ��ȣ �˰���
@author Copyright (c) 2013 by KISA
@remarks http://seed.kisa.or.kr/
*/

#ifndef SHA256_H
#define SHA256_H

#ifdef  __cplusplus
extern "C" {
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IN
#define IN
#endif

#ifndef INOUT
#define INOUT
#endif

#undef BIG_ENDIAN
#undef LITTLE_ENDIAN

#if defined(USER_BIG_ENDIAN)
	#define BIG_ENDIAN
#elif defined(USER_LITTLE_ENDIAN)
	#define LITTLE_ENDIAN
#else
	#if __ANDROID__
		#define BIG_ENDIAN
	#elif defined(_MSC_VER)
		#define LITTLE_ENDIAN
	#else
		//#error
	#endif
#endif

typedef unsigned long ULONG_SHA;
typedef ULONG_SHA* ULONG_PTR_SHA;

typedef unsigned int UINT_SHA;
typedef UINT_SHA* UINT_PTR_SHA;

typedef signed int SINT_SHA;
typedef SINT_SHA* SINT_PTR_SHA;

typedef unsigned char UCHAR_SHA;
typedef UCHAR_SHA* UCHAR_PTR_SHA;

typedef unsigned char BYTE_SHA;

#define SHA256_DIGEST_BLOCKLEN	64
#define SHA256_DIGEST_VALUELEN	32

typedef struct{
	UINT_SHA uChainVar[SHA256_DIGEST_VALUELEN / 4];
	UINT_SHA uHighLength;
	UINT_SHA uLowLength;
	BYTE_SHA szBuffer[SHA256_DIGEST_BLOCKLEN];
} SHA256_INFO;

/**
@brief ���⺯���� ���̺����� �ʱ�ȭ�ϴ� �Լ�
@param Info : SHA256_Process ȣ�� �� ���Ǵ� ����ü
*/
void SHA256_Init( OUT SHA256_INFO *Info );

/**
@brief ���⺯���� ���̺����� �ʱ�ȭ�ϴ� �Լ�
@param Info : SHA256_Init ȣ���Ͽ� �ʱ�ȭ�� ����ü(���������� ���ȴ�.)
@param pszMessage : ����� �Է� ��
@param inLen : ����� �Է� �� ����
*/
void SHA256_Process(OUT SHA256_INFO *Info, IN const BYTE_SHA *pszMessage, IN UINT_SHA uDataLen);

/**
@brief �޽��� �����̱�� ���� �����̱⸦ ������ �� ������ �޽��� ����� ������ �����Լ��� ȣ���ϴ� �Լ�
@param Info : SHA256_Init ȣ���Ͽ� �ʱ�ȭ�� ����ü(���������� ���ȴ�.)
@param pszDigest : ��ȣ��
*/
void SHA256_Close(OUT SHA256_INFO *Info, OUT BYTE_SHA *pszDigest);

/**
@brief ����� �Է� ���� �ѹ��� ó��
@param pszMessage : ����� �Է� ��
@param pszDigest : ��ȣ��
@remarks ���������� SHA256_Init, SHA256_Process, SHA256_Close�� ȣ���Ѵ�.
*/
void SHA256_Encrpyt(IN const BYTE_SHA *pszMessage, IN UINT_SHA uPlainTextLen, OUT BYTE_SHA *pszDigest);

#ifdef  __cplusplus
}
#endif

#endif
