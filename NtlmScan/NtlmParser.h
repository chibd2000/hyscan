#ifndef NTLMPARSER_H_H
#define NTLMPARSER_H_H
#define _CRT_SECURE_NO_WARNINGS
#include "NtlmInfo.h"
#include "public.h"

class NtlmParser
{
public:
	NtlmParser();
	~NtlmParser();
	static void NtlmParser::parser(string ntlmInfoString, NtlmInfo* p1NtlmInfo);

};
#endif
