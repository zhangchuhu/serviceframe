#ifndef _PXGLOBALCONF_H_20111027
#define _PXGLOBALCONF_H_20111027
#include <map>
#include <vector>
#include <string>
#include "server_common/seda/stage/stage_base.h"

#include "global_typedef.h"


namespace server
{
	namespace dao_proxy
	{

	struct IProcessNotify{
		virtual ~IProcessNotify(){}
		virtual void process_notify(const std::string& name,const std::vector<IPPORTYPE>& ipports,int groupid) = 0;
	};
	struct ILicenceNotify{
		virtual ~ILicenceNotify(){}
		virtual void licence_notify(const std::map<std::string,std::string>& licence_key) = 0;
	};
	struct IThrottleNotify{
		virtual ~IThrottleNotify(){}
		virtual void throttle_notify(const std::map<std::string,std::string>& locate,int tickets) = 0;
	};
	struct IRouteNotify{
		virtual ~IRouteNotify(){}
		virtual void route_notify(const std::map<std::string,std::string>& locate,const std::map<std::string,std::string>& rule) = 0;
	};

	class PxGlobalConf
	{
	public:
		PxGlobalConf();
		~PxGlobalConf();

		// -1 fail and exit, 0 success
		int init(std::string conf_file_path);

		int init_from_mem(std::string xml_content);

		/**
		 * @return 
				0	success
				-1	parse xml fail
				-2	save fail
				-3	md5 not match
		 */
		int32_t updateGlobalConf(const std::string& strXmlContent);
		/**
		 * @return:
				0	success
				-1	read file fail
				-2	parse xml fail
		 */
		int32_t loadFromFile();
		// change md5
		// -1 save fail, -2 read file fail
		int32_t saveToFile();
		int32_t saveToBackUpFile();
		///
		std::string getGlobalConfMd5();

		void getall_daoadd(IProcessNotify* prcdao_cb);
		void getall_proxyadd(IProcessNotify* prcproxy_cb);
		void getall_licence(ILicenceNotify* licence_cb);
		void getall_throttle_limit(IThrottleNotify* thr_cb);
		void getall_route_rule(IRouteNotify* route_cb);

	public:
		struct PrcAdd
		{
			//static
			std::vector<IPPORTYPE> ipports;
			int groupid;
		};

	public:
		std::string last_load_from_file;
	public:
		void set_dao_item(std::string& serverid,PrcAdd& address);
		void set_proxy_item(std::string& serverid,PrcAdd& address);
		void del_dao_item(std::string& serverid);
		void del_proxy_item(std::string& serverid);
		void get_all_dao(std::map<std::string, PrcAdd>& outdat);
		void get_all_proxy(std::map<std::string, PrcAdd>& outdat);

		int check_validate();

		std::map<int,int>& get_hash_ring();
	private:
		seda::linux_mutex m_mtxConf;
		std::map<std::string, PrcAdd> dao_list;///key is serverid
		std::map<std::string, PrcAdd> proxy_list;///key is serverid

		typedef std::map<std::string,std::string> LOCATE_TYPE;

		std::map<LOCATE_TYPE, int> throttle_rule;
		std::map<LOCATE_TYPE, ROUTESTRATEGY_TYPE > routestrategy_rule;

		std::map<LOCATE_TYPE, int> licence_rule;//开放接口列表

		std::map<int,int> m_hashring_group2weight;

		std::string m_strGlobalConfMd5;
		std::string m_strGlobalConfFilePath;

		// -1 fail, 0 success, -2 xml wrong
		int32_t parseGlobalConfXml(const std::string& strXml, const std::string& md5);

		int32_t gl_inner_saveToFile(std::string& fpath);
		// return 0:ok -1:fail 
		static int validate_conf_para(
			std::string& error_info
			,std::map<std::string, PrcAdd>& load_dao_list
			,std::map<std::string, PrcAdd>& load_proxy_list
			,std::map<LOCATE_TYPE, int>& load_throttle_rule
			,std::map<LOCATE_TYPE, ROUTESTRATEGY_TYPE >& load_routestrategy_rule
			,std::map<int,int>& load_m_hashring_group2weight
			,std::map<PxGlobalConf::LOCATE_TYPE, int>& load_licence_rule);
	};

	}
}

#endif	// _PXGLOBALCONF_H_20111027

