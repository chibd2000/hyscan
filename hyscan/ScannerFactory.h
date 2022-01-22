#ifndef SCANNERFACTORY_H_H
#define SCANNERFACTORY_H_H
#include "s_smb_scanner.h"
#include "s_wmi_scanner.h"
#include "s_oxid_scanner.h"
#include "s_winrm_scanner.h"
#include "s_weak_scanner.h"
#include "s_port_scanner.h"
#include "s_net_scanner.h"
#include "s_web_scanner.h"
#include "s_ldap_scanner.h"
class ScannerFactory
{
public:
	ScannerFactory();
	~ScannerFactory();
public:
	WMIScanner createScanner(string scannerName)
	{
		WMIScanner wmiScanner;
		return wmiScanner;
	}
};
#endif