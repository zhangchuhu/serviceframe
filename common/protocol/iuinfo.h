#ifndef IUInfo_Protocol_H
#define IUInfo_Protocol_H
#include "common/int_types.h"
#include "common/unistr.h"
#include "const.h"

namespace protocol {

namespace uinfo {
struct IUInfoWatcher;

struct MiniInfo{
	UniString account;//	�ʺţ����䣨ע�᲻��֤���䣩����64�Ĵ�@�ַ����ַ���
	UniString nick; //�ǳƣ�255λ
	SEX sex; //�Ա��С�Ů
};

struct UInfo : public MiniInfo {
	UniString passwd;//���� 6-32
	long birthday; //���գ���/��/��
	int area;//����
	int province; //ʡ��
	int city; //���У��������
	bool validate;
	UniString resume;//���˼�飺1024λ
	long updateTime;//�ϴε�¼ʱ��
	uint32_t updateIp;//�ϴε�¼IP
};

struct IUInfoWatchBase{
	virtual void watch(IUInfoWatcher *) = 0;
	virtual void revoke(IUInfoWatcher *) = 0;
};

struct IUInfo: public virtual IUInfoWatchBase{
	
	virtual uint32_t getVersion() const=0;
	virtual UniString getAccount() const = 0;
	
	virtual UniString getNick() const = 0;
	virtual void setNick(const UniString &) = 0;

	virtual void setPasswd(const UniString &) = 0;

	virtual SEX getSex() const = 0;
	virtual void setSex(SEX) = 0;

	virtual long getBirthday() const = 0;
	virtual void setBirthday(long) = 0;

	virtual int getArea() const = 0;
	virtual void setArea(int) = 0;

	virtual int getProvince() const = 0;
	virtual void setProvince(int) = 0;

	virtual int getCity() const = 0;
	virtual void setCity(int) = 0;

	virtual bool getValidate() const = 0;

	virtual UniString getResume() const = 0;
	virtual void setResume(const UniString &) = 0;

	virtual long getUpdateTime() const = 0;
	virtual uint32_t getUpdateIp() const = 0;

	virtual uint32_t getUserJF() const = 0;

	//����refresh
	virtual void refresh() = 0;
	virtual void update() = 0;

	//virtual void release() = 0;

	virtual UniString getSign() = 0;

	virtual void setSign(const UniString &s) = 0;

	virtual UniString getPassport() const= 0;

	virtual UniString getIntro() const = 0;

	virtual void setIntro(const UniString &intro) = 0; 

	virtual UniString getImid() = 0; 

	virtual uint64_t getId() = 0;
	
	/**
		set 0 ��ʾȡ��֧�ֵ���ӡ�
	**/
	virtual void setWC2010(int countryCode) = 0;

	/**
		Ĭ�Ϸ��� 0 ��ʾû��ѡ��֧�ֵ���ӡ�
	**/
	virtual int getWC2010() = 0;

};


}
}
#endif
