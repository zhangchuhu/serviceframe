#pragma once
#include "DaemonClient.h"
#include "IServerSelector.h"

namespace core
{

#define CONFIG_MULTI_ROUTE_TRANSFER_INIT(SeverSelectorInstance) \
	\
	__clientDaemon.SetServerSelector(&SeverSelectorInstance);\
	SeverSelectorInstance.RegisterServerSuffix(SUFFIX_IMBRIDGE, server::ENUM_HASH_SERVERIPID, ENUM_FUNC_FNVHASH);

    class CImDaemonClient:
        public DaemonClient,
		public server::IServerSelectorAware
    {
    public:
	    virtual bool dispatchByServerIdSV(uint32_t serverId, uint32_t uri, sox::Marshallable &);
    };
}

