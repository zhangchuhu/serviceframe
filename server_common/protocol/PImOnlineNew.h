#ifndef PROTOCOL_IM_ONLINE_H_INCLUDE
#define PROTOCOL_IM_ONLINE_H_INCLUDE

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"

#include <sys/time.h>
#include <vector>
#include "common/protocol/Im_Const.h"

namespace server
{
	namespace imonline
	{
		// �û�������Ϣ
		struct CImUserStatusCommon : public sox::Marshallable
		{
			CImUserStatusCommon() 
				: m_uUid(0), m_uLinkdId(-1), m_status(protocol::IMOFFLINE){}
			CImUserStatusCommon(uint32_t id, uint32_t linkid, protocol::E_IMSTATUS st) 
				: m_uUid(id), m_uLinkdId(linkid), m_status(st) {}

			uint32_t m_uUid;
			uint32_t m_uLinkdId;
			protocol::E_IMSTATUS m_status;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uLinkdId;
				p.push_uint8(m_status);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uLinkdId;
				m_status = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		// �û�������Ϣ
		struct CUserOnlineInfoNew : public CImUserStatusCommon
		{
			uint32_t m_uOnlineIp;            // �����״̬�ı��imonline����ip
			uint32_t m_uVersion;             // �汾�ţ�����
			struct timeval m_timesmapt;

			CUserOnlineInfoNew() : m_uOnlineIp(0), m_uVersion(0) 
			{
				m_timesmapt.tv_sec = m_timesmapt.tv_usec = 0;
			}
			CUserOnlineInfoNew(uint32_t uUid) : m_uOnlineIp(0), m_uVersion(0) 
			{
				m_uUid = uUid;
				m_timesmapt.tv_sec = m_timesmapt.tv_usec = 0;
			}
			CUserOnlineInfoNew(uint32_t uid, uint32_t linkd, protocol::E_IMSTATUS stat, uint32_t onlineIp, uint32_t uVersion) 
				: CImUserStatusCommon(uid, linkd, stat), m_uOnlineIp(onlineIp), m_uVersion(uVersion)
			{
				m_timesmapt.tv_sec = m_timesmapt.tv_usec = 0;
			}
			CUserOnlineInfoNew(uint32_t uid, uint32_t linkd, protocol::E_IMSTATUS stat, uint32_t onlineIp, uint32_t uVersion, timeval timesmapt)
				: CImUserStatusCommon(uid, linkd, stat), m_uOnlineIp(onlineIp), m_uVersion(uVersion)
			{
				m_timesmapt = timesmapt;
			}

			bool Valid() const          { return m_uLinkdId != (uint32_t)-1;    }
			bool IsOffline() const      { return m_status == protocol::IMOFFLINE; }
			bool IsNotOffline() const   { return m_status != protocol::IMOFFLINE; } 
			bool IsHide() const         { return m_status == protocol::IMHIDE;    }
			bool IsOnline() const       { return m_status == protocol::IMONLINE;  }
			bool LaterThan(timeval t) const     
			{
				if (m_timesmapt.tv_sec > t.tv_sec || (m_timesmapt.tv_sec == t.tv_sec && m_timesmapt.tv_usec > t.tv_usec))
					return true;

				return false;
			}

			bool SameOnline(uint32_t onlineSrvIp)     const { return m_uOnlineIp == onlineSrvIp; }
			bool SameLinkd(uint32_t linkdSrvId)       const { return m_uLinkdId == linkdSrvId;   }

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uLinkdId  << m_uOnlineIp << m_uVersion 
					<< (uint32_t)m_timesmapt.tv_sec 
					<< (uint32_t)m_timesmapt.tv_usec;
				p.push_uint8(m_status);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uLinkdId >> m_uOnlineIp >> m_uVersion;
				m_timesmapt.tv_sec  = p.pop_uint32();
				m_timesmapt.tv_usec = p.pop_uint32();
				m_status = (protocol::E_IMSTATUS)p.pop_uint8();
			}

		};

		// ����ı�����״̬

		struct PSS_ChangeStatusReq : public CImUserStatusCommon
		{
			enum {uri = (1 << 8 | protocol::IMONLINED_SVID)};

			uint32_t m_uVersion;

			PSS_ChangeStatusReq() : m_uVersion(0) {}
			PSS_ChangeStatusReq(uint32_t id, uint32_t linkid, protocol::E_IMSTATUS st, uint32_t uVersion) 
				: CImUserStatusCommon(id, linkid, st), m_uVersion(uVersion) 
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				CImUserStatusCommon::marshal(p);
				p << m_uVersion;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				CImUserStatusCommon::unmarshal(p);
				if (p.size() != 0)
				{
					try
					{
						p >> m_uVersion;
					}
					catch(...)
					{
						m_uVersion = 0;
					}
				}
				else
				{
					m_uVersion = 0;
				}
			}
		};

		// �û��ı�����״̬�������Ӧ

		struct PSS_ChangeStatusRes : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::IMONLINED_SVID};

			PSS_ChangeStatusRes()	{}
			PSS_ChangeStatusRes(uint32_t uid, protocol::E_IMSTATUS status) : m_uUid(uid), m_status(status) {}

			uint32_t m_uUid;
			protocol::E_IMSTATUS m_status;

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uUid;
				pk.push_uint16(m_status);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uUid;
				m_status = (protocol::E_IMSTATUS)up.pop_uint16();
			}
		};

		// imlinkd �������Լ���½���û�������״̬

		struct CImUserStatus : public sox::Marshallable
		{
			uint32_t m_uUid;
			protocol::E_IMSTATUS m_status;
			uint32_t m_uVersion;

			CImUserStatus() : m_uUid(0), m_status(protocol::IMOFFLINE), m_uVersion(0) {}
			CImUserStatus(uint32_t uUid, protocol::E_IMSTATUS stat, uint32_t uVersion) : m_uUid(uUid), m_status(stat), m_uVersion(uVersion) {}

			virtual void marshal(sox::Pack &pk) const 
			{
				pk << m_uUid << m_uVersion;
				pk.push_uint8(m_status);
			}
			virtual void unmarshal(const sox::Unpack &up) 
			{
				up >> m_uUid >> m_uVersion;
				m_status = (protocol::E_IMSTATUS)up.pop_uint8();
			}
		};

		struct PSS_RecoverUserStatusRes : public sox::Marshallable
		{
			enum {uri = (2 << 8) | protocol::IMONLINED_SVID};
			PSS_RecoverUserStatusRes(){}

			uint32_t  m_uLinkdId;
			std::vector<CImUserStatus>  m_vecStatus;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uLinkdId;
				marshal_container(p, m_vecStatus);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uLinkdId;
				sox::unmarshal_container(p, std::back_inserter(m_vecStatus));
			}
		};

		// �����½imonline

		struct PSS_UserLoginReq : public CImUserStatusCommon
		{
			// �ɵ�½����Э��ʹ�õģ����𣿣�
			// enum {uri = (4 << 8 | protocol::IMONLINED_SVID)};
			// imonline2ʹ�õ��� 
			// enum {uri = (7 << 8 | protocol::IMONLINE2_SVID)};
			enum {uri = (20 << 8 | protocol::IMONLINED_SVID)};

			uint32_t m_uVersion;

			PSS_UserLoginReq() : m_uVersion(0) {}
			PSS_UserLoginReq(uint32_t id, uint32_t linkid, protocol::E_IMSTATUS estat, uint32_t uVersion) 
				: CImUserStatusCommon(id, linkid, estat), m_uVersion(uVersion)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				CImUserStatusCommon::marshal(p);
				p << m_uVersion;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				CImUserStatusCommon::unmarshal(p);
				if (p.size() != 0)
				{
					try
					{
						p >> m_uVersion;
					}
					catch(...)
					{
						m_uVersion = 0;
					}
				}
				else
				{
					m_uVersion = 0;
				}
			}
		};

		// �ɹ���½imonline ��Ӧ

		struct PSS_UserLoginRes : public sox::Marshallable
		{
			enum {uri = (5 << 8 | protocol::IMONLINED_SVID)};

			PSS_UserLoginRes(){}
			PSS_UserLoginRes(uint32_t id) : m_uUid(id)
			{
			}

			uint32_t m_uUid;
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid ;
			}
		};

		// imlinkd�������Լ���½���û�������onlineindex���� linkd-uid ����
		
		struct PSS_LinkdUserRes : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::IMONLINED_SVID};
			PSS_LinkdUserRes(){}
			PSS_LinkdUserRes(uint32_t uLinkd, const std::vector<uint32_t>& vecUid)
				: m_uLinkdId(uLinkd), m_vecUid(vecUid)
			{

			}

			uint32_t m_uLinkdId;
			std::vector<uint32_t> m_vecUid;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uLinkdId;
				marshal_container(p, m_vecUid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uLinkdId;
				sox::unmarshal_container(p, std::back_inserter(m_vecUid));
			}
		};

		// online-index ֪ͨimonline ɾ��imlinkd

		struct PSS_NotifyRemoveImlinkd : sox::Marshallable
		{
			enum {uri = (7 << 8) | protocol::IMONLINED_SVID};
			
			PSS_NotifyRemoveImlinkd(EImFrontEndNo front = ENUM_FRONT_TCP) : m_emFrontEnd(front) {}
			PSS_NotifyRemoveImlinkd(EImFrontEndNo front, uint32_t linkd, uint32_t netType, const std::set<uint32_t>& setV)
				: m_emFrontEnd(front), m_uLinkdId(linkd), m_netType(netType), m_vecUids(setV.begin(), setV.end())
			{}

			EImFrontEndNo  m_emFrontEnd;
			uint32_t m_uLinkdId;
			uint32_t m_netType;                          // imlinkd ���������ͣ���imtransfer�㲥ʱҪ�õ�

			std::vector<uint32_t> m_vecUids;
            
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uLinkdId << m_netType;
				p.push_uint8(m_emFrontEnd);
				sox::marshal_container(p, m_vecUids);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uLinkdId >> m_netType;
				m_emFrontEnd = (EImFrontEndNo)p.pop_uint8();
				sox::unmarshal_container(p, std::back_inserter(m_vecUids));
			}
		};


		// imonline �� imlinkd check �û���״̬

		struct PSS_UserStatusCheck : public sox::Marshallable
		{
			enum {uri = (9 << 8) | protocol::IMONLINED_SVID};

			PSS_UserStatusCheck() {}
			PSS_UserStatusCheck(uint32_t uUid, uint32_t uUri) : m_uUid(uUid), m_uUri(uUri) {}

			uint32_t m_uUid;
			uint32_t m_uUri;       // ԭ����uri

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uUid << m_uUri;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uUri;
			}
		};

		// check ����Ӧ

		struct PSS_UserStatusCheckRes : public CImUserStatusCommon
		{
			enum {uri = (10 << 8 | protocol::IMONLINED_SVID)};

			PSS_UserStatusCheckRes(){}
			PSS_UserStatusCheckRes(uint32_t id, uint32_t linkid, protocol::E_IMSTATUS estat, uint32_t uUri, uint32_t uVersion) 
				: CImUserStatusCommon(id, linkid, estat), m_uUri(uUri), m_uVersion(uVersion), m_emFront(ENUM_FRONT_TCP)
			{
			}

			uint32_t m_uUri;
			uint32_t m_uVersion;
			EImFrontEndNo m_emFront;


			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUri << m_uVersion;
				p.push_uint8(m_emFront);
				CImUserStatusCommon::marshal(p);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUri >> m_uVersion;
				m_emFront = (EImFrontEndNo)p.pop_uint8();
				CImUserStatusCommon::unmarshal(p);
			}
		};

		// ���������imonline �㲥����״̬�ı��

		struct PSS_NotifyOtherOnline : sox::Marshallable
		{
			enum {uri = (11 << 8 | protocol::IMONLINED_SVID)};

			EImFrontEndNo   m_emFront;
			uint32_t        m_uFromOnlineGrp;
			std::vector<CUserOnlineInfoNew> m_vecCStatus;

			PSS_NotifyOtherOnline(EImFrontEndNo emFront = ENUM_FRONT_TCP) : m_emFront(emFront) {}
			PSS_NotifyOtherOnline(EImFrontEndNo emFront, uint32_t ufrom, const CUserOnlineInfoNew& st) 
				: m_emFront(emFront), m_uFromOnlineGrp(ufrom), m_vecCStatus(1, st) 
			{

			}
			PSS_NotifyOtherOnline(EImFrontEndNo emFront, uint32_t ufrom, const std::vector<CUserOnlineInfoNew>& vecU) 
				: m_emFront(emFront), m_uFromOnlineGrp(ufrom), m_vecCStatus(vecU) 
			{

			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uFromOnlineGrp;
				p.push_uint8(m_emFront);
				marshal_container(p, m_vecCStatus);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uFromOnlineGrp;
				m_emFront = (EImFrontEndNo)p.pop_uint8();
				unmarshal_container(p, back_inserter(m_vecCStatus));
			}
		};

		// imlinkd ��ʼ��ʱ�����������Լ���½���û�������״̬��hash���

		struct PSS_StatusFromImlinkd : sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::IMONLINED_SVID};
			PSS_StatusFromImlinkd(){}
			PSS_StatusFromImlinkd(uint32_t uLinkd, const std::vector<CImUserStatus>& vec)
				: m_uLinkdId(uLinkd), m_vecStatus(vec)
			{}

			uint32_t  m_uLinkdId;
			std::vector<CImUserStatus>  m_vecStatus;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uLinkdId;
				marshal_container(p, m_vecStatus);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uLinkdId;
				sox::unmarshal_container(p, std::back_inserter(m_vecStatus));
			}
		};

		// recover ʧ��ʱ��Ҫ��imlinkd�ȷ�С���ٷ����

		struct PSS_Forerunner : public sox::Marshallable
		{
			enum {uri = (23 << 8) | protocol::IMONLINED_SVID};

			PSS_Forerunner(EImFrontEndNo emFront = ENUM_FRONT_TCP) : m_emFrontNo(emFront), m_uLinkdId(-1) {}
			PSS_Forerunner(EImFrontEndNo emFront, uint32_t uLinkdId) : m_emFrontNo(emFront), m_uLinkdId(uLinkdId) {}

			EImFrontEndNo m_emFrontNo;
			uint32_t      m_uLinkdId;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uLinkdId;
				p.push_uint8(m_emFrontNo);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uLinkdId;
				m_emFrontNo = (EImFrontEndNo)p.pop_uint8();
			}
		};

		struct PSS_ExceptRecovrReq : public sox::Marshallable
		{
			enum {uri = (23 << 8) | protocol::IMLINKD_SVID};
			PSS_ExceptRecovrReq(EImFrontEndNo emFront = ENUM_FRONT_TCP) : m_emFrontNo(emFront) {}

			EImFrontEndNo m_emFrontNo;
	
			virtual void marshal(sox::Pack &p) const 
			{
				p.push_uint8(m_emFrontNo);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				m_emFrontNo = (EImFrontEndNo)p.pop_uint8();
			}
		};

		// imlinkd ����imonline�Ĳ�����

		struct CMakeUpPacketFlags : public sox::Marshallable
		{
			CMakeUpPacketFlags() : m_uFlags(0) {}
			CMakeUpPacketFlags(uint32_t uFlags) : m_uFlags(uFlags) {}

			uint32_t    m_uFlags;           // Ĭ��ֵ��  ��Ҫ���Ͳ�������client           true
											//           ����Ҫ��imtransfer�㲥״̬���    0

			// ǰ�˱��
			void SetFrontEnd(EImFrontEndNo emFront)
			{
				m_uFlags &= 0xffffff00;
				m_uFlags |= ((uint32_t)emFront & 0xff);
			}

			EImFrontEndNo GetFrontEnd() const
			{
				return (EImFrontEndNo)(m_uFlags & 0xff);
			}

			// �Ƿ���Ҫ�Ѳ����������ͻ���
			// Ĭ����Ҫ����
			void SetNotRepairFlag(bool bFlag)
			{
				m_uFlags &= 0xfffff0ff;
				m_uFlags |= (((uint32_t)bFlag << 8) & 0xf00);
			}

			bool GetNotRepairFlag() const
			{
				return (bool)((m_uFlags >> 8) & 0xf);
			}

			// �Ƿ���Ҫ���޸���״̬��imtransfer�㲥
			// Ĭ�ϲ��㲥   0
			void SetBroadcastFlag(uint32_t uFlag)
			{
				m_uFlags &= 0xffff0fff;
				m_uFlags |= ((uFlag << 12) & 0xf000);
			}

			uint32_t GetBroadcastFlag() const
			{
				return ((m_uFlags >> 12) & 0xf);
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uFlags;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uFlags;
			}
		};

		struct PSS_MakeUpPacket : public CMakeUpPacketFlags
		{
			enum {uri = (24 << 8) | protocol::IMONLINED_SVID};

			PSS_MakeUpPacket() : m_uUid (INVALID_UID), m_ufromLinkd(INVALID_IMLINKD) {}
			PSS_MakeUpPacket(EImFrontEndNo emFront, uint32_t uUid, uint32_t uLinkd, uint32_t uUri, const std::string& str)
				: m_uUid(uUid), m_ufromLinkd(uLinkd), m_oriUri(uUri), m_oriPacket(str)
			{
				SetFrontEnd(emFront);
			}

			uint32_t    m_uUid;
			uint32_t    m_ufromLinkd;
			uint32_t    m_oriUri;
			std::string m_oriPacket;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uUid << m_ufromLinkd << m_oriUri << m_oriPacket;
				CMakeUpPacketFlags::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_ufromLinkd >> m_oriUri >> m_oriPacket;
				CMakeUpPacketFlags::unmarshal(p);
			}
		};

		// imlinkdȷ���û�״̬���imonline2�Ļ�Ӧ

		struct PSS_MakeUpResFromLinkd : public CMakeUpPacketFlags
		{
			enum {uri = (25 << 8) | protocol::IMONLINED_SVID};
			PSS_MakeUpResFromLinkd() 
				: m_uUid(INVALID_UID), m_uOldLinkdId(INVALID_IMLINKD), m_uLinkdId(INVALID_IMLINKD), m_uVersion(0), m_emStatus(protocol::IMOFFLINE), m_emFront(ENUM_FRONT_TCP)  {}
			PSS_MakeUpResFromLinkd(uint32_t uNewLinkd, protocol::E_IMSTATUS emStatus, uint32_t uVersion, const PSS_MakeUpPacket& tmp)
				: m_uUid(tmp.m_uUid), m_uOldLinkdId(tmp.m_ufromLinkd), m_uLinkdId(uNewLinkd), m_uVersion(uVersion), m_emStatus(emStatus)
			{
				m_emFront = tmp.GetFrontEnd();
				m_uFlags = tmp.m_uFlags;
			}

			uint32_t   m_uUid;
			uint32_t   m_uOldLinkdId;
			uint32_t   m_uLinkdId;
			uint32_t   m_uVersion;
			protocol::E_IMSTATUS m_emStatus;
			EImFrontEndNo        m_emFront;	    // �����������⣬��ʱ�������ֶΣ���ʵ����ͨ��m_flagsȡ
			
			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uUid << m_uOldLinkdId << m_uLinkdId << m_uVersion;
				p.push_uint8(m_emStatus);
				p.push_uint8(m_emFront);
				CMakeUpPacketFlags::marshal(p);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uUid >> m_uOldLinkdId >> m_uLinkdId >> m_uVersion;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
				m_emFront  = (EImFrontEndNo)p.pop_uint8();
				if (p.size() > 0)
				{
					try
					{
						CMakeUpPacketFlags::unmarshal(p);
					}
					catch (...)
					{
						m_uFlags = 0;
					}
				}
				else
				{
					m_uFlags = 0;
				}
			}
		};

		
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		//���ӵ�����udpǰ�˵������Ϣ

		struct SStatusChangeCommon : public sox::Marshallable
		{
			uint32_t m_uUid;
			uint32_t m_uImLinkdId;
			protocol::E_IMSTATUS m_emStatus;
			EImFrontEndNo m_emRouteNumber;

			SStatusChangeCommon() : m_uUid(0), m_uImLinkdId(-1), m_emStatus(protocol::IMOFFLINE), m_emRouteNumber(ENUM_FRONT_TCP) {}
			SStatusChangeCommon(uint32_t uUid, uint32_t uImLinkdId, protocol::E_IMSTATUS emStatus, EImFrontEndNo emRouteNumber) : 
			m_uUid(uUid), m_uImLinkdId(uImLinkdId), m_emStatus(emStatus), m_emRouteNumber(emRouteNumber) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uImLinkdId;
				p.push_uint8(m_emStatus);
				p.push_uint8(m_emRouteNumber);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uImLinkdId;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
				m_emRouteNumber = (EImFrontEndNo)p.pop_uint8();
			}
		};

		//��¼
		struct PSS_LoginImOnlineCommon : public SStatusChangeCommon
		{
			enum {uri = (32 << 8) | protocol::IMONLINED_SVID};

			PSS_LoginImOnlineCommon() {}
			PSS_LoginImOnlineCommon(uint32_t uUid, uint32_t uImLinkdId, protocol::E_IMSTATUS emStatus, EImFrontEndNo emRouteNumber) : 
			SStatusChangeCommon(uUid, uImLinkdId, emStatus, emRouteNumber) {}

			virtual void marshal(sox::Pack &p) const
			{
				SStatusChangeCommon::marshal(p);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				SStatusChangeCommon::unmarshal(p);
			}
		};

		//��¼��Ӧ
		struct PSS_LoginImOnlineCommonRes : public sox::Marshallable
		{
			enum {uri = (33 << 8) | protocol::IMONLINED_SVID};

			uint32_t m_uUid;

			PSS_LoginImOnlineCommonRes() {}
			PSS_LoginImOnlineCommonRes(uint32_t uUid) : m_uUid(uUid) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
			}
		};

		//״̬�ı�
		struct PSS_StatusChangeCommon : public SStatusChangeCommon
		{
			enum {uri = (34 << 8) | protocol::IMONLINED_SVID};

			PSS_StatusChangeCommon() {}
			PSS_StatusChangeCommon(uint32_t uUid, uint32_t uImLinkdId, protocol::E_IMSTATUS emStatus, EImFrontEndNo emRouteNumber) : 
			SStatusChangeCommon(uUid, uImLinkdId, emStatus, emRouteNumber) {}

			virtual void marshal(sox::Pack &p) const
			{
				SStatusChangeCommon::marshal(p);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				SStatusChangeCommon::unmarshal(p);
			}
		};

		//״̬�ı��Ӧ
		struct PSS_StatusChangeCommonRes : public sox::Marshallable
		{
			enum {uri = (35 << 8) | protocol::IMONLINED_SVID};

			uint32_t m_uUid;

			PSS_StatusChangeCommonRes() {}
			PSS_StatusChangeCommonRes(uint32_t uUid) : m_uUid(uUid) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid;
			}
		};

		//����ָ�UDP�û��б�
		struct PSS_RecoverUdpUserStatus : public sox::Marshallable
		{
			enum {uri = (36 << 8) | protocol::IMONLINED_SVID};

			virtual void marshal(sox::Pack &p) const
			{
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
			}
		};

		//�ָ�UDP�û��б��Ӧ
		struct PSS_RecoverUdpUserStatusRes : public sox::Marshallable
		{
			enum {uri = (37 << 8) | protocol::IMONLINED_SVID};

			uint32_t m_uImlinkdId;
			//std::map<uint32_t, std::vector<uint32_t> > m_mapUid;
			std::map<EImFrontEndNo, std::vector<uint32_t> > m_mapUid;
			bool m_bPush; //�Ƿ������ƣ�Ĭ��false����ʾ��imonline����Ҫ��

			PSS_RecoverUdpUserStatusRes() : m_bPush(false) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uImlinkdId;
				p.push_uint8(m_bPush);
				//sox::marshal_container(p, m_mapUid);

				p.push_uint32(m_mapUid.size());
				for (std::map<EImFrontEndNo, std::vector<uint32_t> >::const_iterator iter = m_mapUid.begin(); iter != m_mapUid.end(); iter ++)
				{
					p.push_uint8(iter->first);
					sox::marshal_container(p, iter->second);
				}
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uImlinkdId;
				m_bPush = (bool)p.pop_uint8();
				//sox::unmarshal_container(p, std::inserter(m_mapUid, m_mapUid.begin()));

				uint32_t uMapSize = p.pop_uint32();
				for (uint32_t i = 0; i < uMapSize; i ++)
				{
					EImFrontEndNo emRouteNumber = (EImFrontEndNo)p.pop_uint8();
					std::vector<uint32_t> vecUid;
					sox::unmarshal_container(p, std::back_inserter(vecUid));

					m_mapUid.insert( std::make_pair(emRouteNumber, vecUid) );
				}
			}
		};

		struct SStatusChangeCommonNoImLink : public sox::Marshallable
		{
			uint32_t m_uUid;
			//uint32_t m_uImLinkdId;
			protocol::E_IMSTATUS m_emStatus;
			EImFrontEndNo m_emRouteNumber;

			SStatusChangeCommonNoImLink() {}
			SStatusChangeCommonNoImLink(uint32_t uUid, protocol::E_IMSTATUS emStatus, EImFrontEndNo emRouteNumber) : 
			m_uUid(uUid), m_emStatus(emStatus), m_emRouteNumber(emRouteNumber) {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid /*<< m_uImLinkdId*/ ;
				p.push_uint8(m_emStatus);
				p.push_uint8(m_emRouteNumber);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid /*>> m_uImLinkdId*/;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
				m_emRouteNumber = (EImFrontEndNo)p.pop_uint8();
			}
		};

		//����״̬�ı�
		struct PSS_BatchStatusChangeCommon : public sox::Marshallable
		{
			enum {uri = (38 << 8) | protocol::IMONLINED_SVID};

			uint32_t m_uImlinkdId;
			protocol::E_IMSTATUS m_emStatus;
			std::map<EImFrontEndNo, std::vector<uint32_t> > m_mapUid;

			PSS_BatchStatusChangeCommon() {}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uImlinkdId;
				p.push_uint8(m_emStatus);

				p.push_uint32(m_mapUid.size());
				for (std::map<EImFrontEndNo, std::vector<uint32_t> >::const_iterator iter = m_mapUid.begin(); iter != m_mapUid.end(); iter ++)
				{
					p.push_uint8(iter->first);
					sox::marshal_container(p, iter->second);
				}
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uImlinkdId;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();

				uint32_t uMapSize = p.pop_uint32();
				for (uint32_t i = 0; i < uMapSize; i ++)
				{
					EImFrontEndNo emRouteNumber = (EImFrontEndNo)p.pop_uint8();
					std::vector<uint32_t> vecUid;
					sox::unmarshal_container(p, std::back_inserter(vecUid));

					m_mapUid.insert( std::make_pair(emRouteNumber, vecUid) );
				}
			}
		};

		//��Recover�޷�����ʱ��imonline_new -> imlink�����к�

		struct PSS_UdpExceptRecoverReq : public sox::Voidmable
		{
			enum {uri = (39 << 8) | protocol::IMLINKD_SVID};
		};

		struct PSS_UdpForerunner : public sox::Marshallable
		{
			enum {uri = (40 << 8) | protocol::IMONLINED_SVID};

			PSS_UdpForerunner() : m_uLinkdId(-1) {}
			PSS_UdpForerunner(uint32_t uLinkdId) : m_uLinkdId(uLinkdId) {}

			uint32_t      m_uLinkdId;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uLinkdId;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uLinkdId;
			}
		};


		// app status

		struct PSS_AppLogin : public sox::Marshallable
		{
			enum {uri = (41 << 8) | protocol::IMONLINED_SVID};

			uint32_t m_uUid;
			uint32_t m_uImLinkdId;
			uint32_t m_uAppId;
			protocol::E_IMSTATUS m_emStatus;

			PSS_AppLogin() : m_uUid(0), m_uImLinkdId(NONE_SERVER_ID), m_uAppId(NONE_SERVER_ID), m_emStatus(protocol::IMOFFLINE) {}
			PSS_AppLogin(uint32_t uUid, uint32_t uImLinkdId, protocol::E_IMSTATUS emStatus, uint32_t uAppId) : 
			m_uUid(uUid), m_uImLinkdId(uImLinkdId), m_uAppId(uAppId), m_emStatus(emStatus) 
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uImLinkdId << m_uAppId;
				p.push_uint8(m_emStatus);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uImLinkdId >> m_uAppId;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		struct PSS_AppLoginRes : sox::Marshallable
		{
			enum {uri = (42 << 8) | protocol::IMONLINED_SVID};

			uint32_t m_uUid;
			uint32_t m_uAppId;
			protocol::E_IMSTATUS m_emStatus;

			PSS_AppLoginRes() {}
			PSS_AppLoginRes(uint32_t uUid, uint32_t uAppId, protocol::E_IMSTATUS emSt) 
				: m_uUid(uUid), m_uAppId(uAppId), m_emStatus(emSt)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUid << m_uAppId;
				p.push_uint8(m_emStatus);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUid >> m_uAppId;
				m_emStatus = (protocol::E_IMSTATUS)p.pop_uint8();
			}
		};

		// app change status

		struct PSS_AppChangeStatus : public PSS_AppLogin
		{
			PSS_AppChangeStatus() {}
			PSS_AppChangeStatus(uint32_t uUid, uint32_t uImLinkdId, protocol::E_IMSTATUS emStatus, uint32_t uAppId) : 
				PSS_AppLogin(uUid, uImLinkdId, emStatus, uAppId) 
			{
			}

			enum {uri = (43 << 8) | protocol::IMONLINED_SVID};
		};

		struct PSS_AppChangeStatusRes : public PSS_AppLoginRes
		{
			PSS_AppChangeStatusRes() {}
			PSS_AppChangeStatusRes(uint32_t uUid, uint32_t uAppId, protocol::E_IMSTATUS emSt) 
				: PSS_AppLoginRes(uUid, uAppId, emSt)
			{
			}
			enum {uri = (44 << 8) | protocol::IMONLINED_SVID};
		};

		struct PSS_AppRecoverImOnIndex : public sox::Voidmable
		{
			enum {uri = (45 << 8) | protocol::IMONLINED_SVID};
		};

		struct PSS_AppRecoverImOnline : public sox::Voidmable
		{
			enum {uri = (46 << 8) | protocol::IMONLINED_SVID};
		};
	}
}


#endif
