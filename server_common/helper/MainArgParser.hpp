/***
自定制的main函数参数分析器
建议使用boost/parser.hpp
***/

#ifndef __MAIN_ARG_PARSER_HPP
#define __MAIN_ARG_PARSER_HPP

#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<sstream>
#include<iterator>

using namespace std;

template <typename T =char>
class MainArgParser
{
public:

  typedef vector<basic_string<T> > string_array_t;

  MainArgParser(int argc, char ** argv)
  {
    for(int i=0; i<argc; ++i)
    {
      m_Content.push_back(basic_string<T>(argv[i]));
    }
    m_pcerr = &cerr;
  };
  
  MainArgParser(int argc, wchar_t ** argv)
  {
    for(int i=0; i<argc; ++i)
    {
      m_Content.push_back(basic_string<T>(argv[i]));
    }
    m_pcerr = &wcerr;
  };

  virtual ~MainArgParser()
  {

  };

public:

  //--option (switch a boolean true)
  int SwtchOpt(const basic_string<T> & strOpt, bool& bVar) const
  {
    int res = _SwtchOpt(strOpt, bVar);
    if(res)
    {
      *m_pcerr << strOpt << ": true" << endl; 
    }
    else
    {
      *m_pcerr << strOpt << ": undefine(default=false)" << endl;
    }
    
    return res;
  }

  //--option value
  template<typename TYPE>
  int GetOptVal(const basic_string<T> & strOpt, TYPE& tVar) const
  {
    int res = _GetOptVal(strOpt, tVar);
    *m_pcerr << strOpt << ": " << tVar << endl;
    return res;
  }

protected:

  //--option (switch a boolean true)
  int _SwtchOpt(const basic_string<T> & strOpt, bool& bVar) const
  {
    typename string_array_t::const_iterator itCont;
    itCont = find(m_Content.begin(), m_Content.end(), strOpt);
    if ( itCont != m_Content.end() )
    { 
      bVar = true;   
      return 1;
    }
    else
    {
      bVar = false;
      return 0;
    }
  };

  //--option value
  template<typename TYPE>
  int _GetOptVal(const basic_string<T> & strOpt, TYPE& tVar) const
  {
    int ret = 0;
    typename string_array_t::const_iterator itCont;
    itCont = find(m_Content.begin(), m_Content.end(), strOpt);
    if ( itCont != m_Content.end() )
    { 
      if( ++ itCont != m_Content.end() )
      {
        basic_istringstream<T> issOpt(*itCont);
        issOpt >> tVar;
        ret = 1;
      }
    }
    return ret;
  };

  //--option value(=string "...")
  int _GetOptVal(const basic_string<T> & strOpt, basic_string<T>& tVar) const
  {
    int ret = 0;
    typename string_array_t::const_iterator itCont;
    itCont = find(m_Content.begin(), m_Content.end(), strOpt);
    if ( itCont != m_Content.end() )
    { 
      ++ itCont;
      if( (*itCont)[0] == '\"')
      {
        //tVar = itCont->erase(0, 1);
        ret = 1;
        tVar = *itCont;

        do
        {
          ++ itCont;
          tVar += *itCont;
        }
        while(itCont->find_first_of('\"') == itCont->length());

        //tVar.erase(tVar.find_first_of("\""));
        //tVar.erase(tVar.find_last_of("\""));

      }
      else
      {
        if( itCont != m_Content.end() )
        {
          tVar = *itCont;
          ret = 1;
        }
      }
    }
    return ret;
  };
private:
  string_array_t m_Content;

  mutable basic_ostream<T> * m_pcerr;
};

#endif // __MAIN_ARG_PARSER_HPP

