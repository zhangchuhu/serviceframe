#ifndef IM_PROTOCOL_IM_APP_OFFLINE_MSG_H_INCLUDE
#define IM_PROTOCOL_IM_APP_OFFLINE_MSG_H_INCLUDE

#include "core/base_svid.h"
#include "packet.h"
#include "protocol/const.h"

namespace protocol
{
	namespace imapp
	{
		// app offline msg

		struct PCS_AppGetOfflineMsg : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::IMAPP_OFFLINEMSG_SVID};

			uint32_t m_uAppId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;
			}
		};

		struct SAppOfflineMsgRecord : public sox::Marshallable
		{
			uint32_t     m_uFromId; 
			uint32_t     m_uSendTime;
			uint32_t     m_uSeqId;
			std::string  m_strText;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFromId << m_uSendTime << m_uSeqId << m_strText;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uFromId >> m_uSendTime >> m_uSeqId >> m_strText;
			}
		};

		struct PCS_AppGetOfflineMsgRes : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::IMAPP_OFFLINEMSG_SVID};

			uint32_t m_uAppId;
			std::vector<SAppOfflineMsgRecord> m_vecMsg;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;
				sox::marshal_container(p, m_vecMsg);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;
				sox::unmarshal_container(p, std::back_inserter(m_vecMsg));
			}
		};

		// app offline msg count

		struct PCS_AppGetOfflineMsgCount : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::IMAPP_OFFLINEMSG_SVID};

			uint32_t m_uAppId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;
			}
		};

		struct PCS_AppGetOfflineMsgCountRes : public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::IMAPP_OFFLINEMSG_SVID};

			uint32_t m_uAppId;
			std::map<uint32_t, uint32_t> m_mapUid2Count;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;
				sox::marshal_container(p, m_mapUid2Count);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;
				sox::unmarshal_container(p, std::inserter(m_mapUid2Count, m_mapUid2Count.begin()));
			}
		};

		// delete readed msg

		struct PCS_AppRemoveOfflineMsg : public sox::Marshallable
		{
			enum {uri = (5 << 8) | protocol::IMAPP_OFFLINEMSG_SVID};

			uint32_t m_uAppId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uAppId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uAppId;
			}
		};
	}
}

#endif
