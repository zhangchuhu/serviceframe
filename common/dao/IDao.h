#ifndef __IDAO_H__
#define __IDAO_H__

#include <mysql++/mysql++.h>
#include <mysql++/custom.h>
#include "server/dblib/MysqlConnCreator.h"
#include <stdarg.h>
#include "commonUtil.h"




namespace server{
namespace dao{
	//RUCD 时需要调用的一些基础方法
	struct IMysqlCallBack{
		virtual mysqlpp::SQLQueryParms &getParam() = 0;
		virtual bool onResult(mysqlpp::Result &result, void *) = 0;
		virtual void onException(mysqlpp::BadQuery &bq) = 0;
		virtual bool isPreview() = 0;
	};

	class AbstractCallBack:public IMysqlCallBack{
	protected:
		bool bPreview;
		mysqlpp::SQLQueryParms params;
		static uint32_t uParamIndex;
	public:
		virtual bool isPreview(){return bPreview;}
		void setPreview(bool b){bPreview = b;}
		virtual void onException(mysqlpp::BadQuery &bq){printf("exec error:%s", bq.what());}
		virtual mysqlpp::SQLQueryParms &getParam(){return params;}
		template<class  C> AbstractCallBack &addParam(C &p){
			params << p;
			return *this;
		}
	};


	//定义CRUD 接口模板
	class MysqlHelperTempalte: public db::ISqlConnFactoryAware{
		std::string name;
	public:
		MysqlHelperTempalte(std::string name="dspeak"){};
		bool commonSqlExe(AbstractCallBack *cb, void *obj, const char *cntrl_string, ...){
	//release va_arg, addParam...
			uint32_t uParam;
			std::string strParam;
			int pos = 0;
			va_list va;

			//get the type of sql
			std::string strCmdType = (char*)cntrl_string;
			strCmdType = common_util::trim(strCmdType);
			strCmdType = strCmdType.substr(0,6);
			printf("strCmdType:[%s]\n", strCmdType.data());
			//
			va_start(va, cntrl_string);
			while(cntrl_string[pos]){
				if ('%' == cntrl_string[pos]){
					switch (cntrl_string[pos+2]){
						case 'q':
							printf("catch q => ");
							const char* ccParamTmp;						
							ccParamTmp = va_arg(va, char*);
							strParam = (char *)ccParamTmp;
							cb->addParam(strParam);
							printf("get resluts:[%s]\n", strParam.data());	
							pos += 2;
							break;	
						default:
							printf("catch defult!-mean uint32_t =>");
							uParam = va_arg(va, unsigned int);
							cb->addParam(uParam);						
							printf("get resluts:[%u]\n", uParam);											
							pos++;
					} 
				}
				pos++;

			}
			va_end(va);
	//~
	//query ...		
			mysqlpp::Connection *c = getSqlConnFactory()->getConnectionByName(name);
			if(c){
					mysqlpp::Query q = c->query();
					q << (std::string)cntrl_string;
					q.parse();
	//				if(cb && cb->isPreview()){
					if(cb ){
						printf("sqlPreView:[%s]\n", q.preview(cb->getParam()).data());
					}
					try{
						if ("select" == strCmdType){
							mysqlpp::Result res = q.store(cb->getParam());
							if(cb){
								cb->onResult(res, obj);
							}
						}
						else if("update" == strCmdType){
							mysqlpp::ResNSel nres = q.execute(cb->getParam());
							printf(">>>  update results info >>>\n isSuccess[%d]\n AffectRows[%lld]\n info about query result[%s]\n<<<<<<<<<<\n", nres.success, nres.rows, nres.info.data());
						}
						else if("insert" == strCmdType){
							mysqlpp::ResNSel nres = q.execute(cb->getParam());
							printf(">>>  insert results info >>>\n isSuccess[%d]\n AffectRows[%lld]\n info about query result[%s]\n<<<<<<<<<<\n", nres.success, nres.rows, nres.info.data());	
						}
						else if("delete" == strCmdType){
							mysqlpp::ResNSel nres = q.execute(cb->getParam());
							printf(">>>  delete results info >>>\n isSuccess[%d]\n AffectRows[%lld]\n info about query result[%s]\n<<<<<<<<<<\n", nres.success, nres.rows, nres.info.data());
							
						}			
						else{
							printf("Illegal sqlCmdType\n");
						}
						return true;
					}catch(mysqlpp::BadQuery &bq){
						printf("exec error:%s\n", bq.what());
						mysqlpp::Connection *c = getSqlConnFactory()->getConnectionByName(name);
						if(!c){
							printf("re-get connection %s error.", name.data());
							return false;
						}					
						if(cb){
							cb->onException(bq);
						}
					}
			}
			printf("commonExe err!\n");
			return false;
		}
		
	};

 }
}
#endif

