#ifndef __ImTransferIdHelper_h__
#define __ImTransferIdHelper_h__

#include <string.h>
#include <algorithm>
#include<map>
#include<vector>
#include"./ConsistentHashing.h"
#include"server_common/server-lib/iclient.h"

namespace server
{
	class ImTransferIDHelper
	{
	public:
		ImTransferIDHelper();

		//��ӷ�����
		bool addServer(uint32_t imtransid);

		//ɾ��������
		bool removeServer(uint32_t imtransid);

		//��uidת����imlinkid,���imlinkid����Ϊ��,���� NONE_SERVER_ID
		uint32_t uid2ImTransid(uint32_t uid);

		//imlinkid�Ƿ����
		bool imtransidExists(uint32_t imtransid);

		//ȡȫ��imlink����
		void getImtransMapSet(std::vector<uint32_t> &vtImTransSet);

		void clearServer();

	private:
		UserConsistentHash m_hash;
		std::vector<uint32_t> m_imTransSet;			//imtransfer�ļ���
	};

	class WatchImTransferDaemonClient : public core::IDaemonClientAwareW
	{
	public:
		//������˼�е������,Ӧ���ǳ�ʼ��ʱDaemon������ǰ�������б�
		virtual void onRefreshSV();

		virtual void onRecoverRefreshSV();
		
		//����������
		virtual void onAddSV(const sdaemon::FavorStrSV & fs);		

		//������ɾ��
		virtual void onServerRemovedSV(const sdaemon::FavorStrSV & fs);

		virtual void onServerStatusChangeSV(const sdaemon::FavorStrSV & fs);

		//ImTransfer�����仯
		virtual void onTransferIDSetChanged(){}

	protected:
		ImTransferIDHelper m_imTransIdHelper;
	};
}

#endif		//__ImTransferIdHelper_h__
