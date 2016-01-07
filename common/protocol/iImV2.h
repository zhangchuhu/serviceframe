#pragma once
//wuji 2009.07.03������Ϊ��IM��2���¼ӵĹ����ṩ�ӿ�
//wuji start
//�⼾�趨��Ľӿ����ֹ�������

//�������Asny��ͷ�������ú������첽��������������Ҫ�ص�������ӳ�����������)



//wuji end
#include <map>
#include <vector>
#include <string>
#include <list>
#include "common/protocol/iImBlackList.h"

#include "common/int_types.h"
#include "common/protocol/ilist.h"
#include "common/unistr.h"
#include "common/protocol/const.h"
//#include "./res_code.h"
#include "common/res_code.h"
#include "common/protocol/iImBlackList.h"

#include "common/protocol/IGroupList.h" // for group login. luowl

namespace protocol
{
	namespace im
	{
		enum E_OTHERLIST_OP{BLACKLIST_SYNC,BLOCKLIST_SYNC,BLACKLIST_DELFR,BlOCKLIST_DELFR,BLACKLIST_ADDTO,BlOCKLIST_ADDTO};
		

		enum E_UI_CELUE_OP{ 
			UI_CHECKBYBUDDY=1,   //��Ҫ��Ϣ��֤
			UI_REJECT=2,                  //�ܾ��κ��˼�Ϊ����
			UI_CHECKJIFEN=3,         //���ֲ���
			UI_CHECKQUESTION=4,//����
			UI_NOCELUE=5,			  //û�в���
			UI_EBANNED=SS_EBANNED,	//�Է����������
			UI_ADDTOOMUCHTODAY = RES_EADDBUDDYTOOMUCHTODAY,     //����Ӻ��ѹ���
			UI_ADDTOOMUCHFORME = RES_EADDBUDDYTOOMUCHFORME,    //���˵ĺ���������������
			UI_ADDTOOMUCHFORBUDDY = RES_EADDBUDDYTOOMUCHFORBUDDY};   //�Է��ĺ���������������

		enum E_UI_PRESENCE_TYPE{UI_CHANEL=1,UI_GAME=2,UI_HEADPHOTO=3};
		
		enum E_UI_CHANNEL_ADD_BUDDY_RES_TYPE { 
			UI_CABRT_SUCCESS = RES_SUCCESS,					//ͨ��У��,�����ɹ�
			UI_CABRT_HAVE_NOT_IM = 2 ,							//�û�û�е�½��IM
			UI_CABRT_NEED_VALID= 3,									//��Ҫ��֤����֤
			UI_CABRT_INCORRECT = 4,									//��֤����ȷ
			UI_CABRT_ADD_SELF = 5,									//����Լ�������
			UI_CABRT_RES_ENONEXIST = RES_ENONEXIST,	 /* Ŀ��(������û�)������ */
			UI_CABRT_ERROR,

		};

		struct IImSetWatcher//UIʵ�ֵĽӿ�
		{
			virtual void OnUpdateMyCeLue()=0;//�����û���֤���͵Ļص�
			virtual void OnGetMyCeLue()=0;//Ansy_GetCeLue�Ļص�
			
	
		};
		struct IImSetWatchBase 
		{
			virtual void watch(IImSetWatcher *) = 0;
			virtual void revoke(IImSetWatcher *) = 0;
		};

		struct IImSet : public virtual IImSetWatchBase//������صĵײ�ʵ�ֵĽӿ�
		{
			virtual ~IImSet(){}
			
			/*
			*  �����ӿڣ������û�����֤����
			*/
	
			virtual void Ansy_SetCeLue(E_UI_CELUE_OP uCeLue) = 0;// ��������UI_CHECKBYBUDDY=1, UI_REJECT=2ʱ��ʹ��

/*  �����ӿڣ�protocol ʵ��
*  IM ���˺�����֤����  
*  @param jifenValue :  �û�������ֵ��
*/
			virtual void Ansy_SetJiFenLimit(uint32_t uJIFen)=0;//�������������ʱ��ʹ��
			/*
* ����������protocol ʵ��
* IM ��֤���ѵ����ʡ�
* @param 
*/
			virtual void Ansy_SetQuestionLimit( const UniString& strQuestion, const UniString&strAnswer)=0;//����������������ʹ�ʱ��ʹ��

			//����������������ʹ�ʱ��ʹ��	
			virtual void SetQuestionLimit( const UniString& strQuestion, const UniString&strAnswer,bool bIsCheck/*�Ƿ���Ҫ2��ȷ��*/)=0;//����������������ʹ�ʱ��ʹ��
			

			virtual void Ansy_GetCeLue() = 0;//�����֤����

			virtual void GetCeLue(E_UI_CELUE_OP &uCeLue) = 0;//�����֤����
			virtual void GetJiFenLimit(uint32_t &uJIFen)=0;//��û�������
			virtual void GetQuestionLimit( UniString &strQuestion, UniString &strAnswer)=0;//�������ʹ�
			virtual void GetImid(UniString& strImid)=0;//���Imid
			virtual void GetQuestionLimit( UniString &strQuestion, UniString &strAnswer,bool& bIsCheck/*true������Ҫ2��ȷ��*/)=0;//�������ʹ�
			


		};


		//�������ӿ�
// 		struct IBlackList:public virtual IImListWatchBase//�ײ�ʵ�ֵĽӿ�
// 		{
// 			//����������������
// 			virtual void Ansy_MoveToBlackList(const HULI& strBuddyId ) = 0;
// 			//�����ѴӺ�����ɾ��
// 			virtual void Ansy_DeleteFromBlackList(const HULI& strBuddyId) = 0;
// 			virtual void Ansy_GetBackList()=0;//ȡ�غ������б�
// 
// 		
// 			virtual void GetBackList(std::list<HULI>& lstData)=0;//ȡ�غ������б�
// 			virtual bool IsInBlackList(const HULI& strBuddyId ) = 0;//true������
// 
// 		};
		//��������UIʵ�ֵĽӿ���ʹ��IImListWatcher

		//�����������ӿ�
		struct IBlockList :public virtual IImListWatchBase//�ײ�ʵ�ֵĽӿ�
		{
			
			//��ӵ�����������
			virtual void Ansy_MoveToShield(const HULI& strBuddyId) = 0;
			//�ӱ�����ɾ��ĳ��
			virtual void Ansy_DeleteFromShield(const HULI& strBuddyId) = 0;
			virtual void Ansy_GetShield()=0;//ȡ�ر����������б�
			virtual void GetShield(std::list<HULI>& lstData)=0;//ȡ�ر����������б�
			virtual bool IsInBlockList(const HULI& strBuddyId )=0;//true������
		};
		

		//����������UIʵ�ֵĽӿ���ʹ��IImListWatcher

		
		//������һЩ�ӿ�
		struct IImPlayIMSetWatcher//UIʵ�ֵĽӿ�
		{
			virtual void OnStartPlayIMRes(const bool& bIsOk) = 0;//
			virtual void OnStopPlayIMRes(const bool& bIsOk) = 0;//
			//virtual void OnRequestImidRes(const uint32_t& uImid,const bool& bIsOk/*true����ɹ�*/)=0;
		};

		struct IImPlayIMSetWatchBase 
		{
			virtual void watch(IImPlayIMSetWatcher *) = 0;
			virtual void revoke(IImPlayIMSetWatcher *) = 0;
		};


		struct IImPlayIM:public virtual IImPlayIMSetWatchBase//�ײ�ʵ�ֵĽӿ�
		{
			//			virtual void Ansy_RequestImid()=0;//����YY��
//			virtual void Ansy_StartPlayIM() = 0;//��ʼʹ��IM
//			virtual void Ansy_StopPlayIM()=0;//����ʹ��IM
			virtual void SetRequestImidFlag(bool bIsRequest/*true��������YY��*/ )=0;//����YY��
		};


		//����Ƶ����Ϸ���
		struct IImChanelSetWatcher//UIʵ�ֵĽӿ�
		{
			virtual void OnInviteToChannel(const HULI& strBuddyId,const HULI& strChanelId,const UniString& strData )=0;//�յ����˷�������������Ϣ
			virtual void OnInviteBuddyToChannelRes(const HULI& strBuddyId,const HULI& strChanelId,const bool& bIsOk/*true��������ɹ���false����ʧ��*/)=0;//���������Ƶ���Ĳ����Ľ��

			virtual void OnUpdateMyCurrentGame() = 0;//
			virtual void OnUpdateMyCurrentChannel()=0;//
			virtual void OnUpdateMyLogo()=0;//Ansy_ChangeMyLogo�Ļص�

			virtual void OnGetMyLogo()=0;//Ansy_GetMyLogo�Ļص�
			virtual void OnGetMyGame()=0;//Ansy_GetMyCurrentGame�ص�

		
		};

		struct IImChanelSetWatchBase 
		{
			virtual void watch(IImChanelSetWatcher *) = 0;
			virtual void revoke(IImChanelSetWatcher *) = 0;
		};


		struct IImChanelSet:public virtual IImChanelSetWatchBase//�ײ�ʵ�ֵĽӿ�
		{
			virtual void Ansy_UpdateMyCurrentGame(const HULI& strGameId,const UniString &strGameData) = 0;//

			virtual void Ansy_UpdateMyCurrentChannel(const HULI& strChanelId,const UniString& strData)=0;//
			virtual void Ansy_InviteBuddyToChannel(const HULI& strBuddyId,const HULI& strChanelId,const UniString& strData)=0;//

			//����������ѽ�Ƶ��
			//strBuddyList...Ҫ�������Ƶ���ĺ����б�
			//strCahnnelID...Ƶ��ID
			//strData........Ƶ����Ϣ
			virtual void Ansy_InviteMultiToChannel(const std::vector<HULI>& strBuddyList, const HULI& strChannelID, const UniString& strData)=0;

			virtual void Ansy_GetMyCurrentGame()=0;//��ȡ�Լ����õ���Ϸ����

			//modify by myf
			static const uint32_t CUSTOM_LOGO_NUMBER = 0;			//�Զ���ͷ����
			static const uint32_t DEFAULT_LOGO_NUMBER = -1;			//Ĭ��ͷ����   uint32����Ϊ-1
			virtual void Ansy_ChangeMyLogo(uint32_t uImage, const UniString &strLogoPath)=0;//�û�����ͷ��
			//modify end
			virtual void Ansy_GetMyLogo()=0;//��ȡͷ��
			
			//��ȡͷ��
			virtual uint32_t GetMyLogo(UniString &strLogoPath)=0;/*modify by myf,  ���strLogoPath����, �����Զ���ͷ��·��*/
			
			
			virtual HULI GetCurrentPlayGame(UniString &strData)=0;//��ȡ�Լ��ڵ���Ϸ���մ���û������Ϸ
			
			
			virtual void GetCurrentChannel(HULI &strChanelId,UniString& strData)=0;//��ȡ�Լ��ڵ�Ƶ��,�մ���û����Ƶ��

			
		};

////////////////////////////////////

		//����
		struct CObjUserInfo1
		{
			HULI m_strUid;//uint32_t m_uId;//�û�ID
			UniString m_strNickname;//�û��ǳ�
			protocol::uinfo::SEX m_sex;
			
			uint32_t m_uBirthDay;
			uint32_t m_uArea;
			uint32_t m_uProvince;
			uint32_t m_uCity;
			UniString m_strResume;
			uint32_t m_uVersion;
			uint32_t m_uJiFen;
			UniString m_strSign;
			UniString m_strIntro;

			protocol::E_IMSTATUS m_stat;//״̬
			

			uint32_t m_uHeadPhoto;//ͷ��,û�о���-1
			uint32_t m_uImId;//yy�̺ţ�û�о���-1

		};

		//İ���˵���Ϣ�ı�
		enum EUserChangeType
		{
			ENUM_USER_CHANGE_STATUS = 0,  //״̬�ı�
			ENUM_USER_CHANGE_HEADPHOTO,	  //ͷ��ı�
			ENUM_USER_CHANGE_NICK,  //�ǳƸı�
			ENUM_USER_CHANGE_SIGN   //ǩ���ı�
		};
		struct SUserChangeContain
		{
			SUserChangeContain() {}
			SUserChangeContain(protocol::E_IMSTATUS emSt) : m_emStatus(emSt) {}
			SUserChangeContain(uint32_t headIdx) : m_uHeadIdx(headIdx) {}

			protocol::E_IMSTATUS m_emStatus; //״̬
			uint32_t m_uHeadIdx; //ͷ����
			std::string m_strNick; //�ǳ�
			std::string m_strSign; //ǩ��
		};

		struct IImOtherSetWatcher//UIʵ�ֵĽӿ�
		{
			virtual void OnGetUserInfo(const HULI& strBuddyId,const CObjUserInfo1 &objUinfo)=0;

			//İ����״̬�ı�
			virtual void OnUserStatusChange(const HULI& strBuddyId, const protocol::E_IMSTATUS &emStatus) {}
			//İ����ͷ��ı�
			virtual void OnUserHeadPhotoChange(const HULI& strBuddyId, const uint32_t &uHeadIdx) {}
			//İ�����ǳƸı�
			virtual void OnUserNickChange(const HULI& strBuddyId, const UniString &strNick) {}
			//İ����ǩ���ı�
			virtual void OnUserSignChange(const HULI& strBuddyId, const UniString &strSign) {}
		};

		struct IImOtherSetWatchBase 
		{
			virtual void watch(IImOtherSetWatcher *) = 0;
			virtual void revoke(IImOtherSetWatcher *) = 0;
		};


		struct IImOtherSet:public virtual IImOtherSetWatchBase//�ײ�ʵ�ֵĽӿ�
		{
			virtual void Ansy_GetUserInfo(const HULI& strBuddyId,bool bIsUpdate=false/*Ĭ���Ƕ����ػ���*/)=0;//ȡĳ���û���������Ϣ
			virtual E_IMSTATUS GetStrangerStatus(uint32_t uUid)=0;
		};

	}

}
