#pragma once

#include <map>
#include <vector>
#include "common/int_types.h"

namespace protocol
{
	namespace goffline
	{

		//�ײ���Ҫʵ�ֵĽӿڣ���UI����
		struct IGofflineAction
		{
			//virtual void Get(const std::map<uint32_t/*Ⱥid*/,std::vector<uint32_t/*�û�ID*/> >)=0;
			virtual void Get()=0;

		};
	}
}
