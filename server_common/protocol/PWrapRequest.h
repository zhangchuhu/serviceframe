#pragma once 

#include "common/core/base_svid.h"
#include "common/packet.h"

namespace protocol
{
	namespace imlinkd
	{

		//定义UDP分片报文
		struct PCS_UdpPart : public sox::Marshallable
		{
			enum {uri = (265 << 8) | protocol::IMLINKD_SVID};

			uint64_t m_uSeqId;		//包序号
			uint32_t m_uTotalPart;	//分片总数
			uint32_t m_uPartNumber; //分片序号

			//分片报文
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

		//选路选路报文的封装
		struct PCS_WrapRequest : public sox::Marshallable
		{
			enum {uri = (266 << 8) | protocol::IMLINKD_SVID};

			//原报文
			uint32_t m_uUri;
			std::string m_strPacket;

			//路由部分
			std::vector<uint32_t> m_vecKey; //批量构造KEY
			uint32_t m_uSvid; //目标服务器

			uint8_t m_uRetryCount; //重发次数
			uint8_t m_uRouterNumber; //路数选择

			uint32_t m_uTaskId; //用于去重

			PCS_WrapRequest()
			{
				m_uRetryCount = 0;
				m_uRouterNumber = 0;
				m_uTaskId = 0;
			}

			//设置基础包
			void SetPacket(uint32_t uUri, const std::string &strPacket)
			{
				m_uUri = uUri;
				m_strPacket = strPacket;
			}
			//设置包装选项
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

			//原报文
			uint32_t m_uUri;
			std::string m_strPacket;

			//路由部分
			uint8_t m_uRetryCount; //重发次数
			uint8_t m_uRouterNumber; //路数选择

			uint32_t m_uTaskId; //用于去重

			PSS_WrapRequest()
			{
				m_uRetryCount = 0;
				m_uRouterNumber = 0;
				m_uTaskId = 0;
			}

			//设置基础包
			void SetPacket(uint32_t uUri, const std::string &strPacket)
			{
				m_uUri = uUri;
				m_strPacket = strPacket;
			}
			//设置包装选项
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
