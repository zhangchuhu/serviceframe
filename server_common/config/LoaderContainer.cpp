#include "LoaderContainer.h"
#include <signal.h>
#include <unistd.h>
#include <stdexcept>
#define RELOAD_CHECKTIME 500

using namespace server::config;
static bool reload = false;

#define DEFAULT_XML "../bin/server.xml"
#define ALLOW_XML "../bin/allow.xml"
#define	ALLOW_URL "http://allow.yy.duowan.com/allow.xml"

static void hup_handler(int ){
	reload = true;
}

LoaderContainer::LoaderContainer()
: configFile(DEFAULT_XML)
{
  init();
}

LoaderContainer::LoaderContainer( const std::string & file )
: configFile(file)
{
  if (configFile.empty() ) {
    configFile = DEFAULT_XML;
  }
  init();
}

LoaderContainer::~LoaderContainer(){
}

void LoaderContainer::addLoader(IConfigLoader *loader){
	loaders.insert(loader);
}

void LoaderContainer::loadAll(){
	/*FILE *fp_a = fopen(ALLOW_XML, "r");

	while(!fp_a){
		
	}

	for(std::set<IConfigLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it){
		log(Info, "reload trigger");
		(*it)->load(fp_a);
	}

	fclose(fp_a);*/

	FILE *fp = fopen(configFile.c_str(), "r");
	if(fp){
		for(std::set<IConfigLoader*>::iterator it = loaders.begin(); it != loaders.end(); ++it){
			log(Info, "reload trigger");
			(*it)->load(fp);
		}

		fclose(fp);
	}
}

void LoaderContainer::handle(int sig){
	if(reload){
		loadAll();
		reload = false;
	}
	select_timeout(RELOAD_CHECKTIME);
}

void server::config::LoaderContainer::init()
{
  select_timeout(RELOAD_CHECKTIME);

  if(signal(SIGHUP, hup_handler) == SIG_ERR){
    throw std::runtime_error("signal SIG_HUP");
  }
}
