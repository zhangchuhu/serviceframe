#pragma once

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/res_code.h"

namespace protocol
{
	namespace imgeneralapp
	{

		// 应用编号定义
		// 取值范围 0~65535
		#define APP_ALL			0
		#define APP_GAMECARD	20005

		// 服务器标志选项
		enum ENUM_SERVER_FLAG
		{
			PROXYINFO_FLAG = 1, // CProxyInfo路由
			WRAPHEAD_FLAG = 2,  // PSS_WrapHead报文
		};

		// 各用应用通知数据
		// 可能会有几个字节的多余消耗
		struct ImAppBaseInfo: public sox::Marshallable
		{
			// 应用编号
			// 显示定义16位，防止出错
			uint16_t m_uAppId;

			// 有效数据载体
			uint32_t m_uPayloadType;
			std::string m_strPayload;

			// UI上下文，服务器透传
			uint32_t m_uContextUri;
			std::string m_strContext;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;

				p << m_uPayloadType;
				p.push_varstr32(m_strPayload.data(), m_strPayload.size());

				p << m_uContextUri;
				p.push_varstr32(m_strContext.data(), m_strContext.size());
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;

				p >> m_uPayloadType;
				m_strPayload = p.pop_varstr32();

				p >> m_uContextUri;
				m_strContext = p.pop_varstr32();
			}
		};

		struct ImAppBaseInfoSimple: public sox::Marshallable
		{
			// 应用编号
			// 显示定义16位，防止出错
			uint16_t m_uAppId;

			// 有效数据载体
			uint32_t m_uPayloadType;
			std::string m_strPayload;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;

				p << m_uPayloadType;
				p.push_varstr32(m_strPayload.data(), m_strPayload.size());
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;

				p >> m_uPayloadType;
				m_strPayload = p.pop_varstr32();
			}
		};


		// 设置一个App应用
		struct PCS_ImAppDataReq : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (1 << 8) | protocol::IMGENERALAPP_SVID}; 

			enum SRV_PARAM
			{
				USER_IP = 1
			};

			ImAppBaseInfo m_baseInfo;	
			std::map<uint32_t, std::string> m_mapSrvParm;

			virtual void marshal(sox::Pack &p) const
			{
				m_baseInfo.marshal(p);
				sox::marshal_container(p, m_mapSrvParm);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_baseInfo.unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_mapSrvParm, m_mapSrvParm.begin()));
			}
		};

		// 设置App应用返回
		struct PCS_ImAppDataRes : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (2 << 8) | protocol::IMGENERALAPP_SVID}; 

			uint32_t m_uResCode;
			ImAppBaseInfo m_baseInfo;
			std::map<uint32_t, std::string> m_mapSrvParm;

			PCS_ImAppDataRes() : m_uResCode(RES_SUCCESS) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uResCode;
				m_baseInfo.marshal(p);
				sox::marshal_container(p, m_mapSrvParm);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uResCode;
				m_baseInfo.unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_mapSrvParm, m_mapSrvParm.begin()));
			}
		};

		// 获取一个App应用
		struct PCS_GetImAppData : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (3 << 8) | protocol::IMGENERALAPP_SVID}; 

			uint32_t m_uIdWho;
			ImAppBaseInfo m_baseInfo;	
			std::map<uint32_t, std::string> m_mapSrvParm;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uIdWho;
				m_baseInfo.marshal(p);
				sox::marshal_container(p, m_mapSrvParm);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uIdWho;
				m_baseInfo.unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_mapSrvParm, m_mapSrvParm.begin()));
			}
		};

		// 获取一个App应用返回
		struct PCS_GetImAppDataRes : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (4 << 8) | protocol::IMGENERALAPP_SVID}; 

			uint32_t m_uIdWho;
			ImAppBaseInfo m_baseInfo;	
			std::map<uint32_t, std::string> m_mapSrvParm;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uIdWho;
				m_baseInfo.marshal(p);
				sox::marshal_container(p, m_mapSrvParm);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uIdWho;
				m_baseInfo.unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_mapSrvParm, m_mapSrvParm.begin()));
			}
		};

		// 批量获取App应用
		struct PCS_BatchGetImAppData : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (5 << 8) | protocol::IMGENERALAPP_SVID}; 

			uint16_t m_uAppId;
			uint32_t m_uPayloadType;
			std::vector<uint32_t> m_vecUid;	

			// UI上下文，服务器透传
			uint32_t m_uContextUri;
			std::string m_strContext;

			std::map<uint32_t, std::string> m_mapSrvParm;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId << m_uPayloadType;
				sox::marshal_container(p, m_vecUid);

				p << m_uContextUri;
				p.push_varstr32(m_strContext.data(), m_strContext.size());

				sox::marshal_container(p, m_mapSrvParm);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId >> m_uPayloadType;
				sox::unmarshal_container(p, std::back_inserter(m_vecUid));

				p >> m_uContextUri;
				m_strContext = p.pop_varstr32();

				sox::unmarshal_container(p, std::inserter(m_mapSrvParm, m_mapSrvParm.begin()));
			}
		};

		// 批量获取App应用返回
		struct PCS_BatchGetImAppDataRes : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (6 << 8) | protocol::IMGENERALAPP_SVID}; 

			uint16_t m_uAppId;
			std::map<uint32_t, ImAppBaseInfoSimple> m_mapInfoSimple;

			// UI上下文，服务器透传
			uint32_t m_uContextUri;
			std::string m_strContext;

			std::map<uint32_t, std::string> m_mapSrvParm;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;
				sox::marshal_container(p, m_mapInfoSimple);

				p << m_uContextUri;
				p.push_varstr32(m_strContext.data(), m_strContext.size());

				sox::marshal_container(p, m_mapSrvParm);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;
				sox::unmarshal_container(p, std::inserter(m_mapInfoSimple, m_mapInfoSimple.begin()));

				p >> m_uContextUri;
				m_strContext = p.pop_varstr32();

				sox::unmarshal_container(p, std::inserter(m_mapSrvParm, m_mapSrvParm.begin()));
			}
		};

		// 用于批量转发的uid集合

		struct ImAppUidVector: public sox::Marshallable
		{
			std::vector<uint32_t> m_vecUid;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecUid));
			}
		};


		// 收到来自服务主动通知的App应用变更
		// APP更新通知（DBD变更服务器 -> im_appproxy）
		struct PCS_ImAppDataUpdate : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (7 << 8) | protocol::IMGENERALAPP_SVID}; 

			enum
			{
				BROADCAST_FLAG = 1,		// 广播选项
				BATCHFORWARD_FLAG = 2,	// 批量转发选项
				UPDATECACHE_FLAG = 3	// 缓存更新选项
			};

			uint32_t m_uIdWho;
			ImAppBaseInfo m_baseInfo;
			std::map<uint32_t, std::string> m_mapSrvParm;

			PCS_ImAppDataUpdate()
			{
				setBroadcast(true);
			}
			// 设置向好友广播选项
			void setBroadcast(bool b)
			{
				m_mapSrvParm[BROADCAST_FLAG] = (b ? "1" : "0");
			}
			bool isBroadcast()
			{
				std::map<uint32_t, std::string>::iterator iter = m_mapSrvParm.find(BROADCAST_FLAG);
				if (iter != m_mapSrvParm.end() && iter->second == "1")
				{
					return true;
				}
				return false;
			}
			// 设置批量向指定用户转发
			void setBatchForward(const std::vector<uint32_t>& vecUid)
			{
				ImAppUidVector param;
				param.m_vecUid = vecUid;

				std::string strPacket;
				sox::PacketToString(param, strPacket);

				m_mapSrvParm[BATCHFORWARD_FLAG] = strPacket;
			}
			void getBatchForward(std::vector<uint32_t>& vecUid)
			{
				std::map<uint32_t, std::string>::iterator iter = m_mapSrvParm.find(BATCHFORWARD_FLAG);
				if (iter != m_mapSrvParm.end())
				{
					ImAppUidVector param;
					if (sox::StringToPacket(iter->second, param))
					{
						vecUid = param.m_vecUid;
					}
				}
			}
			// 设置更新缓存选项
			void setUpdateCache(bool b)
			{
				m_mapSrvParm[UPDATECACHE_FLAG] = (b ? "1" : "0");
			}
			bool isUpdateCache()
			{
				std::map<uint32_t, std::string>::iterator iter = m_mapSrvParm.find(UPDATECACHE_FLAG);
				if (iter != m_mapSrvParm.end() && iter->second == "1")
				{
					return true;
				}
				return false;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uIdWho;
				m_baseInfo.marshal(p);
				sox::marshal_container(p, m_mapSrvParm);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				m_mapSrvParm.clear();

				p >> m_uIdWho;
				m_baseInfo.unmarshal(p);
				sox::unmarshal_container(p, std::inserter(m_mapSrvParm, m_mapSrvParm.begin()));
			}
		};

		// 用于im_appcache主从服务器之间的数据同步
		// 仅服务器使用
		struct PSS_SyncImAppData : public PCS_GetImAppDataRes
		{
			enum {uri = APP_ALL << 16 | (8 << 8) | protocol::IMGENERALAPP_SVID}; 
		};
		struct PSS_SyncBatchImAppData : public PCS_BatchGetImAppDataRes
		{
			enum {uri = APP_ALL << 16 | (9 << 8) | protocol::IMGENERALAPP_SVID}; 
		};

		// 有些情况下，发起PCS_ImAppDataUpdate的服务器需要接收响应
		struct PCS_ImAppDataUpdateRes : public PCS_ImAppDataUpdate
		{
			enum {uri = APP_ALL << 16 | (10 << 8) | protocol::IMGENERALAPP_SVID}; 
		};

		//end new protocol
		//-------------------------------------------------------------------------------------
	}
}
