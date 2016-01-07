#pragma once

#include "const.h"
#include "common/int_types.h"
#include "common/protocol/PVipRing.h"

namespace protocol
{
	namespace imvip
	{
		struct PCS_VipInfo : public sox::Marshallable
		{
			PCS_VipInfo():grade_(0),vip_type_(0) {}
			bool isVip() { return grade_ > 0; }

			uint32_t grade_;
			uint32_t vip_type_;
			virtual void marshal(sox::Pack &p) const
			{
				p << grade_<<vip_type_;
			}

			virtual void unmarshal(const sox::Unpack &up)
			{
				up >> grade_ >> vip_type_;
			}
		};

		// �����Ա��Ϣ
		enum EPlDiamonPrivi 
		{
			ENUM_PLDIAMOND_NICK = 0,       // ��ɫ�ǳ�
            ENUM_PLDIAMOND_SUPER,          // ��������
            ENUM_PLDIAMOND_EXVIP,          // �����Ա
			ENUM_PLDIAMOND_GAME,           // ��Ϸ����     
			ENUM_SUPER_VIP_GRADE,          // ������Ա�ȼ�   0 ��ʾ���ǳ�����Ա
		};

		struct PlDiamondInfo : public sox::Marshallable
		{
			uint8_t                      m_grade;                // 0 �������Ա  1 ~ 7
			uint8_t                      m_type;                 // 0 �·�  1 ���
			uint32_t                     m_upgradeTime;          // ����ʱ��
			std::map<uint32_t, uint8_t>  m_privi;                // ��Ȩ�ֶ�

			PlDiamondInfo() : m_grade(0), m_type(0), m_upgradeTime(0) {}

			virtual void marshal(sox::Pack& p) const
			{
				sox::PackBuffer buffer;
				sox::Pack pack(buffer);
				pack << m_grade << m_type << m_upgradeTime;
				sox::marshal_container(pack, m_privi);
				p.push_varstr(pack.data(), pack.size());
			}

			virtual void unmarshal(const sox::Unpack& p)
			{
				try{
					sox::Varstr vs = p.pop_varstr_ptr();
					sox::Unpack un(vs.data(), vs.size());
					un >> m_grade >> m_type >> m_upgradeTime;
					sox::unmarshal_container(un, std::inserter(m_privi, m_privi.begin()));
				}
				catch (const sox::UnpackError& )
				{
				}
			}
		};
	
		// ow ��Ա��Ϣ
		struct OwVipInfo : public sox::Marshallable
		{
			uint8_t  m_isVip;             // 1 yes, 0 not

			OwVipInfo() : m_isVip(0) {}
			virtual void marshal(sox::Pack& p) const
			{
				sox::PackBuffer buffer;
				sox::Pack pack(buffer);
				pack << m_isVip;
				p.push_varstr(pack.data(), pack.size());
			}

			virtual void unmarshal(const sox::Unpack& p)
			{
				try{
					sox::Varstr vs = p.pop_varstr_ptr();
					sox::Unpack un(vs.data(), vs.size());
					un >> m_isVip;
				}
				catch (const sox::UnpackError& )
				{
				}
			}
		};

		class IImVipWatcher
		{
		public:
			virtual ~IImVipWatcher() {}
			
			virtual void OnGetBuddyVipInfoRes(const std::map<uint32_t, PCS_VipInfo>& info) = 0;    // for im
			virtual void OnGetGrpUserVipInfoRes(const std::map<uint32_t, PCS_VipInfo>& info) = 0;  // for group

			//  if ( uGrade == 0 )      user is not vip         
			virtual void OnVipInfoChange(const uint32_t& uUid, const PCS_VipInfo& info) = 0;             // for im
			virtual void OnGrpVipInfoChange(const uint32_t& uUid, const PCS_VipInfo& info) = 0;          // for group

			// for single user
			virtual void OnGetVipInfoRes(const uint32_t& uUid, const PCS_VipInfo& info) = 0;

			virtual void OnGetBatchColorRingInfoRes(const std::map<uint32_t, std::vector<PCS_ColorRingInfo> >& infos) = 0;

			/*	
			* �������ñ����֪ͨ
			*   bIsSetRing: �Ƿ�������������
			*   info: ���bIsSetRing==true, ��infoΪ�����õ�������Ϣ.
			*         ���bIsSetRing==false, info.idx��ֵû������,���Ժ���
			*/
			virtual void OnColorRingInfoChange(const uint32_t& uUid, const bool& bIsSetRing, const PCS_ColorRingInfo &info) = 0;

			/*
			* �����û��������ѷ��������ش�����
			* @param uid���õ������û�uid
			* @param return_code���ý��
			*/
			virtual void OnSetWatchUserRes(const uint32_t& uid,const uint32_t& return_code) = 0;

			/*
			* ȡ���û��������ѷ��������ش�����
			* @param uid���õ������û�uid
			* @param return_code���ý��
			*/
			virtual void OnCancelWatchUserRes(const uint32_t& uid,const uint32_t& return_code) = 0;

			/*
			* �û���¼ʱ��ȡ���������б�
			* @param uid_ringId �����û�uid��Ӧ������id�б�,ringId == 0 ��ʾ��Чid
			*/
			virtual void OnGetWatcherListRes(const std::map<uint32_t,uint32_t>& uid_ringId) = 0;

			/*
			* web�������������б�
			* @param uid_ringId �����û�uid��Ӧ������id�б�,ringId == 0 ��ʾ��Чid
			*/
			virtual void OnWebSetWatcherListRes(const std::map<uint32_t,uint32_t>& uid_ringId) = 0;

            /*
             *web��������ǩ��ͬ���ͻ���
             *@param vip_mood_string ͬ�����ͻ��˵������ַ���
             *@param uid���û���uid
             */
            virtual void OnWebVipMoodStringRes(const uint32_t& uid, const std::string& vip_mood_string) = 0;

			/*
			* ��ȡ��Ա����ʹ�õı���������������������е��û�����ʹ�øñ���
			*/
			virtual void OnVipExpWhiteList(const std::map< uint32_t, std::vector<uint32_t> >& mapUid2Lists) = 0;

			// �ϴ��Զ��嶯̬ͷ���Ȩ��Ӧ
			virtual void OnVipUpLogoAuthRes(const std::string& strContext, const std::string& strFileName, const std::string& strCookie) = 0;

			// im���߻��ֻص�����
			virtual void OnGetUserOffLineScoreForIm(const uint32_t& uid,const uint32_t& score,const uint32_t& add_score) = 0;

			// Ⱥ���߻��ֻص�����
			virtual void OnGetUserOffLineScoreForGrp(const uint32_t& uid,const uint32_t& score,const uint32_t& add_score) = 0;

			// Ƶ�����߻��ֻص�����
			virtual void OnGetUserOffLineScoreForChannel(const uint32_t& uid,const uint32_t& score,const uint32_t& add_score) = 0;

			/* �û���¼��ȡvͼ����,�ͻ��˻ص�����
			 * @param vec_activity: ���û���صĻ��id�б�
			 */
			virtual void OnGetVipActivityRes(const std::vector<uint32_t>& m_vec_activity) = 0;
		   /* �û����vͼ��,���صĽ��
			* @param uid: �û���uid
			* @param return_code���ý��,1:�ɹ�;0:ʧ��
			*/
			virtual void OnCancelActivityRes(const uint32_t& uid,const uint32_t& return_code) = 0;

			// ��Ա֪ͨ��Ϣ
			//   @param id:  ��Ϣ����ID�����Ժ���ܵ�ͳ������ʹ�ã���ʱû�ã�
			virtual void OnVipMsg(const uint32_t& id, const std::string& strMsg) = 0;			

			// ��ȡ������Ϣ����
			// @param  type  1 ���Ѽ��Լ�  2 ����İ����
			virtual void onGetPlDiamondInfoRes(const uint32_t& type, const std::map<uint32_t, PlDiamondInfo>& uid2Info) {}

			// �����������߻���
			// @param  type  1 �����б�   2 Ⱥ��Ա   3 Ƶ���б�
			virtual void onGetPlDiamondOffScoreRes(const uint32_t& uid, const uint32_t& score, const uint32_t& addScore, const uint32_t& type) {}

			virtual void onGetOwVipInfoRes(const uint32_t& type, const std::map<uint32_t, OwVipInfo>& uid2Info) {}
		};

		class IImVipWatcherBase
		{
		public:
			virtual ~IImVipWatcherBase() {}

			virtual void watch(IImVipWatcher *w) = 0;
			virtual void revoke(IImVipWatcher *w) = 0;
		};

		class IImVip : public IImVipWatcherBase
		{
		public:
			virtual ~IImVip() {}

			// ��ȡ���ѵĻ�Ա��Ϣ
			virtual void GetBuddyVipInfo(const std::vector<uint32_t>& vecUids) = 0;

			// ��ȡȺ��Ա�Ļ�Ա��Ϣ
			virtual void GetGrpUserVipInfo(const std::vector<uint32_t>& vecUids) = 0;

			// ��ȡİ���˵Ļ�Ա��Ϣ
			virtual void GetSingleVipInfo(uint32_t uPeerId) = 0;

			virtual void GetBatchColorRingInfo(const std::vector<uint32_t>& vecUids) = 0;

			/*
			* vip�û����ú���Ϊ�������Ѷ���
			* @param uid ���������û���uid
			*/
			virtual void SetWatchUser(uint32_t uid) = 0;

			/*
			* vip�û�ȡ������Ϊ�������Ѷ���
			* @param uid ���������û���uid
			*/
			virtual void CancleWatchUser(uint32_t uid) = 0;

			/*
			* vip�û���¼ʱ��ȡ���������б�
			*/
			virtual void GetWatcherList() = 0;

			/*
			* ��ȡ��Ա����ʹ�õı����������������ȷ���Լ���vip�����
			*/
			virtual void GetVipExpWhiteList(const std::vector<uint32_t>& vecUids) = 0;

			// ��̬ͷ��
			// strFileName  �ϴ�ͼƬ���ļ�����  strContext  �ͻ���������
			virtual void UpLogoAuth(const std::string& strFileName, const std::string& strContext) = 0;
			virtual void ChangeSysLogo(const uint32_t uLogoId) = 0;

			// �������߻���ui�������ú���
			// uid ��������uid
			// ������Դ 1 im  2 group  3 channel
			virtual void GetUserOffLineScore(const uint32_t uid,const uint32_t from) = 0;
			
			/* �û���¼�����ȡvͼ����
			 * param uid �û���uid
			 */
			virtual void GetVipActivity(const uint32_t& uid) = 0;
			/* �û����vͼ��,ȡ��vͼ��Ļ��
			 * param uid �û���uid
			 * param m_vec_activity �û���صĻid
			 */
			virtual void CancleActivity(uint32_t uid,std::vector<uint32_t> m_vec_activity) = 0;

			// ��ȡ�����Ա��Ϣ������������Ϣ��½��ʱ��protocol����
			// @param  type   1 ���Ѽ��Լ��� 2 ����İ����
			virtual void GetPlDiamondInfo(uint32_t type, const std::vector<uint32_t>& vecUids) = 0;

			// ��ȡ�����Ա���߻���
			// @param  type   1 �����б�   2 Ⱥ��Ա   3 Ƶ���б�
			virtual void GetPlDiamondOffScore(uint32_t type, uint32_t uid) = 0;

			// ��ȡ�����Ա��Ϣ
			// @param  type   Ԥ��
			virtual void GetOwVipInfo(uint32_t type, const std::vector<uint32_t>& vecUids) = 0;
		};
	}
}
