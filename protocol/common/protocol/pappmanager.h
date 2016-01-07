#ifndef PAPP_MANAGER_H_
#define PAPP_MANAGER_H_
#include "common/core/base_svid.h"
#include "common/packet.h"
#include "common/protocol/const.h"
#include "common/protocol/zip_tpl.h"
#include "protocol/CodeInfo.h"
#include <vector>
#include <set>
#include <map>
#include <iterator>
#include "common/res_code.h"

namespace protocol {

namespace session {

struct VersionItem2 : public sox::Marshallable
{
	uint32_t appVersion;
	std::string appUrl;
	std::string app7zMd5;
	std::string appBinaryMd5;
	//! 支持的yy最高版本号(包含)
	uint32_t yyMax;
	//! 支持的yy最低版本号(包含)
	uint32_t yyMin;
	//! 扩展字段，保留以后使用
	std::string expand;

	virtual void marshal(sox::Pack &p) const {
		p << appVersion << appUrl << app7zMd5;
		p << appBinaryMd5 << yyMax << yyMin << expand;
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> appVersion >> appUrl >> app7zMd5;
		up >> appBinaryMd5 >> yyMax >> yyMin >> expand;
	}
};

struct AppInfoPack2 : public sox::Marshallable
{
	//! 应用id
	uint32_t appId;
	//! 应用名称
	std::string name;
	//! 应用主文件名
	std::string fileName;
	//! 推荐版本,保留但不使用
	std::string appDesc;
	//! 应用描述,最大256byte
	uint32_t recommandVersion;
	//! 是否延时下载,0否,1是
	uint16_t delayFlag;
	//! 插件类型,如js.dll,exe,flash之类
	uint16_t appBinaryType;
	//! 插件位置类型,如.三栏应用,盒子应用
	uint16_t appPosType;
	//! 版本列表
	std::vector<VersionItem2> versionItems;
    //! 排序
	uint32_t order;
    //! 是否为OW添加的应用
	uint8_t opByOw;
	//! 消息路由表
	std::vector<uint64_t> routeTable;
	//! sdk 版本号
	uint16_t sdkVersion;
	//! 扩展字段，保留以后使用
	std::string expand;

	virtual void marshal(sox::Pack &p) const {
		p << appId << name << fileName << appDesc << recommandVersion;
		p << delayFlag << appBinaryType << appPosType;
		sox::marshal_container(p, versionItems);
		p << order << opByOw;
		sox::marshal_container(p, routeTable);
		p << sdkVersion << expand;
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> appId >> name >> fileName >> appDesc >> recommandVersion;
		up >> delayFlag >> appBinaryType >> appPosType;
		sox::unmarshal_container(up, std::back_inserter(versionItems));
		up >> order >> opByOw;
		sox::unmarshal_container(up, std::back_inserter(routeTable));
		up >> sdkVersion >> expand;
	}
};

//! 频道到应用列表
struct Sid2Apps : public sox::Marshallable
{
	//! 子频道应用列表
	std::vector<uint32_t> appIds;

	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, appIds);
	}

	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(appIds));
	}
};

//! 频道属性
struct SidAttr : public sox::Marshallable
{
	uint32_t frontApp;
	std::string expand;

	virtual void marshal(sox::Pack &p) const {
		p << frontApp << expand;
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> frontApp >> expand;
	}
};

//! 子频道应用数组
struct Sid2AppsMap : public sox::Marshallable
{
	//! 子频道应用数组
	std::map<uint32_t, Sid2Apps> sid2Apps;
	//! 子频道应用属性列表
	std::map<uint32_t, SidAttr> sid2Attribs;

	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, sid2Apps);
		sox::marshal_container(p, sid2Attribs);
	}

	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::inserter(sid2Apps, sid2Apps.begin()));
		sox::unmarshal_container(up, std::inserter(sid2Attribs, sid2Attribs.begin()));
	}
};

//! 应用静态数据数组
struct AppInfoPackMap : public sox::Marshallable
{
	//! 应用静态数据数组
	std::map<uint32_t, AppInfoPack2> appPacks;

	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, appPacks);
	}

	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::inserter(appPacks, appPacks.begin()));
	}
};

//! linkd查询频道的应用协议
struct PGetAppsReq : public sox::Marshallable
{
	enum {uri = (1001 << 8 | protocol::APPMGR_SVID)};

	//! 用户uid
	uint32_t uid;
	//! 顶级频道号
	uint32_t sid;
	//! session group id
	uint32_t sessGroup;
	//! yy版本信息
	uint32_t yyVer;
	//! yysdk版本号
	uint32_t yySdkVer;
	//! 扩展字段
	std::string expand;

	virtual void marshal(sox::Pack &p) const {
		p << uid << sid << sessGroup;
		p << yyVer << yySdkVer << expand;
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> uid >> sid >> sessGroup;
		up >> yyVer >> yySdkVer >> expand;
	}
};

//! 添加应用
struct PAddAppReq : public sox::Marshallable
{
	enum {uri = (1003 << 8 | protocol::APPMGR_SVID)};

	//! 顶级频道ID
	uint32_t sid;
	//! 应用静态数据,AppInfoPackMap marshal后的形态
	std::string appInfos;
	//! 子频道应用对应列表,Sid2AppsMap marshal后的形态(可能压缩)
	std::string sidApps;
	//! 全频道应用
	std::vector<uint32_t> globalApps;
	//! 1 为压缩 0 为不压缩
	uint8_t appInfosGzipFlag;
	//! appinfos压缩前的长度
	uint32_t appInfosLenOri;
	//! 1 为压缩 0 为不压缩
	uint8_t sidAppsGzipFlag;
	//! sidapps压缩前的长度
	uint32_t sidAppsLenOri;
	//! 该频道当前拥有的应用列表
	std::set<uint32_t> hasApps;
	//! 客户端超时重新获取的间隔, 以秒为单位
	uint32_t regetTimeout;
	//! 扩展字段
	std::string expand;


	virtual void marshal(sox::Pack &p) const {
		p << sid;
		p.push_varstr32(appInfos.data(), appInfos.length());
		p.push_varstr32(sidApps.data(), sidApps.length());
		sox::marshal_container(p, globalApps);
		p << appInfosGzipFlag << appInfosLenOri;
		p << sidAppsGzipFlag << sidAppsLenOri;// << expand;
		sox::marshal_container(p, hasApps);
		p << regetTimeout;
		p.push_varstr32(expand.data(), expand.length());
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid;
		appInfos = up.pop_varstr32();
		sidApps = up.pop_varstr32();
		sox::unmarshal_container(up, std::back_inserter(globalApps));
		up >> appInfosGzipFlag >> appInfosLenOri;
		up >> sidAppsGzipFlag >> sidAppsLenOri;// >> expand;
		unmarshal_container(up, std::inserter(hasApps, hasApps.begin()));
		up >> regetTimeout;
		expand = up.pop_varstr32();
	}
};

//! 移除应用
struct PDelAppReq : public sox::Marshallable
{
	enum {uri = (1005 << 8 | protocol::APPMGR_SVID)};

	//! 顶级频道ID
	uint32_t sid;
	//! 子频道应用对应列表 Sid2AppsMap marshal后的形态(可能压缩)
	std::string sidApps;
	//! 全频道应用
	std::vector<uint32_t> globalApps;
	//! 1 为压缩 0 为不压缩
	uint8_t sidAppsGzipFlag;
	//! sidapps压缩前的长度
	uint32_t sidAppsLenOri;
	//! 扩展字段
	std::string expand;

	virtual void marshal(sox::Pack &p) const {
		p << sid;
		p.push_varstr32(sidApps.data(), sidApps.length());
		sox::marshal_container(p, globalApps);
		p << sidAppsGzipFlag << sidAppsLenOri;// << expand;
		p.push_varstr32(expand.data(), expand.length());
	}

	virtual void unmarshal(const sox::Unpack &up) {
		up >> sid;
		sidApps = up.pop_varstr32();
		sox::unmarshal_container(up, std::back_inserter(globalApps));
		up >> sidAppsGzipFlag >> sidAppsLenOri;// >> expand;
		expand = up.pop_varstr32();
	}
};

}
}
#endif /*PAPP_MANAGER_H_*/

