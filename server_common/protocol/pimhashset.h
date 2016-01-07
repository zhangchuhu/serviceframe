#ifndef P_IM_HASH_SET_H_SERVER_FILE
#define  P_IM_HASH_SET_H_SERVER_FILE

#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/����ŵĵط�
#include "packet.h"//sox::Marshallable�Ķ���
#include "protocol/const.h"


namespace protocol
{
	namespace im
	{
		//�ͻ�����������ϴ�������Ϣ
		struct PSS_ReportHashTableInfo : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::IMHASHSET_SVID};
			PSS_ReportHashTableInfo (){}
			PSS_ReportHashTableInfo(const std::string &strIP, uint32_t uProcessId, uint32_t uServerId,  const std::vector<uint32_t> &vtHashSet, const std::string &strMemo)
			{
			}

			std::string m_strIP;					//Server��IP
			uint32_t m_uProcessId;				//���̺�
			uint32_t m_uServerId;				//
			std::vector<uint32_t> m_vtHashSet;			//hash����Ԫ����
			std::string m_strMemo;				//��ע

			virtual void marshal(sox::Pack &p) const
			{
				p << m_strIP <<m_uProcessId<<m_uServerId<<m_strMemo ;
				marshal_container(p,  m_vtHashSet);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_strIP >> m_uProcessId >> m_uServerId >> m_strMemo;
				sox::unmarshal_container(p, std::back_inserter(m_vtHashSet));
			}

			static bool bIsBigger(const PSS_ReportHashTableInfo & a  , const PSS_ReportHashTableInfo& b)
			{
				if(a.m_strMemo.size() <4 && b.m_strMemo.size() <4 )
				{
					return (strcasecmp(a.m_strIP.c_str(), b.m_strIP.c_str()) >=0);
				}
				if(a.m_strMemo.size() < 4)
				{
					return false;
				}
				if(b.m_strMemo.size() < 4)
				{
					return true;
				}
				int result = strcasecmp(a.m_strMemo.substr(0,4).c_str(), b.m_strMemo.substr(0,4).c_str());
				if(result > 0)  
				{
					return true;
				}
				else if(result < 0)
				{
					return false;
				}
				else
				{
					result = strcasecmp(a.m_strIP.c_str(), b.m_strIP.c_str());
					if(result > 0)
					{
						return true;
					}
					else if(result < 0)
					{
						return false;
					}
					else
					{
						return (a.m_uServerId >= b.m_uServerId);
					}
				}
			}
		};
	}
}


#endif 
 
