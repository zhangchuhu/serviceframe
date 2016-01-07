#ifndef _PKICKOFF_H_
#define _PKICKOFF_H_

#include "int_types.h"
#include "core/base_svid.h"
#include "protocol/const.h"
#include "packet.h"
#include <string>
#include <vector>

namespace server{
	namespace kickoff{


		struct PKickOnly : public sox::Marshallable{
			enum {uri = (115 << 8 | 51)};
			uint32_t uid;
			std::string note; //自定义封禁原因
			uint32_t ban_type; //使用新弹窗协议的要都使用新类型
			std::string buttonName; //弹窗右下角的按钮文案，根据需要定制。无特别需求，填"确定"
			std::string buttonClickUrl; //弹窗右下角按钮对应的跳转网页链接。无跳转需求的填空

			PKickOnly() : ban_type(1000) {}  //设置ban_type默认值为1000

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << note << ban_type << buttonName << buttonClickUrl;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> note >> ban_type >> buttonName >> buttonClickUrl;
			}
		};
		
		struct PKickOffAndBan : public sox::Marshallable{
			enum {uri = (116 << 8 | 51)};
			uint32_t uid;
			uint32_t time_interval; // 封多长时间（以小时为单位）
			std::string note; //自定义封禁原因
			uint32_t ban_type; //使用新弹窗协议的要都使用新类型
			std::string buttonName; //弹窗右下角的按钮文案，根据需要定制。无特别需求，填"确定"
			std::string buttonClickUrl; //弹窗右下角按钮对应的跳转网页链接。无跳转需求的填空

			PKickOffAndBan() : ban_type(1000) {}  //设置ban_type默认值为1000

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << time_interval << note << ban_type << buttonName << buttonClickUrl;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> time_interval >> note >> ban_type >> buttonName >> buttonClickUrl;
			}
		};

		//发给linkd的新踢人协议
		struct PKickLinkd : public sox::Marshallable {
			enum {uri = (117 << 8 | 51)};
			uint32_t uid;
			std::string note;	//自定义封禁原因
			std::string buttonName; //弹窗右下角的按钮文案，根据需要定制。无特别需求，填"确定"
			std::string buttonClickUrl; //弹窗右下角按钮对应的跳转网页链接。无跳转需求的填空

			virtual void marshal(sox::Pack &pk) const {
				pk << uid << note << buttonName << buttonClickUrl;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> uid >> note >> buttonName >> buttonClickUrl;
			}
		};

	  struct POnlyKickOff2 : public sox::Marshallable
	  {
	    enum { uri = (118 << 8 | 51) };

	    std::string admin;
	    uint32_t uid;
	    std::string note;

	    POnlyKickOff2() : uid(0) {}

	    virtual void marshal(sox::Pack &pk) const 
	    {
	      pk << admin << uid << note;
	    }
	    virtual void unmarshal(const sox::Unpack &up)
	    {
	      up >> admin >> uid >> note;
	    }
	  };

	  struct PKickOffAndBanUID2 : public sox::Marshallable
	  {
	    enum {uri = (119 << 8 | 51)};

	    std::string admin;
	    uint32_t uid;
	    uint32_t time_interval;
	    std::string note;
	    uint32_t ban_type;
	    
	    PKickOffAndBanUID2() : uid(0), time_interval(0), ban_type(0) {}
	    
	    virtual void marshal(sox::Pack &pk) const
	    {
	      pk << admin << uid << time_interval << note << ban_type;
	    }
	    virtual void unmarshal(const sox::Unpack &up)
	    {
	      up >> admin >> uid >> time_interval >> note >> ban_type;
	    }
	  };

	  struct PKickOffAndBanUIDS2 : public sox::Marshallable
	  {
	    enum {uri = (120 << 8 | 51)};

	    std::vector<uint32_t> uids;

	    std::string admin;    
	    uint32_t time_interval;
	    std::string note;
	    uint32_t ban_type;
	    
	    PKickOffAndBanUIDS2() : time_interval(0), ban_type(0) {}
	    
	    virtual void marshal(sox::Pack &pk) const
	    {
	      sox::marshal_container(pk, uids);
	      pk << admin << time_interval << note << ban_type;
	    }
	    virtual void unmarshal(const sox::Unpack &up)
	    {
	      sox::unmarshal_container(up, std::back_inserter(uids));
	      up >> admin >> time_interval >> note >> ban_type;
	    }
	  };  

	  struct PUnBanUID2 : public sox::Marshallable
	  {
	    enum { uri = (121 << 8 | 51) };

	    std::string admin;
	    std::string note;
	    std::vector<uint32_t> uids;

	    virtual void marshal(sox::Pack &pk) const
	    {
	      pk << admin  << note;
	      sox::marshal_container(pk, uids);
	    }

	    virtual void unmarshal(const sox::Unpack &up)
	    {
	      up >> admin >> note;
	      sox::unmarshal_container(up, std::back_inserter(uids));
	    }
	  };

		// inherited from PKickOnly
		struct PKickOnly2 : public sox::Marshallable
		{
			enum { uri = (122 << 8 | 51) };

			std::string admin;
			uint32_t uid;
			std::string note; //自定义封禁原因
			uint32_t ban_type; //使用新弹窗协议的要都使用新类型
			std::string buttonName; //弹窗右下角的按钮文案，根据需要定制。无特别需求，填"确定"
			std::string buttonClickUrl; //弹窗右下角按钮对应的跳转网页链接。无跳转需求的填空

			PKickOnly2() : uid(0), ban_type(1000) {}  //设置ban_type默认值为1000

			virtual void marshal(sox::Pack &pk) const {
				pk << admin << uid << note << ban_type << buttonName << buttonClickUrl;
			}
			virtual void unmarshal(const sox::Unpack &up) {
				up >> admin >> uid >> note >> ban_type >> buttonName >> buttonClickUrl;
			}
		};	  

	}
}

#endif
