 
 

#include "host_lookup.h"

#include <base/net.h>
#include <base/str.h>

CHostLookup::CHostLookup(const char *pHostname, int Nettype)
{
	str_copy(m_aHostname, pHostname);
	m_Nettype = Nettype;
	Abortable(true);
}

void CHostLookup::Run()
{
	m_Result = net_host_lookup(m_aHostname, &m_Addr, m_Nettype);
}
