#ifndef PSTOREGTOPICU_H
#define PSTOREGTOPICU_H

#include <string>
#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"
#include "common/protocol/pgtopicbase.h"

using namespace protocol::gtopic;

namespace protocol {
namespace store {

	//protocol send it to access for requesting readed msg number
	struct PCS_UserGTopicCountReq :public sox::Marshallable{
		enum {uri = (11 << 8 | protocol::ACCESS_SVID)};

		uint32_t m_uGroupId;
		uint32_t m_uFolderId;
		std::vector<uint32_t> vecTopicId;
		virtual void marshal(sox::Pack &p) const {
			p << m_uGroupId << m_uFolderId;
			sox::marshal_container(p, vecTopicId);
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> m_uGroupId >> m_uFolderId;
			sox::unmarshal_container(p, std::back_inserter(vecTopicId));
		}
	};

	struct PCS_UserGTopicCountRes :public sox::Marshallable{
		enum {uri = (11 << 8 | protocol::ACCESS_SVID)};

		uint32_t m_uGroupId;
		uint32_t m_uFolderId;
		std::map<uint32_t, uint32_t>  mapTopicCount;

		virtual void marshal(sox::Pack &p) const {
			p << m_uGroupId << m_uFolderId;
			sox::marshal_container(p, mapTopicCount);
		}
		virtual void unmarshal(const sox::Unpack &p) {
			p >> m_uGroupId >> m_uFolderId;
			sox::unmarshal_container(p, std::inserter(mapTopicCount,
					mapTopicCount.begin()));
			//	virtual void unmarshal(const sox::Unpack &p) {
			//		unmarshal_container(p, std::inserter(mapTopicCount,
			//				mapTopicCount.begin()));
			//
			//		//			for (uint32_t count = p.pop_uint32(); count > 0; --count)
			//		//			{
			//		//				//typename OutputIterator::container_type::value_type tmp;
			//		//				std::map<GIDFIDTID, uint32_t>::value_type tmp;
			//		//				p >> tmp;
			//		//				//*i = tmp;
			//		//				//++i;
			//		//				//mapTopicCount.insert(tmp);
			//		//				mapTopicCount[tmp.first]=tmp.second;
			//		//			}
			//	}
		}
	};

	struct PCS_UserGTopicsReadInfo : public sox::Marshallable
	{
		enum {uri = (12 << 8) | protocol::ACCESS_SVID};

		std::vector<protocol::gtopic::STopicReadInfo>  m_vecTopics;


		virtual void marshal(sox::Pack &p) const
		{
			marshal_container(p, m_vecTopics);
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			unmarshal_containerEx(p, m_vecTopics);
		}

	};

	struct PCS_TokenReq:public sox::Marshallable 
	{
		enum {uri = (13 << 8) | protocol::ACCESS_SVID};

		uint32_t uToken;

		virtual void marshal(sox::Pack &p) const 
		{
			p  << uToken;
		}

		virtual void unmarshal(const sox::Unpack &p) 
		{
			p  >> uToken;
		}
	};

	struct PCS_TokenRes:public sox::Marshallable 
	{
		enum {uri = (13 << 8) | protocol::ACCESS_SVID};

		uint32_t bChecked;
		virtual void marshal(sox::Pack &p) const 
		{
			p << bChecked;
		}

		virtual void unmarshal(const sox::Unpack &p) 
		{
			p >> bChecked;
		}
	};

	struct PCS_UserGTopicCountAllReq :public sox::Marshallable{
				enum {uri = (14 << 8 | protocol::ACCESS_SVID)};
				uint32_t uLastTime;
				uint32_t uVersion;

				virtual void marshal(sox::Pack &p) const {
					p<<uLastTime<<uVersion;
				}
				virtual void unmarshal(const sox::Unpack &p) {
					p>>uLastTime>>uVersion;
				}
			};

	struct PCS_UserGTopicCountAllRes :public sox::Marshallable{
			enum {uri = (14 << 8 | protocol::ACCESS_SVID)};
			std::map<uint32_t, STopicReadInfoA>  mapTopicCount;
			//std::vector<STopicReadInfoA>  mapTopicCount;

			virtual void marshal(sox::Pack &p) const {
				sox::marshal_container(p, mapTopicCount);
			}
			virtual void unmarshal(const sox::Unpack &p) {
				sox::unmarshal_container(p, std::inserter(mapTopicCount,
						mapTopicCount.begin()));
			}
		};
} // store
} // server

#endif



