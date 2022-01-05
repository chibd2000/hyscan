#include "Kerberos.h"
#include "asn/kerberos_asn1.h"
#include "asn/kull_m_kerberos_oss_asn1_internal.h"


Kerberos::Kerberos()
{
}


Kerberos::~Kerberos()
{
}

int Kerberos::tgtAsk(int argc, wchar_t * argv[])
{
	/*
	PKIWI_AUTH_INFOS infos;
	PKULL_M_SOCK socket;
	KULL_M_ASN1_AS_REP *AsRep = NULL;
	KULL_M_ASN1_EncKDCRepPart *encAsRepPart = NULL;
	PKULL_M_KERBEROS_ASN1_SAVEKDCREP_CALLBACK callback = NULL;

	if (kull_m_string_args_byName(argc, argv, L"ptt", NULL, NULL))
		callback = kuhl_m_kerberos_ptt_data;
	if (infos = kull_m_kerberos_asn1_Authinfos_create(argc, argv))
	{
		if (socket = kull_m_kerberos_asn1_net_AddressSocket_create(infos->w_realm, KERBEROS_DEFAULT_PORT, argc, argv, TRUE))
		{
			if (kull_m_kerberos_asn1_AsReqAsRep(infos, socket, NULL, NULL, &AsRep, &encAsRepPart, NULL))
			{
				kull_m_kerberos_asn1_KdcRep_save(AsRep, encAsRepPart, NULL, NULL, callback);
				ossFreePDU(&kull_m_kerberos_asn1_world, KULL_M_ASN1_EncASRepPart_PDU, encAsRepPart);
				ossFreePDU(&kull_m_kerberos_asn1_world, KULL_M_ASN1_AS_REP_PDU, AsRep);
			}
			kull_m_kerberos_asn1_net_AddressSocket_delete(socket);
		}
		kull_m_kerberos_asn1_Authinfos_delete(infos);
	}
	return STATUS_SUCCESS;*/
}
