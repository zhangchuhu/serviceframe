#pragma once
//wuji 2009.07.03建立，为了IM第2版新加的功能提供接口
//wuji start
//吴季鸿定义的接口名字规则如下

//如果是以Asny开头，表明该函数是异步操作函数（即需要回调函数反映函数操作结果)



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
			UI_CHECKBYBUDDY=1,   //需要消息验证
			UI_REJECT=2,                  //拒绝任何人加为好友
			UI_CHECKJIFEN=3,         //积分策略
			UI_CHECKQUESTION=4,//问题
			UI_NOCELUE=5,			  //没有策略
			UI_EBANNED=SS_EBANNED,	//对方拉入黑名单
			UI_ADDTOOMUCHTODAY = RES_EADDBUDDYTOOMUCHTODAY,     //今天加好友过多
			UI_ADDTOOMUCHFORME = RES_EADDBUDDYTOOMUCHFORME,    //本人的好友数量超过限制
			UI_ADDTOOMUCHFORBUDDY = RES_EADDBUDDYTOOMUCHFORBUDDY};   //对方的好友数量超过限制

		enum E_UI_PRESENCE_TYPE{UI_CHANEL=1,UI_GAME=2,UI_HEADPHOTO=3};
		
		enum E_UI_CHANNEL_ADD_BUDDY_RES_TYPE { 
			UI_CABRT_SUCCESS = RES_SUCCESS,					//通过校验,操作成功
			UI_CABRT_HAVE_NOT_IM = 2 ,							//用户没有登陆过IM
			UI_CABRT_NEED_VALID= 3,									//需要验证码验证
			UI_CABRT_INCORRECT = 4,									//验证不正确
			UI_CABRT_ADD_SELF = 5,									//添加自己做好友
			UI_CABRT_RES_ENONEXIST = RES_ENONEXIST,	 /* 目标(对象或用户)不存在 */
			UI_CABRT_ERROR,

		};

		struct IImSetWatcher//UI实现的接口
		{
			virtual void OnUpdateMyCeLue()=0;//设置用户验证类型的回调
			virtual void OnGetMyCeLue()=0;//Ansy_GetCeLue的回调
			
	
		};
		struct IImSetWatchBase 
		{
			virtual void watch(IImSetWatcher *) = 0;
			virtual void revoke(IImSetWatcher *) = 0;
		};

		struct IImSet : public virtual IImSetWatchBase//设置相关的底层实现的接口
		{
			virtual ~IImSet(){}
			
			/*
			*  新增接口，设置用户的验证类型
			*/
	
			virtual void Ansy_SetCeLue(E_UI_CELUE_OP uCeLue) = 0;// 当策略是UI_CHECKBYBUDDY=1, UI_REJECT=2时候使用

/*  新增接口，protocol 实现
*  IM 个人好友验证设置  
*  @param jifenValue :  用户的输入值。
*/
			virtual void Ansy_SetJiFenLimit(uint32_t uJIFen)=0;//当策略是算积分时候使用
			/*
* 新增函数，protocol 实现
* IM 验证好友的提问。
* @param 
*/
			virtual void Ansy_SetQuestionLimit( const UniString& strQuestion, const UniString&strAnswer)=0;//当策略是设置问题和答案时候使用

			//当策略是设置问题和答案时候使用	
			virtual void SetQuestionLimit( const UniString& strQuestion, const UniString&strAnswer,bool bIsCheck/*是否需要2次确认*/)=0;//当策略是设置问题和答案时候使用
			

			virtual void Ansy_GetCeLue() = 0;//获得验证类型

			virtual void GetCeLue(E_UI_CELUE_OP &uCeLue) = 0;//获得验证类型
			virtual void GetJiFenLimit(uint32_t &uJIFen)=0;//获得积分限制
			virtual void GetQuestionLimit( UniString &strQuestion, UniString &strAnswer)=0;//获得问题和答案
			virtual void GetImid(UniString& strImid)=0;//获得Imid
			virtual void GetQuestionLimit( UniString &strQuestion, UniString &strAnswer,bool& bIsCheck/*true代表需要2次确认*/)=0;//获得问题和答案
			


		};


		//黑名单接口
// 		struct IBlackList:public virtual IImListWatchBase//底层实现的接口
// 		{
// 			//将好友拉到黑名单
// 			virtual void Ansy_MoveToBlackList(const HULI& strBuddyId ) = 0;
// 			//将好友从黑名单删除
// 			virtual void Ansy_DeleteFromBlackList(const HULI& strBuddyId) = 0;
// 			virtual void Ansy_GetBackList()=0;//取回黑名单列表
// 
// 		
// 			virtual void GetBackList(std::list<HULI>& lstData)=0;//取回黑名单列表
// 			virtual bool IsInBlackList(const HULI& strBuddyId ) = 0;//true代表在
// 
// 		};
		//黑名单的UI实现的接口是使用IImListWatcher

		//被屏蔽名单接口
		struct IBlockList :public virtual IImListWatchBase//底层实现的接口
		{
			
			//添加到被屏蔽名单
			virtual void Ansy_MoveToShield(const HULI& strBuddyId) = 0;
			//从被屏蔽删除某人
			virtual void Ansy_DeleteFromShield(const HULI& strBuddyId) = 0;
			virtual void Ansy_GetShield()=0;//取回被屏蔽名单列表
			virtual void GetShield(std::list<HULI>& lstData)=0;//取回被屏蔽名单列表
			virtual bool IsInBlockList(const HULI& strBuddyId )=0;//true代表在
		};
		

		//屏蔽名单的UI实现的接口是使用IImListWatcher

		
		//其他的一些接口
		struct IImPlayIMSetWatcher//UI实现的接口
		{
			virtual void OnStartPlayIMRes(const bool& bIsOk) = 0;//
			virtual void OnStopPlayIMRes(const bool& bIsOk) = 0;//
			//virtual void OnRequestImidRes(const uint32_t& uImid,const bool& bIsOk/*true代表成功*/)=0;
		};

		struct IImPlayIMSetWatchBase 
		{
			virtual void watch(IImPlayIMSetWatcher *) = 0;
			virtual void revoke(IImPlayIMSetWatcher *) = 0;
		};


		struct IImPlayIM:public virtual IImPlayIMSetWatchBase//底层实现的接口
		{
			//			virtual void Ansy_RequestImid()=0;//申请YY号
//			virtual void Ansy_StartPlayIM() = 0;//开始使用IM
//			virtual void Ansy_StopPlayIM()=0;//结束使用IM
			virtual void SetRequestImidFlag(bool bIsRequest/*true代表申请YY号*/ )=0;//申请YY号
		};


		//设置频道游戏相关
		struct IImChanelSetWatcher//UI实现的接口
		{
			virtual void OnInviteToChannel(const HULI& strBuddyId,const HULI& strChanelId,const UniString& strData )=0;//收到别人发过来的邀请消息
			virtual void OnInviteBuddyToChannelRes(const HULI& strBuddyId,const HULI& strChanelId,const bool& bIsOk/*true代表邀请成功，false代表失败*/)=0;//邀请好友入频道的操作的结果

			virtual void OnUpdateMyCurrentGame() = 0;//
			virtual void OnUpdateMyCurrentChannel()=0;//
			virtual void OnUpdateMyLogo()=0;//Ansy_ChangeMyLogo的回调

			virtual void OnGetMyLogo()=0;//Ansy_GetMyLogo的回调
			virtual void OnGetMyGame()=0;//Ansy_GetMyCurrentGame回调

		
		};

		struct IImChanelSetWatchBase 
		{
			virtual void watch(IImChanelSetWatcher *) = 0;
			virtual void revoke(IImChanelSetWatcher *) = 0;
		};


		struct IImChanelSet:public virtual IImChanelSetWatchBase//底层实现的接口
		{
			virtual void Ansy_UpdateMyCurrentGame(const HULI& strGameId,const UniString &strGameData) = 0;//

			virtual void Ansy_UpdateMyCurrentChannel(const HULI& strChanelId,const UniString& strData)=0;//
			virtual void Ansy_InviteBuddyToChannel(const HULI& strBuddyId,const HULI& strChanelId,const UniString& strData)=0;//

			//批量邀请好友进频道
			//strBuddyList...要邀请进入频道的好友列表
			//strCahnnelID...频道ID
			//strData........频道信息
			virtual void Ansy_InviteMultiToChannel(const std::vector<HULI>& strBuddyList, const HULI& strChannelID, const UniString& strData)=0;

			virtual void Ansy_GetMyCurrentGame()=0;//获取自己设置的游戏数据

			//modify by myf
			static const uint32_t CUSTOM_LOGO_NUMBER = 0;			//自定义头像编号
			static const uint32_t DEFAULT_LOGO_NUMBER = -1;			//默认头像编号   uint32不能为-1
			virtual void Ansy_ChangeMyLogo(uint32_t uImage, const UniString &strLogoPath)=0;//用户更改头像
			//modify end
			virtual void Ansy_GetMyLogo()=0;//获取头像
			
			//获取头像
			virtual uint32_t GetMyLogo(UniString &strLogoPath)=0;/*modify by myf,  添加strLogoPath参数, 返回自定义头像路径*/
			
			
			virtual HULI GetCurrentPlayGame(UniString &strData)=0;//获取自己在的游戏，空代表没有玩游戏
			
			
			virtual void GetCurrentChannel(HULI &strChanelId,UniString& strData)=0;//获取自己在的频道,空代表没有在频道

			
		};

////////////////////////////////////

		//其他
		struct CObjUserInfo1
		{
			HULI m_strUid;//uint32_t m_uId;//用户ID
			UniString m_strNickname;//用户昵称
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

			protocol::E_IMSTATUS m_stat;//状态
			

			uint32_t m_uHeadPhoto;//头像,没有就是-1
			uint32_t m_uImId;//yy短号，没有就是-1

		};

		//陌生人的信息改变
		enum EUserChangeType
		{
			ENUM_USER_CHANGE_STATUS = 0,  //状态改变
			ENUM_USER_CHANGE_HEADPHOTO,	  //头像改变
			ENUM_USER_CHANGE_NICK,  //昵称改变
			ENUM_USER_CHANGE_SIGN   //签名改变
		};
		struct SUserChangeContain
		{
			SUserChangeContain() {}
			SUserChangeContain(protocol::E_IMSTATUS emSt) : m_emStatus(emSt) {}
			SUserChangeContain(uint32_t headIdx) : m_uHeadIdx(headIdx) {}

			protocol::E_IMSTATUS m_emStatus; //状态
			uint32_t m_uHeadIdx; //头像编号
			std::string m_strNick; //昵称
			std::string m_strSign; //签名
		};

		struct IImOtherSetWatcher//UI实现的接口
		{
			virtual void OnGetUserInfo(const HULI& strBuddyId,const CObjUserInfo1 &objUinfo)=0;

			//陌生人状态改变
			virtual void OnUserStatusChange(const HULI& strBuddyId, const protocol::E_IMSTATUS &emStatus) {}
			//陌生人头像改变
			virtual void OnUserHeadPhotoChange(const HULI& strBuddyId, const uint32_t &uHeadIdx) {}
			//陌生人昵称改变
			virtual void OnUserNickChange(const HULI& strBuddyId, const UniString &strNick) {}
			//陌生人签名改变
			virtual void OnUserSignChange(const HULI& strBuddyId, const UniString &strSign) {}
		};

		struct IImOtherSetWatchBase 
		{
			virtual void watch(IImOtherSetWatcher *) = 0;
			virtual void revoke(IImOtherSetWatcher *) = 0;
		};


		struct IImOtherSet:public virtual IImOtherSetWatchBase//底层实现的接口
		{
			virtual void Ansy_GetUserInfo(const HULI& strBuddyId,bool bIsUpdate=false/*默认是读本地缓存*/)=0;//取某个用户的属性信息
			virtual E_IMSTATUS GetStrangerStatus(uint32_t uUid)=0;
		};

	}

}
