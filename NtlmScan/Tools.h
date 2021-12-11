#ifndef TOOLS_H_H
#define TOOLS_H_H
#define _CRT_SECURE_NO_WARNINGS
#include "public.h"
#define _IP_MARK "."
#define DEFAULTBASE64CHAR "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
string WcharToChar(wchar_t* wc);
wstring StringToWString(const std::string& str);
int cidr2ipmask(char* cidr, unsigned int* ip, unsigned int* mask);
string int2ip(unsigned int num);
string base64decode(string const& encodedString, string base64chars);
#endif