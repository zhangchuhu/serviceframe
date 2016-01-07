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
			//���������
			VOICE_MID_AND_MUSIC_MID = dwASSEMBLE_CODEC_TYPE_BASE + 1,				//7,������ΪVOICE_MID��������ΪMUSIC_MID
			VOICE_HIGH_AND_MUSIC_MID = dwASSEMBLE_CODEC_TYPE_BASE + 2,				//8,������ΪVOICE_HIGH��������ΪMUSIC_HIGH
			VOICE_HIGHER_AND_MUSIC_MID_JITTER = dwASSEMBLE_CODEC_TYPE_BASE + 3, 
			//�Զ�����������
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
			virtual UniString getAliaseId() const = 0; // ��λid

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
			 * ��ȡ����logo��index
			 *
			 * @return	>= 1	��ʾ�ͻ������õĹ���logo��index��
						= 0		��ʾ��ʾ�û��ϴ��Ĺ���logo��
						-1		�Ҳ���guild logo��Ϣ

			 */
			virtual int getGuildLogoIndex() const = 0;

			/**
			 * ��ȡ�Զ��幫��logo��url
			 *
			 * @return	���ַ���	�Ҳ�����Ƶ�����ͻ��˿��Կ��������ù���logo��
						��Ϊ��		logo��url
			 */
			virtual UniString getCustomLogoURL() const = 0;
			/*
					�ο�������ֳ���
			*/
			virtual int getGuestMaxTxtLen() const = 0;
			/*
					�Ƿ������ο�����
			*/
			virtual bool canGuestJoinMic() const = 0;

			/*
					����Ƶ���������ĵĵȴ�ʱ��
			*/
			virtual int getGuestWaitTime() const = 0;

			/*
					�Ƿ������ο�����
			*/
			virtual bool isEnableGuestLimit() const = 0;

			/*	
					 ���ƴ����ٶ�ֻ���ο���Ч
					false �������ˣ� true ֻ���ο�
			*/
			virtual bool isLimitTxtOnlyForGuest() const = 0;

			/*	
					 �����οͽ�����Ƶ��
					false �����ƣ� true ����
			*/
			virtual bool isLimitGuestAccess() const = 0;

			/*	
					 ����ģʽ���Ƿ��ֹ�ο�����
					 true��ʾ����������false��ʾ��ֹ����
			*/
			virtual bool canGuestVoiceChat() const = 0;

			virtual ChannelStyle getStyle() const = 0;

			/*	
					 ���ƹ��������д�url��Ϣ
					false ��ʾ�����ԣ�true��ʾ����
			*/
			virtual bool canSendUrlTxt() const = 0;

			/*	
					 ��ȡ�ο������˻�������
			*/
			virtual int getEnrollmentJiFenLimit() const = 0;

			/*	
					 ��ȡ�ο��������˵��
			*/
			virtual UniString getEnrollmentNotice() const = 0;

			virtual int getUInfoDisplayConfig() const = 0;

			virtual int getServiceStatus() const = 0;

			virtual UniString getSessBulletin() const = 0;

			virtual bool isLimitGuestAccessTop() const = 0;
		};
		
		
	}
}
