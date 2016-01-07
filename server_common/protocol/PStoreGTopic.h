#ifndef PSTOREGTOPIC_H_INCLUDED
#define PSTOREGTOPIC_H_INCLUDED

#include <string>

#include "common/packet.h"
#include "common/core/base_svid.h"
#include "common/protocol/const.h"

namespace server {
namespace store {

	union PStoreGTopicReqMask
	{
		struct Bits {
			bool bModifyTime:1;
			bool bFlag:1;
			bool bTitle:1;
			bool bDescription:1;
			bool bPermission:1;
		} bits;
		uint32_t uMaskWord;
	};
	
	union PStoreGTopicFlagMask
	{
		struct Bits {
			bool Pinned:1;	// ÖÃ¶¥
			bool prime:1;		// ¾«»ª
		} bits;
		uint32_t uMaskWord;
	};

	struct PSS_CreateTopicReq:public sox::Marshallable{
		enum {uri = (7 << 8 | protocol::ACCESS_SVID)};

		uint32_t m_uGrpID;
		uint32_t m_uFldrID;
		uint32_t m_uTopicID;
		uint32_t m_uSender;
		uint32_t m_uCreateTime;
		PStoreGTopicFlagMask m_objFlag;
		uint32_t m_uPermission;
		std::string m_strTitle;
		std::string m_strDescription;

		PSS_CreateTopicReq(){}

		virtual void marshal(sox::Pack &p) const
		{
			p << m_uGrpID
			<< m_uFldrID
			<< m_uTopicID
			<< m_uSender
			<< m_uCreateTime
			<< m_objFlag.uMaskWord
			<< m_uPermission
			<< m_strTitle
			<< m_strDescription;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_uGrpID
			>> m_uFldrID
			>> m_uTopicID
			>> m_uSender
			>> m_uCreateTime
			>> m_objFlag.uMaskWord
			>> m_uPermission
			>> m_strTitle
			>> m_strDescription;
		}
	};

	struct PSS_DeleteTopicReq:public sox::Marshallable{
		// member : uint32_t uGrpID,uint32_t uFldrID,uint32_t sender,vector<uint32_t vecTIDs
		enum {uri = (8 << 8 | protocol::ACCESS_SVID)};

		uint32_t m_uGrpID;
		uint32_t m_uFldrID;
		uint32_t m_uSender;
		std::vector<uint32_t> vecTIDs;

		PSS_DeleteTopicReq(){}

		virtual void marshal(sox::Pack &p) const
		{
			p << m_uGrpID
			<< m_uFldrID
			<< m_uSender;
			marshal_container(p, vecTIDs);
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_uGrpID
			>> m_uFldrID
			>> m_uSender;
			unmarshal_container(p, std::back_inserter(vecTIDs));
		}
	};

	struct PSS_UpdateTopicReq:public sox::Marshallable{
		enum {uri = (9 << 8 | protocol::ACCESS_SVID)};

		PStoreGTopicReqMask m_objMask;  // GTopicReqMask in pgtopicbase.h
		uint32_t m_uGrpID;
		uint32_t m_uFldrID;
		uint32_t m_uTopicID;
		uint32_t m_uSender;
		uint32_t m_uModifyTime;
		PStoreGTopicFlagMask m_objFlag;
		uint32_t m_uPermission;
		std::string m_strTitle;
		std::string m_strDescription;

		PSS_UpdateTopicReq(){}

		virtual void marshal(sox::Pack &p) const
		{
			p << m_objMask.uMaskWord
			<< m_uGrpID
			<< m_uFldrID
			<< m_uTopicID
			<< m_uSender
			<< m_uModifyTime
			<< m_objFlag.uMaskWord
			<< m_uPermission
			<< m_strTitle
			<< m_strDescription;
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_objMask.uMaskWord
			>> m_uGrpID
			>> m_uFldrID
			>> m_uTopicID
			>> m_uSender
			>> m_uModifyTime
			>> m_objFlag.uMaskWord
			>> m_uPermission
			>> m_strTitle
			>> m_strDescription;
		}
	};

	struct PSS_DeleteGroupOrFolder : public sox::Marshallable
	{
		enum {uri = (6 << 8 | protocol::ACCESS_SVID)};
		
		uint32_t m_uGrpID; 
		// if del group, contain group ids and folder ids in group
		// if del folder, contain folder id
		std::set<uint32_t> m_setFldID; 

		PSS_DeleteGroupOrFolder():
			m_uGrpID(0){}

		PSS_DeleteGroupOrFolder(uint32_t uGrpID, std::set<uint32_t>& setFldID):
			m_uGrpID(uGrpID),
			m_setFldID(setFldID)
		{
		}

		virtual void marshal(sox::Pack &p) const
		{
			p << m_uGrpID;
			sox::marshal_container(p, m_setFldID);
		}

		virtual void unmarshal(const sox::Unpack &p)
		{
			p >> m_uGrpID;
			sox::unmarshal_container(p, std::inserter(m_setFldID, m_setFldID.begin()));
		}
	};

	
} // store
} // server

#endif


