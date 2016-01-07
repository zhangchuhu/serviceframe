#include "CookieHelper.h"
#include "openssl/blowfish.h"
#include "core/sox/logger.h"
#include "core/sox/snox.h"
#include "common/protocol/const.h"
#include <stdio.h>

using namespace server;
using namespace std;

static char bf_key[] = "123456)(*&^%$#@!";
static unsigned char cbc_iv [8]={0xfe,0xdc,0xba,0x98,0x76,0x54,0x32,0x10};

BF_KEY server::CookieHelper::m_key;
bool server::CookieHelper::m_bInitBFKey = false;

CookieHelper::CookieHelper(uint32_t u, const std::string &n, uint32_t ip, const std::string &passwd, const char *from, uint64_t tid){
	InitBFKey();

	uid = u;
	passport = n;
	for(std::string::iterator it = passport.begin(); it != passport.end(); ++it){
		if(*it == ':'){
			*it = '.';
		}
	}
	this->ip = ip;
	time = sox::env::now;
	this->from = from;
	for(std::string::iterator it = this->from.begin(); it != this->from.end(); ++it){
		if(*it == ':'){
			*it = '.';
		}
	}

	version = 1;
	sha_passwd = passwd;
	termId = tid;
}

CookieHelper::CookieHelper(const CookieHelper& ck){
	InitBFKey();

	uid = ck.uid;
	passport = ck.passport;
	for(std::string::iterator it = passport.begin(); it != passport.end(); ++it){
		if(*it == ':'){
			*it = '.';
		}
	}
	ip = ck.ip;
	time = sox::env::now;
	from = ck.from;
	for(std::string::iterator it = from.begin(); it != from.end(); ++it){
		if(*it == ':'){
			*it = '.';
		}
	}

	version = 1;
	sha_passwd = ck.sha_passwd;
	extension = ck.extension;
	termId = ck.termId;
}

enum css{
	version_s,
	time_s,
	uid_s,
	ip_s,
	from_s,
	nick_s,
	passwd_s,
	extension_s,
	tid_s,
	end_s
};

void CookieHelper::InitBFKey(void)
{
	if (!m_bInitBFKey)
	{
		m_bInitBFKey = true;
		BF_set_key(&m_key, strlen(bf_key),(unsigned char *)bf_key);
	}
}

CookieHelper CookieHelper::parse(const std::string &str){
	if(str.length() > 512){
		CookieHelper ck(INVALID_UID, "", 0, "", "", 0);
		return ck;
	}
	InitBFKey();

	char out[512];
	memset(out, 0, sizeof out);
	unsigned char iv[8];
	memcpy(iv, cbc_iv, sizeof iv);

	BF_cbc_encrypt((const unsigned char *)str.data(), (unsigned char *)out, str.length(),&m_key, iv, BF_DECRYPT);

	const char delmi[] = ":";
	char *p=strtok(out, delmi);
	css status = version_s;

	int v = 0;
	uint32_t t = 0;
	uint32_t u = INVALID_UID;
	uint32_t i = 0;
	std::string f;
	std::string n;
	while(p)
	{
		switch(status){
			case version_s:
				status = time_s;
				v = atoi(p);
				//log(Info, "version: %d", v);
				break;
			case time_s:
				status = uid_s;
				t = atoi(p);
				//log(Info, "time: %u", t);
				break;
			case uid_s:
				status = ip_s;
				u = strtoul(p, NULL, 10);
				//log(Info, "uid: %u", u);
				break;
			case ip_s:
				status = from_s;
				i = atoi(p);
				//log(Info, "ip: %u", i);
				break;
			case from_s:
				status = nick_s;
				f = p;
				//log(Info, "from: %s", f.data());
				break;
			case nick_s:
				status = end_s;
				n = p;
				//log(Info, "nick: %s", n.data());
				break;
			default:
				break;
		}

		p = strtok(NULL, delmi);
	}

	log(Info, "client cookie=%u:%u:%u:%u:%s:%s", v, t, u, i, f.c_str(), n.c_str());

	if(status != end_s){
		CookieHelper ck(INVALID_UID, "", 0, "", f.c_str(), 0);
		return ck;
	}else{
		CookieHelper ck(u, n, i, "", f.c_str(), 0);
		ck.time = t;
		ck.version = v;

		return ck;
	}
}

//version:time:uid:ip:from:nick
std::string CookieHelper::toCookie() const{
	char in[512];
	if(passport.length() + from.length() < 300){
		sprintf(in, "%u:%lu:%u:%u:%s:%s", version, time, uid, ip, from.data(), passport.data());
		log(Info, "cookies: %s", in);
	}else{
		sprintf(in, "%u:%lu:%u:%u:%s:%s", version, time, uid, ip, "error", "overflow");
	}

	char out[512];
	memset(out, 0, sizeof out);
	unsigned char iv[8];
	memcpy(iv, cbc_iv, sizeof iv);

	int length = strlen(in);
	BF_cbc_encrypt((unsigned char *)in, (unsigned char *)out, length, &m_key, iv ,BF_ENCRYPT);

	/*BF_cbc_encrypt(cbc_out,cbc_in,len,
	&key,iv,BF_DECRYPT);*/
	return std::string(out, (length + 7) & ~7);
}

#define COOKIE_TIMEOUT 60 * 60 * 24 * 7

bool CookieHelper::isValid(uint32_t uid) const{
	if(this->uid != uid){
		log(Info, "uid invalid.");
		return false;
	}else if(this->uid == INVALID_UID){
		log(Info, "cookie invalid.");
		return false;
	}else if(sox::env::now - time > COOKIE_TIMEOUT){
		log(Info, "cookie out of date.");
		return false;
	}

	return true;
}


CookieHelper CookieHelper::parse20(const std::string &str){
	if(str.length() > 512){
		CookieHelper ck(INVALID_UID, "", 0, "", "", 0);
		return ck;
	}
	InitBFKey();

	char out[512];
	memset(out, 0, sizeof out);
	unsigned char iv[8];
	memcpy(iv, cbc_iv, sizeof iv);

	BF_cbc_encrypt((const unsigned char *)str.data(), (unsigned char *)out, str.length(), &m_key, iv, BF_DECRYPT);

	const char delmi[] = ":";
	char *p=strtok(out, delmi);
	css status = version_s;

	int v = 0;
	uint32_t t = 0;
	uint32_t u = INVALID_UID;
	uint32_t i = 0;
	std::string f;
	std::string n;
	std::string pwd;
	while(p)
	{
		switch(status){
			case version_s:
				status = time_s;
				v = atoi(p);
				//log(Info, "version: %d", v);
				break;
			case time_s:
				status = uid_s;
				t = atoi(p);
				//log(Info, "time: %u", t);
				break;
			case uid_s:
				status = ip_s;
				//u = atol(p);
				// TODO(lzd): atol转换不出大于2^31-1的数。
				//u = uint32_t(strtoll(p, NULL, 10));
				u = strtoul(p, NULL, 10);
				//log(Info, "uid: %u", u);
				break;
			case ip_s:
				status = from_s;
				i = atoi(p);
				//log(Info, "ip: %u", i);
				break;
			case from_s:
				status = nick_s;
				f = p;
				//log(Info, "from: %s", f.data());
				break;
			case nick_s:
				status = passwd_s;
				n = p;
				//log(Info, "nick: %s", n.data());
				break;
			case passwd_s:
				status = end_s;
				pwd = p;
				//log(Info, "passwd: %s", pwd.data());
				break;
			default:
				break;
		}
		
		p = strtok(NULL, delmi);
	}

	log(Info, "client cookie=%u:%u:%u:%u:%s:%s", v, t, u, i, f.c_str(), n.c_str());

	if(status != end_s){
		CookieHelper ck(INVALID_UID, "", 0, "", f.c_str(), 0);
		return ck;
	}else{
		CookieHelper ck(u, n, i, pwd, f.c_str(), 0);
		ck.time = t;
		ck.version = v;

		return ck;
	}
}

//version:time:uid:ip:from:nick
std::string CookieHelper::toCookie20() const{
	char in[512];
	char newCookieText[512];
	memset(newCookieText, 0, sizeof(char) * 512);
	if(passport.length() + from.length() < 300){
		sprintf(in, "%u:%lu:%u:%u:%s:%s:%s", version, time, uid, ip, from.data(), passport.data(), sha_passwd.data());
		sprintf(newCookieText, "%u:%lu:%u:%u:%s:%s", version, time, uid, ip, from.c_str(), passport.c_str());
		log(Info, "cookies: %s", newCookieText);
	}else{
		sprintf(in, "%u:%lu:%u:%u:%s:%s:%s", version, time, uid, ip, "error", "overflow", sha_passwd.data());
	}

	char out[512];
	memset(out, 0, sizeof out);
	unsigned char iv[8];
	memcpy(iv, cbc_iv, sizeof iv);

	int length = strlen(in);
	BF_cbc_encrypt((unsigned char *)in, (unsigned char *)out, length, &m_key, iv ,BF_ENCRYPT);
	
	/*BF_cbc_encrypt(cbc_out,cbc_in,len,
		&key,iv,BF_DECRYPT);*/
	return std::string(out, (length + 7) & ~7);
}

#define COOKIE_TIMEOUT 60 * 60 * 24 * 7

bool CookieHelper::isValid20(std::string account, const string& sha_passwd) const{
	if(this->passport != account){
		log(Info, "account invalid.");
		return false;
	}else if(this->uid == INVALID_UID){
		log(Info, "cookie invalid.");
		return false;
	}else if(sox::env::now - time > COOKIE_TIMEOUT){
		log(Info, "cookie out of date.");
		return false;
	}else if(sha_passwd != this->sha_passwd){
		log(Info, "passwd invalid.");
		return false;
	}

	return true;
	//return this->uid == uid && 
	//		this->uid != INVALID_UID && 
	//		(sox::env::now - time < COOKIE_TIMEOUT) &&
	//		(sha_passwd == this->sha_passwd);
}

bool CookieHelper::isValid20NoTime(uint32_t uid, std::string account, const string& sha_passwd) const{
	if(this->passport != account){
		log(Info, "account invalid.");
		return false;
	}else if(this->uid == INVALID_UID || this->uid != uid){
		log(Info, "cookie invalid.");
		return false;
	}else if(sha_passwd != this->sha_passwd){
		log(Info, "passwd invalid.");
		return false;
	}

	return true;
}


CookieHelper CookieHelper::parse30(const std::string &str){
	if(str.length() > 512){
		CookieHelper ck(INVALID_UID, "", 0, "", "", 0);
		return ck;
	}
	InitBFKey();

	char out[512];
	memset(out, 0, sizeof out);
	unsigned char iv[8];
	memcpy(iv, cbc_iv, sizeof iv);

	BF_cbc_encrypt((const unsigned char *)str.data(), (unsigned char *)out, str.length(), &m_key, iv, BF_DECRYPT);

	const char delmi[] = ":";
	char *p=strtok(out, delmi);
	css status = version_s;

	int v = 0;
	uint32_t t = 0;
	uint32_t u = INVALID_UID;
	uint32_t i = 0;
	std::string f;
	std::string n;
	std::string pwd;
	std::string ext;
	while(p)
	{
		switch(status){
			case version_s:
				status = time_s;
				v = atoi(p);
				//log(Info, "version: %d", v);
				break;
			case time_s:
				status = uid_s;
				t = atoi(p);
				//log(Info, "time: %u", t);
				break;
			case uid_s:
				status = ip_s;
				u = strtoul(p, NULL, 10);
				//log(Info, "uid: %u", u);
				break;
			case ip_s:
				status = from_s;
				i = atoi(p);
				//log(Info, "ip: %u", i);
				break;
			case from_s:
				status = nick_s;
				f = p;
				//log(Info, "from: %s", f.data());
				break;
			case nick_s:
				status = passwd_s;
				n = p;
				//log(Info, "nick: %s", n.data());
				break;
			case passwd_s:
				status = extension_s;
				pwd = p;
				//log(Info, "passwd: %s", pwd.data());
				break;
			case extension_s:
				status = end_s;
				ext = p;
				//log(Info, "passwd: %s", pwd.data());
				break;
			default:
				break;
		}
		
		p = strtok(NULL, delmi);
	}

	//version:time:uid:ip:from:nick:password:extension
	log(Info, "(%u)client cookie=%u:%u:%u:%u:%s:%s:**:%s", 
		status, v, t, u, i, f.c_str(), n.c_str(), ext.c_str());

	//compatible with toCookie and toCookie20
	// cookie30
	if(status == end_s){
		CookieHelper ck(u, n, i, pwd, f.c_str(), 0);
		ck.time = t;
		ck.version = v;
		return ck;
	}

	// cookie20
	if(status == extension_s){
		CookieHelper ck(u, n, i, pwd, f.c_str(), 0);
		ck.time = t;
		ck.version = v;
		return ck; 
	}

	// cookie(10)
	if(status == passwd_s){
		CookieHelper ck(u, n, i, "", f.c_str(), 0);
		ck.time = t;
		ck.version = v;
		return ck; 
	}

	CookieHelper ck(INVALID_UID, "", 0, "", f.c_str(), 0);
	return ck;

}

//version:time:uid:ip:from:nick:password:extension
std::string CookieHelper::toCookie30() const{
	char in[512];
	if(passport.length() + from.length() + extension.length() < 300){
		sprintf(in, "%u:%lu:%u:%u:%s:%s:%s:%s", version, time, uid, ip, from.data(), passport.data(), sha_passwd.data(), extension.data());
		log(Info, "cookies: %u:%lu:%u:%u:%s:%s:**:%s", 
			version, time, uid, ip, from.c_str(), passport.c_str(), extension.c_str());
	}else{
		sprintf(in, "%u:%lu:%u:%u:%s:%s:%s:%s", version, time, uid, ip, "error", "overflow", sha_passwd.data(), "ext");
	}

	char out[512];
	memset(out, 0, sizeof out);
	unsigned char iv[8];
	memcpy(iv, cbc_iv, sizeof iv);

	int length = strlen(in);
	BF_cbc_encrypt((unsigned char *)in, (unsigned char *)out, length, &m_key, iv ,BF_ENCRYPT);
	
	/*BF_cbc_encrypt(cbc_out,cbc_in,len,
		&key,iv,BF_DECRYPT);*/
	return std::string(out, (length + 7) & ~7);
}

bool CookieHelper::isValid30(uint32_t uid, const std::string & account, 
	const std::string & sha_passwd, const std::string & extension) const{
	if(this->passport != account){
		log(Info, "account invalid.");
		return false;
	}else if(this->uid == INVALID_UID || this->uid != uid){
		log(Info, "cookie invalid.");
		return false;
	}else if(sha_passwd != this->sha_passwd){
		log(Info, "passwd invalid.");
		return false;
	}else if(extension != this->extension){
		log(Info, "extension invalid");
		return false;
	}
	return true;
}


const std::string &CookieHelper::getPassport() const{
	return passport;
}

const std::string &CookieHelper::getExtension() const{
	return extension;
}

void CookieHelper::dump() const{
	log(Debug, "cookie from:%s ip:%s uid:%u time:%u version:%d nick:%s", from.data(), sox::addr_ntoa(ip).data(), uid, (uint32_t)time, version, passport.data());
}

CookieHelper CookieHelper::parse40(const std::string &str){
	if(str.length() > 512){
		CookieHelper ck(INVALID_UID, "", 0, "", "", 0);
		return ck;
	}
	InitBFKey();

	char out[512];
	memset(out, 0, sizeof out);
	unsigned char iv[8];
	memcpy(iv, cbc_iv, sizeof iv);

	BF_cbc_encrypt((const unsigned char *)str.data(), (unsigned char *)out, str.length(), &m_key, iv, BF_DECRYPT);

	const char delmi[] = ":";
	char *p=strtok(out, delmi);
	css status = version_s;

	int v = 0;
	uint32_t t = 0;
	uint32_t u = INVALID_UID;
	uint32_t i = 0;
	uint64_t tid = 0;
	std::string f;
	std::string n;
	std::string pwd;
	while(p)
	{
		switch(status){
			case version_s:
				status = time_s;
				v = atoi(p);
				//log(Info, "version: %d", v);
				break;
			case time_s:
				status = uid_s;
				t = atoi(p);
				//log(Info, "time: %u", t);
				break;
			case uid_s:
				status = ip_s;
				//u = atol(p);
				// TODO(lzd): atol转换不出大于2^31-1的数。
				//u = uint32_t(strtoll(p, NULL, 10));
				u = strtoul(p, NULL, 10);
				//log(Info, "uid: %u", u);
				break;
			case ip_s:
				status = from_s;
				i = atoi(p);
				//log(Info, "ip: %u", i);
				break;
			case from_s:
				status = nick_s;
				f = p;
				//log(Info, "from: %s", f.data());
				break;
			case nick_s:
				status = passwd_s;
				n = p;
				//log(Info, "nick: %s", n.data());
				break;
			case passwd_s:
				status = tid_s;
				pwd = p;
				//log(Info, "passwd: %s", pwd.data());
				break;
			case tid_s:
				status = end_s;
				tid = strtoull(p, NULL, 10);
				//log(Info, "uid: %u", u);
				break;
			default:
				break;
		}
		
		p = strtok(NULL, delmi);
	}

	log(Info, "client cookie=%u:%u:%u:%u:%s:%s:*(%u):%llu", v, t, u, i, f.c_str(), n.c_str(), pwd.size(), tid);

	CookieHelper ck(u, n, i, pwd, f.c_str(), tid);
	ck.time = t;
	ck.version = v;

	return ck;
}

//version:time:uid:ip:from:nick:password:termId
std::string CookieHelper::toCookie40() const{
	char in[512];
	char newCookieText[512];
	memset(newCookieText, 0, sizeof(char) * 512);
	if(passport.length() + from.length() < 300){
		sprintf(in, "%u:%lu:%u:%u:%s:%s:%s:%llu", version, time, uid, ip, from.data(), passport.data(), sha_passwd.data(), termId);
		sprintf(newCookieText, "%u:%lu:%u:%u:%s:%s:*:%llu", version, time, uid, ip, from.c_str(), passport.c_str(), termId);
		log(Info, "cookies: %s", newCookieText);
	}else{
		sprintf(in, "%u:%lu:%u:%u:%s:%s:%s:%llu", version, time, uid, ip, "error", "overflow", sha_passwd.data(), termId);
	}

	char out[512];
	memset(out, 0, sizeof out);
	unsigned char iv[8];
	memcpy(iv, cbc_iv, sizeof iv);

	int length = strlen(in);
	BF_cbc_encrypt((unsigned char *)in, (unsigned char *)out, length, &m_key, iv ,BF_ENCRYPT);
	
	/*BF_cbc_encrypt(cbc_out,cbc_in,len,
		&key,iv,BF_DECRYPT);*/
	return std::string(out, (length + 7) & ~7);
}


