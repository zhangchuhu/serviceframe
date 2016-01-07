#ifndef IPROXY_H_20091105
#define IPROXY_H_20091105

#include "common/unistr.h"

namespace core
{
	struct IProxyTestConnCreator;
}

namespace protocol {
	enum ProxyType
	{
		NO_PROXY = 0,
		SOCKS_PROXY,	// socks代理
		HTTP_PROXY		// http代理
	};

	/**
	 * NetworkProxyInfo 的字段说明：
		1. proxyType为NO_PROXY，其他字段不填。
		2. proxyType为SOCKS_PROXY或HTTP_PROXY，ip和port必须填，user、password根据实际情况填写
	 */
	struct NetworkProxyInfo
	{
		ProxyType	proxyType;
		
		UniString	ip;		// 域名或ip
		int			port;

		UniString	user;
		UniString	password;
	};

	struct IAsyncProxyTestWatcher
	{
		/**
		 * @param	bWorking	true，连接代理服务器
								false，连接代理服务器失败
		 */
		virtual void proxyTestResult(bool bWorking) = 0;
	};

	// IConn回调通知IProxy代理服务器是否工作
	struct IProxyTestResultCallBack
	{
		virtual void isProxyWorking(bool bWorking) = 0;
	};

	struct IProxy
	{
		virtual ~IProxy() {}

		virtual void revokeTestWacher(IAsyncProxyTestWatcher* pWatcher) = 0;

		/**
		 * UI用于测试是否能连上代理服务器，UI窗口关闭时需要调用 revokeTestWacher()
		 *
		 * @param	proxyInfo	代理信息
		 * @param	pWatcher	watcher，回调通知代理是否可用
		 */
		virtual void isProxyServerWorking(const NetworkProxyInfo& proxyInfo, IAsyncProxyTestWatcher* pWatcher) = 0;

		/**
		 * UI调用改接口通知protocol设置代理服务器信息
		 *
		 * @param	proxyInfo	代理信息
		 */
		virtual void setProxyInfo(const NetworkProxyInfo& proxyInfo) = 0;

		/**
		 * protocol内部用
		 */
		virtual NetworkProxyInfo getProxyInfo(void) = 0;

		/**
		 * protocol内部用
		 */
		virtual void setProxyTestConnCreator(core::IProxyTestConnCreator* pConnCreator) = 0;
	};
}
#endif	// IPROXY_H_20091105
