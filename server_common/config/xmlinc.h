#pragma once

#ifdef XML_NULL
	#define XML_API
#else
	#ifdef XML_EXPORTS
	#define XML_API __declspec(dllexport)
	#else
	#define XML_API __declspec(dllimport)
	#endif
#endif

