#ifndef PROTOCOL_IM_VOICE_H_INCLUDE
#define PROTOCOL_IM_VOICE_H_INCLUDE

#include "common/packet.h"
#include "common/protocol/ImVoiceCommonDef.h"

namespace protocol
{
	namespace imvoice
	{
		// ����ʼ��������

		struct PCS_ImVoiceStartReq : public sox::Marshallable
		{
			enum {uri = (20 << 8) | protocol::IMTRANS_SVID};

			uint32_t                  m_uTmpSid;         // ��ʱƵ��id
			uint32_t                  m_uInitiator;      // ������
			std::string               m_strText;         // �û��ǳƵ���Ϣ
			
			
			virtual void marshal(sox::Pack &p) const
			{
				p << m_uTmpSid << m_uInitiator << m_strText;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uTmpSid >>m_uInitiator >> m_strText;
			}
		};

		// ��ʼ������������Ӧ

		struct PCS_ImVoiceStartRes : public sox::Marshallable
		{
			enum {uri = (21 << 8) | protocol::IMTRANS_SVID};

			VoiceTag_T    m_strTag;
			uint8_t       m_uAgree;                    // 1 agree;  0 reject
			uint32_t      m_uUid;                      // ͬ��Ự���û�uid

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strTag << m_uAgree << m_uUid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strTag >> m_uAgree >> m_uUid;
			}
		};

		// �˳�Ƶ��֪ͨ

		struct PCS_ImVoiceExit : public sox::Marshallable
		{
			enum {uri = (22 << 8) | protocol::IMTRANS_SVID};

			uint32_t      m_uUid;                 // �˳�Ƶ�����û�uid
			VoiceTag_T    m_strTag;
			EImVoiceError m_error;                // �˳�Ƶ����ԭ��
			EImVoiceErrorStat m_errSt;            // ����ͳ�Ƶ�����

			PCS_ImVoiceExit() {}
			PCS_ImVoiceExit(uint32_t uUid, const VoiceTag_T& mark, EImVoiceError err, EImVoiceErrorStat errSt)
				: m_uUid(uUid), m_strTag(mark), m_error(err), m_errSt(errSt)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strTag << m_uUid;
				p.push_uint32(m_error);
				p.push_uint32(m_errSt);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strTag >> m_uUid;
				m_error = (EImVoiceError)p.pop_uint32();
				if (p.size() != 0)
				{
					try
					{
						m_errSt = (EImVoiceErrorStat)p.pop_uint32();
					}
					catch (...)
					{
						m_errSt = ENUM_VOICE_STAT_NOMAL;
					}
				}
				else
				{
					m_errSt = ENUM_VOICE_STAT_NOMAL;
				}
			}
		};

		// ͳ����Ϣ

		struct PCS_ImVoiceStatInfo : public sox::Marshallable
		{
			enum {uri = (23 << 8) | protocol::IMTRANS_SVID};

			VoiceTag_T    m_strTag;
			uint8_t       m_uConnType;
			uint32_t      m_uJoinDelay;
			uint32_t      m_uWaitDelay;

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strTag << m_uConnType << m_uJoinDelay << m_uWaitDelay;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strTag >> m_uConnType >> m_uJoinDelay >> m_uWaitDelay;
			}
		};
	}
}

#endif
