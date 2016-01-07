#ifndef __P_SYS_MSG_H__
#define __P_SYS_MSG_H__

#include "common/protocol/const.h"
#include "core/base_svid.h"
#include "packet.h"
#include "protocol/const.h"
#include <string>

namespace protocol{
	namespace sysmsg{

		struct PSysMsg : public sox::Marshallable
		{
			enum { uri = (1 << 8) | SYSMSGD_SVID};

			std::string title;
			std::string msg;

            //new field for im2.2
            std::string m_strMsgType;
            SysMsgDisplayType m_enDisType;
            std::string m_strMsgUrl;

            PSysMsg()
                : title(""), msg(""), m_strMsgType(""), m_enDisType(ENUM_SYSMSG_AUTO_CLOSE)
            {
            }

			virtual void marshal(sox::Pack &p) const
            {
                p << title << msg << m_strMsgType;
                p.push_uint8((uint8_t) m_enDisType);
                p << m_strMsgUrl;
            }
			
            virtual void unmarshal(const sox::Unpack &p) 
            { 
                p >> title >> msg >> m_strMsgType;
                m_enDisType = (SysMsgDisplayType) p.pop_uint8();
                p >> m_strMsgUrl;
            }
		};

		struct PCS_ComplainResult: public sox::Marshallable
		{
			enum {uri = (3 << 8) | SYSMSGD_SVID};
			std::string m_strGuid;
			uint32_t m_uSeqId;
			uint32_t m_uChannelId;
			uint32_t m_uChannelAid;
			std::string m_strResult;
			std::string m_strAction;


			virtual void marshal(sox::Pack &p) const
			{
				p << m_uChannelAid << m_uSeqId << m_uChannelId << m_strGuid << m_strResult << m_strAction ;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{ 
				p >> m_uChannelAid >> m_uSeqId >> m_uChannelId  >> m_strGuid >> m_strResult >> m_strAction;
			}
		};
			
			
		struct PCS_VipLoginSysMsg : public sox::Marshallable
		{
			enum { uri = (200 << 8) | SYSMSGD_SVID};

			uint32_t m_uType;
			std::string m_strXml;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uType << m_strXml;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{ 
				p >> m_uType >> m_strXml;
			}
		};

		struct PCS_SysMsg : public sox::Marshallable
		{
			enum { uri = (100 << 8) | SYSMSGD_SVID};

			PCS_SysMsg() : m_id(0), m_msgType(0), m_strMsg("") {}

			uint32_t m_id;
			uint32_t m_msgType;
			std::string m_strMsg;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_id << m_msgType << m_strMsg;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{ 
				p >> m_id >> m_msgType >> m_strMsg;
			}
		};

		struct PCS_SysMsgRes : public sox::Marshallable
		{
			enum { uri = (101 << 8) | SYSMSGD_SVID};

			PCS_SysMsgRes() {}

			PCS_SysMsgRes(uint32_t id, uint32_t uid)
				: m_id(id), m_uid(uid)
			{

			}

			uint32_t m_id;
			uint32_t m_uid;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_id << m_uid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{ 
				p >> m_id >> m_uid;
			}
		};
	}
}
#endif
