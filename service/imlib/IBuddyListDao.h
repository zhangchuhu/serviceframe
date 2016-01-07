#pragma once
#include "common/int_types.h"
#include <vector>
#include <string>
#include "common/protocol/pimcommon.h"


namespace server
{
	namespace imdao
	{

		struct CBuddyListDaoData
		{
		public:
			CBuddyListDaoData()
			{
				m_uId=-1;
				m_uPid=1;
				m_uBid=-1;
				m_uIsDel=0;
			}

			CBuddyListDaoData(uint32_t uId,uint32_t uBid,uint32_t uPid,const std::string& strRemark)
			{
				m_uId=uId;
				m_uPid=uPid;
				m_uBid=uBid;
				m_uIsDel=0;
				m_strRemark=strRemark;
			}
		public:
			uint32_t m_uId;
			uint32_t m_uBid;
			uint32_t m_uPid;
			std::string m_strRemark;
			uint8_t m_uIsDel;
		};



		struct IBuddyListDao
		{
			virtual bool Delete(uint32_t uId,uint32_t uBid)=0;
			virtual bool Update(uint32_t uId,uint32_t uBid,const std::string& strRemark)=0;//�޸ĺ��ѱ�ע
			virtual bool Update(uint32_t uId,uint32_t uBid,uint32_t uPid)=0;//�ƶ����ѵ�ĳ����
			virtual bool Add(const CBuddyListDaoData& objData)=0;
			virtual bool IsExist(uint32_t uId,uint32_t uBid)=0;
			virtual bool Get(uint32_t uId,std::vector<server::imdao::CBuddyListDaoData>& vecData)=0;//ȡ�����ĺ����б�
			virtual bool GetAll(uint32_t uId,std::vector<server::imdao::CBuddyListDaoData>& vecData)=0;//ȡ�����б����������ĺͱ�����ɾ���ģ�

			virtual bool Get(uint32_t uId,std::vector<uint32_t>& vecData)=0;//ȡ�����ĺ����б�
			virtual bool GetAll(uint32_t uId,std::vector<uint32_t>& vecData)=0;//ȡ�����б����������ĺͱ�����ɾ���ģ�

			virtual bool Get(uint32_t uId,std::vector<protocol::im::CBuddyListNoRemark>& vecData)=0;//ȡ�����ĺ����б�
			virtual bool Get(uint32_t uId,std::map<uint32_t, std::string>& mapBidToRemark)=0;//ȡ�����ĺ����б�
			
			//virtual bool Get(uint32_t uId,std::vector<protocol::im::CSimpleBInfo1>& vecData)=0;//ȡ�����ĺ����б�
			virtual bool GetCount(uint32_t uId,uint32_t &nCount)=0;//ȡ���Ѹ���
			
			

		};


		class IBuddyListDaoAware
		{
		protected:
			IBuddyListDao* m_pObjBuddyListDao;
		public:
			void SetBuddyListDao(IBuddyListDao *pObj)
			{
				m_pObjBuddyListDao = pObj;
			}
			IBuddyListDao *GetBuddyListDao()
			{
				return m_pObjBuddyListDao;
			}
		};
	}
}

