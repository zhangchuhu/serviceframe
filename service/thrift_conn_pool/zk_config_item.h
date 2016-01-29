#ifndef __ZK_CONFIG_ITEM_H__
#define __ZK_CONFIG_ITEM_H__

#include <string>
#include <map>
#include <sstream>

#include "libzkclient.h"

namespace libzk { 
#define kConfigType   "type" 
#define kConfigName   "name" 
#define kConfigHostName   "hostname" 
#define kConfigIp   "ip" 
#define kConfigPort   "port" 
#define kConfigPassWord   "password" 
#define kConfigComment   "comment" 
#define kConfigGroupId   "group_id" 
#define kTypeThrift   "thrift" 
#define kTypeOther "other"
#define kTypeRedis   "redis" 
#define kTypeMemcache   "memcache" 
#define kTypeProc  "proc"
#define kTypeOrg   "org"

#define kNormalMode "normal_mode"
#define kGroupMode "group_mode"
#define kIpMode "ip_mode"

	class ZkConfigItem: public ZkClientAware  {

	public:

		ZkConfigItem() ;

		ZkConfigItem( const std::string& key,const std::string& config_type,const std::string& config_name );

		~ZkConfigItem();

	public:

		int32_t Init(const std::string& key) ;

		virtual void AddString(const std::string& field_name,const std::string& value );

		virtual void AddInt(const std::string& field_name,int32_t value);

		virtual int32_t GetIntValue( const std::string& field_name ) ;

		virtual std::string GetStringValue( const std::string& field_name ) ; 

		virtual std::string key(){ return key_; }

		virtual ZkConfigItem* Clone();

	public:

		std::string key_;

		std::map<std::string,std::string> fields_pairs_;

	};
}
#endif // __ZK_CONFIG_ITEM_H__


