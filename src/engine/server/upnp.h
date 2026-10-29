#ifndef ENGINE_SERVER_UPNP_H
#define ENGINE_SERVER_UPNP_H

#include <base/types.h>


struct IGDdatas;
struct UPNPDev;
struct UPNPUrls;


class CUPnP
{
	NETADDR m_Addr;
	UPNPUrls *m_pUpnpUrls;
	IGDdatas *m_pUpnpData;
	UPNPDev *m_pUpnpDevice;
	bool m_Enabled;

public:
	void Open(NETADDR Address);
	void Shutdown();
};

#endif
