#ifndef _P_MEMCACHE_H_
#define _P_MEMCACHE_H_

#include "core/base_svid.h"
#include "int_types.h"

#include "protocol/const.h"
#include "packet.h"
#include "imlib/ImDataStruct.h"

#include <string>
#include <vector>

namespace server
{
	namespace mc
	{
		//wuji start

		//ͨ�����ڴ�ṹ��������ȡ��������Щ����һ��int���ͱ���������
		struct PMC_IMByUINT32 : public sox::Marshallable
		{
			PMC_IMByUINT32(){}
			
			uint32_t  m_nId;//������-1,����ʹ���޷�������ʾ
			virtual void marshal(sox::Pack &p) const{
				p.push_uint32(m_nId);
			}
			virtual void unmarshal(const sox::Unpack &p){
				m_nId=(int)p.pop_uint32();
			}
		};

		//ͨ�����ڴ�ṹ��������ȡ��������Щ����һ��int���ͱ���������
		struct PMC_IMByString : public sox::Marshallable
		{
			PMC_IMByString(){}
			
			std::string m_strData;//
			virtual void marshal(sox::Pack &p) const
			{
				p<<m_strData;
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p>>m_strData;
			}
		};

		struct PMC_IMUserCheckCode : public sox::Marshallable
		{
			PMC_IMUserCheckCode(){}
			
			std::string m_strCode;//��֤��
			uint32_t m_uCreateTime;//��֤������ʱ��

			
			
			virtual void marshal(sox::Pack &p) const{
				p<<m_strCode<<m_uCreateTime;
				
			}
			virtual void unmarshal(const sox::Unpack &p){
				p>>m_strCode>>m_uCreateTime;
				
			}
		};
		

		struct PMC_IMUserMaxJiFen : public sox::Marshallable
		{
			PMC_IMUserMaxJiFen(){}
			
			uint32_t m_uMaxJiFen;
			
			
			virtual void marshal(sox::Pack &p) const{
				p<<m_uMaxJiFen;
				
			}
			virtual void unmarshal(const sox::Unpack &p){
				p>>m_uMaxJiFen;
				
			}
		};


		struct PMC_IMUserInfo : public sox::Marshallable
		{
			PMC_IMUserInfo(){}
			
			uint32_t m_uCeLie;//��֤����

			
			virtual void marshal(sox::Pack &p) const{
				p<<m_uCeLie;
				
			}
			virtual void unmarshal(const sox::Unpack &p){
				p>>m_uCeLie;
				
			}
		};


		//Ϊ�����ڴ����汣���û���¼ʱ�����õ�һЩ��Ϣ
		struct PMC_LoginInfo : public sox::Marshallable
		{
			PMC_LoginInfo(){}
			PMC_LoginInfo(uint32_t uSearchCount)
			{
				m_uSearchCount=uSearchCount;//��������
			}
			
			uint32_t m_uSearchCount;
			
			virtual void marshal(sox::Pack &p) const{
				p<<m_uSearchCount;
				
			}
			virtual void unmarshal(const sox::Unpack &p){
				p>>m_uSearchCount;
				
			}
		};
		//wuji end

		struct PMC_uid_Buddylist : public sox::Marshallable{

			std::vector<uint32_t> sim_buddylist;

			virtual void marshal(sox::Pack &p) const{
				sox::marshal_container(p, sim_buddylist);
			}
			virtual void unmarshal(const sox::Unpack &p){
				sox::unmarshal_container(p, std::back_inserter(sim_buddylist));
			}
		};

		struct PMC_Simple_Buddylist : public sox::Marshallable{

			std::vector<server::im::SimpleBInfo> buddylist;

			virtual void marshal(sox::Pack &p) const{
				sox::marshal_container(p, buddylist);
			}
			virtual void unmarshal(const sox::Unpack &p){
				sox::unmarshal_container(p, std::back_inserter(buddylist));
			}
		};

		struct PMC_OnlineStat : public sox::Marshallable{

			protocol::E_IMSTATUS st;
			uint32_t linkd_id;

			virtual void marshal(sox::Pack &p) const{
				p.push_uint8(st);
				p.push_uint32(linkd_id);
			}
			virtual void unmarshal(const sox::Unpack &p){
				st = (protocol::E_IMSTATUS)p.pop_uint8();
				linkd_id = p.pop_uint32();
			}
		};

		struct PMC_Simple_Uinfo : public sox::Marshallable{
			PMC_Simple_Uinfo(){}
			PMC_Simple_Uinfo(std::string& name, uint32_t jifen, std::string& sig, uint32_t ver, protocol::uinfo::SEX s)
				: nickname(name), rank(jifen), sign(sig), version(ver), sex(s) { }

			std::string nickname;
			uint32_t	rank;
			std::string	sign;
			uint32_t version;
			protocol::uinfo::SEX sex;

			virtual void marshal(sox::Pack &p) const
			{
				p << nickname << rank << sign << version;
				p.push_uint8(sex);
			}
			virtual void unmarshal(const sox::Unpack &p)
			{
				p >> nickname >> rank >> sign >> version;
				sex = (protocol::uinfo::SEX)p.pop_uint8();
			}
		};



	}
}
#endif

