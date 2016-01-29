#include "zk_config_item.h"
#include "core/sox/logger.h"

using namespace std;
using namespace libzk;
ZkConfigItem::ZkConfigItem()
{

}
ZkConfigItem::~ZkConfigItem()
{

}
ZkConfigItem::ZkConfigItem( const std::string& key,const std::string& config_type,const std::string& config_name ):key_( key ) {
	AddString( kConfigType,config_type );
	AddString( kConfigName,config_name );
}

int32_t ZkConfigItem::Init( const std::string& key) {
	log(Info,"[ZkConfigItem::%s] key=%s",__FUNCTION__,key.data());
	key_ = key;
	std::vector<string> children;
	zk_client->getChildren(key,children,false);
	for (uint32_t i=0;i<children.size();i++)
	{
		string value;
		string path = key+"/"+children[i];
		zk_client->getNodeData(path,value,false);
		AddString(children[i],value);
	}

}

void ZkConfigItem::AddString(const std::string& field_name,const std::string& value ) {

	fields_pairs_[field_name] = value;
}

void ZkConfigItem::AddInt( const std::string& field_name,int32_t value) {

	std::stringstream ss;

	ss<< value;

	fields_pairs_[field_name] = ss.str();
}


int ZkConfigItem::GetIntValue( const std::string& field_name ) {

	return atoi( fields_pairs_[field_name].c_str() );
}

std::string ZkConfigItem::GetStringValue( const std::string& field_name ) {

	return fields_pairs_[field_name];
}

ZkConfigItem* ZkConfigItem::Clone() {

	ZkConfigItem* item = new ZkConfigItem;

	item->key_ = this->key_;

	item->fields_pairs_.insert( this->fields_pairs_.begin(),this->fields_pairs_.end() );

	return item;
}
