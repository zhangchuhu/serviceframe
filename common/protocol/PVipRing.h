#ifndef PROTOCOL_PVIP_RING_H_INCLUDE
#define PROTOCOL_PVIP_RING_H_INCLUDE

#include "common/core/base_svid.h"
#include "common/packet.h"

namespace protocol
{
	namespace imvip
	{
		struct PCS_GetBatchColorRingInfoBak : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::VIP_RING_SVID};

			std::vector<uint32_t> m_vecUids;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecUids));
			}
		};

		struct PCS_ColorRingInfo : public sox::Marshallable
		{
			PCS_ColorRingInfo() {}

			PCS_ColorRingInfo(uint32_t id, uint32_t t, uint32_t index) :
				uid(id), type(t), idx(index) 
			{}

			uint32_t uid;
			uint32_t type;
			uint32_t idx;

			virtual void marshal(sox::Pack &p) const {
				p << uid << type << idx;
			}

			virtual void unmarshal(const sox::Unpack &p) {
				p >> uid >> type >> idx;
			}
		};

		struct PCS_GetBatchColorRingInfoRes : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::VIP_RING_SVID};

			std::map<uint32_t, std::vector<PCS_ColorRingInfo> > infos;

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint32(uint32_t(infos.size()));

				std::map<uint32_t, std::vector<PCS_ColorRingInfo> >::const_iterator it;

				for (it = infos.begin(); it != infos.end(); ++it) {
					p << it->first;
					sox::marshal_container(p, it->second);
				}
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				for (uint32_t count = p.pop_uint32(); count > 0; --count) {
					uint32_t tmp;
					std::vector<PCS_ColorRingInfo> tmp1;

					p >> tmp;
					sox::unmarshal_container(p, std::back_inserter(tmp1));
					infos.insert(std::make_pair(tmp, tmp1));
				}
			}
		};

		struct PCS_GetBatchColorRingInfo : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::VIP_RING_SVID};

			std::vector<uint32_t> m_vecUids;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecUids);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecUids));
			}
		};

		struct PCS_ColorRingChange : public PCS_ColorRingInfo
		{
			enum {uri = (4 << 8) | protocol::VIP_RING_SVID};

			bool isDelRing;

			virtual void marshal(sox::Pack &p) const
			{
				p << isDelRing;
				PCS_ColorRingInfo::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> isDelRing;
				PCS_ColorRingInfo::unmarshal(p);				
			}
		};

		struct PCS_SetWatchUser : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::VIP_WEB_RING_SVID};

			uint32_t uid_;
			uint32_t watch_uid_;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid_ << watch_uid_;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_ >> watch_uid_;
			}
		};

		struct PCS_SetWatchUserRes : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::VIP_WEB_RING_SVID};

			uint32_t uid_;
			uint32_t watch_uid_;
			uint32_t return_code_;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid_ << watch_uid_ <<return_code_;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_ >> watch_uid_ >> return_code_;
			}
		};
		struct PCS_CancelWatchUser : public PCS_SetWatchUser
		{
			enum {uri = (3 << 8) | protocol::VIP_WEB_RING_SVID};
		};

		struct PCS_CancelWatchUserRes : public PCS_SetWatchUserRes
		{
			enum {uri = (4 << 8) | protocol::VIP_WEB_RING_SVID};
		};

		struct PCS_GetWatcherList : public sox::Marshallable
		{
			enum {uri = (24 << 8) | protocol::VIP_RING_SVID};

			uint32_t uid_;
		
			virtual void marshal(sox::Pack &p) const
			{
				p << uid_ ;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_ ;
			}
		};

		struct PCS_GetWatcherListRes : public sox::Marshallable
		{
			enum {uri = (25 << 8) | protocol::VIP_RING_SVID};

			uint32_t uid_;
			std::map<uint32_t,uint32_t> uid_ringId;

			virtual void marshal(sox::Pack &p) const
			{
				p << uid_ ;
				sox::marshal_container(p, uid_ringId);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uid_ ;
				sox::unmarshal_container(p, std::inserter(uid_ringId, uid_ringId.begin()));
			}
		};

		struct PCS_WebSetWatcherListRes : public PCS_GetWatcherListRes
		{
			enum {uri = (26 << 8) | protocol::VIP_RING_SVID};
		};

	}
}

#endif
