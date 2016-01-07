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
		uint32_t m_linkdID;          // ����imlinkd
		uint32_t m_transID;          // ����imtransfer

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

	// ˫�߲���ʱ��ѡ�����ڹ㲥��imtransfer
	class ICImTransHelper
	{
	public:
		virtual ~ICImTransHelper() {}

		// ����uid���û�����imlinkd�������ͣ���ȡimtransfer
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

		// ����transferID���͹㲥��
		virtual void BroadcastS(uint32_t uid,
							uint32_t transId,
							uint32_t uri,
							const sox::Marshallable& obj) = 0;

		// ����transferID����ת����
		virtual void FowardS(uint32_t uid,
							uint32_t transId,
							const vector<uint32_t> &vecRecvUid,
							uint32_t uri,
							const sox::Marshallable& obj) = 0;

		// �㲥������imtransfer
		virtual void RouteAllT(uint32_t uUri, const sox::Marshallable& obj) = 0;

		//��������Ҫ�����ǽ��ض��İ�·�ɸ�hashset(uid)���imtransfer
		//
		// Uid :        ������Uid
		// linkdId��    ����������imlinkd��ID
		// netType:     ������������������ CTL��CNC
		// Uri��        ԭ��uri
		// objMarshall������
		virtual bool RouteT(uint32_t uid, uint32_t linkdId, uint32_t uri, const sox::Marshallable &objMarshal) = 0;
		virtual bool RouteNetTypeT(uint32_t uid, uint32_t netType, uint32_t uri, const sox::Marshallable &objMarshal) = 0;
		virtual bool RouteAsAnswerT(uint32_t uid, uint32_t uri, const sox::Marshallable &objMarshal) = 0;


		//��������Ҫ�����ǽ�һ�����еİ������PSS_Broadcast��������imtransfer�㲥
		//
		// Uid :        ������Uid
		// linkdId��    ����������imlinkd��ID
		// netType:     ������������������ CTL��CNC
		// Uri��        ԭ��uri
		// objMarshall������
		virtual bool BroadcastT(uint32_t uid, uint32_t linkdId, uint32_t uri, const sox::Marshallable &obj) = 0;
		virtual bool BroadcastNetTypeT(uint32_t uid, uint32_t netType, uint32_t uri, const sox::Marshallable &objMarshal) = 0;
		virtual bool BroadcastAsAnswerT(uint32_t uid,  uint32_t uri, const sox::Marshallable &obj) = 0;

		
		//��������Ҫ�����ǽ�һ�����еİ������PSS_Forward��������imtransferת��
		//
		// Uid :        ������Uid
		// linkdId��    ����������imlinkd��ID
		// netType:     ������������������ CTL��CNC
		// vecRecvUid:  ������Uids
		// Uri��        ԭ��uri
		// objMarshall������
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

///////////////////////////////���߳�ʵ��/////////////////////////
	
	class CImTransHelper : 
		public ICImTransHelper, 
		public CImServersInfo,
		public core::IRouteTarget
	{
		// server::ImTransferIDHelper	m_ImTransIdHelper;
		// ���Ż���
		ImTransferIDHelper m_CTLHelper;

		// ��ͨ���� 
		ImTransferIDHelper m_CNCHelper;

	public:

		// ����uid���û�����imlinkd�������ͣ���ȡimtransfer
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

		// ����transferID���͹㲥��
		virtual void BroadcastS(uint32_t uid,
							uint32_t transId,
							uint32_t uri,
							const sox::Marshallable& obj);

		// ����transferID����ת����
		virtual void FowardS(uint32_t uid,
							uint32_t transId,
							const vector<uint32_t> &vecRecvUid,
							uint32_t uri,
							const sox::Marshallable& obj);

		// �㲥������imtransfer
		virtual void RouteAllT(uint32_t uUri, const sox::Marshallable& obj);

		//��������Ҫ�����ǽ��ض��İ�·�ɸ�hashset(uid)���imtransfer
		//
		// Uid :        ������Uid
		// linkdId��    ����������imlinkd��ID
		// netType:     ������������������ CTL��CNC
		// Uri��        ԭ��uri
		// objMarshall������
		virtual bool RouteT(uint32_t uid, 
							uint32_t linkdId, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		virtual bool RouteNetTypeT(uint32_t uid, 
							uint32_t netType, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);
		
		// answer to imlinkd ���ƣ�
		// ע�⣺ֻ�ʺ��ڿͻ��˷�������Ϣ
		virtual bool RouteAsAnswerT(uint32_t uid, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		
		//��������Ҫ�����ǽ�һ�����еİ������PSS_Broadcast��������imtransfer�㲥
		//
		// Uid :        ������Uid
		// linkdId��    ����������imlinkd��ID
		// netType:     ������������������ CTL��CNC
		// Uri��        ԭ��uri
		// objMarshall������
		virtual bool BroadcastT(uint32_t uid, 
							uint32_t linkdId, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		virtual bool BroadcastNetTypeT(uint32_t uid, 
							uint32_t netType, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		// answer to imlinkd ���ƣ�
		// ע�⣺ֻ�ʺ��ڿͻ��˷�������Ϣ
		virtual bool BroadcastAsAnswerT(uint32_t uid,  
							uint32_t uri, 
							const sox::Marshallable &objMarshal);
		
		//��������Ҫ�����ǽ�һ�����еİ������PSS_Forward��������imtransferת��
		//
		// Uid :        ������Uid
		// linkdId��    ����������imlinkd��ID
		// netType:     ������������������ CTL��CNC
		// vecRecvUid:  ������Uids
		// Uri��        ԭ��uri
		// objMarshall������
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

		// answer to imlinkd ���ƣ�
		// ע�⣺ֻ�ʺ��ڿͻ��˷�������Ϣ
		virtual bool ForwardAsAnswerT(
							uint32_t uid, 
							const vector<uint32_t> &vecRecvUid, 
							uint32_t uri, 
							const sox::Marshallable &objMarshal);

		//---------------------------------------------------------------------
		//IDaemonClient �ӿ�ʵ��

		virtual void onRefreshSV();

		virtual void onAddSV(const sdaemon::FavorStrSV & fs);

		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);

		virtual void onRecoverRefreshSV();

		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

		//---------------------------------------------------------------------

		// ��ԭ��ת��string
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

		// �����û�����limlinkd������������Ӧimtransfer
		virtual bool _RouteBySrvId(uint32_t Uid, uint32_t linkdId, uint32_t Uri, const sox::Marshallable &obj);

		// �����û�����limlinkd�������ͣ�����������Ӧimtransfer
		virtual bool _RouteByNetType(uint32_t Uid, uint32_t netType, uint32_t Uri, const sox::Marshallable &obj);

	private:

		void __AddServer(const sdaemon::FavorStrSV&);
	};

}

#endif
