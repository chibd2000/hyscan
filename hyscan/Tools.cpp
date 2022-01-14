#include "Tools.h"
#include <stdarg.h>
string wchar2Char(wchar_t* wc)
{
	char* m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	string w_str(m_char);
	return w_str;
}

wstring string2Wstring(const std::string& str)
{
	int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t *wide = new wchar_t[num];
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
	std::wstring w_str(wide);
	delete[] wide;
	return w_str;
}

int cidr2ipmask(char* cidr, unsigned int* ip, unsigned int* mask)
{
	//unsigned char cidr_[0x32];
	unsigned char b[5];
	//memcpy((char*)cidr_, cidr, 0x32);
	sscanf(cidr, "%hhx.%hhx.%hhx.%hhx/%hhx", &b[0], &b[1], &b[2], &b[3], &b[4]);
	if (b[4] > 32) {
		return -1;
	}
	
	*ip = (b[0] << 24UL) | (b[1] << 16UL) | (b[2] << 8UL) | (b[3]);
	*mask = (0xFFFFFFFFUL << (32 - b[4])) & 0xFFFFFFFFUL;
}

string int2ip(unsigned int num)
{
	string strRet = "";
	for (int i = 0; i < 4; i++)
	{
		uint32_t tmp = (num >> ((3 - i) * 8)) & 0xFF;

		char chBuf[8] = "";
		_itoa_s(tmp, chBuf, 10);
		strRet += chBuf;

		if (i < 3)
		{
			strRet += _IP_MARK;
		}
	}

	return strRet;
}

// 默认的base表解码
static inline bool is_base64(unsigned char c) {
	return (isalnum(c) || (c == '+') || (c == '/'));
}

string base64decode(string const& encodedString, string base64chars) {
	int in_len = encodedString.size();
	int i = 0;
	int j = 0;
	int in_ = 0;
	unsigned char char_array_4[4], char_array_3[3];
	string ret;

	while (in_len-- && (encodedString[in_] != '=') && is_base64(encodedString[in_])) {
		char_array_4[i++] = encodedString[in_]; in_++;
		if (i == 4) {
			for (i = 0; i <4; i++)
				char_array_4[i] = base64chars.find(char_array_4[i]);

			char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
			char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
			char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

			for (i = 0; (i < 3); i++)
				ret += char_array_3[i];
			i = 0;
		}
	}

	if (i) {
		for (j = i; j <4; j++)
			char_array_4[j] = 0;

		for (j = 0; j <4; j++)
			char_array_4[j] = encodedString.find(char_array_4[j]);

		char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
		char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
		char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

		for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
	}

	return ret;
}

string formatString(string& formatStr, ...){
	
	return NULL;
}

vector<string> split(const string& str, const string& delim) {
	vector<string> vString;
	if ("" == str) return vString;
	//先将要切割的字符串从string类型转换为char*类型  
	char* strs = new char[str.length() + 1]; //不要忘了  
	strcpy(strs, str.c_str());

	char* d = new char[delim.length() + 1];
	strcpy(d, delim.c_str());
	
	char* p = strtok(strs, d);
	while (p) {
		string s = p; //分割得到的字符串转换为string类型  
		vString.push_back(s); //存入结果数组  
		p = strtok(NULL, d);
	}

	delete d;
	delete strs;
	return vString;
}

std::u16string utf82Utf16(const string& u8str, bool addbom, bool* ok)
{
	u16string u16str;
	u16str.reserve(u8str.size());
	if (addbom) {
		u16str.push_back(0xFEFF);	//bom (字节表示为 FF FE)
	}
	std::string::size_type len = u8str.length();

	const unsigned char* p = (unsigned char*)(u8str.data());
	// 判断是否具有BOM(判断长度小于3字节的情况)
	if (len > 3 && p[0] == 0xEF && p[1] == 0xBB && p[2] == 0xBF){
		p += 3;
		len -= 3;
	}

	bool is_ok = true;
	// 开始转换
	for (std::string::size_type i = 0; i < len; ++i) {
		uint32_t ch = p[i];	// 取出UTF8序列首字节
		if ((ch & 0x80) == 0) {
			// 最高位为0，只有1字节表示UNICODE代码点
			u16str.push_back((char16_t)ch);
			continue;
		}
		switch (ch & 0xF0)
		{
		case 0xF0: // 4 字节字符, 0x10000 到 0x10FFFF
		{
					   uint32_t c2 = p[++i];
					   uint32_t c3 = p[++i];
					   uint32_t c4 = p[++i];
					   // 计算UNICODE代码点值(第一个字节取低3bit，其余取6bit)
					   uint32_t codePoint = ((ch & 0x07U) << 18) | ((c2 & 0x3FU) << 12) | ((c3 & 0x3FU) << 6) | (c4 & 0x3FU);
					   if (codePoint >= 0x10000)
					   {
						   // 在UTF-16中 U+10000 到 U+10FFFF 用两个16bit单元表示, 代理项对.
						   // 1、将代码点减去0x10000(得到长度为20bit的值)
						   // 2、high 代理项 是将那20bit中的高10bit加上0xD800(110110 00 00000000)
						   // 3、low  代理项 是将那20bit中的低10bit加上0xDC00(110111 00 00000000)
						   codePoint -= 0x10000;
						   u16str.push_back((char16_t)((codePoint >> 10) | 0xD800U));
						   u16str.push_back((char16_t)((codePoint & 0x03FFU) | 0xDC00U));
					   }
					   else
					   {
						   // 在UTF-16中 U+0000 到 U+D7FF 以及 U+E000 到 U+FFFF 与Unicode代码点值相同.
						   // U+D800 到 U+DFFF 是无效字符, 为了简单起见，这里假设它不存在(如果有则不编码)
						   u16str.push_back((char16_t)codePoint);
					   }
		}
			break;
		case 0xE0: // 3 字节字符, 0x800 到 0xFFFF
		{
					   uint32_t c2 = p[++i];
					   uint32_t c3 = p[++i];
					   // 计算UNICODE代码点值(第一个字节取低4bit，其余取6bit)
					   uint32_t codePoint = ((ch & 0x0FU) << 12) | ((c2 & 0x3FU) << 6) | (c3 & 0x3FU);
					   u16str.push_back((char16_t)codePoint);
		}
			break;
		case 0xD0: // 2 字节字符, 0x80 到 0x7FF
		case 0xC0:
		{
					 uint32_t c2 = p[++i];
					 // 计算UNICODE代码点值(第一个字节取低5bit，其余取6bit)
					 uint32_t codePoint = ((ch & 0x1FU) << 12) | ((c2 & 0x3FU) << 6);
					 u16str.push_back((char16_t)codePoint);
		}
			break;
		default:	// 单字节部分(前面已经处理，所以不应该进来)
			is_ok = false;
			break;
		}
	}
	if (ok != NULL) { *ok = is_ok; }

	return u16str;
}
/*
string octet2String(const char * src_in, int size)
{
	if (src_in[0] == 0x04)
	{
		if ((int)src_in[1] <128)
		{
			return string(src_in + 2, src_in[1]);
		}
		else
		{
			int count_len = (int)src_in[2] - 0x80;
			int content_len = 0;
			for (int i = 0; i<count_len; i++)
			{
				count_len =(count_len << 8) + (int)src_in[2 + i];
			}

			return (content_len > size ? "" : string(src_in + 3, content_len));
		}
	}
	return "";
}

string string2octet(const char* src_in, int size)
{
	
	return "";
	}*/