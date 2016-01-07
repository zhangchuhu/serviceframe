#ifndef __ImLinkIdHelper_h__
#define  __ImLinkIdHelper_h__

#include <string.h>
#include <algorithm>
#include<map>
#include<vector>
#include"./ConsistentHashing.h"
#include"server_common/server-lib/iclient.h"

namespace server
{
	class ImLinkIDHelper
	{
	public:
		ImLinkIDHelper();

		//��ӷ�����
		bool addServer(uint32_t imlinkid);

		//ɾ��������
		bool removeServer(uint32_t imlinkid);

		//��uidת����imlinkid,���imlinkid����Ϊ��,���� NONE_SERVER_ID
		uint32_t uid2Imlinkid(uint32_t uid);

		//imlinkid�Ƿ����
		bool imlinkidExists(uint32_t imlinkid);

		//ȡȫ��imlink����
		void getImLinkMapSet(std::vector<uint32_t> &vtImLinkSet);

		void clearServer();
	private:
		UserConsistentHash m_hash;
		std::vector<uint32_t> m_imLinkdSet;			//imlinkd�ļ���
	};

	class WatchImLinkDaemonClient : public core::IDaemonClientAwareW
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

		//Imlinkd�����仯
		virtual void onLinkIDSetChanged(){}
	protected:
		ImLinkIDHelper m_imlinkIdHelper;
	};
}

#endif		//__ImLinkIdHelper_h__
