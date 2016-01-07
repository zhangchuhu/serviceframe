#pragma once

#include <map>
#include <vector>
#include "common/int_types.h"

namespace protocol
{
	namespace goffline
	{

		//底层需要实现的接口，供UI调用
		struct IGofflineAction
		{
			//virtual void Get(const std::map<uint32_t/*群id*/,std::vector<uint32_t/*用户ID*/> >)=0;
			virtual void Get()=0;

		};
	}
}
