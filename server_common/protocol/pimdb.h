#pragma once

#include <map>
#include <string>
#include <time.h>

#include "core/base_svid.h"
#include "protocol/const.h"
#include "common/protocol/pimcommon.h"


namespace server
{
	namespace im
	{		
	
// 		struct SimpleBInfo : public sox::Marshallable 
// 		{
// 			SimpleBInfo(){}
// 			SimpleBInfo(uint32_t bid, std::string& mark, uint32_t p)
// 				: buddyid(bid), remark(mark), pid(p) {}
// 				uint32_t buddyid; //锟斤拷锟斤拷ID
// 				std::string remark;//锟斤拷锟窖憋拷注
// 				uint32_t pid;//锟斤拷锟斤拷锟斤拷锟斤拷锟斤拷ID
// 
// 				virtual void marshal(sox::Pack &p) const {
// 					p << buddyid << remark << pid;
// 				}
// 				virtual void unmarshal(const sox::Unpack &p) {
// 					p >> buddyid >> remark >> pid;
// 				}
// 		};


		struct 		PCS_IMLogout : public sox::Marshallable 
		{
			enum {uri = (18 << 8) | protocol::IMDB_SVID};
			PCS_IMLogout(){}

			uint32_t uid;

			virtual void marshal(sox::Pack &p) const 
			{
				p << uid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> uid;
			}
		};		

		//获取登陆预备信息
		struct PSS_GetPrepareLoginListReq : public sox::Marshallable {
			enum { uri = (99 << 8) | protocol::IMDB_SVID };
			PSS_GetPrepareLoginListReq(){}

			uint32_t m_uId;											//用户的uid
			uint32_t m_linkId;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId << m_linkId;	
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uId >> m_linkId;
			}
		};

		//获取登陆预备消息的响应
		struct PSS_GetPrepareLoginListRes : public sox::Marshallable {
			enum { uri = (100 << 8) | protocol::IMDB_SVID };
			PSS_GetPrepareLoginListRes(){}

			uint32_t m_uId;											//用户的uid
			uint32_t m_uImid;											//用户的歪歪号
			std::vector<protocol::im::CBuddyListNoRemark> m_vtBuddyList;		//好友列表
			std::vector<uint32_t>		m_vtBlockUid;		//阻塞名单
			std::vector<uint32_t>		m_vtBlackUid;		//黑名单
			std::vector<protocol::im::CFolderList> m_vtFolderList;		//分组列表

			uint32_t m_uResCode;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId << m_uImid << m_uResCode;
				marshal_container(p, m_vtBuddyList);
				marshal_container(p, m_vtBlockUid);
				marshal_container(p, m_vtBlackUid);
				marshal_container(p, m_vtFolderList);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uId >> m_uImid >> m_uResCode;
				unmarshal_container(p, std::back_inserter(m_vtBuddyList));
				unmarshal_container(p, std::back_inserter(m_vtBlockUid));
				unmarshal_container(p, std::back_inserter(m_vtBlackUid));
				unmarshal_container(p, std::back_inserter(m_vtFolderList));
			}
		};

		// 登陆时，在imtransfer请求buddylist的包，临时方案
		struct PSS_PrepareLoginBuddylist : public sox::Marshallable
		{
			enum { uri = (101 << 8) | protocol::IMDB_SVID };
			PSS_PrepareLoginBuddylist(){}

			uint32_t                    m_uLinkdId;
			PSS_GetPrepareLoginListRes  m_resList;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uLinkdId;
				m_resList.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uLinkdId;
				m_resList.unmarshal(p);
			}
		
		};

		struct PSS_GetBuddyRemarkReq : public sox::Marshallable
		{
			enum {uri = (300 << 8) | protocol::IMDB_SVID};
			PSS_GetBuddyRemarkReq(){}

			uint32_t m_uId;
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId;
			}
		};

		struct PSS_GetBuddyRemarkRes : public sox::Marshallable//public PCS_GetBuddyRemarkRes
		{
			enum { uri = (301 << 8) | protocol::IMDB_SVID};
			PSS_GetBuddyRemarkRes(){}

			uint32_t m_uid;
			std::map<uint32_t, std::string> m_mapBidToRemark;
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uid;
				p.push_uint32(m_mapBidToRemark.size());
				for(std::map<uint32_t, std::string>::const_iterator iter = m_mapBidToRemark.begin(); iter != m_mapBidToRemark.end(); ++iter)
				{
					p.push_uint32(iter->first);
					p << iter->second;
				}
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uid;
				uint32_t size = p.pop_uint32();
				for(uint32_t i =0; i < size; ++i)
				{
					uint32_t bid = p.pop_uint32();
					std::string tmp;
					p >> tmp;
					m_mapBidToRemark.insert(make_pair(bid, tmp));
				}
			}
		};

		struct PSS_ChangedRemark : public sox::Marshallable
		{
			enum { uri = (302 << 8) | protocol::IMDB_SVID };
			PSS_ChangedRemark(){}
			//PSS_ChangedRemark(E_ITEMTYPE t, uint32_t ID, const std::string& rem)
			//	: type(t), id(ID), remark(rem){
			//}

			protocol::im::E_ITEMTYPE type;
			uint32_t uid;
			uint32_t bid;
			std::string remark;

			virtual void marshal(sox::Pack &p) const{
				p.push_uint8(type);
				p << uid << bid << remark;
			}

			virtual void unmarshal(const sox::Unpack &p){
				type = (protocol::im::E_ITEMTYPE)p.pop_uint8();
				p >> uid >> bid >> remark;
			}
		};

		struct PSS_GetPartialBuddyRemarkReq : public sox::Marshallable
		{
			enum {uri = (303 << 8) | protocol::IMDB_SVID};
			PSS_GetPartialBuddyRemarkReq(){}

			uint32_t m_uId;
			std::set<uint32_t>	m_setUid;
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uId;
				marshal_container(p, m_setUid);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uId;
				unmarshal_container(p, std::inserter(m_setUid, m_setUid.begin()));
			}
		};

		struct PSS_GetPartialBuddyRemarkRes : public PSS_GetBuddyRemarkRes
		{
			enum { uri = (304 << 8) | protocol::IMDB_SVID};
			PSS_GetPartialBuddyRemarkRes(){}

			std::set<uint32_t>	m_setUid;
			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_setUid);
				PSS_GetBuddyRemarkRes::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::inserter(m_setUid, m_setUid.begin()));
				PSS_GetBuddyRemarkRes::unmarshal(p);
			}
		};

	}
}



