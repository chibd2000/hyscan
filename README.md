# hyscan

## 学习点：

1、ntlm/kerberos协议的学习

2、win32/com的学习

3、ldap/wnet/wmi的学习

4、多线程/线程池框架开发的学习

## 功能

- 收集(Gather)
  - [√] 基于 NTLM type2 135端口探测主机信息
  - [√] 基于 NTLM type2 445端口探测主机信息
  - [ ] 基于 NTLM type2 5985端口探测主机信息
  - [ ] 基于 SMB MS-17010探测
  - [√] 基于 OXID DCOM接口探测多主机网卡
  - [√] 基于 LDAP 委派配置不当探测（约束委派/非约束委派/基于资源的约束委派）
  - [√] 基于 LDAP GPO组策略搜集
  - [ ] 基于 共享资源枚举NetShareEnum
  - [ ] 基于 域管登录定位NetSession
- 利用(Exploit)
  - [ ] 基于 IPC Brute
  - [ ] 基于 WMIC Brute
  - [√] 基于 Default WMI PTH
  - [√] 基于 Default SMB PTH
  - [ ] 基于 GPP漏洞
  - [ ] 基于 TCP加密流量代理
  - [ ] 基于 http.sys端口复用后门
  - [√] 基于 LDAP 利用（添加机器用户/添加DNS记录/基于资源的约束委派利用）
  - [ ] 基于 kerberos TGT票据请求
  - [ ] 基于 kerberos ST票据请求
  
## 参考文章

https://github.com/0x727/ShuiYing_0x727

https://github.com/SkewwG/domainTools
