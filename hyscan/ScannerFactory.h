#ifndef SCANNERFACTORY_H_H
#define SCANNERFACTORY_H_H
#include "BaseScanner.h"
#include "SMBScanner.h"
#include "WMIScanner.h"
#include "OXIDScanner.h"
#include "WinrmScanner.h"
class ScannerFactory
{
public:
	ScannerFactory();
	~ScannerFactory();
public:
	BaseScanner* createScanner(string scannerName)
	{
		BaseScanner* baseScanner = nullptr;
		if (scannerName == "SMBScanner"){
			baseScanner = new SMBScanner();
		}
		else if (scannerName == "WMIScanner"){
			baseScanner = new WMIScanner();
		}
		else if (scannerName == "WinrmScanner"){
			baseScanner = new WinrmScanner();
		}
		else if (scannerName == "OXIDScanner"){
			baseScanner = new OXIDScanner();
		}
		return baseScanner;
	}
};
#endif