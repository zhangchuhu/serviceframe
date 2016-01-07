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
	//! ֧�ֵ�yy��߰汾��(����)
	uint32_t yyMax;
	//! ֧�ֵ�yy��Ͱ汾��(����)
	uint32_t yyMin;
	//! ��չ�ֶΣ������Ժ�ʹ��
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
	//! Ӧ��id
	uint32_t appId;
	//! Ӧ������
	std::string name;
	//! Ӧ�����ļ���
	std::string fileName;
	//! �Ƽ��汾,��������ʹ��
	std::string appDesc;
	//! Ӧ������,���256byte
	uint32_t recommandVersion;
	//! �Ƿ���ʱ����,0��,1��
	uint16_t delayFlag;
	//! �������,��js.dll,exe,flash֮��
	uint16_t appBinaryType;
	//! ���λ������,��.����Ӧ��,����Ӧ��
	uint16_t appPosType;
	//! �汾�б�
	std::vector<VersionItem2> versionItems;
    //! ����
	uint32_t order;
    //! �Ƿ�ΪOW��ӵ�Ӧ��
	uint8_t opByOw;
	//! ��Ϣ·�ɱ�
	std::vector<uint64_t> routeTable;
	//! sdk �汾��
	uint16_t sdkVersion;
	//! ��չ�ֶΣ������Ժ�ʹ��
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

//! Ƶ����Ӧ���б�
struct Sid2Apps : public sox::Marshallable
{
	//! ��Ƶ��Ӧ���б�
	std::vector<uint32_t> appIds;

	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, appIds);
	}

	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::back_inserter(appIds));
	}
};

//! Ƶ������
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

//! ��Ƶ��Ӧ������
struct Sid2AppsMap : public sox::Marshallable
{
	//! ��Ƶ��Ӧ������
	std::map<uint32_t, Sid2Apps> sid2Apps;
	//! ��Ƶ��Ӧ�������б�
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

//! Ӧ�þ�̬��������
struct AppInfoPackMap : public sox::Marshallable
{
	//! Ӧ�þ�̬��������
	std::map<uint32_t, AppInfoPack2> appPacks;

	virtual void marshal(sox::Pack &p) const {
		sox::marshal_container(p, appPacks);
	}

	virtual void unmarshal(const sox::Unpack &up) {
		sox::unmarshal_container(up, std::inserter(appPacks, appPacks.begin()));
	}
};

//! linkd��ѯƵ����Ӧ��Э��
struct PGetAppsReq : public sox::Marshallable
{
	enum {uri = (1001 << 8 | protocol::APPMGR_SVID)};

	//! �û�uid
	uint32_t uid;
	//! ����Ƶ����
	uint32_t sid;
	//! session group id
	uint32_t sessGroup;
	//! yy�汾��Ϣ
	uint32_t yyVer;
	//! yysdk�汾��
	uint32_t yySdkVer;
	//! ��չ�ֶ�
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

//! ���Ӧ��
struct PAddAppReq : public sox::Marshallable
{
	enum {uri = (1003 << 8 | protocol::APPMGR_SVID)};

	//! ����Ƶ��ID
	uint32_t sid;
	//! Ӧ�þ�̬����,AppInfoPackMap marshal�����̬
	std::string appInfos;
	//! ��Ƶ��Ӧ�ö�Ӧ�б�,Sid2AppsMap marshal�����̬(����ѹ��)
	std::string sidApps;
	//! ȫƵ��Ӧ��
	std::vector<uint32_t> globalApps;
	//! 1 Ϊѹ�� 0 Ϊ��ѹ��
	uint8_t appInfosGzipFlag;
	//! appinfosѹ��ǰ�ĳ���
	uint32_t appInfosLenOri;
	//! 1 Ϊѹ�� 0 Ϊ��ѹ��
	uint8_t sidAppsGzipFlag;
	//! sidappsѹ��ǰ�ĳ���
	uint32_t sidAppsLenOri;
	//! ��Ƶ����ǰӵ�е�Ӧ���б�
	std::set<uint32_t> hasApps;
	//! �ͻ��˳�ʱ���»�ȡ�ļ��, ����Ϊ��λ
	uint32_t regetTimeout;
	//! ��չ�ֶ�
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

//! �Ƴ�Ӧ��
struct PDelAppReq : public sox::Marshallable
{
	enum {uri = (1005 << 8 | protocol::APPMGR_SVID)};

	//! ����Ƶ��ID
	uint32_t sid;
	//! ��Ƶ��Ӧ�ö�Ӧ�б� Sid2AppsMap marshal�����̬(����ѹ��)
	std::string sidApps;
	//! ȫƵ��Ӧ��
	std::vector<uint32_t> globalApps;
	//! 1 Ϊѹ�� 0 Ϊ��ѹ��
	uint8_t sidAppsGzipFlag;
	//! sidappsѹ��ǰ�ĳ���
	uint32_t sidAppsLenOri;
	//! ��չ�ֶ�
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

