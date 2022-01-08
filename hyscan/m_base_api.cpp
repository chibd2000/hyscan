#include "m_base_api.h"

MYAPI::MYAPI()
{

}


MYAPI::~MYAPI()
{

}

string MYAPI::ConvertToStringSid(const unsigned char* bsid, const int len){
	if (len < 8)  // at least 8 bytes
	{
		return "";
	}

	char buf[1024] = { 0 };
	string sid("S");

	// revision
	int revision = bsid[0];
	memset(buf, 0, sizeof(buf));
	sprintf_s(buf, "-%d", revision);
	sid.append(buf);

	// 6 types
	unsigned char temp[6] = { 0 };
	for (int i = 0; i < 6; ++i)
	{
		temp[6 - i - 1] = bsid[2 + i];
	}
	long long d3 = 0;
	memcpy(&d3, temp, 6);

	memset(buf, 0, sizeof(buf));
	sprintf_s(buf, "-%ld", d3);
	sid.append(buf);

	// 32bit (4bytes) dashes
	int dashes = (int)bsid[1];  // second byte determines dash number. dashes = total dashes - 2

	if (dashes * 4 != len - 8)
	{
		return "";  // wrong format
	}

	for (int i = 0; i < dashes; ++i)
	{
		unsigned int v = 0;
		memcpy(&v, bsid + 8 + i * 4, 4);

		memset(buf, 0, sizeof(buf));
		sprintf_s(buf, "-%u", v);
		sid.append(buf);
	}

	return sid;
}

// sid -> username
string MYAPI::sid2user(PSID pSid, LPCTSTR lpSystemName){
	SID_NAME_USE sidNameUse = SidTypeUser;
	TCHAR szUserBuffer[64] = { 0 };
	TCHAR pRefDomainName[64] = { 0 };
	DWORD cbName = MAX_PATH;
	DWORD cbRefDomainName = MAX_PATH;
	if (LookupAccountSid(NULL, pSid, szUserBuffer, &cbName, pRefDomainName, &cbRefDomainName, &sidNameUse))
		return string(szUserBuffer);
	return "(null)";
}

// username -> sid
string MYAPI::getCurrentUserSid(){
	TCHAR userName[MAX_PATH] = "";
	DWORD dwNameSize = sizeof(userName);
	PCHAR szSid;

	SID_NAME_USE sidNameUse;
	TCHAR szUserSID[MAX_PATH] = "";
	TCHAR szDomainName[MAX_PATH] = "";
	DWORD dwSidSize = MAX_PATH;
	DWORD dwDomainNameSize = MAX_PATH;

	GetUserName(userName, &dwNameSize);
	LookupAccountName(NULL, userName, szUserSID, &dwSidSize, szDomainName, &dwDomainNameSize, &sidNameUse);
	ConvertSidToStringSid(szUserSID, &szSid);
	return string(szSid);
}