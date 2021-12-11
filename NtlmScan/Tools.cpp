#include "Tools.h"

string WcharToChar(wchar_t* wc)
{
	char* m_char;
	int len = WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), NULL, 0, NULL, NULL);
	m_char = new char[len + 1];
	WideCharToMultiByte(CP_ACP, 0, wc, wcslen(wc), m_char, len, NULL, NULL);
	m_char[len] = '\0';
	string w_str(m_char);
	return w_str;
}


wstring StringToWString(const std::string& str)
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

