#pragma once

class Kerberos
{
public:
	Kerberos();
	~Kerberos();
	int tgtAsk(int argc, wchar_t * argv[]);
};

