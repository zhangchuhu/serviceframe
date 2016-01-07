#ifndef IApply_h__
#define IApply_h__
#include "common/protocol/const.h"
#include "ChannelRoler.h"
namespace protocol{
	namespace uinfo{
		//Ƶ����ID��Ƶ�����ǳƣ���׼�˵�userID,��׼�˵��ǳƣ���ɫ��ͬ���  
		struct ApplyPassInfo{
			std::wstring sid;
			std::wstring asid;
			std::wstring channelName;
			std::wstring adminUid;
			std::wstring adminNick;
			protocol::session::ChannelRoler role;
			bool		 bPass;
		};
	
		struct IApplyResultWatcher{
			virtual void onApplyPassResult(const ApplyPassInfo &) = 0;
		};

		struct IApplyResultWatchBase{
			virtual void watch(IApplyResultWatcher *w) = 0;
			virtual void revoke(IApplyResultWatcher *w) = 0;
		};
		
		struct IApplyWatcher{
			/*
				�÷���
				UIӦ���ȱ��浱ǰ�����Ƶ��sid���ڻص�����ʱ�ͻش���sid��Ƚϣ�
				������ȣ�˵���������Ƶ���ĵĻص�Ӧ���Զ�������
			*/
			virtual void onApply(const std::wstring &sid, ApplyResult res)= 0;
			virtual void onPrepareCheck(const std::wstring &sid, ApplyResult res)= 0;
		};

		struct IApplyGuildWatchBase{
			virtual void watch(IApplyWatcher *w) = 0;
			virtual void revoke(IApplyWatcher *w) = 0;
		};

		//�ýӿ�����Ƶ�����������
		struct IApplyGuild: public IApplyGuildWatchBase{
			virtual ~IApplyGuild(){}
			virtual void apply(const std::wstring &sid, const std::wstring &reason) = 0;
			virtual void prepareCheck(const std::wstring  &sid) = 0;

			virtual void watchResult(IApplyResultWatcher *w) = 0;
			virtual void revokeResult(IApplyResultWatcher *w) = 0;
		};

		struct ApplyGuildInfo{
			std::wstring sid;
			std::wstring asid;
			std::wstring name;
			std::wstring applyReason;
		};

		struct IApplyGuildListWatcher{
			virtual void onRefresh(const std::vector<ApplyGuildInfo> &) = 0;
		};

		struct IApplyGuildListWatchBase{
			virtual void watch(IApplyGuildListWatcher *) = 0;
			virtual void revoke(IApplyGuildListWatcher *) = 0;
		};
		//������ע����ȡ
		//�ýӿ�������ʾ�����б�
		struct IApplyGuildList: public IApplyGuildListWatchBase{
			virtual ~IApplyGuildList(){}
			//�ȴ����������أ���Զ�ɹ�
			virtual void abandon(const std::wstring &sid) = 0;
			//������������ȡ�����б�
			virtual void sync() = 0;
		};

	}
}
#endif // IApply_h__