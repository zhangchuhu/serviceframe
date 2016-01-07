#ifndef PROTOCOL_GCHECK_H
#define PROTOCOL_GCHECK_H

#include "core/base_svid.h"
#include "packet.h"
#include <string>
#include <vector>

namespace protocol
{
	namespace gcheck
	{
		struct PCS_GroupCommon: virtual public sox::Marshallable
		{

			uint32_t m_uUsrID;
			uint32_t m_uGrpID;
			uint32_t m_uFldID;
			uint32_t m_uSign;
			PCS_GroupCommon()
			{
				m_uUsrID=-1;
				m_uGrpID=-1;
				m_uFldID=-1;
				m_uSign=-1;
			}

			PCS_GroupCommon(uint32_t uId,uint32_t uGid)
			{
				m_uUsrID=uId;
				m_uGrpID=uGid;
				m_uFldID=uGid;//不关心的话，必须设置成一样
				m_uSign=-1;
			}

			PCS_GroupCommon(uint32_t uId,uint32_t uGid,uint32_t uFid)
			{
				m_uUsrID=uId;
				m_uGrpID=uGid;
				m_uFldID=uFid;
				m_uSign=-1;
			}


			void SetCheckData(uint32_t uId,uint32_t uGid)
			{
				m_uUsrID=uId;
				m_uGrpID=uGid;
				m_uFldID=uGid;//不关心的话，必须设置成一样
				m_uSign=-1;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUsrID << m_uGrpID << m_uFldID << m_uSign;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUsrID >> m_uGrpID >> m_uFldID >> m_uSign;
			}
		};


		struct PCS_GetGroupSign: public PCS_GroupCommon
		{
			enum {uri = (1 << 8) | protocol::GCHECK_SVID};
			uint32_t m_uSvrID;
			uint32_t m_uAppURI;		// protocol 原始uri
			std::string m_strAppCtx;	// protocol 原始包

			virtual void marshal(sox::Pack &p) const
			{
				PCS_GroupCommon::marshal(p);
				p << m_uSvrID << m_uAppURI << m_strAppCtx;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				PCS_GroupCommon::unmarshal(p);
				p >> m_uSvrID >> m_uAppURI >> m_strAppCtx;
			}

		};

		struct PCS_GetGroupSignRes: public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::GCHECK_SVID};
			uint32_t m_uSign;
			uint32_t m_uAppURI;
			std::string m_strAppCtx;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSign << m_uAppURI << m_strAppCtx;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSign >> m_uAppURI >> m_strAppCtx;
			}
		};

		struct PCS_BatchGetGroupOwner: public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::GCHECK_SVID};
			std::vector<uint32_t> m_vecGrpID;

			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecGrpID);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				unmarshal_container(p, std::back_inserter(m_vecGrpID));
			}
		};

		struct PCS_GetGroupOwnerRes: public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::GCHECK_SVID};
			//uint32_t m_uGrpID;
			//uint32_t m_uUsrID;
			std::map<uint32_t, uint32_t> m_mapGrpOwner;// map from Group ID to Group Owner UID
#if 0
			PCS_GetGroupOwnerRes()
				: m_uGrpID(0), m_uUsrID(0)
			{
			}
#endif
			virtual void marshal(sox::Pack &p) const
			{
				//p << m_uGrpID << m_uUsrID;
				sox::marshal_container(p, m_mapGrpOwner); 
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				//p >> m_uGrpID >> m_uUsrID;
				sox::unmarshal_container(p, std::inserter(m_mapGrpOwner, m_mapGrpOwner.begin()));
			}
		};

//		//Note that the below message is defined in pginfo.h
// 		struct PCS_RoleListViaCached : public PCS_SyncGrpFolderMemberList
// 		{
// 			enum {uri = (8 << 8 | protocol::GCHECK_SVID)};
// 		};

//		//Note that the below message is defined in pginfosrv.h
//		struct PSS_GRoleUpdate  : public sox::Marshallable
//		{
//			enum {uri = (9 << 8) | protocol::GCHECK_SVID};


	}
}

#endif

