#pragma  once
#include <string>
#include "common/unistr.h"

//struct RECT;
//struct POINT;

namespace protocol {
namespace conf {
#define SYSTEM_USER L"@system"

struct IConfig {
	virtual int saveConfig() = 0;
	// 整数
	virtual int getInt(int tag, int _default) = 0;
	virtual void setInt(int tag, int value) = 0;
	// 字符串
	virtual void getStr(int tag, UniString & out) = 0;
	virtual void setStr(int tag, const UniString & str) = 0;
	// 接口没有提供默认值的，在使用前需要自己初始化
	// RECT
	virtual void getRect(int tag, RECT & rect) = 0;
	virtual void setRect(int tag, const RECT & rect) = 0;
	// POINT
	virtual void getPoint(int tag, POINT & point) = 0;
	virtual void setPoint(int tag, const POINT & point) = 0;
	// 二进制
	virtual std::string getBin(int tag, const std::string & _default) = 0;
	virtual void setBin(int tag, const std::string & bin) = 0;

//	virtual void release() = 0;
};

struct IConfigManager{
	virtual IConfig *getConfig(const UniString &uid) = 0;
	//virtual void releaseConfig(IConfig *config) = 0;
	virtual int removeConfig(const UniString &uid) = 0;
};
}
}
