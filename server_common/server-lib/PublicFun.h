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
/* ����oracle���� in �����Ԫ�����1000����������or�ѳ���1000��Ԫ�طֿ� */
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
 * @brief	utf8�����и��ݵ�һ���ֽ��жϸ��ַ�ռ�����ֽ�
 *
 * @param	utf8Str		�ַ��ĵ�һ���ֽ�
 * @return				�ַ�ռ���ֽ������纺����ռ3���ֽ�	
						�ַ��Ƿ��򷵻�0
 *
 * �ο�http://zh.wikipedia.org/wiki/UTF-8#UTF-8.E7.9A.84.E7.B7.A8.E7.A2.BC.E6.96.B9.E5.BC.8F
 */
inline int WordLength(unsigned char head)
{
	unsigned char testHead = head >> 6;
	if (testHead == 2)	// �ַ��ĵ�һ���ֽڿ���Ϊ10xx xxxx
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
 * @brief	��ȡutf8�ַ������ַ�����
 *
 * @param	utf8Str		�����utf8�ַ���
 * @return				�ַ������ַ��ĸ���	
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

