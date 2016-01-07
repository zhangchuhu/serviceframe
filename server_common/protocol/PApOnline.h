#pragma once

#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"


namespace sox
{ // todo 稳定后再放到公共库路径

  //Marshallable的扩展，加入安全的边界处理和版本号字段。VSMarshallable可利用边界或版本号实现一定扩展性
  struct VSMarshallable : public sox::Marshallable
  {
  	size_t m_vendpos; //用于判断边界
  	mutable uint8_t m_vver; //用户可用来标识协议版本(0~15)

  	VSMarshallable() : m_vendpos(0), m_vver(0) {}

  	//设置协议版本
  	inline void SetVVer(uint8_t v) const//0~15
  	{
  		m_vver = v;
  	}

  	//判断边界
  	inline bool IsNotEnd(const sox::Unpack & p)
  	{
  		return (p.size() > m_vendpos);
  	}

  	//用户协议编解码实现，跟Marshallable的marshal/unmarshal用法一样
  	virtual void vsmarshal(sox::Pack &) const = 0;
  	virtual void vsunmarshal(const sox::Unpack &) = 0;

  	//真正的编解码入口
  	virtual void marshal(sox::Pack & p) const {
  		size_t uPos = p.size();
  		p.push_uint32(0); //placeholder
  		vsmarshal(p);
  		size_t uSize = (m_vver << 28) | ((p.size() - uPos - 4) & 0xfffffff); //placeholder长度不计入
  		p.replace_offset_uint32(uPos, uSize);
  	}

  	virtual void unmarshal(const sox::Unpack &p) {
  		uint32_t uSize = p.pop_uint32();
  		m_vver = uSize >> 28;
  		uSize &= 0xfffffff;
  		if(uSize > p.size()) uSize = p.size();
  		m_vendpos = p.size() - uSize;
  		const char * data_bak = p.data();
  		size_t size_bak = p.size();
  		vsunmarshal(p);
  		p.reset(data_bak + uSize, size_bak - uSize);
  	}
  };

	//新增对map的支持
	template <class T1, class T2>
	inline sox::Pack& operator << (sox::Pack& p, const std::map<T1, T2>& map)
	{
		sox::marshal_container(p, map);
		return p;
	}

	template <class T1, class T2>
	inline const sox::Unpack& operator >> (const sox::Unpack& p, std::map<T1, T2>& map)
	{
		sox::unmarshal_container(p, std::inserter(map, map.begin()));
		return p;
	}  
}

namespace server
{
	namespace aponline
	{
		enum EAPOLINESTATUS
		{
			ENUM_OFFLINE = 0, 
			ENUM_ONLINE = 1,
		};	
		
    enum EClientType
    {
      ENUM_CLIENT_PC = 0,      // PC YY
      ENUM_CLIENT_MOBILE,      // Mobile YY
      ENUM_CLIENT_WEBYY,       // Web YY
      ENUM_CLIENT_XMAN,        // XMan
      ENUM_CLIENT_PAD,         // Pad YY
      ENUM_CLIENT_HD,          // Pad YY
    
      /* Non-YY app start */
      ENUM_CLIENT_WONDER_MOBILE = 64,
    
      ENUM_CLIENT_UNKNOWN = 255
    };

    inline uint32_t parseClientType(const std::string& strFrom)
		{
			if (strFrom == "yy")
			{
				return ENUM_CLIENT_PC;
			}
			else if (strFrom.compare(0, 6, "yymweb") == 0)
			{
				// WebYY
				return ENUM_CLIENT_WEBYY;
			}
			else if (strFrom.compare(0, 3, "yym") == 0)
			{
        return ENUM_CLIENT_MOBILE;
			}

			return ENUM_CLIENT_UNKNOWN;
		}
		
		//内存结构
		struct ApOnlineMemDataStruct
		{
			ApOnlineMemDataStruct()
			{
				memset(pcInfo, 0 , 32);
			}
			
			uint8_t uApType;
			uint32_t uAppId;     // APP ID
			uint32_t ip;		 // 客户端ip
			int64_t  uApId;      // AP ID
			uint32_t uTerm;      // 终端类型, PC/Mobile/Pad
			uint32_t uProtocol;  // TCP、UDP,etc
			uint64_t uTimestamp; // 时间戳
			char pcInfo[32];

			void operator=(const ApOnlineMemDataStruct& r)
			{	
				uApType = r.uApType;
				uAppId = r.uAppId;
				ip = r.ip;
				uApId = r.uApId;
				uTerm = r.uTerm;
				uProtocol = r.uProtocol;
				uTimestamp = r.uTimestamp;
				memcpy(pcInfo, r.pcInfo, sizeof(pcInfo));
			}
		};		
		
		enum EAPType
		{
			ENUM_AP_UNIAP,
			ENUM_AP_LINKD, 
			ENUM_UNKNOWN_AP = 255
		};	
		struct PSS_ReportApOnline : public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::APONLINE_SVID};
			
			PSS_ReportApOnline(uint32_t uid, uint32_t svid, uint32_t type, uint32_t time, uint8_t status, 
			                     uint32_t cip, const std::string & pc,  const std::string & from,  const std::string & verStr, 
			                     uint64_t seq, uint32_t tktime, uint16_t port, const std::string & inst):
			  uAppId(259),uAccount(uid),uAppKey(0),uTerminal(0),uApId(0),uApDMSvrID(svid),
			  uTerm(type),uProtocol(1),uTimestamp(time),uStatus(status),
			  uCliIP(cip),strPCInfo(pc),uAPType(ENUM_AP_LINKD),strClientFrom(from),strClientVer(verStr),
			  uAPSeqId(seq),bReconnect(0),uTicketTimestamp(tktime), uCliPort(port), strInstance(inst){}
			
			// 账号
			uint32_t uAppId;
			uint64_t uAccount;
			uint64_t uAppKey;
			uint64_t uTerminal; //新terminal type

			int64_t  uApId;      // AP s2s server ID
			uint32_t uApDMSvrID; // AP daemon server ID
			uint32_t uTerm;      // 终端类型, PC/Mobile/Pad
			uint32_t uProtocol;  // TCP、UDP,etc
			uint64_t uTimestamp; // 时间戳
			uint8_t  uStatus;    // 1: 上线 0: 下线
			uint32_t uCliIP;	 // 客户端ip
			std::string strPCInfo;	// pc info
			uint8_t uAPType;    // 区分AP/linkd等不同前端
			std::string strClientFrom; //YYFrom in cookie
			std::string strClientVer; //
			uint64_t uAPSeqId;
			uint8_t bReconnect;
			uint32_t uTicketTimestamp;
			uint16_t uCliPort;
			std::string strInstance;	// 客户端实例
			
			virtual void marshal(sox::Pack &p) const
			{
				//sox::PackX p(&p_);
				p << uAppId << uAccount << uApId << uApDMSvrID;
				p << uTerm << uProtocol << uTimestamp << uStatus << uCliIP;
				//p.batch_prim_push(uAppId, uAccount, uint64_t(uApId), uApDMSvrID, uTerm, uProtocol, uTimestamp, uStatus, uCliIP);
				p << strPCInfo;
				p << uAPType;
				p << strClientFrom;
				p << strClientVer;
				p << uAPSeqId;
				p << uAppKey << uTerminal;
				p << bReconnect;
				p << uTicketTimestamp;
				p << uCliPort;
				p << strInstance;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				//sox::UnpackX p(&p_);
				p >> uAppId >> uAccount >> uApId >> uApDMSvrID;
				p >> uTerm >> uProtocol >> uTimestamp >> uStatus >> uCliIP;
				//uint64_t uApIDTmp;
				//p.batch_prim_pop(uAppId, uAccount, uApIDTmp, uApDMSvrID, uTerm, uProtocol, uTimestamp, uStatus, uCliIP);
				//uApId = (int64_t)uApIDTmp;
				p >> strPCInfo;
				if (p.size() > 0)
				{
					p >> uAPType;
					p >> strClientFrom;
				}
				if (p.size() > 0)
					p >> strClientVer;
				if (p.size() > 0)
					p >> uAPSeqId;
				if (p.size() > 0)
					p >> uAppKey >> uTerminal;
				if (p.size() > 0)
					p >> bReconnect;
				if (p.size() > 0)
					p >> uTicketTimestamp;
				if (p.size() > 0)
					p >> uCliPort;
				if (p.size() > 0)
					p >> strInstance;
			}

		};

		// 异步获取在线状态的请求及回应
		struct PSS_GetApOnline : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::APONLINE_SVID};

			uint64_t uSeq;       // 序号，用于找回上下文
			// 账号
			uint32_t uAppId;
			std::vector<uint64_t> uAccounts;

			virtual void marshal(sox::Pack &p) const
			{
				p << uSeq << uAppId;
				sox::marshal_container(p, uAccounts);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uSeq >> uAppId;
				sox::unmarshal_container(p, std::back_inserter(uAccounts));
			}
		};
		
		//
		struct APOnlineStruct : public sox::Marshallable
		{
			APOnlineStruct():uAppId(0),uApId(0),uTerm(0),uProtocol(0),uTimestamp(0)
			{

			}
			
			uint32_t uAppId;     // APP ID
			uint32_t uIp;			
			int64_t  uApId;      // AP ID
			uint32_t uTerm;      // 终端类型, PC/Mobile/Pad
			uint32_t uProtocol;  // TCP、UDP,etc
			uint64_t uTimestamp; // 时间戳
			std::string strPCInfo;

			virtual void marshal(sox::Pack &p) const
			{
				p << uAppId << uIp << uApId << uTerm << uProtocol << uTimestamp << strPCInfo;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uAppId >> uIp >> uApId >> uTerm >> uProtocol >> uTimestamp >> strPCInfo;
			}

		};

		typedef std::vector<APOnlineStruct> OnlineVec;

		struct PSS_GetApOnlineRes : public sox::Marshallable
		{
			enum {uri = (3 << 8) | protocol::APONLINE_SVID};

			uint64_t uSeq;       // 序号，用于找回上下文

			// 账号
			uint32_t uAppId;

			std::map<uint64_t, OnlineVec> uAccoutStatus;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uSeq << uAppId;
				sox::marshal_container(pk, uAccoutStatus);
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uSeq >> uAppId;
				sox::unmarshal_container(up, std::inserter(uAccoutStatus, uAccoutStatus.begin()));
			}
		};

	

		// 在线状态的注册及回应
		struct PSS_RegisterApOnlineNotify : public sox::Marshallable
		{
			enum {uri = (4 << 8) | protocol::APONLINE_SVID};

			PSS_RegisterApOnlineNotify() : bNeedAllAccount(false),bNeedCheckUserSt(false)
			{
										 
			}

			bool bNeedAllAccount;	 // 是否需要全量用户
			uint32_t uAppId;		 // 订阅的何种业务
			int64_t  uServId;		 // 订阅者在节点发现体系中的id
			uint64_t  seq;			 //	断线重练用于续传，初次订阅0
			int64_t  uSrvIdOld;		 // 订阅者的前一次订阅时自己的ID，用于在线进程判断上次接受订阅是否是自己，用于续传，初次订阅填0
			bool bNeedCheckUserSt;	 // 是否需要检查用户是否在线

			//会关注连接状态，连接断开不会推送消息，重练以后需要重新注册，如果需要续传需要带上seq和uSrvIdOld

			virtual void marshal(sox::Pack &p) const
			{
				p.push_uint8(bNeedAllAccount);
				p  << uAppId << uServId << seq << uSrvIdOld;
				p.push_uint8(bNeedCheckUserSt);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				bNeedAllAccount = (bool)p.pop_uint8();
				p >> uAppId >> uServId >> seq >> uSrvIdOld;
				if (p.size() > 0)
					bNeedCheckUserSt = (bool)p.pop_uint8();
			}
		};

		struct PSS_RegisterApOnlineNotifyRes : public sox::Marshallable
		{
			enum {uri = (5 << 8) | protocol::APONLINE_SVID};

			uint32_t uAppId;     // 订阅的何种业务
			int64_t  uServerId;  // 订阅进程在节点发现体系中的id

			virtual void marshal(sox::Pack &p) const
			{
				p << uAppId << uServerId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> uAppId >> uServerId;
			}
		};

		struct ApOnlineNotify : public sox::VSMarshallable
		{
			ApOnlineNotify()
			{
			}

			ApOnlineNotify(const ApOnlineMemDataStruct& r, uint64_t account):uAccount(account), uApId(r.uApId),
				uTerm(r.uTerm),uProtocol(r.uProtocol),uTimestamp(r.uTimestamp), uStatus(1),pcInfo(r.pcInfo, 32)
			{
			}

			uint64_t uAccount;   // 与uAppId联合作为账号体系
			int64_t  uApId;      // 所在AP的ID
			uint32_t uTerm;      // 终端类型, PC/Mobile/Pad
			uint32_t uProtocol;  // TCP、UDP,etc
			uint64_t uTimestamp; // 时间戳
			uint32_t ip;         // 客户端ip
			uint8_t  uStatus;    // 1: 上线 0: 下线
			std::string pcInfo;
			std::string client_ver;

			virtual void vsmarshal(sox::Pack &p) const
			{
				p << uAccount << uApId << uTerm << uProtocol << uTimestamp << ip << uStatus << pcInfo << client_ver;
			}

			virtual void vsunmarshal(const sox::Unpack& p)
			{
				size_t vendsizetmp = m_vendpos;
				p >> uAccount >> uApId >> uTerm >> uProtocol >> uTimestamp >> ip >> uStatus >> pcInfo;
				if (p.size()>vendsizetmp)
					p >> client_ver;

			}

		};

		// 在线状态变更的通知(批量方式)
		struct PSS_ApOnlineNotify : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::APONLINE_SVID};

			uint64_t seq;
			
			bool isAllOnline;    //推送数据是否是全量数据

			std::vector<ApOnlineNotify> vecChanged;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << seq ;
				pk.push_uint8(isAllOnline);
				sox::marshal_container(pk, vecChanged);
			}

			virtual void unmarshal(const sox::Unpack& up)
			{
				up >> seq ;
				isAllOnline = (bool)up.pop_uint8();
				sox::unmarshal_containerEx(up, vecChanged);
			}
		};
		
		//  共享内存结构
		struct ApOnlineCmpMemItem
		{
			ApOnlineCmpMemItem()
				: uAppId(0), uAccount(0), uTerm(0), uProtocol(0), uTimestamp(0)
			{

			}

			uint32_t uAppId;     // 订阅的何种业务
			uint64_t uAccount;   // 与uAppId联合作为账号体系

			uint32_t uTerm;      // 终端类型, PC/Mobile/Pad
			uint32_t uProtocol;  // TCP、UDP,etc
			uint64_t uTimestamp; // 时间戳
			uint32_t uCliIP;	 // 客户端ip
			char strPCInfo[32];	// pc info

			void marshal(sox::Pack &p) const
			{
				p << uAppId << uAccount << uTerm << uProtocol << uTimestamp << uCliIP;
				std::string tmp(strPCInfo, 32);
				p << tmp;
			}

			void unmarshal(const sox::Unpack& up)
			{
				up >> uAppId >> uAccount >> uTerm >> uProtocol >> uTimestamp >> uCliIP;
				std::string tmp;
				up >> tmp;
				memcpy(strPCInfo, tmp.data(), 32);
			}
		};

		struct ApOnlineCompareItem : public sox::Marshallable
		{
			ApOnlineCompareItem()
			{
				
			}

			ApOnlineCompareItem(ApOnlineCmpMemItem& onlineItem):uAponlineMemItem(onlineItem)
			{
				
			}

			ApOnlineCmpMemItem  uAponlineMemItem;

			virtual void marshal(sox::Pack &p) const
			{
				uAponlineMemItem.marshal(p);
			}

			virtual void unmarshal(const sox::Unpack& up)
			{
				uAponlineMemItem.unmarshal(up);
			}
		};

		// AP上报全量在线状态
		struct PSS_ApOnlineCompare : public sox::Marshallable
		{
			enum {uri = (7 << 8) | protocol::APONLINE_SVID};

			int64_t  uApId;      // 所在AP的ID

			uint64_t uTimeStamp;  
			uint8_t uAPType;    // 区分AP/linkd等不同前端			

			std::vector<ApOnlineCompareItem> vecUsers;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uApId << uTimeStamp;
				sox::marshal_container(pk, vecUsers);
				pk << uAPType;
			}

			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> uApId >> uTimeStamp;
				sox::unmarshal_containerEx(up, vecUsers);
				if (up.size() > 0)
					up >> uAPType;
			}
		};

		struct PSS_ReportApOnlineAck : public sox::Marshallable
		{
			enum {uri = (8 << 8) | protocol::APONLINE_SVID};
			uint32_t uSeqId;
			// 账号
			uint32_t uAppId;
			uint64_t uAccount;

			int64_t  uApId;      // AP s2s server ID
			uint32_t uApDMSvrID; // AP daemon server ID
			uint32_t uTerm;      // 终端类型, PC/Mobile/Pad
			uint32_t uProtocol;  // TCP、UDP,etc
			uint64_t uTimestamp; // 时间戳
			uint8_t  uStatus;    // 1: 上线 0: 下线

			virtual void marshal(sox::Pack &p) const
			{
				//sox::PackX p(&p_);
				p << uSeqId << uAppId << uAccount << uApId << uApDMSvrID;
				p << uTerm << uProtocol << uTimestamp << uStatus;
				//p.batch_prim_push(uSeqId, uAppId, uAccount, uint64_t(uApId), uApDMSvrID, uTerm, uProtocol, uTimestamp, uStatus);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				//sox::UnpackX p(&p_);
				p >> uSeqId >> uAppId >> uAccount >> uApId >> uApDMSvrID;
				p >> uTerm >> uProtocol >> uTimestamp >> uStatus;
				//uint64_t uApIDTmp;
				//p.batch_prim_pop(uSeqId, uAppId, uAccount, uApIDTmp, uApDMSvrID, uTerm, uProtocol, uTimestamp, uStatus);
				//uApId = (int64_t)uApIDTmp;
			}
		};

		//aponline重建在线通知同机的订阅进程
		struct PSS_OnlineRecoverNotify : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::APONLINE_SVID};

			bool bRecover;
			uint32_t uGroupId;
			
			virtual void marshal(sox::Pack& pk) const
			{
				pk.push_uint8(bRecover);
				pk << uGroupId;
			}
			
			virtual void unmarshal(const sox::Unpack &up)
			{
				bRecover = (bool)up.pop_uint8();
				up >> uGroupId;
			}
		};
		
		struct PSS_RegisterSrvPing : public sox::Marshallable
		{
			enum {uri = (10 << 8) | protocol::APONLINE_SVID};

			int64_t uSrvId;   //serverId
			uint32_t uAppId;

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uSrvId << uAppId;
			}

			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> uSrvId >> uAppId;
			}
		};


		struct PSS_RegisterSrvPingRes : public sox::Marshallable
		{
			enum {uri = (11 << 8) | protocol::APONLINE_SVID};

			bool isReady;  //是否准备好
			bool isReg;
			int64_t uSrvId; 

			virtual void marshal(sox::Pack &pk) const
			{
				pk.push_uint8(isReady);
				pk.push_uint8(isReg);
				pk << uSrvId;
			}
			
			virtual void unmarshal(const sox::Unpack &up)
			{
				isReady = (bool)up.pop_uint8();
				isReg = (bool)up.pop_uint8();
				up >> uSrvId;
			}
		};
		
		struct PSS_CheckOnlineUser : public sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::APONLINE_SVID};

			uint64_t uSeq;       // 序号，用于找回上下文
			uint32_t uAppId;
			std::vector<uint64_t> uAccouts;    //在线

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uSeq << uAppId;
				marshal_container(pk, uAccouts);
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uSeq >> uAppId;
				unmarshal_container(up, std::back_inserter(uAccouts));
			}
		};

		struct PSS_CheckOnlineUserRes : public sox::Marshallable
		{
			enum {uri = (13 << 8) | protocol::APONLINE_SVID};

			uint64_t uSeq;       // 序号，用于找回上下文
			uint32_t uAppId;
			std::vector<uint64_t> uAccoutOnline;    //在线
			std::vector<uint64_t> uAccountOffline;  //离线

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uSeq << uAppId;
				marshal_container(pk, uAccoutOnline);
				marshal_container(pk, uAccountOffline);
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uSeq >> uAppId;
				unmarshal_container(up, std::back_inserter(uAccoutOnline));
				unmarshal_container(up, std::back_inserter(uAccountOffline));
			}
		};	
			
    struct PSS_ReportApOnlineReq : public sox::Marshallable
    {
			enum {uri = (14 << 8) | protocol::APONLINE_SVID};
			PSS_ReportApOnlineReq(){}
			PSS_ReportApOnlineReq(const uint32_t seqId): uSeqId(seqId)
			{}
			
			uint32_t uSeqId;  //请求的SeqId^M
			virtual void marshal(sox::Pack &p) const
			{
				p << uSeqId;
		    }
					
		    virtual void unmarshal(const sox::Unpack &p)
		    {
		    	p >> uSeqId;
			}	
		};

		struct PSS_ReportApOnlineCompactSeq : public sox::Marshallable
		{
			enum {uri = (15 << 8) | protocol::APONLINE_SVID};

			PSS_ReportApOnlineCompactSeq():m_uSeqId(0)
			{}

			PSS_ReportApOnlineCompactSeq(const uint32_t uSeqId, const std::string &strMsg):m_uSeqId(uSeqId), m_strPSS_ReportApOnline(strMsg)
			{}

			uint32_t m_uSeqId;
			std::string m_strPSS_ReportApOnline;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSeqId << m_strPSS_ReportApOnline;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSeqId >> m_strPSS_ReportApOnline;
			}
		};

	
		struct PSS_BatchGetApOnline : public sox::Marshallable
		{
			enum {uri = (16 << 8) | protocol::APONLINE_SVID};

			std::vector<std::string> m_vecStrGetApOnline; //vector of marshalled PSS_GetApOnline

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecStrGetApOnline);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecStrGetApOnline));
			}
		};

		struct PSS_BatchGetApOnlineRes : public sox::Marshallable
		{
			enum {uri = (17 << 8) | protocol::APONLINE_SVID};

			std::vector<std::string> m_vecStrGetApOnlineRes; //vector of marshalled PSS_GetApOnlineRes

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecStrGetApOnlineRes);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecStrGetApOnlineRes));
			}
		};

		struct PSS_CheckUserStatus : public sox::Marshallable
		{
			enum {uri = (18 << 8) | protocol::APONLINE_SVID};

			uint32_t uAppId;
			uint64_t uAccount;
			uint32_t uTerm;
			uint32_t uProtocol;
			std::vector<uint32_t> uLinkds;
			std::vector<uint64_t> uApIds;

			virtual void marshal(sox::Pack &p) const
			{
				p << uAppId << uAccount << uTerm << uProtocol;
				marshal_container(p, uLinkds);
				marshal_container(p, uApIds);
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uAppId >> uAccount >> uTerm >> uProtocol;
				unmarshal_container(up, std::back_inserter(uLinkds));
				unmarshal_container(up, std::back_inserter(uApIds));
			}
		};

		struct PSS_ApCheckStatus : public sox::Marshallable
		{
			enum {uri = (19 << 8) | protocol::APONLINE_SVID};

			uint32_t uAppId;
			uint64_t uAccount;
			uint32_t uTerm;
			uint32_t uProtocol;

			virtual void marshal(sox::Pack &p) const
			{
				p << uAppId << uAccount << uTerm << uProtocol ;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> uAppId >> uAccount >> uTerm >> uProtocol ;
			}
		};

		struct PSS_APOnlineRecover : public sox::VSMarshallable
		{
			enum {uri = (3022 << 8) | protocol::LINKD_SVID};

			std::set<uint32_t> m_setAppID; //empty for all
			std::string strContext;

			virtual void vsmarshal(sox::Pack &p) const
			{
				p << m_setAppID;
				p << strContext;
			}

			virtual void vsunmarshal(const sox::Unpack &p)
			{
				size_t vendsizetmp = m_vendpos;
				p >> m_setAppID;
				if(p.size() > vendsizetmp)
				{
					p >> strContext;
				}
			}
		};

    //
    struct ApOnlineCompareItem2 : public sox::Marshallable
    {
      ApOnlineCompareItem2()
        : uAppId(0), uAccount(0), uTerm(0), uProtocol(0), uTimestamp(0), uCliIP(0)
      {
    
      }
    
      uint32_t uAppId;     // 订阅的何种业务
      uint64_t uAccount;   // 与uAppId联合作为账号体系
    
      uint32_t uTerm;      // 终端类型, PC/Mobile/Pad
      uint32_t uProtocol;  // TCP、UDP,etc
      uint64_t uTimestamp; // 时间戳
      uint32_t uCliIP;   // 客户端ip
      std::string strPCInfo; // pc info
      std::string strClientFrom; // client From
    
      void marshal(sox::Pack &p) const
      {
        p << uAppId << uAccount << uTerm << uProtocol << uTimestamp << uCliIP;
        p << strPCInfo << strClientFrom;
      }
    
      void unmarshal(const sox::Unpack& up)
      {
        up >> uAppId >> uAccount >> uTerm >> uProtocol >> uTimestamp >> uCliIP;
        up >> strPCInfo >> strClientFrom;
      }
    };

    
    // AP上报全量在线状态
    struct PSS_ApOnlineCompare2 : public sox::Marshallable
    {
      enum {uri = (20 << 8) | protocol::APONLINE_SVID};
    
      int64_t  uApId;      // 所在AP的ID
      uint64_t uTimeStamp;  
      std::vector<ApOnlineCompareItem2> vecUsers;
      uint8_t uAPType;    // 区分AP/linkd等不同前端
    
      virtual void marshal(sox::Pack &pk) const
      {
        pk << uApId << uTimeStamp;
        sox::marshal_container(pk, vecUsers);
        pk << uAPType;
      }
    
      virtual void unmarshal(const sox::Unpack &up) 
      {
        up >> uApId >> uTimeStamp;
        sox::unmarshal_containerEx(up, vecUsers);
        up >> uAPType;
      }
    };

		struct ApOnlineCompareItem3 : public sox::VSMarshallable
		{
			ApOnlineCompareItem3()
				: uAppId(0), uAccount(0), uAppKey(0), uTerminal(0), uTerm(0), uProtocol(0), uTimestamp(0), uTicketTimestamp(0)
			{

			}

			uint32_t uAppId;     // 订阅的何种业务
			uint64_t uAccount;   // 与uAppId联合作为账号体系
			uint64_t uAppKey;
			uint64_t uTerminal; //新terminal type

			uint32_t uTerm;      // 终端类型, PC/Mobile/Pad
			uint32_t uProtocol;  // TCP、UDP,etc
			uint64_t uTimestamp; // 时间戳
			uint32_t uCliIP;   // 客户端ip
			std::string strPCInfo; // pc info
			std::string strClientFrom; // client From
			std::string strClientVer;
			uint32_t uTicketTimestamp;
			uint16_t uCliPort;   // 客户端端口
			std::string strInstance; // 客户端实例

			void vsmarshal(sox::Pack &p) const
			{
				p << uAppId << uAccount << uTerm << uProtocol << uTimestamp << uCliIP;
				p << strPCInfo << strClientFrom << strClientVer;
				p << uAppKey << uTerminal;
				p << uTicketTimestamp;
				p << uCliPort;
				p << strInstance;
			}

			void vsunmarshal(const sox::Unpack& up)
			{
			  uint32_t venderSizeTemp = m_vendpos;
				up >> uAppId >> uAccount >> uTerm >> uProtocol >> uTimestamp >> uCliIP;
				up >> strPCInfo >> strClientFrom;
				up >> strClientVer;
				if(up.size() >= venderSizeTemp + sizeof(uAppKey) + sizeof(uTerminal))
				{
					up >> uAppKey >> uTerminal;
				}
				if(up.size() >= venderSizeTemp + sizeof(uTicketTimestamp))
				{
					up >> uTicketTimestamp;
				}
				if (up.size() > venderSizeTemp + sizeof(uCliPort))
				{
					up >> uCliPort;
				}
				if (up.size() > venderSizeTemp)
				{
					up >> strInstance;
				}
			}
		};

		// AP上报全量在线状态
		struct PSS_ApOnlineCompare3 : public sox::Marshallable
		{
			enum {uri = (23 << 8) | protocol::APONLINE_SVID};

			int64_t  uApId;      // 所在AP的ID
			uint64_t uTimeStamp;  
			std::vector<ApOnlineCompareItem3> vecUsers;
			uint8_t uAPType;    // 区分AP/linkd等不同前端
			std::string strContext;    // 回复PSS_APOnlineRecover等消息的上下文

			virtual void marshal(sox::Pack &pk) const
			{
				pk << uApId << uTimeStamp;
				sox::marshal_container(pk, vecUsers);
				pk << uAPType;
				pk << strContext;
			}

			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> uApId >> uTimeStamp;
				sox::unmarshal_containerEx(up, vecUsers);
				up >> uAPType;
				if(!up.empty())
				{
          up >> strContext;
				}
			}
		};
	  
    //Online
    struct PSS_APForceOut : public sox::VSMarshallable
    {
      enum {uri = (3020 << 8) | protocol::LINKD_SVID};

      typedef std::map<uint32_t, std::vector<uint64_t> > MapAppUID_t;

      MapAppUID_t m_mapAppIDUID;//AppID + UID list
      std::string m_strReason; //封禁信息等
      uint32_t m_uReason;
      std::string m_strContext;

      virtual void vsmarshal(sox::Pack &p) const
      {
        p << m_mapAppIDUID << m_strReason << m_uReason << m_strContext;
      }

      virtual void vsunmarshal(const sox::Unpack &p)
      {
        size_t vendsizetmp = m_vendpos;
        p >> m_mapAppIDUID >> m_strReason >> m_uReason;
        if(p.size() > vendsizetmp)
        {
          p >> m_strContext;
        }
      }
    };

    //Online
    struct PSS_APForceOutRes : public sox::VSMarshallable
    {
      enum {uri = (3021 << 8) | protocol::LINKD_SVID};

      typedef std::map<uint32_t, std::vector<uint64_t> > MapAppUID_t;

      MapAppUID_t m_mapAppIDUID;// 踢失败的用户
      std::string m_strContext;
      uint32_t m_uTimeStamp;

      virtual void vsmarshal(sox::Pack &p) const
      {
        p << m_mapAppIDUID << m_strContext << m_uTimeStamp;
      }

      virtual void vsunmarshal(const sox::Unpack &p)
      {
        p >> m_mapAppIDUID >> m_strContext >> m_uTimeStamp;
      }
    };
    

	}
}

