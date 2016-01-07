#ifndef IVIP_SKIN_H_INCLUDE
#define IVIP_SKIN_H_INCLUDE
#include "const.h"
#include "common/int_types.h"
#include "common/packet.h"
#include <map>
#include <vector>

// ��ԱƤ���ӿ�
namespace protocol{ namespace vip{

	/*
	Ƥ������ҳ�����ļ�
	url :  http://dl.vip.yy.com/vipskin/vip_skin.xml
	��ʽ:
	<vipskin>
		<skin id = "1">
			<png>http://xxxxx</png>
			<url>http://xxxxx</url>
			<rgb>��ť��Ӧrgbֵ��uint32_t</rgb>
		</skin>
		....
	</vipskin>
	*/

	struct VipSkinInfo : public sox::Marshallable
	{
		VipSkinInfo() : m_bShow(false), m_uRgb(0) {}

		bool        m_bShow;                // �Ƿ������show
		std::string m_strUrl;               // Ƥ��url��urlΪ�ձ�ʾ�޻�ԱƤ��
		uint32_t    m_uRgb;                 // ��尴ťͼ���rgbֵ #FFFFFF

		virtual void marshal(sox::Pack& p) const 
		{
			p.push_uint8(m_bShow);
			p << m_strUrl << m_uRgb;
		}
		virtual void unmarshal(const sox::Unpack &p)
		{
			m_bShow = (bool)p.pop_uint8();
			p >> m_strUrl >> m_uRgb;
		}
	};

	// protocol�ص�
	class IVipSkinWather
	{
	public:

		virtual ~IVipSkinWather() {}

		// �����Ƿ��������Ƥ���ص�
		virtual void onSetShow(const bool& bShow) = 0;

		// ����Ƥ���ص�
		virtual void onSetSkin(const std::string& strUrl, const uint32_t& uRgb) = 0;

		// Ƥ�����֪ͨ
		virtual void onSkinChange(const uint32_t& uUid, const std::string& strUrl, const uint32_t& uRgb) = 0;

		// �Ƿ������show���֪ͨ
		virtual void onShowChange(const uint32_t& uUid, const bool& bShow) = 0;

		/* 
		*   ��ȡ��Ա���ѵ�Ƥ����Ϣ�Ļص��������Լ���
		*   1���ûص�����������IM��½�ɹ���������������𣨴����2~3�����ʱ����
		*      Ҳ����˵�ڿͻ�����ʾ��2~3�룬protocol�Ż���ص���Ϊ���Ż����飬����
		*      �ͻ��˶��Լ���Ƥ����Ϣ�������ļ����棬�ڻص�֮ǰ�����Լ��ϴε�½��Ƥ��
		*/
		virtual void onGetSkin(const std::map<uint32_t, VipSkinInfo>& mapUid2Skin) = 0;
	};

	class IVipSkinWatcherBase
	{
	public:
		virtual ~IVipSkinWatcherBase() {}

		virtual void watch(IVipSkinWather *w) = 0;
		virtual void revoke(IVipSkinWather *w) = 0;
	};

	class IVipSkin : public IVipSkinWatcherBase
	{
	public:

		virtual ~IVipSkin() {}

		// �����Ƿ��������Ƥ��
		virtual void setShow(bool bShow) = 0;

		// ���û�ԱƤ��
		virtual void setSkin(const std::string& strUrl, uint32_t uRgb) = 0;

		/*
		*  ��ȡ��Ա���ѵ�Ƥ����Ϣ
		*  1����½ʱ����ȡ���л�Ա���ѣ������Լ�����Ƥ����Ϣ
		*     �ǻ�Ա��Ƥ����Ȩ��û��Ҫ��ȡ
		*  2���¼ӻ�Ա���Ѻ󣬻�ȡ���ѵ�Ƥ����Ϣ
		*  
		*  ��ΪUI�޷��� OnGetBuddyVipInfoRes �ж�����������������������ﻹ�ǰ������ӿںϲ�
		*/
		virtual void getSkinInfo(const std::vector<uint32_t>& vecUids) = 0;
	};

}}

#endif
