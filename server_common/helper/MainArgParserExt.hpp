/***
自定制的main函数参数分析器
建议使用boost/parser.hpp
***/

#ifndef __MAIN_ARG_PARSER_EXT_HPP
#define __MAIN_ARG_PARSER_EXT_HPP

#include "core/sox/logger.h"
#include "MainArgParser.hpp"

using namespace std;

template <typename T =char>
class MainArgParserExt : public MainArgParser<T>
{
public:

  MainArgParserExt(int argc, char ** argv)
        : MainArgParser<T>(argc, argv)
  {

  };
  
  MainArgParserExt(int argc, wchar_t ** argv)
        : MainArgParser<T>(argc, argv)
  {

  };

  virtual ~MainArgParserExt()
  {

  };

  //--option (switch a boolean true)
  int SwtchOpt(const basic_string<T> & strOpt, bool& bVar) const
  {
    MainArgParser<T>::SwtchOpt(strOpt, bVar);
    ostringstream ostr;
    ostr << strOpt << " = " << bVar;
    log(Info, "%s", ostr.str().c_str());   
    return 0;
  };

  //--option value
  template<typename TYPE>
  int GetOptVal(const basic_string<T> & strOpt, TYPE& tVar) const
  {
    MainArgParser<T>::GetOptVal(strOpt, tVar);
    ostringstream ostr;
    ostr << strOpt << " = " << tVar;
    log(Info, "%s", ostr.str().c_str());
    return 0;
  };
};

#endif // __MAIN_ARG_PARSER_HPP

