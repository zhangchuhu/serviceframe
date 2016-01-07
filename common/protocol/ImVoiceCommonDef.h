#ifndef IM_VOICE_COMMON_DEF_INCLUDE_H
#define IM_VOICE_COMMON_DEF_INCLUDE_H

#include <vector>
#include <sstream>
#include "common/int_types.h"
#include "common/unistr.h"
#include "common/packet.h"
#include "common/protocol/const.h"

namespace protocol
{
	namespace imvoice
	{

		typedef std::string VoiceTag_T;

		// 语音会话的成员
		struct CImVoiceMemberInfo
		{
			uint32_t                              m_uInitiator;      // 发起人
			std::map<uint32_t, UniString>         m_mapUid2Text;     // map<uid, 用户昵称等>

			CImVoiceMemberInfo() : m_uInitiator(INVALID_UID) {}

			void Reset()
			{
				m_uInitiator = INVALID_UID;
				m_mapUid2Text.clear();
			}

			std::string FormChannelString() const
			{
				std::stringstream ss;
				ss << "IM-" << m_uInitiator << "-";
				for (std::map<uint32_t, UniString>::const_iterator it = m_mapUid2Text.begin(); it != m_mapUid2Text.end(); ++it)
				{
					if (it->first != m_uInitiator)
						ss << it->first << "-";
				}

				return ss.str();
			}

			// 一个语音实例唯一标识
			VoiceTag_T FormTagString() const
			{
				std::stringstream ss;
				std::set<uint32_t> tmpSet;
				for (std::map<uint32_t, UniString>::const_iterator it = m_mapUid2Text.begin(); it != m_mapUid2Text.end(); ++it)
				{
					tmpSet.insert(it->first);
				}

				for (std::set<uint32_t>::iterator itSet = tmpSet.begin(); itSet != tmpSet.end(); ++itSet)
				{
					ss << *itSet << "-";
				}
				/*
				 *	在4.0，A和B语音，通了后，B按断开，然后即刻A又发起B语音
				 *	这时候会出现ENUM_IMVOICE_CONN_CONFLICT错误，
				 *	由于两次发起的VoiceTag_T是一样的，所以在这里处理，加上一个时间戳
				 *	-----------------
				 *	后来被证实不行，问题不在这里在于连接了两次
				 */
				// ss << time(NULL);
				return ss.str();
			}

			std::string MapToString() const
			{
				sox::PackBuffer bf; 
				sox::Pack pk(bf); 
				sox::marshal_container(pk, m_mapUid2Text);
				return std::string(pk.data(), pk.size()); 
			}

			void StringToMap(const std::string& str)
			{
				sox::Unpack up(str.data(), str.size());
				sox::unmarshal_container(up, std::inserter(m_mapUid2Text, m_mapUid2Text.begin()));				
			}
		};

		enum EImVoiceStatus
		{
			ENUM_VOICE_REQ_PEER = 0,         // 正在请求对方
			ENUM_VOICE_RECIVE_REQ,           // 收到对方请求建立自由语音

			//ENUM_VOICE_INIT = 0,
			ENUM_VOICE_CONNECTING,           // 正在建立连接
			ENUM_VOICE_WAITING_PEER,         // 自己已经进入频道，正在等对方
			ENUM_VOICE_CONNECTED,            // 连接已经建立
			ENUM_VOICE_CLOSED,               // 连接关闭

			ENUM_VOICE_ALLOC_CHANNEL       // 申请临时频道
		};

		// 会话关闭的原因
		enum EImVoiceError
		{
			ENUM_CONN_UNKNOW_REASON = 0,                       // 
			ENUM_IMVOICE_CONN_PEER_REJECT,                     // 对方拒绝了
			ENUM_IMVOICE_CONN_SELF_REJECT,                     // 你拒绝了
			ENUM_IMVOICE_CONN_SELF_EXIT,                       // 你终止了语音会话
			ENUM_IMVOICE_CONN_PEER_EXIT,                       // 对方终止了语音会话
			ENUM_IMVOICE_CONN_PEER_TIMEOUT,                    // 超时后，对方还没有进入频道
			ENUM_IMVOICE_CONN_SELF_FAILD,                      // 自己连接频道超时
			ENUM_IMVOICE_CONN_PEER_FAILD,                      // 对方进入频道失败
			ENUM_IMVOICE_CONN_CONFLICT,                        // 重复的实例冲突了
			ENUM_IMVOICE_CONN_REQ_TIMEOUT,                     // 请求对方，超时未响应
			ENUM_IMVOICE_CONN_RECIVE_TIMEOUT,                  // 对方请求语音，超时未处理
			ENUM_IMVOICE_CONN_SAME_HAS_EXIST,                  // 和同一个人的语音会话已经打开了
			ENUM_IMVOICE_CONN_NOMAL
		};

		// 失败统计
		enum EImVoiceErrorStat
		{
			ENUM_VOICE_STAT_NOMAL = 0,             // 正常的终止会话
			ENUM_VOICE_STAT_ALLOC_FAILD,           // 申请临时频道失败
			ENUM_VOICE_STAT_SESSHANDLE,            // SessionHandler事件
			ENUM_VOICE_STAT_NULL_SESSION,          // join时，返回了null指针
			ENUM_VOICE_STAT_FIRST_SELF_TIMEOUT,    // 会话未成功前，自己连接频道超时
			ENUM_VOICE_STAT_FIRST_PEER_TIMEOUT,    // 会话未成功前，对方连接频道超时	
			ENUM_VOICE_STAT_SELF_TIMEOUT,          // 会话开始后，自己连接频道超时         （这个失败统计，包含imtransfer转发Exit失败）
			ENUM_VOICE_STAT_PEER_TIMEOUT,          // 会话开始后，对方连接频道超时
			ENUM_VOICE_STAT_IM_TRANS_TIMEOUT       // IM转发同意语音的信令延时15s以上
		};

//////////////////////////////////////////////////////////////////////////

		inline const char* ImVoiceString(EImVoiceStatus status)
		{
			switch (status)
			{
			case ENUM_VOICE_REQ_PEER:
				return "req peer";
			case ENUM_VOICE_RECIVE_REQ:
				return "recive req";
			case ENUM_VOICE_CONNECTING:
				return "connect to chnale";
			case ENUM_VOICE_WAITING_PEER:
				return "watting peer";
			case ENUM_VOICE_CONNECTED:
				return "connected";
			case ENUM_VOICE_CLOSED:
				return "closed";
			default:
				return "unknow";
			}
		}

		inline const char* ImVoiceString(EImVoiceError err)
		{
			switch(err)
			{
			case ENUM_IMVOICE_CONN_PEER_REJECT:
				return "peer reject";
			case ENUM_IMVOICE_CONN_SELF_REJECT:
				return "self reject";
			case ENUM_IMVOICE_CONN_SELF_EXIT:
				return "self exit";
			case ENUM_IMVOICE_CONN_PEER_EXIT:
				return "peer exit";
			case ENUM_IMVOICE_CONN_PEER_TIMEOUT:
				return "peer conn time out";
			case ENUM_IMVOICE_CONN_SELF_FAILD:
				return "self conn faild";
			case ENUM_IMVOICE_CONN_PEER_FAILD:
				return "peer conn faild";
			case ENUM_IMVOICE_CONN_CONFLICT:
				return "request conflict";
			case ENUM_IMVOICE_CONN_REQ_TIMEOUT:
				return "req peer timeout";
			case ENUM_IMVOICE_CONN_RECIVE_TIMEOUT:
				return "not answer timeout";
            case ENUM_CONN_UNKNOW_REASON:
			default:
				return "unknow";
			}
		}
	}
}

#endif
