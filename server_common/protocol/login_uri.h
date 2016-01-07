/********************************************************************
	created:	2013-1-4   20:00
	filename: 	login_uri.h
	author:		Luke Chou
	copyright:  Guangzhou huaduo Co., Ltd.
	purpose:
    description:
                
---------------------------------------------------------------------
sequence      date                  author          update history
1             2013/1/4             Luke Chou       initial
*********************************************************************/

#ifndef _login_uri_H_2013_1_4
#define _login_uri_H_2013_1_4

#include "common/core/base_svid.h"

namespace protocol
{
	static const unsigned int WRITER_MASTER_SVID = 182;
	static const unsigned int AUTHORIRY_SVID	= 183;
};

enum E_LOGIN_URI
{

	// 密码校验
	URI_UDBVERIFY_PWD_REQ = (6 << 8)|protocol::SERVERLOGIN_SVID,
	URI_UDBVERIFY_PWD_RES = (7 << 8)|protocol::SERVERLOGIN_SVID,
	// 客户端登录:版本2.0
	URI_UDBYYLOGIN_REQ = (8 << 8)|protocol::SERVERLOGIN_SVID,
	URI_UDBYYLOGIN_RES = (9 << 8)|protocol::SERVERLOGIN_SVID,
	// 下发短信验证码:版本2.0
	URI_UDBYYSEND_SMSCODE_REQ = (18 << 8)|protocol::SERVERLOGIN_SVID,
	URI_UDBYYSEND_SMSCODE_RES = (19 << 8)|protocol::SERVERLOGIN_SVID,
	    
	// 业务登录:版本1.0
	URI_UDBLOGIN_REQ = (10 << 8)|protocol::SERVERLOGIN_SVID,
	URI_UDBLOGIN_RES = (11 << 8)|protocol::SERVERLOGIN_SVID,
	// 下发短信验证码:版本1.0
	URI_UDBSEND_SMSCODE_REQ = (16 << 8)|protocol::SERVERLOGIN_SVID,
	URI_UDBSEND_SMSCODE_RES = (17 << 8)|protocol::SERVERLOGIN_SVID,



};


#endif //_login_uri_H_2013_1_4
