#ifndef P_IM_OFFLINEMSG_H_COMMON_FILE
#define  P_IM_OFFLINEMSG_H_COMMON_FILE



//以下3个文件是必须的文件
// #include "common/core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
// #include "common/packet.h"//sox::Marshallable的定义
// #include "common/protocol/const.h"

#include "core/base_svid.h"///*protocol::WUJI_SIMPLE_SVID*/定义放的地方
#include "packet.h"//sox::Marshallable的定义
#include "protocol/const.h"
#include "pimchat2.h"
#include "pimtrans.h"


//using namespace std;

namespace protocol
{
	namespace im
	{
		// 由imchat2发来的离线消息
		struct PSS_FromImChatMsg : public sox::Marshallable 
		{
			enum {uri = (9 << 8) | protocol::IMOFFLINEMSG_SVID};
			PSS_FromImChatMsg (){}
			PSS_FromImChatMsg(uint32_t from, uint32_t to, const std::string& msg,uint32_t uSeqId )
				: fromUid(from), toUid(to),text(msg),m_uSeqId(uSeqId)
			{
			}

			uint32_t fromUid;
			uint32_t toUid;
			std::string text;
			uint32_t m_uSeqId;

			virtual void marshal(sox::Pack &p) const
			{
				p << fromUid << toUid <<m_uSeqId<<text;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> fromUid >> toUid >>m_uSeqId>>text;
			}
		};

		//客户端向服务器上传离线消息
		struct PCS_OfflineMsgUpload : public sox::Marshallable 
		{
			enum {uri = (6 << 8) | protocol::IMOFFLINEMSG_SVID};
			PCS_OfflineMsgUpload (){}
			PCS_OfflineMsgUpload(uint32_t to, const std::string& msg,uint32_t uSeqId )//,uint32_t uSendTime)
				: id(to),text(msg),m_uSeqId(uSeqId)//,m_uSendTime(uSendTime)
			{

			}


			uint32_t id; //dest id on sending , src id on recieving
			std::string text;
			uint32_t m_uSeqId;
			//uint32_t m_uSendTime;//UTC时间

			virtual void marshal(sox::Pack &p) const
			{
				//p << id <<m_uSeqId<<m_uSendTime<<text;
				p << id <<m_uSeqId<<text;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				//p >> id >>m_uSeqId>>m_uSendTime>>text;
				p >> id >>m_uSeqId>>text;
			}
		};


		//服务端向客户端返回离线消息的处理（存储）结果

		struct PSC_OfflineMsgUploadRes: public sox::Marshallable 
		{
			PSC_OfflineMsgUploadRes (){}
			PSC_OfflineMsgUploadRes (uint32_t to, uint32_t seqid, uint32_t err)
				: toid(to), m_uSeqId(seqid), err_type(err){}

				enum {uri = (2 << 8) | protocol::IMOFFLINEMSG_SVID};
				uint32_t toid;
				uint32_t m_uSeqId;
				uint32_t err_type;

				virtual void marshal(sox::Pack &p) const 
				{
					p << toid << m_uSeqId;
					p.push_uint32(err_type);
				}

				virtual void unmarshal(const sox::Unpack &p) 
				{
					p >> toid >> m_uSeqId;
					err_type = p.pop_uint32();
				}
		};


		//客户端向服务端请求离线消息

		struct PCS_OfflineMsgRequest : public sox::Marshallable 
		{
			enum {uri = (3 << 8) | protocol::IMOFFLINEMSG_SVID};
			PCS_OfflineMsgRequest(){}

			virtual void marshal(sox::Pack &p) const
			{

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{

			}
		};

		//服务端向客户端下发离线消息

		//每条离线消息记录的结构
		struct  OfflineMsgRecord: public sox::Marshallable 
		{
			OfflineMsgRecord (){}
			OfflineMsgRecord (uint32_t  from, std::string& msg, uint32_t uSendTime)
				: id(from), text(msg),m_uSendTime(uSendTime)
			{

			}

			uint32_t id; 
			std::string text;
			uint32_t m_uSendTime;//UTC时间
			//std::string m_strSendTime;



			virtual void marshal(sox::Pack &p) const
			{
				p << id <<m_uSendTime<<text;



			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> id >>m_uSendTime>>text;

			}
		};
		//下发的离线消息.每个离线消息携带N条消息记录
        typedef std::vector<OfflineMsgRecord> OLMLIST;

		struct PSC_OfflineMsgRequestRes: public sox::Marshallable 
		{
			enum {uri = (4 << 8) | protocol::IMOFFLINEMSG_SVID};
			PSC_OfflineMsgRequestRes (){}


			//std::vector<OfflineMsgRecord>  msglist;
			OLMLIST   msglist;

			virtual void marshal(sox::Pack &p) const
			{

				sox::marshal_container(p, msglist);

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, std::back_inserter(msglist));
			}
		};




		//客户端收到下发离线消息后，向服务器发出确认消息


		struct PCS_OfflineMsgRequestResACK : public sox::Marshallable 
		{
			enum {uri = (5 << 8) | protocol::IMOFFLINEMSG_SVID};
			PCS_OfflineMsgRequestResACK (){}


			virtual void marshal(sox::Pack &p) const
			{

			}

			virtual void unmarshal(const sox::Unpack &p) 
			{

			}
		};

		//客户端向服务器上传离线消息(强制上传,不作在线检查)
		struct PCS_OfflineMsgUploadForce : public sox::Marshallable 
		{
			enum {uri = (7 << 8) | protocol::IMOFFLINEMSG_SVID};
			PCS_OfflineMsgUploadForce (){}
			PCS_OfflineMsgUploadForce(uint32_t to, const std::string& msg,uint32_t uSeqId )
				: id(to),text(msg),m_uSeqId(uSeqId)
			{
			}

			uint32_t id; //dest id on sending , src id on recieving
			std::string text;
			uint32_t m_uSeqId;

			virtual void marshal(sox::Pack &p) const
			{
				p << id <<m_uSeqId << text;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				p >> id >>m_uSeqId >> text;
			}
		};

		//add by kdjie, 2010.3.25

		//主协议用于取强制离线消息体

		struct OfflineMsgRecordEx : public OfflineMsgRecord
		{
			uint32_t m_uSeqId; //序列号
			uint32_t m_uType; //离线消息类型，1表示强制离线，2表示老版本普通离线

			OfflineMsgRecordEx() {};
			OfflineMsgRecordEx(uint32_t uFromId, std::string &strMsg, uint32_t uSendTime, uint32_t uSeqId, uint32_t uType) 
				: OfflineMsgRecord(uFromId, strMsg, uSendTime), m_uSeqId(uSeqId), m_uType(uType)
			{
			}

			virtual void marshal(sox::Pack &p) const
			{
				OfflineMsgRecord::marshal(p);
				p << m_uSeqId << m_uType;
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				OfflineMsgRecord::unmarshal(p);
				p >> m_uSeqId >> m_uType;;
			}
		};

		typedef std::vector<OfflineMsgRecordEx> OLMListEx;

		struct PCS_OfflineMsgRequestForce : public sox::Marshallable
		{
			enum {uri = (10 << 8) | protocol::IMOFFLINEMSG_SVID};

			PCS_OfflineMsgRequestForce() {};

			virtual void marshal(sox::Pack &p) const
			{
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};

		struct PSC_OfflineMsgRequestForceRes : public sox::Marshallable
		{
			enum {uri = (11 << 8) | protocol::IMOFFLINEMSG_SVID};

			OLMListEx m_MsgList;

			PSC_OfflineMsgRequestForceRes() {};

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_MsgList);
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, std::back_inserter(m_MsgList));
			}
		};

		//取普通离线消息数量

		struct PCS_GetOfflineMsgCount : public sox::Marshallable
		{
			enum {uri = (12 << 8) | protocol::IMOFFLINEMSG_SVID};

			PCS_GetOfflineMsgCount() {};

			virtual void marshal(sox::Pack &p) const
			{
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};

		struct PSC_GetOfflineMsgCountRes : public sox::Marshallable
		{
			enum {uri = (13 << 8) | protocol::IMOFFLINEMSG_SVID};

			//消息发送人-普通离线消息数量
			std::map<uint32_t, uint32_t> m_mapMsgCount;

			PSC_GetOfflineMsgCountRes() {};

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_mapMsgCount);
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
				sox::unmarshal_container(p, std::inserter(m_mapMsgCount, m_mapMsgCount.begin()));
			}
		};

		//新版本删除普通离线消息计数
		struct PCS_OfflineMsgCountACK : public sox::Marshallable 
		{
			enum {uri = (14 << 8) | protocol::IMOFFLINEMSG_SVID};

			PCS_OfflineMsgCountACK (){}

			virtual void marshal(sox::Pack &p) const
			{
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};

		//新版本删除强制离线消息
		struct PCS_OfflineMsgForceACK : public sox::Marshallable 
		{
			enum {uri = (15 << 8) | protocol::IMOFFLINEMSG_SVID};

			PCS_OfflineMsgForceACK (){}

			virtual void marshal(sox::Pack &p) const
			{
			}

			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};

		//add end
		
		struct PCS_GetPeerMessage: public sox::Voidmable
		{
			enum {uri = (16 << 8) | protocol::IMOFFLINEMSG_SVID};
		};

		struct PCS_RemovePeerMessage: public sox::Marshallable
		{
			enum {uri = (17 << 8) | protocol::IMOFFLINEMSG_SVID};
			std::vector<uint32_t> m_vecMsgId;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecMsgId);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecMsgId));
			}
		};

		struct PCS_BatchPeerMessage: public sox::Marshallable
		{
			enum {uri = (18 << 8) | protocol::IMOFFLINEMSG_SVID};
			std::vector<PCS_PeerMessage> m_vecMsg;

			virtual void marshal(sox::Pack &p) const
			{
				sox::marshal_container(p, m_vecMsg);
			}

			virtual void unmarshal(const sox::Unpack &p)
			{
				sox::unmarshal_container(p, std::back_inserter(m_vecMsg));
			}
		};

		struct PCS_OfflineMsgRequestForce4Mobile : public sox::Marshallable
		{
			enum {uri = (19 << 8) | protocol::IMOFFLINEMSG_SVID};

			PCS_OfflineMsgRequestForce4Mobile() {};

			virtual void marshal(sox::Pack &p) const
			{
			}
			virtual void unmarshal(const sox::Unpack &p) 
			{
			}
		};

	}
}



#endif 
 
