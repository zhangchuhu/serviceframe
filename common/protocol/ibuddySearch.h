#pragma once

#include "common/protocol/iImV2.h"

#include "common/protocol/ibuddylist.h"
#include "common/protocol/ilist.h"
#include "common/iproperty.h"

//wuji start
//�⼾�趨��Ľӿ����ֹ�������

//�������Asny��ͷ�������ú������첽��������������Ҫ�ص�������ӳ�����������)


//wuji end
namespace protocol
{
	namespace im
	{
		struct IBuddySearchWatcher//UIҪʵ�ֵĽӿڣ��������յ���Ϣ��Ҫ�ص���UI�ӿ�
		{
			virtual ~IBuddySearchWatcher()
			{

			}

			//return the user's basic info and online status
			virtual void onSearchRes(const HULI& hid, const uint32_t& resCode, const protocol::E_IMSTATUS& st) = 0;
			//virtual void onSearchRes(const sox::properties_type& binfo) = 0;

			//���ڱ�ʾ���󱻷���������(����������ظ�)
			//virtual void onReqAddBuddyRes(const HULI& bid, const UniString& nickname) = 0;

			//���ڱ�ʾ��Ӻ������󱻶Է��ܾ����ǽ���
			virtual void onPeerResp(const HULI& peerid, const E_WAITCHECKLIST_OP& op, const UniString& msg_left) = 0;

			virtual void onReqAddMe(const HULI& peerid, const UniString& left_msg) = 0;

			//wuji start
// 			*  @param hid  Ҫ�����û���UID��
// 				*  @param resCode  �����û���״̬�룬RES_SUCCESS��ʾ���ڣ�RES_ENONEXIST ��ʾ�����ڡ�
// 				*  @param SecurityCode  ������֤�롣���ռ�Ϊ����Ҫ��֤�룬�����ע���룬������Ҫ�����û�����ע���룬������ʾ�����Ľ����

//			virtual void onSearchRes(const HULI& hid, const uint32_t& resCode, const protocol::E_IMSTATUS& st,const UniString& SecurityCode) = 0;
			virtual void onNeedCode(const UniString& SecurityCode) = 0;//������������Ҫ��֤��

// 			/*
			//ͨ������ص���ȷ�������û�����֤����
// 			  @param buddyID , ���Ӻ��ѵ���֤��Ϣ���͡�ͨ������ֵ�Ĳ�ͬ����������ͬ����ʾ��*  ��style �������ʱ��msg �ŷǿգ���ʾ���⣬��������¿ա�
// 			*/
			
			//Asny_GetBuddyCeLue�Ļص�
			virtual void onBuddyValidateStyle(const  HULI& buddyID,const E_UI_CELUE_OP& uCeLue,const uint32_t& uQuestionId,const UniString& msg)=0;			


			virtual void OnReqAddBuddy_ByJiFenRes(const HULI& buddyID,const bool& bIsOk,const uint32_t& uJiFen) = 0;//ʹ�û���������Ӻ��ѵ�UI�ص�

			virtual void OnReqAddBuddy_QuestionRes(const  HULI& buddyID,const bool& bIsOk) = 0;//ʹ�ûش�����������Ӻ��ѵ�UI�ص�

			virtual void OnReqAddBuddy_ByOpRes(const  HULI& buddyID,const bool& bIsOk) = 0;//ʹ������1��Ӻ��ѵ�UI�ص�

			

			//wuji end

			//add by myf

			/*Ƶ����Ӻ���UI�ص�
			* ��Ӧ�ӿ�:Asny_ReqChannelAddBuddy
			* @param uResCmd: ����״̬ö��
			* @param bid: ������û���uid
			* @param valid_key: ��֤���Key,  ���û���Ҫ������֤��ʱ,valid_keyΪ�ǿ�
			* @param bin: ��֤��ͼƬ���׵�ַ,���û���Ҫ������֤��ʱ,binΪ�ǿ�.
			* @param size: ��֤��ͼƬ��ռ�ڴ泤��;���û���Ҫ������֤��ʱ,sizeΪ��0
			*/
			virtual void OnChannelAddBuddyRes(const E_UI_CHANNEL_ADD_BUDDY_RES_TYPE &uResCmd, const HULI& bid, const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) = 0;

			/*��������UI�ص�
			* ��Ӧ�ӿ�:Asny_searchBuddy2
			* @param resCode: ����״̬ö��
			* @param uid: ������û���uid
			* @param st: ������û�������״̬
			* @param valid_key: ��֤���Key,  ���û���Ҫ������֤��ʱ,valid_keyΪ�ǿ�
			* @param bin: ��֤��ͼƬ���׵�ַ,���û���Ҫ������֤��ʱ,binΪ�ǿ�.
			* @param size: ��֤��ͼƬ��ռ�ڴ泤��;���û���Ҫ������֤��ʱ,sizeΪ��0
			*/
			virtual void onSearchRes2(const E_UI_CHANNEL_ADD_BUDDY_RES_TYPE &resCode, const HULI& uid, const protocol::E_IMSTATUS& st, const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) = 0;
			
			//myf add end

			// 3.6 ++
			//ȡ���Ѳ�����Ҫ��֤��
			virtual void OnGetBuddyCeLueVerifyRes(const uint32_t &uBid, const E_UI_CELUE_OP& uCeLue, const UniString& msg,
				const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) {};

			//��֤�Ӻ�����Ҫ��֤��
			virtual void OnReqAddBuddyNeedVerify(const E_UI_CHANNEL_ADD_BUDDY_RES_TYPE &resCode, const uint32_t& uBid, const uint32_t &uPid, const UniString& msg_left,
				const std::wstring& valid_key,  const char* const & bin, const uint32_t &size) {};
		};

		struct IOpBuddySearchWatcher
		{
			virtual ~IOpBuddySearchWatcher(){}
			virtual void watch(IBuddySearchWatcher *w) = 0;
			virtual void revoke(IBuddySearchWatcher *w) = 0;
		};	

		struct IBuddySearch : public virtual IOpBuddySearchWatcher//�ײ����ʵ�ֵĽӿ�,��UIʹ��
			
		{
			virtual ~IBuddySearch(){}
			virtual void searchBuddy(const UniString& imid) = 0;
			virtual void ReqAddBuddy(const HULI &hid/*������˵�uid*/, const HULI &pid, const UniString& msg_left) = 0;//��������Ҫ���������֤����Ӻ��ѷ�ʽ

			//wuji start

			/*
			*  ��չ�ӿڣ�������һ����֤����������û������Ĵ�������3��ʱ�򣬾ͻ������֤�롣
			*  �����������ѵġ��ص��ӿ�onSearchRes
			*  @param hid, �û�UID
			*/
			virtual void Asny_searchBuddy(const UniString& imid, const UniString& securityCode) = 0;


			virtual void Asny_ReqAddBuddy_ByJiFen(const HULI &hid/*������˵�uid*/, const HULI &pid) = 0;//��������Ҫͨ��������֤����Ӻ��ѷ�ʽ
			virtual void Asny_ReqAddBuddy_Question(const HULI &hid/*������˵�uid*/, const HULI &pid, uint32_t uQuestionId, const UniString& msg_left)=0;//��������Ҫͨ���ش�������������Ӻ��ѷ�ʽ

			virtual void Asny_GetBuddyCeLue(const HULI &hid/*������˵�uid*/) = 0;//��ñ�����˵���֤����


			//wuji end

			//add by myf

			/*
			* Ƶ������Ӻ���
			* �ص��ӿ�:OnChannelAddBuddyRes
			* @param uid: ������ߵ�uid
			* @param strKey: ��֤���Key,  ���û�����Ҫ������֤��ʱ,strKey�����ַ�������
			* @param strValue: �û��������֤��;���û�����Ҫ������֤��ʱ,strValue�����ַ�������
			*/
			virtual void	Asny_ReqChannelAddBuddy(const HULI &uid,  const UniString& strKey= _TEXT(""), const UniString& strValue =_TEXT("")) = 0;

			/*
			*  ��չ�ӿڣ�������ͼƬ��֤����������û������Ĵ�������3��ʱ�򣬾���Ҫ������֤�롣
			*  �����������ѵġ��ص��ӿ�onSearchRes2
			*  @param imid: �û���imid
			*  @param strKey: ��֤���Key,  ���û�����Ҫ������֤��ʱ,strKey�����ַ�������
			*  @param strValue: �û��������֤��;���û�����Ҫ������֤��ʱ,strValue�����ַ�������
			*/
			virtual void Asny_searchBuddy2(const UniString& imid, const UniString& strKey= _TEXT(""), const UniString& strValue =_TEXT("")) = 0;

			//myf add end

			//������Ӻ���ǰ�������ñ�ע�ͷ���
			//uBGid ��ʾ����id
			virtual void SetAddBuddyInfo(uint32_t uBid, uint32_t uBGid, const UniString &wstrRemark) {};

			// 3.6 ++

			//����֤���ȡ���Ѳ���
			virtual void GetBuddyCeLueVerify(uint32_t uBid) {};
			//����֤����֤�Ӻ���
			virtual void ReqAddBuddyVerify(uint32_t uBid, uint32_t uPid, const UniString& msg_left,
				const UniString& strKey= _TEXT(""), const UniString& strValue =_TEXT("")) {};
		};
	}
}