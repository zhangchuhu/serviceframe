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
		SOCKS_PROXY,	// socks����
		HTTP_PROXY		// http����
	};

	/**
	 * NetworkProxyInfo ���ֶ�˵����
		1. proxyTypeΪNO_PROXY�������ֶβ��
		2. proxyTypeΪSOCKS_PROXY��HTTP_PROXY��ip��port�����user��password����ʵ�������д
	 */
	struct NetworkProxyInfo
	{
		ProxyType	proxyType;
		
		UniString	ip;		// ������ip
		int			port;

		UniString	user;
		UniString	password;
	};

	struct IAsyncProxyTestWatcher
	{
		/**
		 * @param	bWorking	true�����Ӵ��������
								false�����Ӵ��������ʧ��
		 */
		virtual void proxyTestResult(bool bWorking) = 0;
	};

	// IConn�ص�֪ͨIProxy����������Ƿ���
	struct IProxyTestResultCallBack
	{
		virtual void isProxyWorking(bool bWorking) = 0;
	};

	struct IProxy
	{
		virtual ~IProxy() {}

		virtual void revokeTestWacher(IAsyncProxyTestWatcher* pWatcher) = 0;

		/**
		 * UI���ڲ����Ƿ������ϴ����������UI���ڹر�ʱ��Ҫ���� revokeTestWacher()
		 *
		 * @param	proxyInfo	������Ϣ
		 * @param	pWatcher	watcher���ص�֪ͨ�����Ƿ����
		 */
		virtual void isProxyServerWorking(const NetworkProxyInfo& proxyInfo, IAsyncProxyTestWatcher* pWatcher) = 0;

		/**
		 * UI���øĽӿ�֪ͨprotocol���ô����������Ϣ
		 *
		 * @param	proxyInfo	������Ϣ
		 */
		virtual void setProxyInfo(const NetworkProxyInfo& proxyInfo) = 0;

		/**
		 * protocol�ڲ���
		 */
		virtual NetworkProxyInfo getProxyInfo(void) = 0;

		/**
		 * protocol�ڲ���
		 */
		virtual void setProxyTestConnCreator(core::IProxyTestConnCreator* pConnCreator) = 0;
	};
}
#endif	// IPROXY_H_20091105
