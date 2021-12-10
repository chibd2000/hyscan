#include "NtlmParser.h"

NtlmParser::NtlmParser()
{
	
}


NtlmParser::~NtlmParser()
{

}


void NtlmParser::parser(string ntlmInfoString, NtlmInfo* p1NtlmInfo){
	SYSTEMTIME sysTime;
	wstring wStr;
	int offset = 0;
	int size = 0;
	NtlmInfo* pNtlmInfo = (NtlmInfo*)(ntlmInfoString.data() + BASE_NTLMSSP);
	//wprintf(L"MACHINE_NAME: %s\n", wStr.assign(wstring(pNtlmInfo->MACHINE_NAME), 0, *pNtlmInfo->MACHINE_LENGTH/2).c_str());	
	memcpy((char*)p1NtlmInfo->MACHINE_NAME, (char*)(wStr.assign(wstring(pNtlmInfo->MACHINE_NAME), 0, *pNtlmInfo->MACHINE_LENGTH / 2).c_str()), *pNtlmInfo->MACHINE_LENGTH);
	//wprintf(L"%s\n", p1NtlmInfo->MACHINE_NAME);

	offset += pNtlmInfo->OFFSET_2 + 4;
	size = *((char*)pNtlmInfo + pNtlmInfo->OFFSET_2 + 2) / 2;
	//wprintf(L"\tNETBIOS_COMPUTER_NAME[0]: %s\n", wStr.assign(wstring((wchar_t*)((char*)pNtlmInfo + offset)), 0, size).c_str());
	memcpy((char*)p1NtlmInfo->NETBIOS_ATTR[0].NETBIOS_COMPUTER_NAME, (char*)(wStr.assign(wstring((wchar_t*)((char*)pNtlmInfo + offset)), 0, size).c_str()), size*2);
	//wprintf(L"\t%s\n", p1NtlmInfo->NETBIOS_ATTR[0].NETBIOS_COMPUTER_NAME);

	offset += *((char*)pNtlmInfo + offset - 2) + 4;
	size = *((char*)pNtlmInfo + offset - 2) / 2;
	//wprintf(L"\tNETBIOS_COMPUTER_NAME[1]: %s\n", wStr.assign(wstring((wchar_t*)((char*)pNtlmInfo + offset)), 0, size).c_str());
	memcpy((char*)p1NtlmInfo->NETBIOS_ATTR[1].NETBIOS_COMPUTER_NAME, (char*)(wStr.assign(wstring((wchar_t*)((char*)pNtlmInfo + offset)), 0, size).c_str()), size*2);
	//wprintf(L"\t%s\n", p1NtlmInfo->NETBIOS_ATTR[1].NETBIOS_COMPUTER_NAME);

	offset += *((char*)pNtlmInfo + offset -2) + 4;
	size = *((char*)pNtlmInfo + offset - 2) / 2;
	//wprintf(L"\tDNS_COMPUTER_NAME[0]: %s\n", wStr.assign(wstring((wchar_t*)((char*)pNtlmInfo + offset)), 0, size).c_str());
	memcpy((char*)p1NtlmInfo->DNS_ATTR[0].DNS_COMPUTER_NAME, (char*)(wStr.assign(wstring((wchar_t*)((char*)pNtlmInfo + offset)), 0, size).c_str()), size*2);
	//wprintf(L"\t%s\n", p1NtlmInfo->DNS_ATTR[0].DNS_COMPUTER_NAME);

	offset += *((char*)pNtlmInfo + offset - 2) + 4;
	size = *((char*)pNtlmInfo + offset - 2) / 2;
	//wprintf(L"\tDNS_COMPUTER_NAME[1]: %s\n", wStr.assign(wstring((wchar_t*)((char*)pNtlmInfo + offset)), 0, size).c_str());
	memcpy((char*)p1NtlmInfo->DNS_ATTR[1].DNS_COMPUTER_NAME, (char*)(wStr.assign(wstring((wchar_t*)((char*)pNtlmInfo + offset)), 0, size).c_str()), size*2);
	//wprintf(L"\t%s\n", p1NtlmInfo->DNS_ATTR[1].DNS_COMPUTER_NAME);

	//offset += *((char*)pNtlmInfo + offset - 2) + 4;
	//FileTimeToSystemTime((FILETIME*)((char*)pNtlmInfo + offset - 4), &sysTime);
	//printf("\tTIMESTAMP: %d:%d:%d %d:%d:%d", sysTime.wYear, sysTime.wMonth, sysTime.wDay, sysTime.wHour, sysTime.wMinute, sysTime.wMilliseconds);
	
}