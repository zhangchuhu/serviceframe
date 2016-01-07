#ifndef __psysmsgd_h__
#define __psysmsgd_h__

#include "packet.h"
#include "core/base_svid.h"
#include "common/protocol/pSysMsg.h"
#include "common/protocol/pimdb.h"

namespace server{
	namespace sysmsg{
		//用于监听用户登陆
		struct PNotifyUserLogin : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::SYSMSGD_SVID};
			PNotifyUserLogin(){}

			uint32_t linkd_id;
			uint32_t u_id;
			
			virtual void marshal(sox::Pack &p) const
			{
				p << u_id << linkd_id;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> u_id >> linkd_id;
			}

			virtual ~PNotifyUserLogin()
			{

			}
		};

		//批量发送系统消息
		struct PSS_BatchSysMsg: public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::SYSMSGD_SVID};

            protocol::sysmsg::PSysMsg m_objSysMsg;
			std::vector<uint32_t> uids;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_objSysMsg;
                sox::marshal_container(p, uids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
                p >> m_objSysMsg;
                sox::unmarshal_container(p, std::back_inserter(uids));
			}
		};


		// Add by chenyongqiang 2010-07-08
 		// 用户详细信息
		struct SysMsgUInfoDetail :public protocol::im::SUserinfo
		{		
			uint32_t uid;
			uint32_t imlinkd;
			uint32_t status;	
			uint32_t birthday;		
			uint32_t city;
			uint32_t jifen;
			uint32_t province;	
			uint32_t sex;	
	
			virtual void marshal(sox::Pack &p) const	
			{			
				SUserinfo::marshal(p);
				p << uid << imlinkd << status << birthday << city << jifen << province << sex;		
			}	
	
			virtual void unmarshal(const sox::Unpack &p)		
			{			
				SUserinfo::unmarshal(p);
				p >> uid >> imlinkd >> status >> birthday >> city >> jifen >> province >> sex;	
			}	
		};	
	
		// 用于从imsysmsgcache批量请求用户详细信息		
		struct PSS_BatchGetSysMsgUInfoDetail : public sox::Marshallable	
		{	
			enum {uri = (3 << 8) | protocol::SYSMSGD_SVID};	
			std::vector<SysMsgUInfoDetail> udetails;		
		
			virtual void marshal(sox::Pack &p) const	
			{			
				sox::marshal_container(p, udetails);	
			}		
		
			virtual void unmarshal(const sox::Unpack &p)	
			{	
				sox::unmarshal_container(p, std::back_inserter(udetails));
			}		
		};

		// 用于从imsysmsgcache批量请求用户详细信息回应		
		struct PSS_BatchGetSysMsgUInfoDetailRes : public PSS_BatchGetSysMsgUInfoDetail	
		{	
			enum {uri = (4 << 8) | protocol::SYSMSGD_SVID};	
		};
		// End



		// 用户详细信息 双路
		struct SysMsgUInfoDetailMul :public protocol::im::SUserinfo
		{
			SysMsgUInfoDetailMul() 
				: tcpLink(NONE_SERVER_ID), tcpStatus(protocol::IMOFFLINE), 
				udpLink(NONE_SERVER_ID) , udpStatus(protocol::IMOFFLINE)
			{}

			uint32_t uid;
			uint32_t tcpLink;
			uint32_t tcpStatus;	
			uint32_t udpLink;
			uint32_t udpStatus;	
			uint32_t birthday;		
			uint32_t city;
			uint32_t jifen;
			uint32_t province;	
			uint32_t sex;	

			virtual void marshal(sox::Pack &p) const	
			{			
				SUserinfo::marshal(p);
				p << uid << tcpLink << tcpStatus << udpLink << udpStatus << birthday << city << jifen << province << sex;		
			}	

			virtual void unmarshal(const sox::Unpack &p)		
			{			
				SUserinfo::unmarshal(p);
				p >> uid >> tcpLink >> tcpStatus >> udpLink >> udpStatus >> birthday >> city >> jifen >> province >> sex;	
			}	
		};	

		// 用于从imsysmsgcache批量请求用户详细信息		
		struct PSS_BatchGetSysMsgUInfoDetailMul : public sox::Marshallable	
		{	
			enum {uri = (5 << 8) | protocol::SYSMSGD_SVID};	
			std::vector<SysMsgUInfoDetailMul> udetails;		

			virtual void marshal(sox::Pack &p) const	
			{			
				sox::marshal_container(p, udetails);	
			}		

			virtual void unmarshal(const sox::Unpack &p)	
			{	
				sox::unmarshal_container(p, std::back_inserter(udetails));
			}		
		};

		// 用于从imsysmsgcache批量请求用户详细信息回应		
		struct PSS_BatchGetSysMsgUInfoDetailMulRes : public PSS_BatchGetSysMsgUInfoDetailMul	
		{	
			enum {uri = (6 << 8) | protocol::SYSMSGD_SVID};	
		};


		// 对外接口 --- 系统消息服务
		enum E_PROT_TYPE {
			PROT_TYPE_XML,
			PROT_TYPE_TP
		};

		struct PSS_SysMsg : public sox::Marshallable
		{
			enum {uri = (10 << 8) | protocol::SYSMSGD_SVID};

			E_PROT_TYPE m_protType;
			std::string m_strMsg;

			virtual void marshal(sox::Pack &p) const	
			{
				p.push_uint8(m_protType);
				p.push_varstr32(m_strMsg.data(), m_strMsg.size());
			}	

			virtual void unmarshal(const sox::Unpack &p)		
			{
				m_protType = (E_PROT_TYPE)p.pop_uint8();
				m_strMsg = p.pop_varstr32();
			}	
		};

		struct PSS_SysMsgRes : public sox::Marshallable
		{
			enum {uri = (11 << 8) | protocol::SYSMSGD_SVID};

			uint32_t m_rc;
			uint32_t m_id;

			virtual void marshal(sox::Pack &p) const	
			{
				p << m_rc << m_id;		
			}	

			virtual void unmarshal(const sox::Unpack &p)		
			{
				p >> m_rc >> m_id;
			}	
		};

		// 发送至cosysmsg
		struct PSS_CoSysMsg : public sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::SYSMSGD_SVID};

			bool m_bSendAll;
			uint32_t m_id;
			uint32_t m_msgType;
			std::string m_strUserSet;
			std::string m_strFilter;
			std::string m_strStrategy;
			std::string m_strMsg;
			uint32_t time_from;
			uint32_t time_to;
			uint8_t m_status; //online, login, online and login

			virtual void marshal(sox::Pack &p) const	
			{
				p.push_uint8(m_bSendAll);
				p << m_id << m_msgType << m_strUserSet << m_strFilter << m_strStrategy 
					<< m_strMsg << time_from << time_to << m_status;		
			}	

			virtual void unmarshal(const sox::Unpack &p)		
			{
				m_bSendAll = (bool)p.pop_uint8();
				p >> m_id >> m_msgType >> m_strUserSet >> m_strFilter >> m_strStrategy 
					>> m_strMsg >> time_from >> time_to >> m_status;
			}	
		};

		//发送至用户
		struct PSS_SysMsg4UserSet: public sox::Marshallable
		{
			enum {uri = (13 << 8) | protocol::SYSMSGD_SVID};

			protocol::sysmsg::PCS_SysMsg m_objSysMsg;
			std::vector<uint32_t> uids;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_objSysMsg;
				sox::marshal_container(p, uids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_objSysMsg;
				sox::unmarshal_container(p, std::back_inserter(uids));
			}
		};

		struct PSS_SysMsgCompleteNotify: public sox::Marshallable
		{
			enum {uri = (14 << 8) | protocol::SYSMSGD_SVID};

			PSS_SysMsgCompleteNotify() {}

			PSS_SysMsgCompleteNotify(uint32_t id) : m_id(id) {}
            
			uint32_t m_id;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_id;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_id;
			}
		};

		struct PSS_SysMsgUpdateStat: public sox::Marshallable
		{
			enum {uri = (15 << 8) | protocol::SYSMSGD_SVID};

			PSS_SysMsgUpdateStat() {}


			virtual void marshal(sox::Pack &p) const
			{
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};

		struct PSS_SysMsg4Peer: public sox::Marshallable
		{
			enum {uri = (16 << 8) | protocol::SYSMSGD_SVID};

			PSS_SysMsg4Peer() : 
				m_uFromId(0), m_uToId(0), m_msgType(0), m_strMsg(""), m_bSaveOffline(false) 
			{}

			uint32_t m_uFromId;
			uint32_t m_uToId;
			uint32_t m_msgType;		//reserve
			std::string m_strMsg;
			bool m_bSaveOffline;


			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFromId << m_uToId << m_msgType << m_strMsg << m_bSaveOffline;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uFromId >> m_uToId >> m_msgType >> m_strMsg >> m_bSaveOffline;
			}
		};

		struct PSS_SysMsg4Mass: public sox::Marshallable
		{
			enum {uri = (17 << 8) | protocol::SYSMSGD_SVID};

			PSS_SysMsg4Mass() : 
				m_bSendAll(false), m_msgType(0), m_strFilter(""), m_strStrategy(""), 
				m_strMsg(""), time_from(0), time_to(0), m_status(0)
			{}

			bool m_bSendAll;
			uint32_t m_msgType;	//reserve
			std::set<uint32_t> m_userSet;
			std::string m_strFilter;
			std::string m_strStrategy;
			std::string m_strMsg;
			uint32_t time_from;
			uint32_t time_to;
			uint8_t m_status; //online 1, login 2, online and login 3


			virtual void marshal(sox::Pack &p) const
			{
				p << m_bSendAll << m_msgType <<  m_strFilter << m_strStrategy 
					<< m_strMsg << time_from << time_to << m_status;
				sox::marshal_container(p, m_userSet);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_bSendAll >> m_msgType >> m_strFilter >> m_strStrategy 
					>> m_strMsg >> time_from >> time_to >> m_status;

				sox::unmarshal_container(p, std::inserter(m_userSet, m_userSet.begin()));
			}
		};
	}
}

#endif

