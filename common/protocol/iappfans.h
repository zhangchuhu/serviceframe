#ifndef __IIMAPPFANS_H__
#define __IIMAPPFANS_H__

#include <string>
#include "common/unistr.h"
#include "const.h"
#include "iImV2.h"
#include "pmsg.h"
#include "iimsearch.h"
#include <exception>

namespace protocol{
	namespace imapp{

		// ��ϵ������
		enum ContactProperty {
			FAN = 0,               // ��˿
			CARE = 1,              // ��ע����
		};

		struct CSlimContactInfo
		{
			uint32_t uid;          // ��ϵ�˵�uid
			std::string nickname;  // ��ϵ�˵��ǳ�
		};

		// ��������ϵ����Ϣ
		struct CContactInfo
		{
			uint32_t uid;            // ��ϵ�˵�uid
			std::string nickname;    // �ǳ�
			uint32_t head_image_version;       // ͷ��Сͼ
			bool vip;               // �Ƿ�vip��true��ʾ��vip��false��ʾ����vip
			bool bAlreadyDetail;	 //�Ƿ��Ѿ���ȡ����ϸ��Ϣ
		};

		struct IAppJyFansRegister  
		{
			//��ȡ�����Ժ����б�
			virtual void OnGetSlimContactList(const ContactProperty &type, const std::vector<CSlimContactInfo> &vtData) = 0;

			//��ȡ����ϸ������Ϣ
			virtual void OnGetContactInfoList(const std::vector<CContactInfo> &vtData) = 0;

			//��ӷ�˿��Ӧ
			virtual void OnAddRes(const uint32_t &uid, const uint32_t &operationResult) = 0;
			
			//ɾ����˿��Ӧ
			virtual void OnDelRes(const uint32_t &uid,const uint32_t &operationResult) = 0;

			//��ĳ�˹�ע
			virtual void OnFansAdd(const uint32_t &bid) = 0;
		};

		struct IAppJyFansWatcher//�ײ���Ҫʵ�ֵĽӿ�,��UI����
		{
			virtual ~IAppJyFansWatcher(){};
			virtual void watch(IAppJyFansRegister *w) = 0;
			virtual void revoke(IAppJyFansRegister *w) = 0;
		};


		class IAppJyFansBase : virtual public IAppJyFansWatcher
		{
		public:
			//��ȡ��ϵ����Ϣ��Ӧ
			virtual void GetSlimContactList(ContactProperty Type) = 0;

			//��ȡ��˿��ϸ��Ϣ(�����һ�ε���,�ֿ�����������λص��Ŀ���,ԭ���Ǳ�������л�����ǲ���,ֱ�ӷ���,û�л������һ����,��������ȡ���ٷ���)
			virtual void GetContactInfo(const std::vector<uint32_t>& contactIDs) = 0;

			//��עĳ��
			virtual void AddFans(const uint32_t uPeerId, const std::string& nickname) = 0;

			//���ٹ�עĳ��
			virtual void DelFans(const uint32_t uPeerId) = 0;

			//ͬ���ӿ�,���ݱ��ػ���,ֱ�ӷ����Ƿ��Ƿ�˿���ע����
			virtual bool IsMyFans(uint32_t uid, ContactProperty eMyType) = 0;
		};


	};
};


#endif //__IIMAPPFANS_H__
