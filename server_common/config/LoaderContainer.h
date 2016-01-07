#ifndef LOADER_CONTAINER_H_FXW
#define LOADER_CONTAINER_H_FXW
#include "core/sox/snox.h"
#include <set>
#include <string>

#include "IConfigLoader.h"

namespace server{
	namespace config{

		class RemoteUrlLoader{
		public:

		};

		class LoaderContainer: public sox::Handler{
			std::set<IConfigLoader *> loaders;

      std::string configFile;

      void init();
		public:
			LoaderContainer();
      LoaderContainer( const std::string & file );

			~LoaderContainer();
			void addLoader(IConfigLoader *loader);
			
			void loadAll();

			//void setFileName();

			virtual void handle(int sig);
		};
	}
}
#endif
