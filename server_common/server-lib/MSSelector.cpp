#include "MSSelector.h"
#include "int_types.h"
#include "server_common/protocol/daemon.h"
#include "core/sox/logger.h"

using namespace std;
using namespace core;
using namespace sdaemon;

namespace server{
	namespace msselector{
		uint32_t MSSelector(vector<uint32_t>& servers){
			uint32_t min_id_server = *(servers.begin());
			for (vector<uint32_t>::iterator it = servers.begin(); it != servers.end(); ++it){
				if (*it < min_id_server){
					min_id_server = *it;
				}
			}

			return min_id_server;
		}
/*
		EMSFlag MSGetMyType(IDaemonClient *client, IDaemonServer *server, string sstr){
			vector<uint32_t> campaigners;
			vector<FavorStr> servers = client->getServers();
			for(vector<FavorStr>::iterator it = servers.begin(); it != servers.end(); ++it){
				if ((*it).sstr == sstr){
					log(Info, "campaigner: %d", (*it).serverId);
					campaigners.push_back((*it).serverId);
				}
			}
			campaigners.push_back(server->getServerId());
			uint32_t masterid =  MSSelector(campaigners);

			if (masterid == server->getServerId())
				return MASTER;
			else
				return SLAVER;
		}
*/		
		EMSFlag MSGetMyTypeSV(IDaemonClient *client, IDaemonServer *server, string sstr){
			vector<uint32_t> campaigners;
			vector<FavorStrSV> servers = client->getServersSV();
			for(vector<FavorStrSV>::iterator it = servers.begin(); it != servers.end(); ++it){
				if ((*it).sstr == sstr){
					log(Info, "campaigner: %d", (*it).serverId);
					campaigners.push_back((*it).serverId);
				}
			}
			campaigners.push_back(server->getServerId());
			uint32_t masterid =  MSSelector(campaigners);

			if (masterid == server->getServerId())
				return MASTER;
			else
				return SLAVER;
		}		
	}
}

