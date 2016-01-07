#pragma once

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/res_code.h"

namespace protocol
{
	namespace imgeneralapp
	{

		// Ӧ�ñ�Ŷ���
		// ȡֵ��Χ 0~65535
		#define APP_ALL			0
		#define APP_GAMECARD	20005

		// ��������־ѡ��
		enum ENUM_SERVER_FLAG
		{
			PROXYINFO_FLAG = 1, // CProxyInfo·��
			WRAPHEAD_FLAG = 2,  // PSS_WrapHead����
		};

		// ����Ӧ��֪ͨ����
		// ���ܻ��м����ֽڵĶ�������
		struct ImAppBaseInfo: public sox::Marshallable
		{
			// Ӧ�ñ��
			// ��ʾ����16λ����ֹ����
			uint16_t m_uAppId;

			// ��Ч��������
			uint32_t m_uPayloadType;
			std::string m_strPayload;

			// UI�����ģ�������͸��
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
			// Ӧ�ñ��
			// ��ʾ����16λ����ֹ����
			uint16_t m_uAppId;

			// ��Ч��������
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


		// ����һ��AppӦ��
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

		// ����AppӦ�÷���
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

		// ��ȡһ��AppӦ��
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

		// ��ȡһ��AppӦ�÷���
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

		// ������ȡAppӦ��
		struct PCS_BatchGetImAppData : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (5 << 8) | protocol::IMGENERALAPP_SVID}; 

			uint16_t m_uAppId;
			uint32_t m_uPayloadType;
			std::vector<uint32_t> m_vecUid;	

			// UI�����ģ�������͸��
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

		// ������ȡAppӦ�÷���
		struct PCS_BatchGetImAppDataRes : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (6 << 8) | protocol::IMGENERALAPP_SVID}; 

			uint16_t m_uAppId;
			std::map<uint32_t, ImAppBaseInfoSimple> m_mapInfoSimple;

			// UI�����ģ�������͸��
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

		// ��������ת����uid����

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


		// �յ����Է�������֪ͨ��AppӦ�ñ��
		// APP����֪ͨ��DBD��������� -> im_appproxy��
		struct PCS_ImAppDataUpdate : public sox::Marshallable
		{
			enum {uri = APP_ALL << 16 | (7 << 8) | protocol::IMGENERALAPP_SVID}; 

			enum
			{
				BROADCAST_FLAG = 1,		// �㲥ѡ��
				BATCHFORWARD_FLAG = 2,	// ����ת��ѡ��
				UPDATECACHE_FLAG = 3	// �������ѡ��
			};

			uint32_t m_uIdWho;
			ImAppBaseInfo m_baseInfo;
			std::map<uint32_t, std::string> m_mapSrvParm;

			PCS_ImAppDataUpdate()
			{
				setBroadcast(true);
			}
			// ��������ѹ㲥ѡ��
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
			// ����������ָ���û�ת��
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
			// ���ø��»���ѡ��
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

		// ����im_appcache���ӷ�����֮�������ͬ��
		// ��������ʹ��
		struct PSS_SyncImAppData : public PCS_GetImAppDataRes
		{
			enum {uri = APP_ALL << 16 | (8 << 8) | protocol::IMGENERALAPP_SVID}; 
		};
		struct PSS_SyncBatchImAppData : public PCS_BatchGetImAppDataRes
		{
			enum {uri = APP_ALL << 16 | (9 << 8) | protocol::IMGENERALAPP_SVID}; 
		};

		// ��Щ����£�����PCS_ImAppDataUpdate�ķ�������Ҫ������Ӧ
		struct PCS_ImAppDataUpdateRes : public PCS_ImAppDataUpdate
		{
			enum {uri = APP_ALL << 16 | (10 << 8) | protocol::IMGENERALAPP_SVID}; 
		};

		//end new protocol
		//-------------------------------------------------------------------------------------
	}
}
