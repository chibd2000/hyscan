#ifndef PUBLIC_H_H
#define PUBLIC_H_H
#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>	
#include <thread>
#include <timezoneapi.h>
#include <vector>
#include <mutex>
#include "NtlmInfo.h"
#pragma comment(lib,"ws2_32.lib")
using namespace std;

extern vector<NtlmInfo> vNtlmInfo;
/*prepare for public func, lib and variable, write in 2021.12.09 15.54 @zpchcbd*/
#define THREADMAX 50
//#define DEBUG
#define WMIPORT 135
#define NETBIOSPORT 139
#define SMBPORT 445
#define WINRMPORT 5985
typedef unsigned char		byte;
typedef unsigned short      WORD;
#endif