#ifndef  __chunk_proto_handlerL_H__
#define  __chunk_proto_handlerL_H__


#include <nsox/nu_request.h>
#include <nsox/property.h>
#include <set>
#include <list>

namespace chunk
{
enum CHUNK_SERCER_ISP
{
	 CSI_ISP_TL = 1,
	 CSI_ISP_CNC
};

enum   CHUNK_SERVER_SVID
{
		SVID_CHUNK_NAMED   = 1,
		SVID_CHUNK_SERVER  = 2,
		SVID_CHUNK_DATA	   = 4
};


enum chunk_listen_port
{
		CLP_DATA	 = 2500,
		CLP_MASTER	 = 2501,
		CLP_SLAVE	 = 2502,
		CLP_NAMED	 = 2333,
		CLP_NAMED_BACKUP =2334
};


struct  chunk_res_code
{
		enum{
				SUCCESS				= 200,
				REQUEST_RETRY		= 301,
				NOT_CHANGED			= 304,
				OBJECT_NOT_FOUND	= 404,
				VERSION_ERROR		= 405,
				ACCESS_DENIED		= 406,
				SERVER_ERROR		= 500
		};
};


typedef uint32_t chunk_id_t;
typedef uint32_t server_id_t;
typedef uint32_t cluster_id_t;



struct big_string : public std::string
{
public:
		big_string(){}
		big_string(const char* p):std::string(p){
		}

		big_string& operator=(const std::string& str){
				*(std::string*)this = str;
				return *this;
		}
};



inline const sox::Unpack& operator >> (const sox::Unpack& p, big_string& bstr)
{
		bstr = p.pop_varstr32();
		return p;
}

inline sox::Pack & operator << (sox::Pack & p, const big_string& bstr)
{
		p.push_varstr32(bstr.data(), bstr.size());
		return p;
}

struct chunk_data : public sox::Marshallable 
				  , public std::map<std::string, 
								    big_string> 	

{
		virtual void marshal(sox::Pack & pk) const
		{
				sox::marshal_container(pk, *this);
		}
		virtual void unmarshal(const sox::Unpack& upk)
		{
				sox::unmarshal_container(upk, sox::properties::replace_iterator(*this));
		}
};


struct chunk_request : public nproto::request
{
		uint32_t context;

		virtual ~chunk_request(){}
		virtual void pack(sox::Pack& pk){
				nproto::request::pack(pk);
				pk << context;
		}
		virtual void unpack(sox::Unpack& upk){
				nproto::request::unpack(upk);
				upk >> context;
		}
};

struct chunk_response : public nproto::response
{
		uint32_t context;

		virtual ~chunk_response(){}
		virtual void pack(sox::Pack& pk){
				nproto::response::pack(pk);
				pk << context;
		}
		virtual void unpack(sox::Unpack& upk){
				nproto::response::unpack(upk);
				upk >> context;
		}
};


struct PReadData :
		public chunk_request
{
		
		enum{uri = SVID_CHUNK_DATA << 16 | 10 };

		chunk_id_t cid; // chunkid
		typedef std::list<std::string>::iterator iterator;
		std::list<std::string> keys;

		virtual ~PReadData(){}
		virtual void pack(sox::Pack& pk){
				chunk_request::pack(pk);
				pk << cid;
				sox::marshal_container(pk, keys);
		}
		virtual void unpack(sox::Unpack& upk){
				chunk_request::unpack(upk);
				upk >> cid;
				sox::unmarshal_container(upk, std::back_inserter(keys));
		}


};

struct PReadDataRep : 
		public chunk_response
{
		
		enum{uri = SVID_CHUNK_DATA << 16 | 11 };
		chunk_id_t cid;
		chunk_data chunk;

		virtual ~PReadDataRep(){}
		virtual void pack(sox::Pack& pk){
				chunk_response::pack(pk);
				pk << cid << chunk;						
		}
		virtual void unpack(sox::Unpack& upk){
				chunk_response::unpack(upk);
				upk >> cid >> chunk;
		}
};

struct PWriteData :
		public chunk_request
{
		
		enum{uri = SVID_CHUNK_DATA << 16 | 12 };
		chunk_id_t cid;
		chunk_data chunk;

		virtual ~PWriteData(){}
		virtual void pack(sox::Pack& pk){
				chunk_request::pack(pk);
				pk << cid << chunk;						
		}
		virtual void unpack(sox::Unpack& upk){
				chunk_request::unpack(upk);
				upk >> cid >> chunk;
		}
};

struct PWriteDataRep :
		public chunk_response
{
		enum{uri = SVID_CHUNK_DATA << 16 | 13 };
		virtual ~PWriteDataRep(){}

		chunk_id_t cid;
		virtual void pack(sox::Pack& pk){
				chunk_response::pack(pk);
				pk << cid;						
		}
		virtual void unpack(sox::Unpack& upk){
				chunk_response::unpack(upk);
				upk >> cid;
		}
};

//
struct PWriteVersionData : 
		public PWriteData
{
		
		enum{uri = SVID_CHUNK_SERVER << 16 | 14 };
		uint32_t version; //版本号

		virtual ~PWriteVersionData(){}
		virtual void pack(sox::Pack& pk){
				PWriteData::pack(pk);
				pk << version;						
		}
		virtual void unpack(sox::Unpack& upk){
				PWriteData::unpack(upk);
				upk >> version;
		}
};

struct PWriteVerDataRep : chunk_response
{		
		enum{uri = SVID_CHUNK_SERVER << 16 | 15 };
		chunk_id_t cid;

		virtual ~PWriteVerDataRep(){}
		virtual void pack(sox::Pack& pk){
				chunk_response::pack(pk);
				pk << cid;						
		}
		virtual void unpack(sox::Unpack& upk){
				chunk_response::unpack(upk);
				upk >> cid;
		}
};


struct PChunkSnap : public chunk_request
{
		enum{uri = SVID_CHUNK_SERVER << 16 | 16 };

		chunk_id_t cid; // chunkid
		uint32_t version; //版本号

		virtual ~PChunkSnap(){}
		virtual void pack(sox::Pack& pk){
				chunk_request::pack(pk);
				pk << cid << version;						
		}
		virtual void unpack(sox::Unpack& upk){
				chunk_request::unpack(upk);
				upk >> cid >> version;
		}
};

struct PChunkSnapRep :
				public chunk_response
{		
		enum{uri = SVID_CHUNK_SERVER << 16 | 17 };

		chunk_id_t cid;
		uint32_t version; //版本号
		chunk_data chunk;
		
		virtual ~PChunkSnapRep(){}
		virtual void pack(sox::Pack& pk){
				chunk_response::pack(pk);
				pk << cid << chunk << version;						
		}
		virtual void unpack(sox::Unpack& upk){
				chunk_response::unpack(upk);
				upk >> cid >> chunk >> version;
		}

};

/************************************************************************/
/* 返回master所有的chunk列表                                            */
/************************************************************************/
struct PMasterSnap  : 
				public chunk_request
{
		virtual ~PMasterSnap(){}
		enum{uri = SVID_CHUNK_SERVER << 16 | 118 };
};

struct PMasterSnapRep : 
		public chunk_response
{		
		enum{uri = SVID_CHUNK_SERVER << 16 | 119 };

		std::list<chunk_id_t> chunknames;

		virtual ~PMasterSnapRep(){}
		virtual void pack(sox::Pack& pk){
				chunk_response::pack(pk);
				sox::marshal_container(pk, chunknames);					
		}
		virtual void unpack(sox::Unpack& upk){
				chunk_response::unpack(upk);
				sox::unmarshal_container(upk, std::back_inserter(chunknames));					
		}

};




struct PRegSlave : 
				public chunk_request
{		
		enum{uri = SVID_CHUNK_SERVER << 16 | 120 };
		server_id_t slaveid;

		virtual ~PRegSlave(){}
		virtual void pack(sox::Pack& pk){
				chunk_request::pack(pk);
				pk << slaveid;						
		}
		virtual void unpack(sox::Unpack& upk){
				chunk_request::unpack(upk);
				upk >> slaveid;
		}


};
struct PRegSlaveRep :
		public chunk_response
{
		enum{uri = SVID_CHUNK_SERVER << 16 | 121 };
		virtual ~PRegSlaveRep(){}
};




namespace named
{
		struct chunk_server : public sox::Marshallable
		{
				uint32_t		ip;
				uint16_t		port;
				uint16_t		isp;
				virtual void marshal(sox::Pack & pk) const
				{
						pk << ip << port << isp;
				}
				virtual void unmarshal(const sox::Unpack& upk)
				{
						upk >> ip >> port >> isp;
				}
		};
		struct PRegServer : public chunk_request 
		{
				enum{uri = SVID_CHUNK_NAMED << 16 | 222 };

				server_id_t suid; //old suid
				server_id_t nmsid;
				chunk_server cs;

				virtual ~PRegServer(){}
				virtual void pack(sox::Pack& pk){
						chunk_request::pack(pk);
						pk << suid << nmsid <<  cs;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_request::unpack(upk);
						upk >>  suid >> nmsid >> cs;
				}
		};		


		struct PRegServerRep : public chunk_response
		{
				enum{uri = SVID_CHUNK_NAMED << 16 | 223 };
				server_id_t suid; //server unique id
				server_id_t nmsid;

				virtual ~PRegServerRep(){}
				virtual void pack(sox::Pack& pk){
						chunk_response::pack(pk);
						pk << suid << nmsid;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_response::unpack(upk);
						upk >> suid >> nmsid;
				}
		};

		struct PKillServer : public chunk_request 
		{
				enum{uri = SVID_CHUNK_NAMED << 16 | 224 };

				server_id_t suid;

				virtual ~PKillServer(){}
				virtual void pack(sox::Pack& pk){
						chunk_request::pack(pk);
						pk << suid;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_request::unpack(upk);
						upk >> suid;
				}
		};

		struct PKillServerRep : public chunk_response {

				virtual ~PKillServerRep(){}
			   enum{uri = SVID_CHUNK_NAMED << 16 | 225 };
		};

		struct PSetCluster : public chunk_request{
				enum{uri = SVID_CHUNK_NAMED << 16 | 226 };

				server_id_t	    clusid;
				cluster_id_t	masterid;
				chunk_server	master;

				virtual ~PSetCluster(){}
				virtual void pack(sox::Pack& pk){
						chunk_request::pack(pk);
						pk << clusid << masterid << master;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_request::unpack(upk);
						upk >> clusid >> masterid >> master;
				}
		};

		struct PSetMasterRep : public chunk_response {
				enum{uri = SVID_CHUNK_NAMED << 16 | 227 };

				server_id_t suid;
				uint16_t error;

				virtual ~PSetMasterRep(){}
				virtual void pack(sox::Pack& pk){
						chunk_response::pack(pk);
						pk << suid << error;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_response::unpack(upk);
						upk >> suid >> error;
				}
		};

		struct PHeartBeat : public chunk_request {
				enum{uri = SVID_CHUNK_NAMED << 16 | 228 };


				server_id_t suid;

				virtual ~PHeartBeat(){}
				virtual void pack(sox::Pack& pk){
						chunk_request::pack(pk);
						pk << suid ;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_request::unpack(upk);
						upk >> suid;
				}
		};

		struct PHeartBeatRep : public chunk_response {
				enum{uri = SVID_CHUNK_NAMED << 16 | 229 };
				server_id_t suid;

				virtual ~PHeartBeatRep(){}
				virtual void pack(sox::Pack& pk){
						chunk_response::pack(pk);
						pk << suid ;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_response::unpack(upk);
						upk >> suid;
				}
		};		

		//查找 chunk的位置

		struct PChunkPos : public chunk_request
		{
				enum{uri = SVID_CHUNK_NAMED << 16 | 230 };

				chunk_id_t cid; //chunk id

				virtual ~PChunkPos(){}
				virtual void pack(sox::Pack& pk){
						chunk_request::pack(pk);
						pk << cid ;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_request::unpack(upk);
						upk >> cid;
				}
		};

		struct chunk_pos : public sox::Marshallable
		{
				chunk_server master;
				std::list<chunk_server> slaves; // do really need it 


				virtual void marshal(sox::Pack & pk) const
				{
						pk << master ;
						sox::marshal_container(pk, slaves);
				}
				virtual void unmarshal(const sox::Unpack& upk)
				{
						upk >> master;
						sox::unmarshal_container(upk, std::back_inserter(slaves));
				}
		};		


		struct PChunkPosRep : public chunk_response
		{
				enum{uri = SVID_CHUNK_NAMED << 16 | 231 };

				chunk_id_t cid;
				chunk_pos  cps;

				virtual ~PChunkPosRep(){}
				virtual void pack(sox::Pack& pk){
						chunk_response::pack(pk);
						pk << cid << cps ;	
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_response::unpack(upk);
						upk >> cid >>  cps;
				}
		};


		struct PChunkNotify : public chunk_request 
		{
				enum{uri = SVID_CHUNK_NAMED << 16 | 234 };

				enum chunk_event
				{
						ce_add = 0,
						ce_del
				};

				chunk_id_t cid;
				uint32_t event;

				virtual ~PChunkNotify(){}
				virtual void pack(sox::Pack& pk){
						chunk_request::pack(pk);
						pk << cid << event;							
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_request::unpack(upk);
						upk >> cid >> event;
				}

		};


		struct PChunkNotifyRep : public chunk_response 
		{
				enum{uri = SVID_CHUNK_NAMED << 16 | 235 };

				server_id_t suid;
				chunk_id_t cid;
				uint32_t event;
				
				virtual ~PChunkNotifyRep(){}
				virtual void pack(sox::Pack& pk){
						chunk_response::pack(pk);
						pk << suid << cid << event;							
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_response::unpack(upk);
						upk >> suid >> cid >> event;
				}
		};


		struct  PCreateChunk : public chunk_request{
				enum{uri = SVID_CHUNK_NAMED << 16 | 236 };

				chunk_id_t cid; //chunk id

				virtual ~PCreateChunk(){}
				virtual void pack(sox::Pack& pk){
						chunk_request::pack(pk);
						pk << cid ;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_request::unpack(upk);
						upk >> cid;
				}
		};

		struct  PCreateChunkRep : public PChunkPosRep{

				virtual ~PCreateChunkRep(){}
				enum{uri = SVID_CHUNK_NAMED << 16 | 237 };
		};


		struct PRemoveChunk : public chunk_request{
				enum{uri = SVID_CHUNK_NAMED << 16 | 238 };

				chunk_id_t cid; //chunk id

				virtual ~PRemoveChunk(){}
				virtual void pack(sox::Pack& pk){
						chunk_request::pack(pk);
						pk << cid ;						
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_request::unpack(upk);
						upk >> cid;
				}
		};

		struct PRemoveChunkRep : public chunk_response{
				enum{uri = SVID_CHUNK_NAMED << 16 | 239 };
				chunk_id_t cid;

				virtual ~PRemoveChunkRep(){}
				virtual void pack(sox::Pack& pk){
						chunk_response::pack(pk);
						pk << cid;							
				}
				virtual void unpack(sox::Unpack& upk){
						chunk_response::unpack(upk);
						upk >> cid;
				}
		};

		namespace backup
		{
				struct net_chunk_info : public sox::Marshallable
				{
						chunk_id_t		cid;
						cluster_id_t	clusid;
						uint32_t		success;

						virtual ~net_chunk_info(){}
						virtual void marshal(sox::Pack & pk) const
						{
								pk << cid << clusid << success;
						}
						virtual void unmarshal(const sox::Unpack& upk)
						{
								upk >> cid >> clusid >> success;
						}
				};
				struct net_cluster_info : public sox::Marshallable 
				{
						cluster_id_t			clusid;
						uint32_t				count;
						server_id_t				master;
						std::list<server_id_t>  slaves;

						virtual ~net_cluster_info(){}
						virtual void marshal(sox::Pack & pk) const
						{
								pk << clusid << count << master;
								sox::marshal_container(pk, slaves);
						}
						virtual void unmarshal(const sox::Unpack& upk)
						{
								upk >> clusid >> count >> master;
								sox::unmarshal_container(upk, std::back_inserter(slaves));
						}
				};

				struct net_server_info : public sox::Marshallable{
						server_id_t		suid;
						uint32_t		ip;
						uint16_t		port;
						uint16_t		isp;

						virtual ~net_server_info(){}
						virtual void marshal(sox::Pack & pk) const
						{
								pk << suid << ip << port << isp;
						}
						virtual void unmarshal(const sox::Unpack& upk)
						{
								upk >> suid >>  ip >> port >> isp;
						}
				};

				struct net_server_manager : public sox::Marshallable
				{
						std::list<net_server_info>		used_servers;
						std::list<net_server_info>		unused_servers;
						server_id_t						alloc_suid;

						virtual ~net_server_manager(){}
						virtual void marshal(sox::Pack & pk) const
						{
								pk << alloc_suid;
								sox::marshal_container(pk, used_servers);
								sox::marshal_container(pk, unused_servers);

						}
						virtual void unmarshal(const sox::Unpack& upk)
						{
								upk >> alloc_suid;
								sox::unmarshal_container(upk, std::back_inserter(used_servers));
								sox::unmarshal_container(upk, std::back_inserter(unused_servers));
						}
				};

				struct net_cluster_manager : public sox::Marshallable
				{
						cluster_id_t alloc_clusid;
						std::list<net_cluster_info>		clusters;
						
						virtual ~net_cluster_manager(){}
						virtual void marshal(sox::Pack & pk) const
						{
								pk << alloc_clusid;
								sox::marshal_container(pk, clusters);
						}
						virtual void unmarshal(const sox::Unpack& upk)
						{
								upk >> alloc_clusid;
								sox::unmarshal_container(upk, std::back_inserter(clusters));
						}
				};

				struct net_chunk_manager : public sox::Marshallable
				{
						std::list<net_chunk_info>		chunks;

						virtual ~net_chunk_manager(){}
						virtual void marshal(sox::Pack& pk)const{
								sox::marshal_container(pk, chunks);
						}
						virtual void unmarshal(const sox::Unpack& upk){
								sox::unmarshal_container(upk, std::back_inserter(chunks));
						}		
				};

				struct PNamedSnap : public chunk_request
				{
						virtual ~PNamedSnap(){}
						enum{uri = SVID_CHUNK_NAMED << 16 | 700 };
				};

				struct PNamedSnapRep : public chunk_response
				{
						enum{uri = SVID_CHUNK_NAMED << 16 | 701 };

						server_id_t				nmsid;
						net_chunk_manager		chunks;
						net_cluster_manager		clusters;
						net_server_manager		servers;

						virtual ~PNamedSnapRep(){}

						virtual void pack(sox::Pack& pk){
								chunk_response::pack(pk);
								pk << nmsid << chunks << clusters << servers;
						}
						virtual void unpack(sox::Unpack& upk){
								chunk_response::unpack(upk);
								upk >> nmsid >> chunks >> clusters >> servers;
						}						
 				};
		}

}
}

#endif //__chunk_proto_handlerL_H__

