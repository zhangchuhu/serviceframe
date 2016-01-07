#pragma once
#include <vector>


#include "core/base_svid.h"
#include "packet.h"//sox::Marshallable的定义
#include "protocol/const.h"
#include "server_common/protocol/psession.h"


namespace server
{
	namespace session
	{
		// 频道操作日志,新协议
		struct PMsgToLog2 :  public sox::Marshallable
		{
			enum {uri = (1 << 8) | protocol::SESSION_SAVE_LOG_SVID};

			uint32_t		topmost_sid;	// 顶级频道id
			SesisonOPType	type;			// 操作类型
			uint32_t		admin_uid;		// 管理员
			std::string		admin_nick;		// 管理员昵称
			uint32_t		id;				// 被操作对象（user or session）
			std::string		id_nick;		// 被操作对象昵称
			std::string		op_event;		// 事件描述
			std::string		remark;			// 备注
			uint32_t m_uTime;//发送时刻(发送端负责填）

			virtual void marshal(sox::Pack &pk) const
			{
				pk << topmost_sid;
				pk.push_uint8(type);
				pk << admin_uid << admin_nick
					<< id << id_nick
					<< op_event << remark<<m_uTime;

			}
			virtual void unmarshal(const sox::Unpack &upk)
			{
				upk >> topmost_sid;
				type = (SesisonOPType)upk.pop_uint8();
				upk >> admin_uid >> admin_nick
					>> id >> id_nick
					>> op_event >> remark>>m_uTime;
			}
		};


		//session发过来的日志数据
		struct PCS_UserBehavior : public sox::Marshallable 
		{
			enum {uri = (2 << 8) | protocol::SESSION_SAVE_LOG_SVID};
			PCS_UserBehavior ()
			{
				m_uSeqId=0;
				m_uExtraData=0;

			}
			std::vector<PMsgToLog2> m_vecData;
			uint32_t m_uSeqId;//流水号
			uint32_t m_uExtraData;//如果流水号不能区分唯一的包，可以使用这个附加数据区分。


			virtual void marshal(sox::Pack &p) const
			{
				marshal_container(p, m_vecData);
				p<<m_uSeqId<<m_uExtraData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vecData));
				p>>m_uSeqId>>m_uExtraData;
			}
		};

		//PCS_UserBehavior的响应
		struct PCS_UserBehaviorRes : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::SESSION_SAVE_LOG_SVID};
			PCS_UserBehaviorRes ()
			{
				m_uSeqId=0;
				m_uExtraData=0;

			}
			
			uint32_t m_uSeqId;//流水号
			uint32_t m_uExtraData;//如果流水号不能区分唯一的包，可以使用这个附加数据区分。


			virtual void marshal(sox::Pack &p) const
			{
				p<<m_uSeqId<<m_uExtraData;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uSeqId>>m_uExtraData;
			}
		};
	}
}

