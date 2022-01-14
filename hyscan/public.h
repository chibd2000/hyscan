#ifndef PUBLIC_H_H
#define PUBLIC_H_H
#define _CRT_SECURE_NO_WARNINGS
#include <WS2tcpip.h>
#include <iostream>
#include <iomanip>
#include <sddl.h>
#include <time.h>
#include <thread>
#include <timezoneapi.h>
#include <vector>
#include <regex>
#include <queue>
#include <map>
#include <mutex>
#include <string>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")
using namespace std;
// extern vector<UserInfo> vUserInfo;

/*prepare for public func, lib and variable, write in 2021.12.09 15.54 @zpchcbd*/
#define THREADMAX 50
//#define DEBUG
#define WMIPORT 135
#define OXIDPORT 135
#define NETBIOSPORT 139
#define SMBPORT 445
#define WINRMPORT 5985
//typedef unsigned char		byte;
//typedef unsigned short      WORD;
typedef struct hostent HOST_ENT;
#endif