#pragma once

#include <vector>
#include <string>

#include "iproperty.h"
#include "protocol/const.h"
#include "core/base_svid.h"
//#include "./pverifycodepng.h"
//#include "common/protocol/iImV2.h"
#include "pimcommon.h"

using namespace std;

namespace protocol
{
	namespace gofflinemsg
	{
		struct CGofflineMsgData:public sox::Marshallable 
		{

		public:
			CGofflineMsgData()
			{
				m_uSendUid=-1;//发送人
				m_uReceiveUid=-1;//接收人
				m_uSendTime=0;//发送时间，以数据库存盘时间为准，UTC时间
				m_uLength=0;//消息长度
				m_uGid=-1;//

			}

			CGofflineMsgData(uint32_t uSendUid,uint32_t uReceiveUid,uint32_t uLength,
				const string& strBody,uint32_t uSendTime,uint32_t uGid)
			{
				m_uSendUid=uSendUid;//发送人
				m_uReceiveUid=uReceiveUid;//接收人
				m_uSendTime=uSendTime;//发送时间，以数据库存盘时间为准，UTC时间
				m_uLength=uLength;//消息长度
				m_strBody=strBody;//消息体
				m_uGid=uGid;

			}

			uint32_t m_uSendUid;//发送人
			uint32_t m_uReceiveUid;//接收人
			uint32_t m_uSendTime;//发送时间，以数据库存盘时间为准，UTC时间
			uint32_t m_uLength;//消息长度
			string m_strBody;//消息体
			uint32_t m_uGid;//

		public:
			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uSendUid<<m_uReceiveUid<<m_uSendTime<<m_uLength<<m_strBody<<m_uGid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uSendUid>>m_uReceiveUid>>m_uSendTime>>m_uLength>>m_strBody>>m_uGid;
			}

		};

		struct PCS_GetGOffLineMsg :  public sox::Voidmable
		{
			enum {uri = (1 << 8) | protocol::GOFFLINEMSG_SVID};
		};


		//本文件描述的消息定义，是客户端和服务器交互的消息定义
		//取离线消息
/*
		struct PCS_GetGOffLineMsg : public sox::Marshallable 
		{
			enum {uri = (1 << 8) | protocol::GOFFLINEMSG_SVID};
			
			PCS_GetGOffLineMsg(){};
			//群id,发送方的用户ID
			map<uint32_t,vector<uint32_t> > m_mapUid;

		PCS_GetGOffLineMsg(){};
		//群id,发送方的用户ID
		map<uint32_t,vector<uint32_t> > m_mapUid;

		virtual void marshal(sox::Pack &p) const 
		{
		map<uint32_t,vector<uint32_t> >::const_iterator pIt ;
		p<<m_mapUid.size();
		for(pIt=m_mapUid.begin();pIt!=m_mapUid.end();pIt++)
		{
		p<<pIt->first;
		marshal_container(p, pIt->second);
		}

		}

		virtual void unmarshal(const sox::Unpack &p) 
		{
		uint32_t uSize;
		uint32_t uGid;
		vector<uint32_t> vecUid;
		p>>uSize;
		for (uint32_t i=0;i<uSize;i++)
		{
		vecUid.clear();
		p>>uGid;
		unmarshal_container(p, std::back_inserter(vecUid));
		m_mapUid[uGid]=vecUid;
		}
		};

*/

		//PCS_GetGOffLineMsg的响应
		struct PCS_GetGOffLineMsgRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (2 << 8) | protocol::GOFFLINEMSG_SVID};
			PCS_GetGOffLineMsgRes(){}

			vector<CGofflineMsgData> m_vecData;

			virtual void marshal(sox::Pack &p) const
			{
				protocol::im::PCS_CommonHead::marshal(p);
				marshal_container(p, m_vecData);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				unmarshal_container(p, std::back_inserter(m_vecData));
			}
		};

		//删除离线消息
		struct PCS_DelGOffLineMsg : public sox::Voidmable
		{
			enum {uri = (3 << 8) | protocol::GOFFLINEMSG_SVID};

		};

		//存储离线消息的响应
		struct PCS_SaveGOffLineMsgRes : public protocol::im::PCS_CommonHead 
		{
			enum {uri = (4 << 8) | protocol::GOFFLINEMSG_SVID};
			PCS_SaveGOffLineMsgRes(){}
			uint32_t m_uSeqId;//客户端发送点对点消息时候提供的流水号
			uint32_t m_uReceiverId;//客户端发送点对点消息的接收方
			uint32_t m_uGid;////客户端发送点对点消息的接收方所在群
			


			virtual void marshal(sox::Pack &p) const
			{
				protocol::im::PCS_CommonHead::marshal(p);
				p<<m_uSeqId<<m_uReceiverId<<m_uGid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				protocol::im::PCS_CommonHead::unmarshal(p);
				p>>m_uSeqId>>m_uReceiverId>>m_uGid;

			}
		};


		struct PCS_DelGOffLineMsgByGid : public sox::Marshallable
		{
			enum {uri = (6 << 8) | protocol::GOFFLINEMSG_SVID};
			PCS_DelGOffLineMsgByGid(){};

			uint32_t m_uGid;

			virtual void marshal(sox::Pack &p) const 
			{
				p<<m_uGid;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p>>m_uGid;
			}
		};







	}
}
