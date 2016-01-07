#pragma once
#include "common/res_code.h"
#include <vector>

#include "common/packet.h"

namespace protocol
{
	namespace im
	{
		//wuji start
		struct PCS_CommonHead: public sox::Marshallable 
		{
			PCS_CommonHead()
			{
				m_uResCode=RES_SUCCESS;

			}
			uint32_t m_uResCode;

			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uResCode;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uResCode;
			}

		};
		//wuji end


		//��½�õ����ݽṹ
		/*
		struct CBuddyList: public sox::Marshallable //��¼ʱ���ڴ��ͺ����б�
		{
			uint32_t m_uBid;
			uint32_t m_uPid;
			uint32_t m_imId;
			std::string m_strRemark;

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid << m_imId << m_uPid << m_strRemark;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid >> m_imId >> m_uPid >> m_strRemark;
			}
		};*/

		//��½�õ����ݽṹ
		struct CBuddyListNoRemark: public sox::Marshallable //��¼ʱ���ڴ��ͺ����б�
		{
			uint32_t m_uBid;
			uint32_t m_uPid;
			//uint32_t m_imId;
			//std::string m_strRemark;
			CBuddyListNoRemark() {}
			CBuddyListNoRemark(uint32_t bid, uint32_t pid) : m_uBid(bid), m_uPid(pid) {}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uBid << /*m_imId <<*/ m_uPid /*<< m_strRemark*/;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uBid >> /*m_imId >>*/ m_uPid/* >> m_strRemark*/;
			}
		};



		struct CFolderList: public sox::Marshallable //��¼ʱ���ڴ��ͷ����б�
		{
			uint32_t m_uFid;
			std::string m_strRemark;
			CFolderList(){}
			CFolderList(uint32_t uFid, const std::string& strRemark)
				: m_uFid(uFid), m_strRemark(strRemark)
			{
			}

			virtual void marshal(sox::Pack &p) const 
			{
				p << m_uFid << m_strRemark ;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> m_uFid >> m_strRemark;
			}
		};

		struct CUidSet: public sox::Marshallable //������Ҫ����uid���ϵ����͵Ļ���
		{
			std::vector<uint32_t> m_vtUid;

			virtual void marshal(sox::Pack &p) const 
			{
				marshal_container(p, m_vtUid);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				unmarshal_container(p, std::back_inserter(m_vtUid));
			}
		};

	}
}





