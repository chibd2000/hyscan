#ifndef SERVICEEXPLOIT_H_H
#define SERVICEEXPLOIT_H_H
#include "public.h"
#include "c_service.h"
///////////////////////////
#include "s_net_scanner.h"
///////////////////////////
#include "m_socket_api.h"
///////////////////////////
#include "Tools.h"
class m_base_exp{
public:
	PortService portService;
public:
	m_base_exp();
	~m_base_exp();
	virtual void checkServiceIsVuln();
};

#endif
