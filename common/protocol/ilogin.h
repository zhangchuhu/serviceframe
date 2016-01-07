#ifndef ILogin_Protocol_H
#define ILogin_Protocol_H
#include "common/unistr.h"
//#include "common/protocol/plogin.h"
#include "common/protocol/const.h"
#include "common/protocol/CodeInfo.h"
namespace protocol {
namespace login {

struct ProxyInfo;
struct ILinkWatcher {
	virtual void onLinkStatus(LinkStatus status, int err) = 0;
	virtual void onLinkStatus2(LinkStatus2 status, int err) = 0;
};

struct ILoginWatchBase{
	virtual void watch(ILinkWatcher * w) = 0;
	virtual void revoke(ILinkWatcher * w) = 0;
};

struct IIMResultWatcher{
	virtual void onIMPassResult(const std::wstring&) = 0;
};

struct IIMResultWatchBase{
	virtual void watch(IIMResultWatcher *w) = 0;
	virtual void revoke(IIMResultWatcher *w) = 0;
};

enum LinkViewStatus{
	CONNECTING,
	LOGINED,
	ENDLOGIN,
	WAITRECONECTING,
};

struct ILogin:public virtual ILoginWatchBase {
	virtual void login(const UniString & mail, const std::string &sha1Pass, bool ispassport, ISPType isp, bool bSavePwd, protocol::E_IMSTATUS st) = 0;
	virtual bool IsEmailLogin() = 0;
	virtual void anonymouseLogin(const UniString &nick, ISPType isp) = 0;
	virtual void startAutoLogin() = 0;
	virtual void setProxy(const ProxyInfo &pi) = 0;
	virtual void logout() = 0;
	virtual UniString getUid(void) const = 0;
	virtual UniString getShutDownMsg() const = 0;
	virtual void forceLogin(const UniString & mail, const std::string &sha1Pass, const std::string &lbsip, int lbsport, bool bSavePwd, protocol::E_IMSTATUS st) = 0;
	virtual void forceLinkd(const UniString & mail, const std::string &sha1Pass, const std::string &linkip, int linkport) = 0;

	virtual void forceConnect(const std::string &linkip, int linkport) = 0;
	virtual LinkStatus getStatus() const = 0;
	virtual int getWaitSecs() const = 0;
	virtual LinkViewStatus translateStatus(LinkStatus s) const = 0; 
	virtual std::string getCookie() = 0;

	//��ȡ�����ļ�
	virtual std::string& getCodeData() = 0;
	//��ȡ�����ļ��б�
	virtual std::vector<server::sendcode::CodeInfo>& getCodeInfoSet() = 0;
	//��ȡcode ID
	virtual uint32_t getCodeID() = 0;
	//�����ȡ�����ļ��б������
	virtual void getCodeVersionInfo() = 0;
	//����ID��ȡ�����ļ�
	virtual void getCode(uint32_t uCodeID) = 0;

	//��ȡ��������,�ⶳʱ�䡣
	virtual std::pair<KickType, int> getFreezeParam() = 0;

	virtual void sendLoginedClients(uint32_t lc) = 0;

	/**
	 * ��ȡȫ�ֺ������Ľ��ʱ��
	 *
	 * @return ���ؽ��ʱ�䣬�ַ�����ʾ��
	 */
	virtual UniString getUnbanDate(void) = 0;

	/**
	 * ��ȡȫ�ֺ�������ԭ�򣨱�ע��
	 *
	 * @return ���ر���ԭ�򣨱�ע����
	 */
	virtual UniString getBanNote(void) = 0;

	virtual void watchResult(IIMResultWatcher *w) = 0;
	virtual void revokeResult(IIMResultWatcher *w) = 0;
};

enum LbsStatus{
	LOW_VERSION,
	CORRECT_VERSION,
	SERVER_SHUTDOWN
};

struct ICheckVersion{
	virtual int checkVersion(UniString &msg) = 0;

	virtual ISPType getClientIsp() const = 0;

	virtual ISPType getTestIsp() const = 0;

	virtual void setTestIsp(ISPType val) = 0;
};
}
}
#endif
