/********************************************************************
	created:	2012-11-22   15:57
	filename: 	lg_monitor_i.h
	author:		Luke Chou
	copyright:  Guangzhou huaduo Co., Ltd.
	purpose:
    description:
                
---------------------------------------------------------------------
sequence      date                  author          update history
1             2012/11/22             Luke Chou       initial
*********************************************************************/

#ifndef _lg_monitor_i_H_2012_11_22
#define _lg_monitor_i_H_2012_11_22

#include <iomanip>
#include<iostream>
#include<string>
#include<sstream>
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "protocol/CodeInfo.h"
#include <vector>
#include "common/res_code.h"

struct ServerInvalidNotifyReq:public sox::Marshallable
{
	enum{uri = (101 << 8)|protocol::LG_COMMON_SVID};
	std::string     _servName;
	uint32_t        _servId;
	std::string     _desc;    // 描述信息
	ServerInvalidNotifyReq():_servId(0){}
	ServerInvalidNotifyReq( const ServerInvalidNotifyReq& rth )
	{
		_servName = rth._servName;
		_servId = rth._servId;
		_desc = rth._desc;
	}

	ServerInvalidNotifyReq& operator=( const ServerInvalidNotifyReq& rth )
	{
		if( this == &rth )
		{
			return *this;
		}

		_servName = rth._servName;
		_servId = rth._servId;
		_desc = rth._desc;

		return *this;
	}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _servName << _servId << _desc;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _servName >> _servId >> _desc;
	}
};

struct ServerInvalidNotifyRes:public sox::Marshallable
{
	enum{uri = (102 << 8)|protocol::LG_COMMON_SVID};
	uint32_t       _duration;  // 恢复所花时间（单位秒）
	ServerInvalidNotifyRes():_duration(0){}

	virtual void marshal(sox::Pack &pk) const
	{
		pk << _duration;
	}

	virtual void unmarshal(const sox::Unpack &up)
	{
		up >> _duration;
	}
};

#endif //_lg_monitor_i_H_2012_11_22
