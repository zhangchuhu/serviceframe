#pragma once
#include "ilist.h"
#include "iaction.h"
#include "common/misc.h"
#include "const.h"
#include "psessionbase.h"
#include "authcode.h"
#include <vector>

namespace protocol{
	namespace session{

		struct MiniInfo{
			UniString uid;
			UniString nick;
			uinfo::SEX gender;
			ChannelRoler role;
			MiniInfo(const UniString &u, const UniString &n, uinfo::SEX s):uid(u), nick(n), gender(s){}
			MiniInfo(){role = NORMAL;};
		};

		struct ISInfoWatcher{
			virtual void onChange() = 0;
		};


		struct ISInfoWatchBase{
			virtual void watch(ISInfoWatcher *) = 0;
			virtual void revoke(ISInfoWatcher *) = 0;
		};

		const unsigned long dwASSEMBLE_CODEC_TYPE_BASE = 6;
		const unsigned long dwCUSTOM_CODEC_TYPE_BASE = 20;
		enum CodecRate{
			CODEC_NONE = -1,
			VOICE_MID = 0,
			MUSIC_MID = 1,
			VOICE_HIGHER = 2,
			VOICE_HIGH = 3,
			VOICE_HIGHER_JITTER = 4, 
			MUSIC_MID_JITTER = 5,
			//麦序高音质
			VOICE_MID_AND_MUSIC_MID = dwASSEMBLE_CODEC_TYPE_BASE + 1,				//7,低音质为VOICE_MID，高音质为MUSIC_MID
			VOICE_HIGH_AND_MUSIC_MID = dwASSEMBLE_CODEC_TYPE_BASE + 2,				//8,低音质为VOICE_HIGH，高音质为MUSIC_HIGH
			VOICE_HIGHER_AND_MUSIC_MID_JITTER = dwASSEMBLE_CODEC_TYPE_BASE + 3, 
			//自定义音质属性
			VOICE_LOW_FOR_MOBILE = dwCUSTOM_CODEC_TYPE_BASE + 1,
			VOICE_HIGHER_QUALITY1 = dwCUSTOM_CODEC_TYPE_BASE + 2,
			VOICE_LOW_FOR_MOBILE_QUALITY2 = dwCUSTOM_CODEC_TYPE_BASE + 3
		};

		struct ISessionInfoBase: public virtual ISInfoWatchBase{
			virtual UniString getName() const = 0;
			virtual UniString getSid() const = 0;
			virtual UniString getPid() const = 0;
			
			virtual bool isPub() const = 0;
			virtual bool isLimit() const = 0;
			virtual UniString getAliaseId() const = 0; // 短位id

			virtual UniString getBulletin(void) const = 0;

			virtual UniString getCreator(void) const = 0;

			virtual uint32_t  getMemberLimited(void) const = 0;
			
			virtual int getArea(void) const = 0;
			virtual int getProvince(void) const = 0;
			virtual int getCity(void) const = 0;

			virtual GuildType getType() const = 0;
			virtual UniString getTypeString() const = 0;

			virtual uint64_t getJifen()  const = 0;
			virtual uint32_t getPaiming() const = 0;

			virtual time_t getBulletinTimeStamp() const = 0;

			virtual UniString getIntroduce() const = 0;

			virtual int		  getMicTime() const = 0;

			virtual bool hasPass(void) const = 0;

			virtual VoiceQC getVoiceQC() const = 0;

			virtual std::vector<MiniInfo> getManager() const = 0;
			virtual std::vector<MiniInfo> getMember() const = 0;

			virtual ISPType getIsp() const = 0;

			virtual bool isLimitTxt() const = 0;

			virtual int getTxtLimitTime() const = 0;
			virtual CodecRate getCodecRate() const = 0;

			virtual MediaTransportStyle getMediaTransportStyle() const = 0;
			/**
			 * 获取公会logo的index
			 *
			 * @return	>= 1	表示客户端内置的公会logo的index，
						= 0		表示表示用户上传的公会logo。
						-1		找不到guild logo信息

			 */
			virtual int getGuildLogoIndex() const = 0;

			/**
			 * 获取自定义公会logo的url
			 *
			 * @return	空字符窜	找不到该频道，客户端可以考虑用内置公会logo。
						不为空		logo的url
			 */
			virtual UniString getCustomLogoURL() const = 0;
			/*
					游客最大文字长度
			*/
			virtual int getGuestMaxTxtLen() const = 0;
			/*
					是否允许游客上麦
			*/
			virtual bool canGuestJoinMic() const = 0;

			/*
					进入频道后允许公聊的等待时间
			*/
			virtual int getGuestWaitTime() const = 0;

			/*
					是否启用游客限制
			*/
			virtual bool isEnableGuestLimit() const = 0;

			/*	
					 限制打字速度只对游客生效
					false 对所有人， true 只对游客
			*/
			virtual bool isLimitTxtOnlyForGuest() const = 0;

			/*	
					 限制游客进入子频道
					false 不限制， true 限制
			*/
			virtual bool isLimitGuestAccess() const = 0;

			/*	
					 自由模式下是否禁止游客语音
					 true表示可以语音，false表示禁止语音
			*/
			virtual bool canGuestVoiceChat() const = 0;

			virtual ChannelStyle getStyle() const = 0;

			/*	
					 限制公聊文字中带url信息
					false 表示不可以，true表示可以
			*/
			virtual bool canSendUrlTxt() const = 0;

			/*	
					 获取游客入会个人积分限制
			*/
			virtual int getEnrollmentJiFenLimit() const = 0;

			/*	
					 获取游客入会事项说明
			*/
			virtual UniString getEnrollmentNotice() const = 0;

			virtual int getUInfoDisplayConfig() const = 0;

			virtual int getServiceStatus() const = 0;

			virtual UniString getSessBulletin() const = 0;

			virtual bool isLimitGuestAccessTop() const = 0;
		};
		
		
	}
}
