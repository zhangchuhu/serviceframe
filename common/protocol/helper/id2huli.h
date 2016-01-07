#pragma once

#include "protocol/ilist.h"
#include <exception>
#include "utf8to.h"
#include "huli_helper.h"

static protocol::HULI uid_to_huli(uint32_t uid)
{
	const size_t bufsize = 20;
	wchar_t buf[bufsize] = {0};
	int idx = bufsize - 5;
	while((uid != 0) && (idx >= 0))
	{
		buf[idx] = uid % 10 + L'0';
		uid /= 10;
		--idx;
	}
	buf[bufsize - 4] = L'_';
	buf[bufsize - 3] = L'@';
	buf[bufsize - 2] = L'U';
	return protocol::HULI(buf + idx + 1);
}

static protocol::HULI fid_to_huli(uint32_t fid)
{
	wchar_t buf[20] = {0};

	swprintf(buf, L"%d_@F", fid);
	protocol::HULI hid(buf);
	return hid;
}

static protocol::HULI sid_to_huli(uint32_t sid)
{
	if(sid != INVALID_UID){
		const size_t bufsize = 20;
		wchar_t buf[bufsize] = {0};
		int idx = bufsize - 5;
		while((sid != 0) && (idx >= 0))
		{
			buf[idx] = sid % 10 + L'0';
			sid /= 10;
			--idx;
		}
		buf[bufsize - 4] = L'_';
		buf[bufsize - 3] = L'@';
		buf[bufsize - 2] = L'S';
		return protocol::HULI(buf + idx + 1);
	}else{
		return L"";
	}
}
//wuji start

static protocol::HULI gameid_to_huli(uint32_t nGameId/*没有玩游戏的情况下为-1*/)
{
	if(nGameId >=0 && nGameId!=uint32_t(-1))
	{
		wchar_t buf[20] = {0};
		swprintf(buf, L"%d_@G", nGameId);
		protocol::HULI hid(buf);
		return hid;
	}
	else
	{
		return _T("");
	}
}


static protocol::HULI number_to_wstring(uint32_t uData)
{
	if(uData >=0 &&uData!=uint32_t(-1))
	{
		wchar_t buf[20] = {0};
		swprintf(buf, L"%u", uData);
		UniString hid(buf);
		return hid;
	}
	else
	{
		return _T("");
	}
}

//wuji end
static uint32_t huli_to_id(const protocol::HULI& hid)
{
	if(!isHuli(hid)){
		//throw std::runtime_error("invalid hid:" + app2net(hid));
		return INVALID_UID;
	}
	//int ret = _wtoi(hid.c_str());//这个范围很容易溢出
	uint32_t ret=_wtol(hid.c_str());
	if(ret == 0)
	{
		return INVALID_UID;
	}else{
		return ret;
	}
}

static uint32_t huli_to_id_safe(const protocol::HULI& hid)
{
	if(!isHuli(hid)){
		return 0;
	}
	uint32_t ret=_wtol(hid.c_str());
	if(ret == 0)
	{
		return INVALID_UID;
	}else{
		return ret;
	}
}

static bool isUser(const protocol::HULI &v){
	if(v.length() > 3){
		std::wstring xx = v.substr(v.length() - 3, 3);
		return xx == _T("_@U");
	}
	return false;
}
