#ifndef IGTOPICMSG_MANAGER_H
#define IGTOPICMSG_MANAGER_H

//#include <vector>
#include "common/protocol/PGTopicMsg.h"

namespace protocol
{
	namespace gmsgcache
	{
		class IGTopicMsgManagerWatcher
		{
		public:
			// ɾ��ĳ��Ա��Ⱥ�������������(ȫ������ͻظ�)�Ķ�������Ӧ
			// rescode : ����ɹ�,���� RES_SUCCESS
			//			 ����Ϊ���� RES_ERETRY	
			// vecFidsΪ UpdateGMemberMsgs �����fid(s)����
			//   ���vecFidsΪ�գ�˵��ɾ������Ⱥ���������з����µ�����(*��ǰ��������)
			//   �����Ϊ�գ���˵��ɾ������gid->fid(s)�µ���������
			virtual void OnUpdateGMemberMsgsRes(const uint32_t& rescode,	  // response code
									const uint32_t& uid,         		  	  // ɾ�������Ĳ�������
									const uint32_t& groupId,                  // ��ɾ����������Ⱥ
									const std::vector<uint32_t>& vecFids) = 0;// ��ɾ������������

			// ɾ��ĳ��Ա��Ⱥ(����������(ȫ������ͻظ�)�Ķ�����ʱ
			// 	vecFidsΪ UpdateGMemberMsgs �����fid(s)����
			//   ���vecFidsΪ�գ�˵��ɾ������Ⱥ���������з����µ�����(*��ǰ��������)
			//   �����Ϊ�գ���˵��ɾ������gid->fid(s)�µ���������
			virtual void OnGMemberUpdateMsgsTimeout(const uint32_t& uid,     // ɾ�������Ĳ�������
									const uint32_t& groupId,                 // ��ɾ����������Ⱥ
									const std::vector<uint32_t>& vecFids) = 0;	// ��ɾ������������									
		};

		class IOpGTopicMsgManager
		{
		public:

			virtual void watch(IGTopicMsgManagerWatcher *w) = 0;

			virtual void revoke(IGTopicMsgManagerWatcher *w) = 0;
		};

		class IGTopicMsgManager : public virtual IOpGTopicMsgManager
		{
		public:

			virtual ~IGTopicMsgManager() {}

			// ����������� ɾ��ĳ��Ա��Ⱥ�������������(ȫ������ͻظ�)��Ŀǰ��Ⱥ(!)����Ա���Բ�����
			// uid: ��ɾ������uid,
			// groupId: ��Ա����ȺID,
			// vecFids: ��Ա������ID, ���vector��Ҫ�ǿ��ǵ�������չ��
			// 	1) �����ɾ��uid��gid��ĳ��fid��������ۣ������뵥��fid
			//  2) �����Ҫɾ��uid��gid�¶��fid���������, ��������fid(s)
			//  3) (*Ŀǰ��Ҫ��������)�����Ҫɾ��uid��gid��(����gid���з���fid(s))����������, vecFids���ÿգ�
			// 
			// text: ɾ���ظ�ʱ�ã��ͻ�������ɾ��֮��ԭ�ظ�����ʾ�����ݡ�
			virtual void UpdateGMemberMsgs(uint32_t uid, uint32_t groupId, const std::vector<uint32_t>& vecFids, 
												const protocol::gmsgcache::GTopicTextChat& text) = 0;
		};		
	}
}

#endif
