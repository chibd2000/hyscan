#ifndef WNETAPI_H_H
#define WNETAPI_H_H
#include "m_w_base_api.h"
#include "Tools.h"
#include <lm.h>
#pragma comment(lib, "Netapi32.lib")

/*
#include "samlib.h"
#pragma comment(lib, "samlib.lib")
#pragma comment(lib, "ntdll.lib")
*/

/*
错误号5，拒绝访问 ： 很可能你使用的用户不是管理员权限的，先提升权限；
错误号51，Windows 无法找到网络路径 : 网络有问题；
错误号53，找不到网络路径 ： ip地址错误；目标未开机；目标lanmanserver服务未启动；目标有防火墙（端口过滤）；
错误号67，找不到网络名 ： 你的lanmanworkstation服务未启动；目标删除了ipc$；
错误号1219，提供的凭据与已存在的凭据集冲突 ： 你已经和对方建立了一个ipc$，请删除再连。
错误号1326，未知的用户名或错误密码 ： 原因很明显了；
错误号1792，试图登录，但是网络登录服务没有启动 ： 目标NetLogon服务未启动。（连接域控会出现此情况）
错误号2242，此用户的密码已经过期 ： 目标有帐号策略，强制定期要求更改密码。
*/

enum IPC_ERROR{
	IPC_SUCCESS = 0,
	IPC_PRIVILEGE_ERROR = 5,
	IPC_NETWORK_ERROR = 67,
	IPC_USER_PASS_ERROR = 1326,
	IPC_PASS_EXPIRE = 2242
};
	
class WNET_API : public m_w_base_api
{
public:
	WNET_API();
	~WNET_API();
	DWORD openConnectBySelf(string& serverName);
	DWORD openConnectByUserPass(string serverName, string username, string password);
	void getLoggedUsers(string& serverName);
	void getNetSession(wstring& serverName);
	void getNetShare(wstring& serverName);
	static vector<wstring> getDomainGroupMembers(wstring serverName, wstring groupName);
	static vector<wstring> getLocalGroupMembers(wstring serverName, wstring groupName);
	//static vector<wstring> getLocalGroupMembersSamr(wstring serverName);
	DWORD closeConnection(string lpRemoteName);
};
#endif
