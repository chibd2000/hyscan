#pragma once
#include "public.h"

class SHA1 final
{
public:
	SHA1();

	void update(const std::string &s);
	void update(std::istream &is);
	std::string final();
	std::string final_bin();

	static std::string from_file(const std::string &filename);
	static std::string encode(const std::string &s);
	static std::string encode_bin(const std::string &s);

private:
	uint32_t digest[5];
	std::string buffer;
	uint64_t transforms;
};


