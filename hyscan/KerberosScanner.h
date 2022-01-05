#pragma once
#include "BaseScanner.h"
class KerberosScanner :
	public BaseScanner
{
public:
	KerberosScanner();
	~KerberosScanner();
	void reqAsEnumUser();
};

