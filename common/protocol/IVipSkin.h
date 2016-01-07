#ifndef IVIP_SKIN_H_INCLUDE
#define IVIP_SKIN_H_INCLUDE
#include "const.h"
#include "common/int_types.h"
#include "common/packet.h"
#include <map>
#include <vector>

// 会员皮肤接口
namespace protocol{ namespace vip{

	/*
	皮肤设置页配置文件
	url :  http://dl.vip.yy.com/vipskin/vip_skin.xml
	格式:
	<vipskin>
		<skin id = "1">
			<png>http://xxxxx</png>
			<url>http://xxxxx</url>
			<rgb>按钮对应rgb值，uint32_t</rgb>
		</skin>
		....
	</vipskin>
	*/

	struct VipSkinInfo : public sox::Marshallable
	{
		VipSkinInfo() : m_bShow(false), m_uRgb(0) {}

		bool        m_bShow;                // 是否向好友show
		std::string m_strUrl;               // 皮肤url，url为空表示无会员皮肤
		uint32_t    m_uRgb;                 // 面板按钮图标的rgb值 #FFFFFF

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

	// protocol回调
	class IVipSkinWather
	{
	public:

		virtual ~IVipSkinWather() {}

		// 设置是否向好友秀皮肤回调
		virtual void onSetShow(const bool& bShow) = 0;

		// 设置皮肤回调
		virtual void onSetSkin(const std::string& strUrl, const uint32_t& uRgb) = 0;

		// 皮肤变更通知
		virtual void onSkinChange(const uint32_t& uUid, const std::string& strUrl, const uint32_t& uRgb) = 0;

		// 是否向好友show变更通知
		virtual void onShowChange(const uint32_t& uUid, const bool& bShow) = 0;

		/* 
		*   获取会员好友的皮肤信息的回调（包括自己）
		*   1、该回调的请求是在IM登陆成功，才向服务器发起（大概有2~3秒的延时）。
		*      也就是说在客户端显示后2~3秒，protocol才会给回调。为了优化体验，建议
		*      客户端对自己的皮肤信息做本地文件缓存，在回调之前加载自己上次登陆的皮肤
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

		// 设置是否向好友秀皮肤
		virtual void setShow(bool bShow) = 0;

		// 设置会员皮肤
		virtual void setSkin(const std::string& strUrl, uint32_t uRgb) = 0;

		/*
		*  获取会员好友的皮肤信息
		*  1、登陆时，获取所有会员好友（包括自己）的皮肤信息
		*     非会员无皮肤特权，没必要获取
		*  2、新加会员好友后，获取好友的皮肤信息
		*  
		*  因为UI无法从 OnGetBuddyVipInfoRes 判断是上面那种情况，所以这里还是把两个接口合并
		*/
		virtual void getSkinInfo(const std::vector<uint32_t>& vecUids) = 0;
	};

}}

#endif
