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

		// �����Ự�ĳ�Ա
		struct CImVoiceMemberInfo
		{
			uint32_t                              m_uInitiator;      // ������
			std::map<uint32_t, UniString>         m_mapUid2Text;     // map<uid, �û��ǳƵ�>

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

			// һ������ʵ��Ψһ��ʶ
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
				 *	��4.0��A��B������ͨ�˺�B���Ͽ���Ȼ�󼴿�A�ַ���B����
				 *	��ʱ������ENUM_IMVOICE_CONN_CONFLICT����
				 *	�������η����VoiceTag_T��һ���ģ����������ﴦ������һ��ʱ���
				 *	-----------------
				 *	������֤ʵ���У����ⲻ��������������������
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
			ENUM_VOICE_REQ_PEER = 0,         // ��������Է�
			ENUM_VOICE_RECIVE_REQ,           // �յ��Է���������������

			//ENUM_VOICE_INIT = 0,
			ENUM_VOICE_CONNECTING,           // ���ڽ�������
			ENUM_VOICE_WAITING_PEER,         // �Լ��Ѿ�����Ƶ�������ڵȶԷ�
			ENUM_VOICE_CONNECTED,            // �����Ѿ�����
			ENUM_VOICE_CLOSED,               // ���ӹر�

			ENUM_VOICE_ALLOC_CHANNEL       // ������ʱƵ��
		};

		// �Ự�رյ�ԭ��
		enum EImVoiceError
		{
			ENUM_CONN_UNKNOW_REASON = 0,                       // 
			ENUM_IMVOICE_CONN_PEER_REJECT,                     // �Է��ܾ���
			ENUM_IMVOICE_CONN_SELF_REJECT,                     // ��ܾ���
			ENUM_IMVOICE_CONN_SELF_EXIT,                       // ����ֹ�������Ự
			ENUM_IMVOICE_CONN_PEER_EXIT,                       // �Է���ֹ�������Ự
			ENUM_IMVOICE_CONN_PEER_TIMEOUT,                    // ��ʱ�󣬶Է���û�н���Ƶ��
			ENUM_IMVOICE_CONN_SELF_FAILD,                      // �Լ�����Ƶ����ʱ
			ENUM_IMVOICE_CONN_PEER_FAILD,                      // �Է�����Ƶ��ʧ��
			ENUM_IMVOICE_CONN_CONFLICT,                        // �ظ���ʵ����ͻ��
			ENUM_IMVOICE_CONN_REQ_TIMEOUT,                     // ����Է�����ʱδ��Ӧ
			ENUM_IMVOICE_CONN_RECIVE_TIMEOUT,                  // �Է�������������ʱδ����
			ENUM_IMVOICE_CONN_SAME_HAS_EXIST,                  // ��ͬһ���˵������Ự�Ѿ�����
			ENUM_IMVOICE_CONN_NOMAL
		};

		// ʧ��ͳ��
		enum EImVoiceErrorStat
		{
			ENUM_VOICE_STAT_NOMAL = 0,             // ��������ֹ�Ự
			ENUM_VOICE_STAT_ALLOC_FAILD,           // ������ʱƵ��ʧ��
			ENUM_VOICE_STAT_SESSHANDLE,            // SessionHandler�¼�
			ENUM_VOICE_STAT_NULL_SESSION,          // joinʱ��������nullָ��
			ENUM_VOICE_STAT_FIRST_SELF_TIMEOUT,    // �Ựδ�ɹ�ǰ���Լ�����Ƶ����ʱ
			ENUM_VOICE_STAT_FIRST_PEER_TIMEOUT,    // �Ựδ�ɹ�ǰ���Է�����Ƶ����ʱ	
			ENUM_VOICE_STAT_SELF_TIMEOUT,          // �Ự��ʼ���Լ�����Ƶ����ʱ         �����ʧ��ͳ�ƣ�����imtransferת��Exitʧ�ܣ�
			ENUM_VOICE_STAT_PEER_TIMEOUT,          // �Ự��ʼ�󣬶Է�����Ƶ����ʱ
			ENUM_VOICE_STAT_IM_TRANS_TIMEOUT       // IMת��ͬ��������������ʱ15s����
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
