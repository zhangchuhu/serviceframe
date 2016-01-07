// test.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../UrlHelper.h"
#include "../id_def.h"

#include <cassert>

int _tmain(int argc, _TCHAR* argv[])
{
	int i1 = 1;
	int i2 = 0;
	int i3 = 0x1FFF;
	int i4 = 0x2000;
	int i5 = 8000;
	int i6 = 38000;

	std::string u1 = uid2LinkUrl(i1);
	assert(u1 == "2.lu/1");
	std::string u2 = uid2LinkUrl(i2);
	assert(u2 == "1.lu/0");
	std::string u3 = uid2LinkUrl(i3);
	assert(u3 == "8192.lu/8191");
	std::string u4 = uid2LinkUrl(i4);
	assert(u4 == "8193.lu/8192");
	std::string u5 = uid2LinkUrl(i5);
	assert(u5 == "8001.lu/8000");
	std::string u6 = uid2LinkUrl(i6);
	assert(u6 == "1273.lu/38000");

	DomainName dn = DomainName::parse(u6);
	assert(dn.isUser());
	assert(dn.getParam() == "38000");
	assert(dn.toDomain() == "1273.lu");

	std::string tu1 = "mm/";
	DomainName d1 = DomainName::parse(tu1);
	assert(d1.getParam() == "");

	std::string tu2 = "mm./";
	DomainName d2 = DomainName::parse(tu2);
	assert(d2.toDomain() == "mm");
	assert(d2.isUser() == false);

	std::string tu3 = ".mm";
	DomainName d3 = DomainName::parse(tu3);
	assert(d3.toDomain() == "mm");
	
	std::vector<std::string> ps;
	ps.push_back("1111");

	assert(DomainName::toDomain(ps, "lu") == "1111.lu");

	uint32_t uxxx;
	assert(DomainName::toUid(u6, uxxx));
	assert(uxxx = 38000);

	std::string us = "lk/31";
	assert(!DomainName::getSid(u6, uxxx));

	assert(DomainName::getSid(us, uxxx));
	assert(uxxx == 31);

	DomainName dnx = DomainName::parse("lk/9");
	assert(dnx.getRoot() == "lk");

	uint32_t xxx;
	DomainName::toUid("27.lu/27", xxx);
	assert(xxx == 27);

	assert(DomainName::getSid("29.im/29", xxx) == false);
	return 0;
}

