#ifndef RSYNCSERVICEEXP_H_H
#define RSYNCSERVICEEXP_H_H
#include "m_base_exp.h"

class m_rsync_exp :
	public m_base_exp
{
public:
	m_rsync_exp(PortService portService);
	~m_rsync_exp();
	//////////////////////////////
	void checkServiceIsVuln();
	void checkUnauth();
};

#endif

