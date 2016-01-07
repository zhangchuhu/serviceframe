#include "UrlHelper.h"
#include "id_def.h"
#include <stdlib.h>
#include <stdio.h>
#define USER_BUCKET_NUM 9182

uint32_t hashUid(uint32_t uid){
	return uid%USER_BUCKET_NUM + 1;
}

DomainName DomainName::parse(const std::string& query){
	DomainName ns;
	size_t pos = query.find_last_of('/');
	if(pos == std::string::npos){
		ns.param = "";
		pos = query.length();
	}else{
		ns.param = query.substr(pos + 1);
	}

	size_t dpos = query.find_first_of('.');
	if(dpos == std::string::npos){
		ns.domains.push_back(query.substr(0, pos));
	}else{
		size_t np = dpos + 1;
		if(pos > np)
			ns.domains.push_back(query.substr(np, pos - np));

		if(dpos > 0)
			ns.domains.push_back(query.substr(0, dpos));
	}
	return ns;
}

std::string DomainName::toDomain(){
	return toDomain(domains, "");
	
}

std::string DomainName::toUrl(){
	return param.empty() ? toDomain() : toDomain().append("/").append(param);
}

std::string DomainName::toDomain(const std::vector<std::string> &s, const std::string &suffix){
	std::string ret;
	for(std::vector<std::string>::const_reverse_iterator it =	s.rbegin(); it != s.rend(); ++it){
		ret.append(*it).append(".");
	}
	if(!ret.empty()){
		if(suffix.empty())
			ret.erase(ret.length() - 1);
		else{
			ret.append(suffix);
		}
	}


	return ret;
}

std::string extactUid(const std::string &url){
	size_t pos = url.find_last_of('/');
	if(pos != std::string::npos){
		return url.substr(pos + 1);
	}else{
		return url;
	}
}

std::string extractParam(const std::string &url){
	size_t pos = url.find_last_of('/');
	if(pos != std::string::npos){
		return url.substr(pos + 1);
	}else{
		return "";
	}
}

std::string url2UidStr(const std::string &url){
	return extactUid(url);
}

/*
std::string uid2Url(uint32_t uid){
	char buf[30];
	sprintf(buf, "%u.%s/%u", hashUid(uid), SUFFIX_UID, uid);
	return buf;
}*/

/*
std::string uid2Url(uint32_t uid, const char* suffix){
	char buf[30];
	sprintf(buf, "%u.%s/%u", hashUid(uid), suffix, uid); //SUFFIX_UID, SUFFIX_IM
	return buf;
}*/

std::string uid2LinkUrl(uint32_t uid){
	char buf[30];
	sprintf(buf, "%u.%s", uid, SUFFIX_ROUTE);
	return buf;
}

std::string uidFrom(uint32_t uid, uint32_t serverId){
	char buf[30];
	sprintf(buf, "%u.%s/%u", uid, SUFFIX_ROUTE, serverId);
	return buf;
}

std::string uidFromUdp(uint32_t uid, uint32_t serverId){
	char buf[30];
	sprintf(buf, "%u.%s/%u", uid, "_ru", serverId);
	return buf;
} 

uint32_t url2Uid(const std::string &url){
	DomainName dn = DomainName::parse(url);
	return atoi(dn.getLeaf().data());
}

bool DomainName::isUser(){
	return !domains.empty() && domains.front() == SUFFIX_ROUTE;
}

const std::string &DomainName::getParam() const{
	return param;
}

std::string DomainName::getRoot() const{
	return domains.empty() ? "" : domains.front();
}

bool DomainName::isRoot() const{
	return domains.size() == 1;
}

std::string DomainName::getLeaf() const{
	return domains.size() > 1 ? domains[1] : "";
}

std::string DomainName::trimParam(const std::string &url){
	return url.substr(0, url.find_last_of('/'));
}

bool DomainName::toUid(const std::string &url, uint32_t &out){
	DomainName dn = DomainName::parse(url);
	if(dn.getRoot() == SUFFIX_ROUTE){
		out = atoi(dn.getLeaf().data());
		if(out == 0 && dn.getLeaf() != "0"){
			return false;
		}else{
			return true;
		}
	}
	return false;
}

bool DomainName::getSid(const std::string &url, uint32_t &out){
	DomainName dn = DomainName::parse(url);
	out = atoi(dn.param.data());
	if(out == 0 && dn.param != "0"){
		return false;
	}else{
		return true;
	}
}

bool DomainName::getSessionId(const std::string &url, uint32_t &out){
	DomainName dn = DomainName::parse(url);
	out = atoi(dn.getLeaf().data());
	if(out == 0 && dn.getLeaf() != "0"){
		return false;
	}else{
		return true;
	}
}

uint32_t DomainName::getParami() const {
	return atoi(param.data());
}
