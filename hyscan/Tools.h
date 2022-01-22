#ifndef TOOLS_H_H
#define TOOLS_H_H
#include "public.h"

#define _IP_MARK "."
string wchar2Char(wchar_t* wc);
wstring string2Wstring(const string& str);
int cidr2ipmask(char* cidr, unsigned int* ip, unsigned int* mask);
string int2ip(unsigned int num);
string base64decode(string const& encodedString, string base64chars);
string formatString(string& formatStr, ...);
vector<string> split(const string& str, const string& delim);
u16string utf8toUtf16le(const string& u8str, bool addbom = false, bool* ok = NULL);
string octet2String(const char * src_in, int size);
BYTE str2byte(const string &str);
string getHexString(const string& jsonString);
string gbk2Utf8(const string& srcString);
string utf82Gbk(const string& srcString);
#endif