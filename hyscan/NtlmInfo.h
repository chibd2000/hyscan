#ifndef NTLMINFO_H_H
#define NTLMINFO_H_H
#define BASE_WMI_NTLMSSP 68
#define BASE_SMB_NTLMSSP 78


/*
oxid resolver struct
*/

class OXID_RESOLOVER_INFORMATION{
public:
	
};


/*
* for ntlm struct, write in 2021.12.09 23.18 @zpchcbd
*/
class NETBIOS_ATTRIBUTEINFORMATION{
public:
	wchar_t INFOR_TYPE;
	wchar_t INFOR_LENGTH;
	wchar_t NETBIOS_COMPUTER_NAME[30];
};

class DNS_ATTRIBUTEINFORMATION{
public:
	wchar_t INFOR_TYPE;
	wchar_t INFOR_LENGTH;
	wchar_t DNS_COMPUTER_NAME[30];
};	

class FILETIME_ {
public:
	unsigned int dwLowDateTime;
	unsigned int dwHighDateTime;
};

class TIMESTAMP_ATTRIBUTEINFORMATION
{
public:
	wchar_t INFOR_TYPE;
	wchar_t INFOR_LENGTH;
	FILETIME_ INFOR_FIELTIME;
};

class VERSION_ATTRIBUTEINFORMATION
{
public:
	char MAJOR_VERSION;
	char MINOR_VERSION;
	char BUILD_NUMBER[2];
	int Revision;
};


class NtlmInfo
{
public:
	NtlmInfo();
	~NtlmInfo();
public:
	/*以特征符NTLMSSP为Base地址*/
	unsigned char NTLMSSP[8];  // NTLMSSP
	int NTLM_MESSAGE_TYPE; // 0x00000002
	unsigned char MACHINE_LENGTH[2]; // 1
	unsigned char MACHINE_MAX_LENGTH[2];
	int OFFSET_1; // name offset
	unsigned char NEGOTIATE_FALGS[4];
	unsigned char NEGOTIATE_CHALLENGE[8];
	unsigned char RESERVED[8];
	unsigned char TARGET_INFO_LENGTH[2];
	unsigned char TARGET_INFO_MAX_LENGTH[2];
	int OFFSET_2;
	VERSION_ATTRIBUTEINFORMATION VERSION_ATTR;
	wchar_t MACHINE_NAME[30];
	NETBIOS_ATTRIBUTEINFORMATION NETBIOS_ATTR[2];
	DNS_ATTRIBUTEINFORMATION DNS_ATTR[2];
	TIMESTAMP_ATTRIBUTEINFORMATION TIME_ATTR;
	//int inAD;
	char ipAddr[0x10];
};

#endif