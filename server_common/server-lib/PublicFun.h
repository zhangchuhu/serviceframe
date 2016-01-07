#ifndef PUBLIC_FUNCTION_H_
#define PUBLIC_FUNCTION_H_
#include <sstream>
#include "int_types.h"

namespace server{
template<class Container> std::string idcat(const Container &cont){
	std::ostringstream ostr;
	if(cont.size() > 0){
		ostr << *cont.begin();
		for(typename Container::const_iterator it = ++cont.begin(); it != cont.end(); ++it){
			ostr << "," << *it;
		}
	}

	return ostr.str();
}

template<class Container> std::string idcat_init(uint32_t init, const Container &cont){
	std::ostringstream ostr;
	ostr << init;
	for(typename Container::const_iterator it = cont.begin(); it != cont.end(); ++it){
		ostr << "," << *it;
	}
	return ostr.str();
}

template<class Container, class FetchOp> std::string idcat(const Container &cont, FetchOp f){
	std::ostringstream ostr;
	if(cont.size() > 0){
		ostr << f(cont.begin());
		for(typename Container::const_iterator it = ++cont.begin(); it != cont.end(); ++it){
			ostr << "," << f(it) ;
		}
	}

	return ostr.str();
}

template<class Container, class FetchOp> std::string idcat_init(uint32_t init, const Container &cont, FetchOp f){
	std::ostringstream ostr;
	ostr << init;
	for(typename Container::const_iterator it = cont.begin(); it != cont.end(); ++it){
		ostr  << "," << f(it);
	}
	return  ostr.str();
}

/************************************************************************/
/* 由于oracle限制 in 里面的元素最多1000个，必须用or把超过1000的元素分开 */
/************************************************************************/
/*template<class Container> std::string idOrCat(const Container &cont){
	std::ostringstream ostr;
	if(cont.size() > 0){
		ostr << *cont.begin();
		int size = 1;
		for(typename Container::const_iterator it = ++cont.begin(); it != cont.end(); ++it, ++size){
			ostr << "," << *it;
			if(size % 1000 == 0 && size != cont.size()){
				ostr << ") or (";
			}
		}
	}

	return ostr.str();
}*/

/**
 * @brief	utf8编码中根据第一个字节判断该字符占几个字节
 *
 * @param	utf8Str		字符的第一个字节
 * @return				字符占的字节数，如汉字是占3个字节	
						字符非法则返回0
 *
 * 参考http://zh.wikipedia.org/wiki/UTF-8#UTF-8.E7.9A.84.E7.B7.A8.E7.A2.BC.E6.96.B9.E5.BC.8F
 */
inline int WordLength(unsigned char head)
{
	unsigned char testHead = head >> 6;
	if (testHead == 2)	// 字符的第一个字节可能为10xx xxxx
	{
		return 1;
	}

	if (head < 128)
	{
		return 1;
	}
	else if (head < 224)
	{
		return 2;
	}
	else if (head < 240)
	{
		return 3;
	}
	else if (head < 248)
	{
		return 4;
	}
	else if (head < 252)
	{
		return 5;
	}
	else if (head < 254)
	{
		return 6;
	}
	else
	{
		return 1;
	}
}

/**
 * @brief	获取utf8字符串的字符个数
 *
 * @param	utf8Str		输入的utf8字符串
 * @return				字符串中字符的个数	
 */
inline int GetWordLengthOfUTF8Str(const std::string& utf8Str)
{
	int retLen = 0;
	for (size_t pos = 0; pos < utf8Str.size(); )
	{
		int tmpLen = WordLength(utf8Str.at(pos));
		++retLen;
		pos += tmpLen;
	}

	return retLen;
}

}
#endif

