#pragma once 

#include "common/core/base_svid.h"
#include "common/packet.h"

namespace protocol
{
	namespace imlinkd
	{

		//����UDP��Ƭ����
		struct PCS_UdpPart : public sox::Marshallable
		{
			enum {uri = (265 << 8) | protocol::IMLINKD_SVID};

			uint64_t m_uSeqId;		//�����
			uint32_t m_uTotalPart;	//��Ƭ����
			uint32_t m_uPartNumber; //��Ƭ���

			//��Ƭ����
			uint32_t m_uUri;
			std::string m_strPacket;

			void SetPartInfo(uint64_t uSeqId, uint32_t uTotalPart, uint32_t uPartNumber, uint32_t uUri, const std::string &strPacket)
			{
				m_uSeqId = uSeqId;
				m_uTotalPart = uTotalPart;
				m_uPartNumber = uPartNumber;

				m_uUri = uUri;
				m_strPacket = strPacket;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uSeqId << m_uTotalPart << m_uPartNumber;
				p << m_uUri;
				p.push_varstr32(m_strPacket.data(), m_strPacket.size());
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uSeqId >> m_uTotalPart >> m_uPartNumber;
				p >> m_uUri;
				m_strPacket = p.pop_varstr32();
			}
		};

		//ѡ·ѡ·���ĵķ�װ
		struct PCS_WrapRequest : public sox::Marshallable
		{
			enum {uri = (266 << 8) | protocol::IMLINKD_SVID};

			//ԭ����
			uint32_t m_uUri;
			std::string m_strPacket;

			//·�ɲ���
			std::vector<uint32_t> m_vecKey; //��������KEY
			uint32_t m_uSvid; //Ŀ�������

			uint8_t m_uRetryCount; //�ط�����
			uint8_t m_uRouterNumber; //·��ѡ��

			uint32_t m_uTaskId; //����ȥ��

			PCS_WrapRequest()
			{
				m_uRetryCount = 0;
				m_uRouterNumber = 0;
				m_uTaskId = 0;
			}

			//���û�����
			void SetPacket(uint32_t uUri, const std::string &strPacket)
			{
				m_uUri = uUri;
				m_strPacket = strPacket;
			}
			//���ð�װѡ��
			void SetWrapOption(const std::vector<uint32_t> &vecKey, uint32_t uSvid, uint32_t uRetryCount, uint32_t uRouterNumber, uint32_t uTaskId)
			{
				m_vecKey = vecKey;
				m_uSvid = uSvid;
				m_uRetryCount = uRetryCount;
				m_uRouterNumber = uRouterNumber;
				m_uTaskId = uTaskId;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUri;
				p.push_varstr32(m_strPacket.data(), m_strPacket.size());

				sox::marshal_container(p, m_vecKey);
				p << m_uSvid;
				p << m_uRetryCount << m_uRouterNumber;
				p << m_uTaskId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUri;
				m_strPacket = p.pop_varstr32();

				sox::unmarshal_container(p,std::back_inserter(m_vecKey));
				p >> m_uSvid;
				p >> m_uRetryCount >> m_uRouterNumber;
				p >> m_uTaskId;
			}
		};

		struct PSS_WrapRequest : public sox::Marshallable
		{
			enum {uri = (267 << 8) | protocol::IMLINKD_SVID};

			//ԭ����
			uint32_t m_uUri;
			std::string m_strPacket;

			//·�ɲ���
			uint8_t m_uRetryCount; //�ط�����
			uint8_t m_uRouterNumber; //·��ѡ��

			uint32_t m_uTaskId; //����ȥ��

			PSS_WrapRequest()
			{
				m_uRetryCount = 0;
				m_uRouterNumber = 0;
				m_uTaskId = 0;
			}

			//���û�����
			void SetPacket(uint32_t uUri, const std::string &strPacket)
			{
				m_uUri = uUri;
				m_strPacket = strPacket;
			}
			//���ð�װѡ��
			void SetWrapOption(uint32_t uRetryCount, uint32_t uRouterNumber, uint32_t uTaskId)
			{
				m_uRetryCount = uRetryCount;
				m_uRouterNumber = uRouterNumber;
				m_uTaskId = uTaskId;
			}

			virtual void marshal(sox::Pack &p) const
			{
				p << m_uUri;
				p.push_varstr32(m_strPacket.data(), m_strPacket.size());

				p << m_uRetryCount << m_uRouterNumber;
				p << m_uTaskId;
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> m_uUri;
				m_strPacket = p.pop_varstr32();

				p >> m_uRetryCount >> m_uRouterNumber;
				p >> m_uTaskId;
			}
		};

	}
}
