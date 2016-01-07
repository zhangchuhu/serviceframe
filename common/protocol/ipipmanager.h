#ifndef IPipManager_Protocol_H
#define IPipManager_Protocol_H

#include "common/unistr.h"

namespace protocol {
namespace pip {

struct IPipManagerWatcher
{
	virtual void onChannelAllocated(DWORD channelId) = 0;
	virtual void onChannelAllocated(const UniString &teamString, DWORD channelId) = 0;
	virtual void onRefreshChannelResp(DWORD err) = 0;
	virtual void onReleaseChannelNameResp(DWORD err) = 0;
	virtual void onDestroyChannelResp(DWORD err) = 0;
	
	virtual void onRegisterUserResp(DWORD err) = 0;
	virtual void onRefreshUserResp(DWORD err) = 0;
	virtual void onDestroyUserResp(DWORD err) = 0;
	virtual void onAuthToken(const std::string & res) = 0;
	virtual void onGetInfoGeneric(const std::string & res) = 0;
	virtual void onSetInfoGeneric(const std::string & res) = 0;
};

struct IPipManagerWatcherBase
{
	virtual void watch(IPipManagerWatcher *) = 0;
	virtual void revoke(IPipManagerWatcher *) = 0;
};

struct IPipManager : public virtual IPipManagerWatcherBase
{
	virtual void refreshUser(DWORD uid) = 0;
	virtual void destroyUser(DWORD uid) = 0;
	virtual void allocateChannel(const UniString &teamString) = 0;
	/**
	 * For im p2p voice. Call back is 'virtual void onChannelAllocated(const UniString &teamString, DWORD channelId) = 0;'.
	 * @param type			pip, imp2p, ...
	 * @param teamString	string id
	 */
	virtual void allocateChannel(const UniString& type, const UniString &teamString) = 0;
	virtual void refreshChannel(DWORD channelId) = 0;
	virtual void releaseChannelName(const UniString &teamString, DWORD channelId) = 0;
	virtual void destroyChannel(DWORD channelId) = 0;
	virtual void registerUser(const UniString &userName, const UniString &password) = 0;
	virtual int getUserRefreshInterval() const = 0;
	virtual int getChannelRefreshInterval() const = 0;
	virtual void sendAuthTokenReq(const std::string& req) = 0;
	virtual void getInfoGeneric(const std::string& req) = 0;
	virtual void setInfoGeneric(const std::string& req) = 0;
};

}//namespace pip
}//namespace protocol

#endif //IPipManager_Protocol_H