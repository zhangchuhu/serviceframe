#ifndef SERVER_BROADCAST_H_
#define SERVER_BROADCAST_H_

#include <map>
#include <string>
#include <vector>
#include <set>
#include "common/iproperty.h"
#include "common/protocol/const.h"
#include "common/core/base_svid.h"

#include "server_common/server-lib/BRouteWriter.h"
#include "server_common/server-lib/id_def.h"

#include "server_common/protocol/pimtrans.h"
#include "common/packet.h"
#include "common/core/sender.h"

#include "common/core/form.h"
#include "common/core/ibase.h"
#include "common/core/iserver.h" //define IDaemonServerAwareW

#include "server_common/server-lib/iclient.h" //define IDaemonClientAwareW
#include "server_common/server-lib/RouteTarget.h" //define IRouteTarget

#include "ImTransferIDHelper.h"
#include "ImServersInfo.h"

namespace server
{
	struct CIMUserTransfer : public sox::Marshallable
	{
		uint32_t m_uid;              // uid
		uint32_t m_linkdID;          // 所在imlinkd
		uint32_t m_transID;          // 所属imtransfer

		CIMUserTransfer() : m_uid(-1), m_linkdID(-1), m_transID(-1)
		{
		}

		CIMUserTransfer(uint32_t uid, uint32_t linkdID) : m_uid(uid), m_linkdID(linkdID), m_transID(-1)
		{
		}

		CIMUserTransfer(uint32_t uid, uint32_t linkdID, uint32_t transID) : m_uid(uid), m_linkdID(linkdID), m_transID(transID)
		{
		}

		virtual void marshal(sox::Pack &pk) const 
		{
			pk << m_uid << m_linkdID << m_transID;
		}

		virtual void unmarshal(const sox::Unpack &up) 
		{
			up >> m_uid >> m_linkdID >> m_transID;
		}
	};

	// 双线部署时，选择用于广播的imtransfer
	class ICImTransHelper
	{
	public:
		virtual ~ICImTransHelper() {}

		// 根据uid和用户所在imlinkd网络类型，获取imtransfer
		virtual uint32_t GetTransferID(uint32_t Uid, uint32_t netType) = 0;

		virtual void BatchGetTransferID(const std::vector<uint32_t>& vecUid,
							std::vector<uint32_t>& vecTransId,
							uint32_t netType) = 0;

		virtual void BatchGetTransferID(const std::set<uint32_t>& setUid,
							std::vector<uint32_t>& vecTransId, 
							uint32_t netType) = 0;

		virtual void BatchGetTransferByimlinkd(const std::vector<uint32_t>& vecUid,
							std::vector<uint32_t>& vecTransId,
							uint32_t linkdId) = 0;

		virtual void BatchGetTransferByimlinkds(std::vector<CIMUserTransfer>& vec) = 0;

		// 根据transferID发送广播包
		virtual void BroadcastS(uint32_t uid,
							uint32_t transId,
							uint32_t uri,
							const sox::Marshallable& obj) = 0;

		// 根据transferID发送转发包
		virtual void FowardS(uint32_t uid,
							uint32_t transId,
							const vector<uint32_t> &vecRecvUid,
							uint32_t uri,
							const sox::Marshallable& obj) = 0;

		// 广播给所有imtransfer
		virtual void RouteAllT(uint32_t uUri, const sox::Marshallable& obj) = 0;

		//本函数主要功能是将特定的包路由给hashset(uid)后的imtransfer
		//
		// Uid :        发送者Uid
		// linkdId：    发送者所在imlinkd的ID
		// netType:     发送者所在网络类型 CTL、CNC
		// Uri：        原包uri
		// objMarshall：包体
		virtual bool RouteT(uint32_t uid, uint32_t linkdId, uint32_t uri, const sox::Marshallable &objMarshal) = 0;
		virtual bool RouteNetTypeT(uint32_t uid, uint32_t netType, uint32_t uri, const sox::Marshallable &objMarshal) = 0;
		virtual bool RouteAsAnswerT(uint32_t uid, uint32_t uri, const sox::Marshallable &objMarshal) = 0;


		//本函数主要功能是将一个已有的包打包成PSS_Broadcast，并交给imtransfer广播
		//
		// Uid :        发送者Uid
		// linkdId：    发送者所在imlinkd的ID
		// netType:     发送者所在网络类型 CTL、CNC
		// Uri：        原包uri
		// objMarshall：包体
		virtual bool BroadcastT(uint32_t uid, uint32_t linkdId, uint32_t uri, const sox::Marshallable &obj) = 0;
		virtual bool BroadcastNetTypeT(uint32_t uid, uint32_t netType, uint32_t uri, const sox::Marshallable &objMarshal) = 0;
		virtual bool BroadcastAsAnswerT(uint32_t uid,  uint32_t uri, const sox::Marshallable &obj) = 0;

		
		//本函数主要功能是将一个已有的包打包成PSS_Forward，并交给imtransfer转发
		//
		// Uid :        发送者Uid
		// linkdId：    发送者所在imlinkd的ID
		// netType:     发送者所在网络类型 CTL、CNC
		// vecRecvUid:  接收者Uids
		// Uri：        原包uri
		// objMarshall：包体
		virtual bool ForwardT(
					uint32_t uid, 
					uint32_t linkdId, 
					const vector<uint32_t> &vecRecvUid, 
					uint32_t uri, 
					const sox::Marshallable &obj) = 0;

		virtual bool ForwardNetTypeT(
					uint32_t uid,
					uint32_t netType,
					const vector<uint32_t> &vecRecvUid, 
					uint32_t uri, 
					const sox::Marshallable &objMarshal) = 0;

		virtual bool ForwardAsAnswerT(
					uint32_t uid, 
					const vector<uint32_t> &vecRecvUid, 
					uint32_t uri, 
					const sox::Marshallable &obj) = 0;

	};

	class ICImTransHelperAware
	{
	protected:
		ICImTransHelper *m_transHelper;
	public:
		virtual ~ICImTransHelperAware(){}
		virtual void SetImTransHelper(ICImTransHelper *p)
		{
			m_transHelper = p;
		}
	};

///////////////////////////////单线程实现/////////////////////////
	
	class CImTransHelper : 
		public ICImTransHelper, 
		public CImServersInfo,
		public core::IRouteTarget
	{
		// server::ImTransferIDHelper	m_ImTransIdHelper;
		// 电信机房
		ImTransferIDHelper m_CTLHelper;

		// 网通机房 
		ImTransferIDHelper m_CNCHelper;

	public:

		// 根据uid和用户所在imlinkd网络类型，获取imtransfer
		virtual uint32_t GetTransferID(uint32_t Uid, uint32_t netType);

		virtual void BatchGetTransferID(const std::vector<uint32_t>& vecUid,
									std::vector<uint32_t>& vecTransId,
									uint32_t netType);

		virtual void BatchGetTransferID(const std::set<uint32_t>& setUid,
									std::vector<uint32_t>& vecTransId, 
									uint32_t netType);

		virtual void BatchGetTransferByimlinkd(const std::vector<uint32_t>& vecUid,
									std::vector<uint32_t>& vecTransId,
									uint32_t linkdId);

		virtual void BatchGetTransferByimlinkds(std::vector<CIMUserTransfer>& vec);

		// 根据transferID发送广播包
		virtual void BroadcastS(uint32_t uid,
							uint32_t transId,
							uint32_t uri,
							const sox::Marshallable& obj);

		// 根据transferID发送转发包
		virtual void FowardS(uint32_t uid,
							uint32_t transId,
							const vector<uint32_t> &vecRecvUid,
							uint32_t uri,
							const sox::Marshallable& obj);

		// 广播给所有imtransfer
		virtual void RouteAllT(uint32_t uUri, const sox::Marshallable& obj);

		//本函数主要功能是将特定的包路由给hashset(uid)后的imtransfer
		//
		// Uid :        发送者Uid
		// linkdId：    发送者所在imlinkd的ID
		// netType:     发送者所在网络类型 CTL、CNC
		// Uri：        原包uri
		// objMarshall：包体
		virtual bool RouteT(uint32_t uid, 
							uint32_t linkdId, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		virtual bool RouteNetTypeT(uint32_t uid, 
							uint32_t netType, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);
		
		// answer to imlinkd 类似，
		// 注意：只适合于客户端发来的消息
		virtual bool RouteAsAnswerT(uint32_t uid, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		
		//本函数主要功能是将一个已有的包打包成PSS_Broadcast，并交给imtransfer广播
		//
		// Uid :        发送者Uid
		// linkdId：    发送者所在imlinkd的ID
		// netType:     发送者所在网络类型 CTL、CNC
		// Uri：        原包uri
		// objMarshall：包体
		virtual bool BroadcastT(uint32_t uid, 
							uint32_t linkdId, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		virtual bool BroadcastNetTypeT(uint32_t uid, 
							uint32_t netType, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		// answer to imlinkd 类似，
		// 注意：只适合于客户端发来的消息
		virtual bool BroadcastAsAnswerT(uint32_t uid,  
							uint32_t uri, 
							const sox::Marshallable &objMarshal);
		
		//本函数主要功能是将一个已有的包打包成PSS_Forward，并交给imtransfer转发
		//
		// Uid :        发送者Uid
		// linkdId：    发送者所在imlinkd的ID
		// netType:     发送者所在网络类型 CTL、CNC
		// vecRecvUid:  接收者Uids
		// Uri：        原包uri
		// objMarshall：包体
		virtual bool ForwardT(
							uint32_t uid,
							uint32_t linkdId,
							const vector<uint32_t> &vecRecvUid, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		virtual bool ForwardNetTypeT(
							uint32_t uid,
							uint32_t netType,
							const vector<uint32_t> &vecRecvUid, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		// answer to imlinkd 类似，
		// 注意：只适合于客户端发来的消息
		virtual bool ForwardAsAnswerT(
							uint32_t uid, 
							const vector<uint32_t> &vecRecvUid, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		//---------------------------------------------------------------------
		//IDaemonClient 接口实现

		virtual void onRefreshSV();

		virtual void onAddSV(const sdaemon::FavorStrSV & fs);

		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);

		virtual void onRecoverRefreshSV();

		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

		//---------------------------------------------------------------------

		// 将原包转成string
		static std::string Pack2String(const sox::Marshallable &obj)
		{
			sox::PackBuffer pb;
			sox::Pack pk(pb);
			obj.marshal(pk);
			std::string strOriPacket;
			strOriPacket.assign(pk.data(), pk.size());

			return strOriPacket;
		}

	protected:

		// 根据用户所在limlinkd，将包发往相应imtransfer
		virtual bool _RouteBySrvId(uint32_t Uid, uint32_t linkdId, uint32_t Uri, const sox::Marshallable &obj);

		// 根据用户所在limlinkd网络类型，将包发往相应imtransfer
		virtual bool _RouteByNetType(uint32_t Uid, uint32_t netType, uint32_t Uri, const sox::Marshallable &obj);

	private:

		void __AddServer(const sdaemon::FavorStrSV&);
	};

}

#endif
