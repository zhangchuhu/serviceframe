#ifndef _IM_DATA_STRUCT_H_
#define _IM_DATA_STRUCT_H_



namespace server{
	namespace im{

		//���������򲻷��Ϲ涨��Ϊ�˲�������ĸĶ�����ʱ����ԭ��������
		struct SimpleBInfo : public sox::Marshallable
		{
			SimpleBInfo(){}

			SimpleBInfo(uint32_t bid, std::string& mark, uint32_t p) : buddyid(bid), remark(mark), pid(p) {}

			uint32_t buddyid;
			std::string remark;
			uint32_t pid;

			virtual void marshal(sox::Pack &p) const {
				p << buddyid << remark << pid;
			}
			virtual void unmarshal(const sox::Unpack &p) {
				p >> buddyid >> remark >> pid;
			}
		};


		//wuji start
		struct CSimpleBInfo1 : public sox::Marshallable
		{
			CSimpleBInfo1()
			{
				buddyid=-1;
				pid=1;
				m_uImid=-1;

			}
			CSimpleBInfo1(uint32_t bid, std::string& mark, uint32_t p) 
				: buddyid(bid), remark(mark), pid(p)
			{
				
			}
			uint32_t buddyid;
			std::string remark;
			uint32_t pid;
			uint32_t m_uImid;

			virtual void marshal(sox::Pack &p) const 
			{
				p << buddyid << remark << pid<<m_uImid;
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> buddyid >> remark >> pid>>m_uImid;
			}
		};
//wuji end
	}
}




#endif
