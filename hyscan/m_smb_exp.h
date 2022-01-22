#pragma once
#include "m_base_exp.h"
class m_smb_exp :
	public m_base_exp
{
public:
	m_smb_exp(PortService portService);
	~m_smb_exp();
	void m_smb_exp::checkServiceIsVuln();
	void m_smb_exp::checkEternalBlue();
	void m_smb_exp::checkSMBGhost();
};

